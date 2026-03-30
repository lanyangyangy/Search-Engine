#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sw/redis++/redis++.h>

using namespace std;
using namespace sw::redis;

// 重载 cout 输出 vector
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    if (!v.empty())
    {
        os << '[';
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, ", "));
        os << "\b\b]"; // 删除末尾的", "
    }
    return os;
}

// 重载 cout 输出 unordered_map
template <typename K, typename V>
std::ostream &operator<<(std::ostream &os, const std::unordered_map<K, V> &umap)
{
    os << '[';
    for (auto item : umap)
    {
        os << "(" << item.first << "," << item.second << "),";
    }
    os << "\b]"; // 删除末尾的","
    return os;
}

// 重载 cout 输出 unorderd_set
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::unordered_set<T> &uset)
{
    os << '(';
    for (auto item : uset)
    {
        os << item << ",";
    }
    os << "\b)"; // 删除末尾的","
    return os;
}

int main()
{
    try
    {
        // 创建一个 Redis 对象，该对象支持移动语义 (movable) 但不可拷贝 (NOT copyable)。
        auto redis = Redis("tcp://127.0.0.1:6379");

        /// ***** 字符串 (STRING) 命令 *****
        redis.set("key", "val");
        // val 的类型是 OptionalString。详情请参阅官方文档的 'API Reference' 部分。
        auto val = redis.get("key"); 
        if (val)
        {
            // 对 val 解引用，以获取 std::string 类型的返回值。
            std::cout << *val << std::endl;
        } // 否则表示 key 不存在。

        /// ***** 列表 (LIST) 命令 *****
        // 将 std::vector 写入 Redis 列表。
        std::vector<std::string> vec = {"a", "b", "c"};
        redis.rpush("list", vec.begin(), vec.end());

        // 将 std::initializer_list 写入 Redis 列表。
        redis.rpush("list", {"a", "b", "c"});

        // 从 Redis 列表读取到 std::vector 中。
        vec.clear();
        redis.lrange("list", 0, -1, std::back_inserter(vec));

        cout << "list: " << vec << endl;

        /// ***** 哈希表 (HASH) 命令 *****
        redis.hset("hash", "field", "val");

        // 实现相同功能的另一种方式。
        redis.hset("hash", std::make_pair("field", "val"));

        // 将 std::unordered_map 写入 Redis 哈希表。
        std::unordered_map<std::string, std::string> m = 
        {
            {"field1", "val1"},
            {"field2", "val2"}
        };
        redis.hmset("hash", m.begin(), m.end());

        // 从 Redis 哈希表读取到 std::unordered_map 中。
        m.clear();
        redis.hgetall("hash", std::inserter(m, m.begin()));

        cout << "hash:" << m << endl;

        // 仅获取特定的值。
        // 注意：由于 field 可能不存在，因此我们需要将其解析保存为 OptionalString 类型的容器。
        std::vector<OptionalString> vals;
        redis.hmget("hash", {"field1", "field2"}, std::back_inserter(vals));

        /// ***** 集合 (SET) 命令 *****
        redis.sadd("set", "m1");

        // 将 std::unordered_set 写入 Redis 集合。
        std::unordered_set<std::string> set = {"m2", "m3"};
        redis.sadd("set", set.begin(), set.end());

        // 将 std::initializer_list 写入 Redis 集合。
        redis.sadd("set", {"m2", "m3"});

        // 从 Redis 集合读取到 std::unordered_set 中。
        set.clear();
        redis.smembers("set", std::inserter(set, set.begin()));

        cout << "set:" << set << endl;

        if (redis.sismember("set", "m1"))
        {
            std::cout << "m1 exists" << std::endl;
        } // 否则表示不存在。

        /// ***** 有序集合 (SORTED SET) 命令 *****
        redis.zadd("sorted_set", "m1", 1.3);

        // 将 std::unordered_map 写入 Redis 有序集合。
        std::unordered_map<std::string, double> scores = 
        {
            {"m2", 2.3},
            {"m3", 4.5}
        };
        redis.zadd("sorted_set", scores.begin(), scores.end());

        // 从 Redis 有序集合读取到 std::vector<std::pair<std::string, double>> 中。
        // 注意：zrangebyscore 的返回结果是有序的，如果将其保存到 `std::unordered_map` 中，将会丢失原来的排序。
        std::vector<std::pair<std::string, double>> zset_result;
        redis.zrangebyscore("sorted_set",
                            UnboundedInterval<string>{}, // 代表区间 (-inf, +inf)
                            std::back_inserter(zset_result));

        // 仅获取成员名称 (Member names)：
        // 传递一个 std::vector<std::string> 类型的插入迭代器作为输出参数。
        std::vector<std::string> without_score;
        redis.zrangebyscore("sorted_set",
                            BoundedInterval<double>(1.5, 3.4, BoundType::CLOSED), 
                            // 代表闭区间 [1.5, 3.4]
                            std::back_inserter(without_score));

        // 同时获取成员名称和分数 (Scores)：
        // 传递一个 std::vector<std::pair<std::string, double>> 类型的尾部插入迭代器作为输出参数。
        std::vector<std::pair<std::string, double>> with_score;
        redis.zrangebyscore("sorted_set",
                            BoundedInterval<double>(1.5, 3.4, BoundType::LEFT_OPEN), 
                            // 代表左开右闭区间 (1.5, 3.4]
                            std::back_inserter(with_score));

        /// ***** 脚本 (SCRIPTING) 命令 *****
        // 脚本返回单个元素。
        auto num = redis.eval<long long>("return 1", {}, {});

        // 脚本返回一个元素数组。
        std::vector<std::string> nums;
        redis.eval("return {ARGV[1], ARGV[2]}", {}, 
                   {"1", "2"}, std::back_inserter(nums));

        // 带有过期时间 (TTL) 的批量设置 (mset) 脚本
        auto mset_with_ttl_script = R"(
        local len = #KEYS
        if (len == 0 or len + 1 ~= #ARGV) then return 0 end
        local ttl = tonumber(ARGV[len + 1])
        if (not ttl or ttl <= 0) then return 0 end
        for i = 1, len do redis.call("SET", KEYS[i], ARGV[i], "EX", ttl) end
        return 1
        )";

        // 设置多个键值对，并指定 60 秒的过期时间。
        auto keys = {"key1", "key2", "key3"};
        std::vector<std::string> args = {"val1", "val2", "val3", "60"};
        redis.eval<long long>(mset_with_ttl_script, keys.begin(), keys.end(), 
                              args.begin(), args.end());

        /// ***** 管道 (Pipeline) *****
        // 创建一个管道。
        auto pipe = redis.pipeline();

        // 打包发送多个命令，并获取所有的回复结果。
        auto pipe_replies = pipe.set("key", "value").get("key")
                                .rename("key", "new-key")
                                .rpush("list", {"a", "b", "c"})
                                .lrange("list", 0, -1)
                                .exec();

        // 根据回复类型和指令索引解析具体的回复结果。
        auto set_cmd_result = pipe_replies.get<bool>(0);

        auto get_cmd_result = pipe_replies.get<OptionalString>(1);

        // 获取 rename 命令的结果
        pipe_replies.get<void>(2);

        auto rpush_cmd_result = pipe_replies.get<long long>(3);

        std::vector<std::string> lrange_cmd_result;
        pipe_replies.get(4, back_inserter(lrange_cmd_result));

        /// ***** 事务 (Transaction) *****
        // 创建一个事务。
        auto tx = redis.transaction();

        // 在一个事务中执行多个命令，并获取所有的回复结果。
        auto tx_replies = tx.incr("num0")
                            .incr("num1")
                            .mget({"num0", "num1"})
                            .exec();

        // 根据回复类型和指令索引解析具体的回复结果。
        auto incr_result0 = tx_replies.get<long long>(0);

        auto incr_result1 = tx_replies.get<long long>(1);

        std::vector<OptionalString> mget_cmd_result;
        tx_replies.get(2, back_inserter(mget_cmd_result));

        /// ***** 通用命令接口 (Generic Command Interface) *****
        // 虽然 redis++ 没有提供原生的 *Redis::client_getname* 接口。
        // 但你可以使用 *Redis::command* 函数来获取客户端名称。
        val = redis.command<OptionalString>("client", "getname");
        if (val)
        {
            std::cout << *val << std::endl;
        }

        // 作用同上。
        auto getname_cmd_str = {"client", "getname"};
        val = redis.command<OptionalString>(getname_cmd_str.begin(), 
                                            getname_cmd_str.end());

        // 同样，没有原生的 *Redis::sort* 接口。
        // 但你可以使用 *Redis::command* 发送 sort 命令对列表进行排序。
        std::vector<std::string> sorted_list;
        redis.command("sort", "list", "ALPHA", std::back_inserter(sorted_list));

        // 使用另一种 *Redis::command* 参数形式来完成相同的排序工作。
        auto sort_cmd_str = {"sort", "list", "ALPHA"};
        redis.command(sort_cmd_str.begin(), sort_cmd_str.end(), 
                      std::back_inserter(sorted_list));

        /// ***** Redis 集群 (Redis Cluster) *****
        // 创建一个 RedisCluster 对象，该对象支持移动语义但不可拷贝。
        auto redis_cluster = RedisCluster("tcp://127.0.0.1:6380");

        // RedisCluster 拥有与单机版 Redis 极其相似的接口。
        redis_cluster.set("key", "value");
        val = redis_cluster.get("key");
        if (val)
        {
            std::cout << *val << std::endl;
        } // 否则表示 key 不存在。

        // 带有哈希标签 (hash-tag) 的键，用于确保它们分配在同一个 Slot 中。
        redis_cluster.set("key{tag}1", "val1");
        redis_cluster.set("key{tag}2", "val2");
        redis_cluster.set("key{tag}3", "val3");

        std::vector<OptionalString> hash_tag_res;
        redis_cluster.mget({"key{tag}1", "key{tag}2", "key{tag}3"},
                           std::back_inserter(hash_tag_res));
    }
    catch (const Error &e)
    {
        // 错误处理逻辑。
        std::cerr << "Redis Error: " << e.what() << std::endl;
    }

    return 0;
}
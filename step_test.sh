#!/bin/bash

# ================= 配置区 =================
SERVER_IP="127.0.0.1"
SERVER_PORT=1234
# 定义并发量递增的阶梯数组
CONCURRENCY_LEVELS=(1000 100000)
# ==========================================

echo "=================================================="
echo "🎯 开始测试 C++ EchoServer ($SERVER_IP:$SERVER_PORT)"
echo "📈 并发阶梯: ${CONCURRENCY_LEVELS[*]}"
echo "=================================================="

for level in "${CONCURRENCY_LEVELS[@]}"; do
    echo "[$(date +'%H:%M:%S')] 🚀 正在瞬间发起 $level 个并发连接，发送数据「振兴中华」..."

    # 记录当前批次开始时间 (纳秒)
    START_TIME=$(date +%s%N)

    # 循环拉起后台进程
    for (( i=1; i<=level; i++ )); do
        # 核心修改点：这里把发送的内容改成了 "振兴中华"
        # 如果你想区分是哪个客户端发的，可以改成 echo "振兴中华-$i"
        ( echo "振兴中华" | nc -w 2 $SERVER_IP $SERVER_PORT > /dev/null 2>&1 ) &
    done

    # 阻塞等待当前批次的所有后台子进程全部执行完毕
    wait

    # 记录结束时间并计算耗时 (毫秒)
    END_TIME=$(date +%s%N)
    ELAPSED_MS=$(( (END_TIME - START_TIME) / 1000000 ))

    echo "[$(date +'%H:%M:%S')] ✅ $level 并发批次执行完毕！本轮总耗时: ${ELAPSED_MS} ms"
    echo "--------------------------------------------------"
    
    # 强制休息 2 秒
    sleep 2
done

echo "🎉 所有阶梯测试结束！请检查服务端运行状态。"

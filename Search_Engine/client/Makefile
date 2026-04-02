# 当前路径
CUR_DIR = .
# 中间文件存放路径
OBJ_DIR = $(CUR_DIR)/obj

# 通常项
CC = g++
OUT = ../bin/Client
FLAGS = -O3 -g -std=c++11

# 源文件列表（当前目录下的所有 .cc 文件）
SRCS = $(wildcard $(CUR_DIR)/*.cc)

# 目标文件列表（将 .cc 路径替换为 obj 目录下的 .o 路径）
OBJS = $(patsubst $(CUR_DIR)/%.cc, $(OBJ_DIR)/%.o, $(SRCS))

# 创建 obj 目录（如果不存在）
$(shell mkdir -p $(OBJ_DIR))

# 确保输出目录存在（../bin）
$(shell mkdir -p $(dir $(OUT)))

ALL : $(OUT)

# 链接所有目标文件
$(OUT) : $(OBJS)
	$(CC) $^ $(FLAGS) -o $@

# 规则：编译 .cc 到 .o
$(OBJ_DIR)/%.o : $(CUR_DIR)/%.cc
	$(CC) $< $(FLAGS) -c -o $@

.PHONY: clean rebuild
clean:
	rm -f $(OUT) $(OBJ_DIR)/*.o
rebuild: clean ALL
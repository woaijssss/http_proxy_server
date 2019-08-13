
BUILD:=debug

# 二进制目标文件输出
# BIN:=whps
# 动态库名称
BIN_NAME:=libwhps
# 动态库版本
BIN_VERSION:=1
# 动态库子版本
BIN_SUBVERSION:=0.0
BIN_SOFT:=$(BIN_NAME).so
BIN_MID:=$(BIN_SOFT).$(BIN_VERSION)
BIN:=$(BIN_MID).$(BIN_SUBVERSION)


# DIRS
PUB:=pub
HTTP:=http
FACTORY:=factory
REGISTER:=register
CONFIG:=config

SRC_DIR:=src
BUILD_DIR:=build
BIN_DIR:=lib

SRC:=$(wildcard $(SRC_DIR)/*.cpp) \
	$(wildcard $(PUB)/*.cpp) \
	$(wildcard $(PUB)/pub_macro/*.cpp) \
	$(wildcard $(PUB)/pub_log/*.cpp) \
	$(wildcard $(PUB)/stdio/*.cpp) \
	$(wildcard $(SRC_DIR)/$(HTTP)/*.cpp) \
	$(wildcard $(SRC_DIR)/$(FACTORY)/*.cpp) \
	$(wildcard $(SRC_DIR)/$(REGISTER)/*.cpp) \
	$(wildcard $(SRC_DIR)/$(CONFIG)/*.cpp)

OBJ:=$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))

CPPFLAGS+=-Iinclude \
		-I$(PUB) \
		-I$(PUB)/pub_macro \
		-I$(PUB)/pub_log \
		-I$(PUB)/stdio \
		-Iinclude/$(HTTP) \
		-Iinclude/$(FACTORY) \
		-Iinclude/$(REGISTER) \
		-Iinclude/$(CONFIG) \
		-Itest_develop

CXXFLAGS+=-Wall -pedantic -Wextra -std=c++11 -MMD -D_GLIBCXX_USE_NANOSLEEP \
	-Wno-deprecated -Wdeprecated-declarations \
	-Wunused-function \
	-Wno-unused-parameter  # 设置此项，不提示“未使用变量”，正式程序需要去掉 

CXXFLAGS+=-fPIC

LDLIBS:=-pthread -lrt -shared

ifeq ($(BUILD), release)
CPPFLAGS+=-DNDEBUG
CFLAGS+=-O2
CXXFLAGS+=-O2
LDFLAGS+=-O2 -s
else
CPPFLAGS+=-D__DEBUG__
CFLAGS+=-O1 -g
CXXFLAGS+=-O1 -g
LDFLAGS+=-O1 -g
endif

.PHONY: all release clean

all: $(BIN_DIR)/$(BIN)
	@:

release:
	@make -s "BUILD=release"

$(BIN_DIR)/$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	@echo "(LD) $@"
	@$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@cd $(BIN_DIR) && ln -fs $(BIN) $(BIN_MID)
	@cd $(BIN_DIR) && ln -fs $(BIN_MID) $(BIN_SOFT) 

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "(CXX) $@"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	echo "(CXX) $@"
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

clean_all:
	@rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*

-include $(OBJ:.o=.d)

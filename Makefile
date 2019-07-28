
BUILD:=debug

# DIRS
PUB:=pub
HTTP:=http
SRC_DIR:=src
BUILD_DIR:=build
BIN_DIR:=bin

SRC:=$(wildcard $(SRC_DIR)/*.cpp) \
	$(wildcard $(PUB)/*.cpp) \
	$(wildcard $(SRC_DIR)/$(HTTP)/*.cpp)



OBJ:=$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
BIN:=whps

CPPFLAGS+=-Iinclude -I$(PUB) -I$(PUB)/pub_macro -Iinclude/$(HTTP)

CXXFLAGS+=-Wall -pedantic -Wextra -std=c++11 -MMD -D_GLIBCXX_USE_NANOSLEEP \
	-Wno-deprecated -Wdeprecated-declarations \
	-Wunused-function \
	-Wno-unused-parameter  # 设置此项，不提示“未使用变量”，正式程序需要去掉 

LDLIBS:=-pthread -lrt

ifeq ($(BUILD), release)
CPPFLAGS+=-DNDEBUG
CFLAGS+=-O2
CXXFLAGS+=-O2
LDFLAGS+=-O2 -s
else
CPPFLAGS+=-DDEBUG
CFLAGS+=-O3 -g
CXXFLAGS+=-O3 -g
LDFLAGS+=-O3 -g
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

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "(CXX) $@"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "(CXX) $@"
	@$(CC) $(CFLAGS) $< -c -o $@

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

clean_all:
	@rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*

-include $(OBJ:.o=.d)

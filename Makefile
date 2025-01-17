
BUILD:=debug

# DIRS
PUB:=pub
HTTP:=http
FACTORY:=factory
REGISTER:=register
CONFIG:=config
OPENSSL:=openssl

SRC_DIR:=src
BUILD_DIR:=build
BIN_DIR:=bin

SRC:= \
	$(wildcard $(PUB)/*.cpp) \
	$(wildcard $(PUB)/pub_macro/*.cpp) \
	$(wildcard $(PUB)/pub_log/*.cpp) \
	$(wildcard $(PUB)/stdio/*.cpp) \
	$(wildcard $(PUB)/stl/*.cpp) \
	$(wildcard $(SRC_DIR)/*.cpp) \
	$(wildcard $(SRC_DIR)/$(HTTP)/*.cpp) \
	$(wildcard $(SRC_DIR)/$(FACTORY)/*.cpp) \
	$(wildcard $(SRC_DIR)/$(REGISTER)/*.cpp) \
	$(wildcard $(SRC_DIR)/$(CONFIG)/*.cpp) \
	$(wildcard $(SRC_DIR)/$(OPENSSL)/*.cpp) \
	$(wildcard test_develop/*.cpp) 

OBJ:=$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
BIN:=whps

CPPFLAGS+=-Iinclude \
		-I$(PUB) \
		-I$(PUB)/pub_macro \
		-I$(PUB)/pub_log \
		-I$(PUB)/stdio \
		-I$(PUB)/stl \
		-Iinclude/$(HTTP) \
		-Iinclude/$(FACTORY) \
		-Iinclude/$(REGISTER) \
		-Iinclude/$(CONFIG) \
		-Iinclude/$(OPENSSL) \
		-Itest_develop

CXXFLAGS+=-Wall -pedantic -Wextra -std=c++11 -MMD \
	-Wno-deprecated -Wdeprecated-declarations \
	-Wunused-function \
	-Wno-unused-parameter  # 设置此项，不提示“未使用变量”，正式程序需要去掉 

LDLIBS:=-lssl -lpthread -lrt -ldl

ifeq ($(BUILD), release)
CPPFLAGS+=-DNDEBUG
CFLAGS+=-O2
CXXFLAGS+=-O2
LDFLAGS+=-O2 -s
else
CPPFLAGS+=-D__DEBUG__
CFLAGS+= -O3 -g
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
	@mkdir -p log
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

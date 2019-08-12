
BUILD:=debug

# DIRS
PUB:=pub
HTTP:=http
FACTORY:=factory
REGISTER:=register
CONFIG:=config

SRC_DIR:=src
BUILD_DIR:=build
BIN_DIR:=../bin

SRC:=$(wildcard ./*.cpp) 

OBJ:=$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
BIN:=whps

CPPFLAGS+= -I. -I../include \
		-I../$(PUB) \
		-I../$(PUB)/pub_macro \
		-I../$(PUB)/pub_log \
		-I../$(PUB)/stdio \
		-I../include/$(HTTP) \
		-I../include/$(FACTORY) \
		-I../include/$(REGISTER) \
		-I../include/$(CONFIG) 

CXXFLAGS+=-Wall -pedantic -Wextra -std=c++11 -MMD -D_GLIBCXX_USE_NANOSLEEP \
	-Wno-deprecated -Wdeprecated-declarations \
	-Wunused-function \
	-Wno-unused-parameter  # 设置此项，不提示“未使用变量”，正式程序需要去掉 

LDLIBS:=-L../lib -pthread -lrt -lwhps

ifeq ($(BUILD), release)
CPPFLAGS+=-DNDEBUG
CFLAGS+=-O2
CXXFLAGS+=-O2
LDFLAGS+=-O2 -s
else
CPPFLAGS+=-D__DEBUG__
CFLAGS+=-O3 -g
CXXFLAGS+=-O3 -g
LDFLAGS+=-O3 -g
endif

#.PHONY: all release clean
.PHONY: all debug clean

$(BIN_DIR)/$(BIN):$(OBJ)
	@mkdir -p $(dir $@)
	@echo "(LD) $@"
	@$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -std=c++11 -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "(CXX) $@"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)/$(BIN)

-include $(OBJ:.o=.d)

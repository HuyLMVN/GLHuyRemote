CC:= clang
CXX:= clang++
SRC_DIR:=src
BUILD_DIR:=build
BIN:=bin
TARGET_OUT:=ModView.exe
TARGET:=$(BIN)/$(TARGET_OUT)

SRCS:= $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/**/**/*.cpp $(SRC_DIR)/**/*.c $(SRC_DIR)/**/**/*.c)
OBJS:=$(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_FLAGS := -Iinclude/extern -Iinclude/local -Iinclude/extern/imgui
CPP_FLAGS:= $(INC_FLAGS) -MMD -MP
CXX_FLAGS:= -O3 -std=c++20 -stdlib=libc++ -g -Wall -Wextra -Wpedantic -Wdeprecated
C_FLAGS:= -O3 -std=c17 -g -Wall -Wextra -Wpedantic -Wdeprecated
LD_FLAGS:= ${CXX_FLAGS} ${C_FLAGS} -L./lib -lglfw3 -lassimp -lopengl32 -lkernel32 -lgdi32 -luser32

$(BUILD_DIR)/%.cpp.o: %.cpp
	if not exist "$(subst /,\,$(@D))" mkdir $(subst /,\,$(@D))
	$(CXX) $(CXX_FLAGS) $(CPP_FLAGS) -c $< -o $@ $(INC_FLAGS)
$(BUILD_DIR)/%.c.o: %.c
	if not exist "$(subst /,\,$(@D))" mkdir $(subst /,\,$(@D))
	$(CC) $(C_FLAGS) $(CPP_FLAGS) -c $< -o $@ $(INC_FLAGS) 
$(TARGET): $(OBJS)
	if not exist "$(subst /,\,$(@D))" mkdir $(subst /,\,$(@D))
	$(CXX) $^ -o $@ $(LD_FLAGS)
.PHONY: all
all: $(TARGET)
.PHONY: run
run: all
	$(TARGET)

.PHONY: clean
clean:
	if exist "$(subst /,\,$(BUILD_DIR))" rmdir /s /q $(subst /,\,$(BUILD_DIR))
	if exist "$(subst /,\,$(BIN))" rmdir /s /q $(subst /,\,$(BIN))

-include $(DEPS)

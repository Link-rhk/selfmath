OBJ_DIR=bin
SRC_DIR=src
OUT_DIR=out

SRC= $(wildcard $(SRC_DIR)/*.cpp)
SRC_SRC= $(notdir $(SRC))
OBJ= $(patsubst %.cpp, %.o, $(SRC_SRC))

MAIN_SRC=main.cpp
MAIN_OBJ=main.o

ALL_OBJS=$(OBJ) $(MAIN_OBJ)

TARGET=main_app

CC = g++
CFLAGS += -I./inc

OBJS_WITH_PATH = $(addprefix $(OBJ_DIR)/, $(ALL_OBJS))

ifeq ($(OS),Windows_NT)
	make_dir =powershell -Command "New-Item -ItemType "Directory" -Force -Path"
	remove = powershell -Command "del -r"
	EXE_EXT= .exe
	move = powershell -Command "mv"
else
	make_dir = mkdir -p
	remove = rm -rf
	EXE_EXT = 
	move = rm 
endif

all: init $(OBJ_DIR)/$(TARGET)$(EXE_EXT)

test: $(OBJS_WITH_PATH)

init:
	@echo "creat dir: $(OBJ_DIR)"
	-$(make_dir) $(OBJ_DIR)
	-$(make_dir) $(OUT_DIR)

$(OBJ_DIR)/$(TARGET)$(EXE_EXT): $(OBJS_WITH_PATH)
	@echo "link: $@"
	$(CC) $(CFLAGS) -o $@ $^ -O2 -Werror
	-$(move) $(OBJ_DIR)/$(TARGET)$(EXE_EXT) $(OUT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "compile: $< -> $@"
	$(CC) $(CFLAGS) -c $< -o $@ -O2

$(OBJ_DIR)/$(MAIN_OBJ): $(MAIN_SRC)
	@echo "compile: $< -> $@"
	$(CC) $(CFLAGS) -c $< -o $@ -O2

clean:
	@echo "clean: $(OBJ_DIR)"
	-$(remove) $(OBJ_DIR)
	-$(remove) $(OUT_DIR)

show:
	@echo $(OBJ)
	@echo $(SRC)
	@echo $(ALL_OBJS)
	@echo $(OBJS_WITH_PATH)
.PHONY: all init clean show test
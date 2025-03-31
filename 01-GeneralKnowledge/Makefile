
CUR_DIR := .
BIN_DIR := $(CUR_DIR)/bin
INC_DIR := $(CUR_DIR)/inc
LIB_DIR := $(CUR_DIR)/lib
OBJ_DIR := $(CUR_DIR)/obj
SRC_DIR := $(CUR_DIR)/src
LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared

C_FLAGS = -I $(INC_DIR)

creat_obj:
	gcc -c -fPIC $(SRC_DIR)/HelloMinhY.c    -o $(OBJ_DIR)/HelloMinhY.o -I $(INC_DIR)
	gcc -c -fPIC $(SRC_DIR)/Introduce.c    -o $(OBJ_DIR)/Introduce.o -I $(INC_DIR)
	gcc -c $(SRC_DIR)/main.c    -o $(OBJ_DIR)/main.o -I $(INC_DIR)

creat_staticlib:
	ar rcs $(LIB_STATIC_DIR)/libHello.a $(OBJ_DIR)/HelloMinhY.o $(OBJ_DIR)/Introduce.o 

creat_sharedlib:
	gcc -shared $(OBJ_DIR)/HelloMinhY.o $(OBJ_DIR)/Introduce.o -o $(LIB_SHARED_DIR)/libHello.so

all: creat_obj creat_sharedlib creat_staticlib	
	gcc  $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -lHello -o $(BIN_DIR)/use-shared-library
	gcc   $(OBJ_DIR)/main.o  -L$(LIB_STATIC_DIR) -lHello -o $(BIN_DIR)/statically-linked
	
clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(LIB_SHARED_DIR)/*.so
	rm -rf $(LIB_STATIC_DIR)/*.a
	rm -rf $(BIN_DIR)/use-shared-library
	rm -rf $(BIN_DIR)/statically-linked


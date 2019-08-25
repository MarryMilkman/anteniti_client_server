STAG_DIR = ./../../../staging_dir/toolchain-mipsel_24kc_gcc-5.5.0_musl
CC = gcc
CFLAGS = -Os -Wall -Wmissing-declarations -g3
CPPFLAGS = -std=c++11 -lssh2 -lcurl
LDFLAGS  = -L$(STAG_DIR)/usr/lib -L$(STAG_DIR)/

LDLIBS =

# Name of the program
NAME	:= server_client

# Directories
OBJ_DIR	:= ./obj/
SRC_DIR	:= ./src/
INC_DIR := ./inc/
INC_DIR_CLIENT := ./inc/client_tools/
INC_DIR_SERV := ./inc/server_tools/


# Source and object files
SRC		:= 				main.cpp \
			server_tools/Server.cpp \
			client_tools/Client.cpp \
			TCP_IP_Worker.cpp \
			SSH_Worker.cpp \
			\
			controllers/BroadcastController.cpp \
			controllers/RouterInfoController.cpp \
			controllers/StatusController.cpp \
			controllers/SettingController.cpp \
			controllers/setting_tools/Setting.cpp \
			controllers/CloudController.cpp \
			\
			ScriptExecutor.cpp \
			FlagsHendler.cpp \
			CustomException.cpp \
			RouterData.cpp \
			Timer.cpp \
			Encryptor.cpp \
			Parser.cpp

OBJ		:= $(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))

# Header files
INC	:= lib.h

# Compiler and flags
# CPPFLAGS	:= -std=c++11 -EL -march=24kc -static #-muclibc -mips32



# Compile and link the program
all: obj_dir $(NAME)

# Link object files into the executable
$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME) -I$(INC_DIR) -I $(INC_DIR_SERV) -I $(INC_DIR_CLIENT)

# Compile object files from source files
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ -c $<  -I $(INC_DIR) -I $(INC_DIR_SERV) -I $(INC_DIR_CLIENT)

# Create a directory for object files
obj_dir:
	@mkdir -p $(OBJ_DIR) $(OBJ_DIR)/server_tools $(OBJ_DIR)/client_tools $(OBJ_DIR)/controllers

# Delete object files
clean:
	rm -rf $(OBJ_DIR)

# Delete all files created with the Makefile
fclean: clean
	rm -f $(NAME)

# Re-make all files
re: fclean all

# GNU will run even if files have names as instructions
.PHONY: all clean fclean re

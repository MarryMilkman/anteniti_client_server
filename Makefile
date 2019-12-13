STAG_DIR = /home/user/zborki_for_openwrt/openwrt-ad7b64e/staging_dir
CC = gcc
CFLAGS = -Os -Wall -Wmissing-declarations -g3 -g
CPPFLAGS = -std=c++11 -lssh2 -lcurl -ljson-c
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
INC_DIR_ENTITY := ./inc/entity/



# Source and object files
SRC		:= 	main.cpp \
			Constant.cpp \
			server_tools/Server.cpp \
			client_tools/Client.cpp \
			\
			controllers/BroadcastController.cpp \
			controllers/RouterInfoController.cpp \
			controllers/info_tools/DeviceInfo.cpp \
			controllers/info_tools/ConnectedDeviceInfo.cpp \
			controllers/StatusController.cpp \
			controllers/SettingController.cpp \
			controllers/setting_tools/Setting.cpp \
			controllers/CloudController.cpp \
			controllers/NotificationController.cpp \
			controllers/SSHTunnelController.cpp \
			controllers/AccessController.cpp	\
			controllers/ConnectionController.cpp	\
			\
			entity/TCP_IP_Worker.cpp \
			entity/SSH_Worker.cpp \
			entity/RouterData.cpp \
			entity/Timer.cpp \
			entity/AskingEntity.cpp \
			entity/Network.cpp \
			entity/EventConnect.cpp \
			\
			ScriptExecutor.cpp \
			FlagsHendler.cpp \
			CustomException.cpp \
			Encryptor.cpp \
			Parser.cpp \
			Informer.cpp \


OBJ		:= $(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))

# Header files
INC	:= lib.h

# Compiler and flags
# CPPFLAGS	:= -std=c++11 -EL -march=24kc -static #-muclibc -mips32



# Compile and link the program
all: obj_dir $(NAME)

# Link object files into the executable
$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME) -I$(INC_DIR) -I $(INC_DIR_SERV) -I $(INC_DIR_CLIENT) -I$(STAG_DIR)/host/include/json-c -I $(INC_DIR_ENTITY)

# Compile object files from source files
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ -c $<  -I $(INC_DIR) -I $(INC_DIR_SERV) -I $(INC_DIR_CLIENT) -I$(STAG_DIR)/host/include/json-c -I $(INC_DIR_ENTITY)

# Create a directory for object files
obj_dir:
	@mkdir -p $(OBJ_DIR) $(OBJ_DIR)/server_tools $(OBJ_DIR)/client_tools $(OBJ_DIR)/controllers $(OBJ_DIR)/controllers/setting_tools $(OBJ_DIR)/controllers/info_tools $(OBJ_DIR)/entity

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

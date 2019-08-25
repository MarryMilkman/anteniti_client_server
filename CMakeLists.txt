cmake_minimum_required(VERSION 3.10.2)

project(MyProject)

add_subdirectory(libssh2-1.8.0)
add_subdirectory(curl-7.65.3)

link_libraries(libssh2)
link_libraries(libcurl)

include_directories (
	../../kinkan/package/server_client/server_client_files/inc
	../../kinkan/package/server_client/server_client_files/inc/client_tools
	../../kinkan/package/server_client/server_client_files/inc/server_tools
	libssh2-1.8.0/include
)

set(SRC_DIRECTORY "/home/user/kinkan/package/server_client/server_client_files/src")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11")


set(SRC
		${SRC_DIRECTORY}/server_tools/Server.cpp
		${SRC_DIRECTORY}/client_tools/Client.cpp

		${SRC_DIRECTORY}/controllers/BroadcastController.cpp
		${SRC_DIRECTORY}/controllers/RouterInfoController.cpp
		${SRC_DIRECTORY}/controllers/StatusController.cpp
		${SRC_DIRECTORY}/controllers/SettingController.cpp
		${SRC_DIRECTORY}/controllers/setting_tools/Setting.cpp

		${SRC_DIRECTORY}/controllers/CloudController.cpp

		${SRC_DIRECTORY}/RouterData.cpp
		${SRC_DIRECTORY}/main.cpp
		${SRC_DIRECTORY}/Timer.cpp

		${SRC_DIRECTORY}/ScriptExecutor.cpp
		${SRC_DIRECTORY}/FlagsHendler.cpp
		${SRC_DIRECTORY}/CustomException.cpp

		${SRC_DIRECTORY}/SSH_Worker.cpp
		${SRC_DIRECTORY}/TCP_IP_Worker.cpp

		${SRC_DIRECTORY}/Encryptor.cpp
		${SRC_DIRECTORY}/Parser.cpp
	)


add_executable(server_client ${SRC})

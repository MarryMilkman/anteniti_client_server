cmake_minimum_required(VERSION 3.5.1)

project(MyProject)

add_subdirectory(libssh2-1.8.0)
add_subdirectory(curl-7.65.3)
add_subdirectory(json-c)

link_libraries(libssh2)
link_libraries(libcurl)
link_libraries(json-c)

include_directories (
	./inc
	./inc/client_tools
	./inc/server_tools
	libssh2-1.8.0/include
	json-c
)

set(SRC_DIRECTORY "./src")

SET(CMAKE_CXX_COMPILER g++-5)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11 -g -lssl")


set(SRC
		${SRC_DIRECTORY}/Constant.cpp
		${SRC_DIRECTORY}/server_tools/Server.cpp
		${SRC_DIRECTORY}/client_tools/Client.cpp

		${SRC_DIRECTORY}/controllers/SSHTunnelController.cpp
		${SRC_DIRECTORY}/controllers/BroadcastController.cpp
		${SRC_DIRECTORY}/controllers/StatusController.cpp
		${SRC_DIRECTORY}/controllers/CloudController.cpp
		${SRC_DIRECTORY}/controllers/NotificationController.cpp
		${SRC_DIRECTORY}/controllers/ConnectionController.cpp
		${SRC_DIRECTORY}/controllers/AccessController.cpp

		${SRC_DIRECTORY}/controllers/SettingController.cpp
		${SRC_DIRECTORY}/controllers/setting_tools/Setting.cpp

		${SRC_DIRECTORY}/controllers/RouterInfoController.cpp
		${SRC_DIRECTORY}/controllers/info_tools/DeviceInfo.cpp
		${SRC_DIRECTORY}/controllers/info_tools/ConnectedDeviceInfo.cpp


		${SRC_DIRECTORY}/RouterData.cpp
		${SRC_DIRECTORY}/main.cpp
		${SRC_DIRECTORY}/Timer.cpp
		${SRC_DIRECTORY}/EventConnect.cpp

		${SRC_DIRECTORY}/ScriptExecutor.cpp
		${SRC_DIRECTORY}/FlagsHendler.cpp
		${SRC_DIRECTORY}/CustomException.cpp

		${SRC_DIRECTORY}/SSH_Worker.cpp
		${SRC_DIRECTORY}/TCP_IP_Worker.cpp
		${SRC_DIRECTORY}/Network.cpp

		${SRC_DIRECTORY}/Encryptor.cpp
		${SRC_DIRECTORY}/Parser.cpp

		${SRC_DIRECTORY}/Mutex.cpp
	)


add_executable(server_client ${SRC})

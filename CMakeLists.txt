cmake_minimum_required(VERSION 3.11)
project(Zia VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")

add_executable(zia
./bref/HttpConstants.h
./bref/HttpRequest.h
./bref/HttpHeader.h
./bref/HttpResponse.h
./config/ConfigManager.cpp
./config/ConfigManager.hpp
./config/Module.cpp
./config/Module.hpp
./config/ModuleManager.cpp
./config/ModuleManager.hpp
./execution_cmd/Execute.cpp
./execution_cmd/Execute.hpp
./network/Client.cpp
./network/Client.hpp
./network/main.cpp
./network/Server.cpp
./network/Server.hpp
./network/HttpRequest.cpp
./network/HttpResponse.cpp
./thread/IThread.hpp
./zia/Zia.cpp
./zia/Zia.hpp
)

target_link_libraries(zia PRIVATE ${CONAN_LIBS} -pthread -lstdc++fs --std=c++17)
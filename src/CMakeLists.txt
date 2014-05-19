# cmakelists file

cmake_minimum_required(VERSION 2.6)

project (hero-server)

SET(CMAKE_CXX_FLAGS_DEBUG "-g -fno-strict-aliasing -ggdb -Wall -Wl,--hash-style=sysv -Wno-deprecated -Wno-parentheses  -Wno-write-strings")
SET(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -fno-strict-aliasing -ggdb -Wall -Wl,--hash-style=sysv")

include_directories ("/usr/local/include" "/usr/local/mysql/include"
"/usr/include/mysql")

link_directories ("/usr/local/lib" "/usr/local/mysql/lib" "/usr/lib64/mysql/")

add_subdirectory (common)
add_subdirectory (class)
add_subdirectory (net)
add_subdirectory (event)
add_subdirectory (utils)
add_subdirectory (game)

add_executable(game.e game/main.cpp)

target_link_libraries(game.e game common event net class utils log4cxx
mysqlclient config++ curl)


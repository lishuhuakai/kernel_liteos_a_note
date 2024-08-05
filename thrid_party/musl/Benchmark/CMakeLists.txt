cmake_minimum_required(VERSION 3.21)
project(musl_benchmark)

set(TARGET_NAME musl_benchmark)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++17")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --verbose")

set(benchmark_DIR ../../third_party_benchmark/build)
find_package(benchmark REQUIRED)

#add_subdirectory(Benchmark/musl)
#include(Benchmark/musl)

if (APPLE)
message(STATUS "In apple system build...")
else()
message(FATAL_ERROR "Only for apple system build...")
endif()
set(EXCLUDE_FILES 
	libc_eventfd.cpp
	libc_inotify.cpp
	libc_ioctl.cpp
	libc_pthread.cpp
	libc_random.cpp
	libc_sched.cpp
	libc_signal.cpp
	libc_syscall.cpp
	libc_sysinfo.cpp
	libc_time.cpp
	libc_unistd.cpp
	libc_dlopen.cpp
	socket_test.cpp
	libc_epoll.cpp)

file(GLOB SOURCE_FILES musl/*.cpp)
foreach(IT IN LISTS EXCLUDE_FILES)
	list(FILTER SOURCE_FILES EXCLUDE REGEX "${IT}")
endforeach()
message(WARNING "glob source files: ${SOURCE_FILES}")

add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_include_directories(${TARGET_NAME}
	AFTER
	PRIVATE ./musl
	PRIVATE ../../third_party_cjson)

target_link_libraries(${TARGET_NAME}
	PRIVATE
	benchmark::benchmark
	${CMAKE_CURRENT_SOURCE_DIR}/../../third_party_cJSON/build/libcjson.dylib)

add_executable(musl_dlopen musl/libc_dlopen.cpp)

target_link_libraries(musl_dlopen
	PRIVATE
	benchmark::benchmark)

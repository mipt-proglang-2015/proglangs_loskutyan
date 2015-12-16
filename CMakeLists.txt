cmake_minimum_required(VERSION 3.3)

set(PYTHON_LIBRARIES "/Library/Frameworks/Python.framework/Versions/3.4/lib/libpython3.4m.dylib")
set(PYTHON_INCLUDE_DIR "/Library/Frameworks/Python.framework/Versions/3.4/include/python3.4m")

set(Python_ADDITIONAL_VERSIONS 3.4)
find_package(PythonLibs 3.4 REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})

python_add_module(fw fw.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

target_link_libraries(fw ${PYTHON_LIBRARIES})
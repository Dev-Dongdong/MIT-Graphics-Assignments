cmake_minimum_required (VERSION 3.0) #最低要求的CMake版本

project(Assignment4) # 项目名称

aux_source_directory (./src/ DIR) # '.'代表当前文件夹 这个指令将遍历当前文件夹下的所有源代码，将它作为一个 list存入变量‘DIR’中

set(IDX raytracer) #定义一个变量‘IDX’，它的值为文件夹的名字，1


link_directories(${PROJECT_SOURCE_DIR}/lib)         # 库目录, -L
include_directories(${PROJECT_SOURCE_DIR}/include)   # 头文件目录, -I

add_executable (${IDX} ${DIR}) #在整个工程里添加一个名为${IDX}的可执行文件，可执行文件的源代码是${DIR}

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

target_link_libraries(${IDX} ${OPENGL_LIBRARIES} glut32)

set(CMAKE_CXX_STANDARD 17) #配置C++ Standard

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}) #配置可执行文件的输出路径

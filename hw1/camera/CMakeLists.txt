cmake_minimum_required (VERSION 3.0)
project (camera)

find_package(OpenGL REQUIRED)

set(ALL_LIBS
	${OPENGL_LIBRARY}
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(camera
    camera.cpp
    ../lib/shader.cpp

    vertex1.vertexshader
    fragment1.fragmentshader    
    vertex2.vertexshader
    fragment2.fragmentshader    
)
target_link_libraries(camera
	${ALL_LIBS}
    -lglfw
    -lGLEW
)
add_custom_command(
    TARGET camera POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "camera" "../camera"
    )


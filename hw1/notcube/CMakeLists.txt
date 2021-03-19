cmake_minimum_required (VERSION 3.0)
project (notcube)

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

add_executable(notcube
    notcube.cpp
    ../lib/shader.cpp

    vertex.vertexshader
    fragment.fragmentshader    
)
target_link_libraries(notcube
	${ALL_LIBS}
    -lglfw
    -lGLEW
)
add_custom_command(
    TARGET notcube POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "notcube" "../notcube"
    )


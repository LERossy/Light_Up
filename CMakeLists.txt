cmake_minimum_required(VERSION 2.8)
project(opengl_w)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)


include_directories(${OPENGL_INCLUDE_DIR} glfw /home/aelhossini/Laila/OpenGL/glad/include /home/aelhossini/Laila/OpenGL/freetype-2.4.0/include ${ASSIMP_INCLUDE_DIR})
link_directories(/home/aelhossini/Laila/OpenGL/freetype-2.4.0/objs)

add_executable(lightup Light_Up.cpp /home/aelhossini/Laila/OpenGL/glad/src/glad.c shader.cpp Texture.cpp Resource_Manager.cpp Game.cpp Object_Renderer.cpp Icon_Renderer.cpp Camera.h Text_Renderer.cpp stb_image.cpp)


target_link_libraries(lightup ${OPENGL_gl_LIBRARY} glfw dl libfreetype.a)


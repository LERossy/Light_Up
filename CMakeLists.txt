cmake_minimum_required(VERSION 3.22)
project(opengl_w)

cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0012 NEW)

set (CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)
#find_package(glfw3 REQUIRED)
#find_package(assimp REQUIRED)

include(FetchContent)

# 2.1
FetchContent_Declare(
    glm
    GIT_REPOSITORY  https://github.com/g-truc/glm.git
    GIT_TAG         origin/master
)

# 2.2
FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif ()


# 3.
FetchContent_Declare(
    freetype
    GIT_REPOSITORY  https://github.com/freetype/freetype.git
    GIT_TAG         origin/master
)

# 4.
FetchContent_Declare(
    assimp
    GIT_REPOSITORY  https://github.com/assimp/assimp.git
    GIT_TAG         origin/master
)

# 5.
FetchContent_Declare(
    glfw
    GIT_REPOSITORY  https://github.com/glfw/glfw.git
    GIT_TAG         origin/master
)

# 6.
FetchContent_MakeAvailable(freetype assimp glfw)


include_directories(${OPENGL_INCLUDE_DIR} glfw glad/include freetype ${ASSIMP_INCLUDE_DIR} ${glm_SOURCE_DIR})
#link_directories(/home/aelhossini/Laila/OpenGL/freetype-2.4.0/objs)

add_executable(lightup Light_Up.cpp glad/src/glad.c shader.cpp Texture.cpp Resource_Manager.cpp Game.cpp Object_Renderer.cpp Icon_Renderer.cpp Camera.h Text_Renderer.cpp stb_image.cpp)



IF (WIN32)

  set_target_properties(
    lightup PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

  get_property(FREETYPE_BINARY_DIR TARGET freetype PROPERTY BINARY_DIR)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT image)

  IF(MSVC_TOOLSET_VERSION)
    SET(MSVC_PREFIX "vc${MSVC_TOOLSET_VERSION}")
    SET(ASSIMP_MSVC_VERSION ${MSVC_PREFIX})
  ELSE()
    IF(MSVC12)
      SET(ASSIMP_MSVC_VERSION "vc120")
    ELSEIF(MSVC14)
      SET(ASSIMP_MSVC_VERSION "vc140")
    ELSEIF(MSVC15)
      SET(ASSIMP_MSVC_VERSION "vc141")
    ELSEIF(MSV142)
      SET(ASSIMP_MSVC_VERSION "vc142")
    ELSEIF(MSVC143)
      SET(ASSIMP_MSVC_VERSION "vc143")
    ENDIF()
  ENDIF()
  
  set(CONFIG_TYPE $<IF:$<CONFIG:Debug>,Debug,Release>)
  set(DEBUG_SYM $<IF:$<CONFIG:Debug>,d,>)
  add_custom_command(TARGET lightup POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSIMP_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG_TYPE}/assimp-${ASSIMP_MSVC_VERSION}-mt${DEBUG_SYM}.dll 	${CMAKE_BINARY_DIR}/${CONFIG_TYPE}/assimp-${ASSIMP_MSVC_VERSION}-mt${DEBUG_SYM}.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLFW_BINARY_DIR}/src/${CONFIG_TYPE}/glfw3${DEBUG_SYM}.dll 	${CMAKE_BINARY_DIR}/${CONFIG_TYPE}/glfw3${DEBUG_SYM}.dll          
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FREETYPE_BINARY_DIR}/${CONFIG_TYPE}/freetype${DEBUG_SYM}.dll 	${CMAKE_BINARY_DIR}/${CONFIG_TYPE}/freetype${DEBUG_SYM}.dll          
        COMMAND_EXPAND_LISTS
    )
  

  # set stuff for windows
  target_link_libraries(lightup ${OPENGL_gl_LIBRARY} freetype glfw assimp)
ELSE()
  # set stuff for other systems
  target_link_libraries(lightup ${OPENGL_gl_LIBRARY} freetype glfw dl assimp)
ENDIF()



#target_link_libraries(lightup ${OPENGL_gl_LIBRARY} glfw dl libfreetype.a)


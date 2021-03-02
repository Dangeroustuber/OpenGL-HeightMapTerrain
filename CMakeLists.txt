
#Indicate the minimum version of CMake needed to use
#this CMakeLists.txt file.
cmake_minimum_required(VERSION 3.12)

# Declare the name of the project.
project(Pacman3D)

# Ask CMake to find the OpenGL package
find_package(OpenGL REQUIRED)

# You can enable this, MSVC for some reason doesn't care about this but clang,gcc and others do.
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") 
 
#https://stackoverflow.com/questions/45955272/modern-way-to-set-compiler-flags-in-cross-platform-cmake-project
set(COMPILER_WARNINGS_AND_ERRORS
	$<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/w44061>
    $<$<CXX_COMPILER_ID:MSVC>:/w44062>
    $<$<CXX_COMPILER_ID:MSVC>:/w44265> 
    $<$<CXX_COMPILER_ID:MSVC>:/permissive->)

# Set a global output directory for libraries and runtime
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Instructs CMake to get into the external
add_subdirectory(external/glfw-3.3.2)
add_subdirectory(external/stb_libraries)
add_subdirectory(external/glew-cmake-2.1.0)
add_subdirectory(external/glm)
add_subdirectory(external/assimp)
add_subdirectory(external/openal-soft-1.21.0)
add_subdirectory(LibCore)
add_subdirectory(RenderModule)
add_subdirectory(MainModule)

 

# Add a new executable to our project
#add_executable(${PROJECT_NAME}
#	"MainModule/Pacman3D/main.cpp"
#	"MainModule/Pacman3D/includes/Camera.cpp" 
#	"MainModule/Pacman3D/includes/Ghost.cpp" 
#	"MainModule/Pacman3D/includes/Map.cpp"
#	"MainModule/Pacman3D/includes/Game.cpp"
#	"MainModule/Pacman3D/includes/Pellets.cpp"
#	"external/stb_libraries/dr_wav_impl.c"
#	"external/stb_libraries/stb_image.cpp"  "MainModule/Helloworld/helloworld.cpp")


#target_compile_definitions(Pacman3D PRIVATE GLEW_STATIC)

#target_link_libraries(Pacman3D
 # PRIVATE
#  libglew_static
 # glfw
 # glm
 # assimp
#  OpenAL
#  dr_waw
#  stb_image
#  LibCore
#  RenderModule
 # OpenGL::GL)

#target_include_directories(Pacman3D PRIVATE include LibCore/includes RenderModule/includes)

#add_custom_command(
 # TARGET ${PROJECT_NAME} POST_BUILD
 # COMMAND ${CMAKE_COMMAND} -E copy_directory
 # ${CMAKE_CURRENT_SOURCE_DIR}/assets
 # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

#add_custom_command(
#  TARGET ${PROJECT_NAME} POST_BUILD
#  COMMAND ${CMAKE_COMMAND} -E copy_directory
 # ${CMAKE_CURRENT_SOURCE_DIR}/sounds
 # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sounds)




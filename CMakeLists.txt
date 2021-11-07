cmake_minimum_required(VERSION 3.16.3)
project (Tilemap)

IF (WIN32)
	find_package(SDL2IMAGE REQUIRED)
ELSEIF(EMSCRIPTEN)
	set(SDL2_INCLUDE_DIRS "${EMSCRIPTEN_ROOT_PATH}/system/include" "${EMSCRIPTEN_ROOT_PATH}/system/include/SDL2/")
	set(CMAKE_CXX_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' --preload-file tiles.png --preload-file map.txt --shell-file ../emscripten.html")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
ELSE()
	find_package(PkgConfig REQUIRED)
	PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
ENDIF()

IF (NOT EMSCRIPTEN)
	find_package(SDL2 REQUIRED)
	include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
ENDIF()

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

add_subdirectory(src)

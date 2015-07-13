# File: CMakeLists.txt
# Date: 22.08.2010
# Plik konfiguracyjny cmake

# minimalna wersja cmake
cmake_minimum_required(VERSION 2.6)

# Nazwa projektu
project(p3engine C CXX)

find_package(Freetype REQUIRED)
find_package(SDL REQUIRED)

# Dodatkowe biblioteki
if(WIN32)
    set(LIBS ${LIBS} "opengl32" "glew32")
else()
    set(LIBS ${LIBS} "GL" "GLEW")
endif()

# Lista plikow zrodlowych projektu
set(
  HEADER_FILES
  "${p3engine_SOURCE_DIR}/src/gf/common.hpp"
  "${p3engine_SOURCE_DIR}/src/gf/ilogger.hpp"
  "${p3engine_SOURCE_DIR}/src/gf/logger.hpp"
  "${p3engine_SOURCE_DIR}/src/gf/iresource.hpp"
  "${p3engine_SOURCE_DIR}/src/gf/resource.hpp"
  "${p3engine_SOURCE_DIR}/src/gf/idevice.hpp"
  "${p3engine_SOURCE_DIR}/src/gf/device.hpp"
  "${p3engine_SOURCE_DIR}/src/gf/iscene.hpp"
  "${p3engine_SOURCE_DIR}/src/gf/scene.hpp"
  "${p3engine_SOURCE_DIR}/src/pf/collision.hpp"
  "${p3engine_SOURCE_DIR}/src/pf/icollision.hpp"
  "${p3engine_SOURCE_DIR}/src/application.hpp"
  "${p3engine_SOURCE_DIR}/src/example.hpp"
)

# Lista plikow zrodlowych projektu
set( 
  SOURCE_FILES
  "${p3engine_SOURCE_DIR}/src/gf/common.cpp"
  "${p3engine_SOURCE_DIR}/src/gf/logger.cpp"
  "${p3engine_SOURCE_DIR}/src/gf/resource.cpp"
  "${p3engine_SOURCE_DIR}/src/gf/device.cpp"
  "${p3engine_SOURCE_DIR}/src/gf/scene.cpp"
  "${p3engine_SOURCE_DIR}/src/pf/collision.cpp"
  "${p3engine_SOURCE_DIR}/src/application.cpp"
  "${p3engine_SOURCE_DIR}/src/example.cpp"
  "${p3engine_SOURCE_DIR}/src/main.cpp"
)

# Plik wykonywalny
add_executable(example ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(example PROPERTIES RUNTIME_OUTPUT_NAME "example")
target_include_directories(example PRIVATE ${FREETYPE_INCLUDE_DIRS} ${SDL_INCLUDE_DIRS})
target_link_libraries(example ${FREETYPE_LIBRARY} ${SDL_LIBRARY} ${LIBS})



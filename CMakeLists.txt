cmake_minimum_required(VERSION 3.0)

set(APP_NAME "SolSystemLib")
project(${APP_NAME})

set(HEADER_PATH include)
set(SOURCE_PATH src)
get_filename_component(src_abs ${SOURCE_PATH} ABSOLUTE)

set(SOURCE_FILES
   include/Sphere.h src/Sphere.cpp
)

find_package(glm CONFIG REQUIRED)

SET (BUILD_SHARED_LIBS TRUE)

add_library(${APP_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${APP_NAME} PRIVATE glm::glm)

set_target_properties(${APP_NAME} PROPERTIES
    VERSION 1
    SOVERSION 1
    EXPORT_NAME ${APP_NAME}
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)
set(IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ${APP_NAME} EXPORT ${APP_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    SolSystemLibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SolSystemLibConfig.cmake
    INSTALL_DESTINATION lib/cmake/SolSystemLib
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SolSystemLibConfig.cmake
    DESTINATION lib/cmake/SolSystemLib
)
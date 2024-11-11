# To learn more about CMake
# https://cmake.org/
# https://cmake.org/getting-started/ 

cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME RT)
# they should be above add_excutable()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-framework OpenCL")

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME}
    main.cpp
    common.h
    color.h
    ray.h
    hittable.h
    hittable_list.h
    sphere.h
    interval.h
    camera.h
    material.h
    aabb.h
    texture.h
    rtw_stb_image.h
    perlin.h
    quad.h
    )

target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})

# ExternalProject 관련 명령어 셋 추가
include(ExternalProject)

# Dependency 관련 변수 설정
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
# set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

# glm
ExternalProject_Add(
    dep_glm
    GIT_REPOSITORY "https://github.com/g-truc/glm"
    GIT_TAG "0.9.9.8"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_BINARY_DIR}/dep_glm-prefix/src/dep_glm/glm
        ${DEP_INSTALL_DIR}/include/glm
    )
set(DEP_LIST ${DEP_LIST} dep_glm)

# stb_image
ExternalProject_Add(
    dep_stb
    GIT_REPOSITORY "https://github.com/nothings/stb"
    GIT_TAG "master"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_BINARY_DIR}/dep_stb-prefix/src/dep_stb/stb_image.h
        ${DEP_INSTALL_DIR}/include/stb/stb_image.h
    )
set(DEP_LIST ${DEP_LIST} dep_stb)

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})

#
# configure : cmake -Bbuild . -DCMAKE_BUILD_TYPE=[Debug|Release]
#
# build : cmake --build build --config Debug
#
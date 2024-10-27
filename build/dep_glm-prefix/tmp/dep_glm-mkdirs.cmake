# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm"
  "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-build"
  "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix"
  "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/tmp"
  "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp"
  "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src"
  "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp${cfgdir}") # cfgdir has leading slash
endif()

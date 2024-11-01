# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp/dep_glm-gitclone-lastrun.txt" AND EXISTS "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp/dep_glm-gitinfo.txt" AND
  "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp/dep_glm-gitclone-lastrun.txt" IS_NEWER_THAN "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp/dep_glm-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp/dep_glm-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"
            clone --no-checkout --depth 1 --no-single-branch --config "advice.detachedHead=false" "https://github.com/g-truc/glm" "dep_glm"
    WORKING_DIRECTORY "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/g-truc/glm'")
endif()

execute_process(
  COMMAND "/usr/bin/git"
          checkout "0.9.9.8" --
  WORKING_DIRECTORY "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '0.9.9.8'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp/dep_glm-gitinfo.txt" "/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp/dep_glm-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/gyeongjukim/my_git/raytracing/build/dep_glm-prefix/src/dep_glm-stamp/dep_glm-gitclone-lastrun.txt'")
endif()

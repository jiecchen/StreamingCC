cmake_minimum_required(VERSION 2.8.7)
project(StreamingCC)
set(CMAKE_BUILD_TYPE Release)

#Specify C++11 flag
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


#Bring the headers, such as Student.h into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")
file(GLOB INCLUDES "include/streamcc_headers/*")
#Generate the shared library from the sources
add_library(streamcc STATIC ${SOURCES})



#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
install(TARGETS streamcc DESTINATION /usr/lib)
install(FILES include/streamcc DESTINATION /usr/local/include)
install(FILES ${INCLUDES} DESTINATION /usr/local/include/streamcc_headers)








#Setup CMake to run tests
enable_testing()

#Prep ourselves for compiling boost
find_package(Boost COMPONENTS unit_test_framework)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})

  #I like to keep test files in a separate source directory called test
  file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp)

  #Run through each source
  foreach(testSrc ${TEST_SRCS})
    #Extract the filename without an extension (NAME_WE)
    get_filename_component(testName ${testSrc} NAME_WE)

    #Add compile target
    add_executable(${testName} ${testSrc})

    #link to Boost libraries AND your targets and dependencies
    target_link_libraries(${testName} ${Boost_LIBRARIES} "streamcc")

    #I like to move testing binaries into a testBin directory
    set_target_properties(${testName} PROPERTIES 
      RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
    
    #Finally add it to test execution - 
    #Notice the WORKING_DIRECTORY and COMMAND
    add_test(NAME ${testName} 
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin 
      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName} )
  endforeach(testSrc)
endif()
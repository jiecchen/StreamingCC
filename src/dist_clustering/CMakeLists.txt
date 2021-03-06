cmake_minimum_required(VERSION 2.8.7)
project(dist)
set(CMAKE_BUILD_TYPE Release)




#Specify C++11 flag
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O3")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "algorithms/*.cc" "experiments/*.cc")
file(GLOB INCLUDES "algorithms/*.h" "experiments/*.h")
#Generate the shared library from the sources
add_library(algorithms STATIC ${SOURCES})
target_link_libraries(algorithms -lblas -llapack)

#I like to keep test files in a separate source directory called test
file(GLOB TEST_SRCS "tests/*.cc")
file(GLOB MPI_SRCS "boost_mpi/*.cc")



#Run through each source
foreach(testSrc ${TEST_SRCS})
  #Extract the filename without an extension (NAME_WE)
  get_filename_component(testName ${testSrc} NAME_WE)

  #Add compile target
  add_executable(${testName} ${testSrc})

  #link to Boost libraries AND your targets and dependencies
  target_link_libraries(${testName} "algorithms")

  #I like to move testing binaries into a testBin directory
  set_target_properties(${testName} PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
    
  #Finally add it to test execution - 
  #Notice the WORKING_DIRECTORY and COMMAND
  add_test(NAME ${testName} 
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin 
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName} )
endforeach(testSrc)


find_package ( MPI )
if ( MPI_FOUND )
   set(CMAKE_CXX_COMPILER "mpic++")
   include_directories(
	./usr/include
   )

   link_directories(
	./usr/lib
   )

foreach(testSrc ${MPI_SRCS})
  #Extract the filename without an extension (NAME_WE)
  get_filename_component(testName ${testSrc} NAME_WE)

  #Add compile target
  add_executable(${testName} ${testSrc})

  #link to Boost libraries AND your targets and dependencies
  target_link_libraries(${testName} "algorithms" -lboost_serialization -lboost_mpi -lblas -llapack)

  #I like to move testing binaries into a testBin directory
  set_target_properties(${testName} PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
    
  #Finally add it to test execution - 
  #Notice the WORKING_DIRECTORY and COMMAND
  add_test(NAME ${testName} 
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin 
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName} )
endforeach(testSrc)

endif( MPI_FOUND )

cmake_minimum_required(VERSION 2.8)

set (INCLUDE_DIR ../include ../libStats ../wordsBuffer)
include_directories(${INCLUDE_DIR}  /usr/local/lib ${HDF5_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${YAMLCPP_INCLUDE_DIR})

add_executable( bio-hist bio-hist.cpp HDF5HistWriter.cpp ProblemConfig.cpp SignalProcessor.cpp SpikesObserver.cpp ErrorFilter.cpp BitsObserver.cpp)
target_link_libraries( bio-hist ${HDF5_LIBRARIES} ${YAMLCPP_LIBRARY} wordsBuffer libStats m)

add_executable( bio-transitions bio-transitions.cpp HDF5HistWriter.cpp ProblemConfig.cpp SignalProcessor.cpp SpikesObserver.cpp ErrorFilter.cpp BitsObserver.cpp)
target_link_libraries( bio-transitions ${HDF5_LIBRARIES} ${YAMLCPP_LIBRARY} wordsBuffer libStats m)

cmake_minimum_required(VERSION 2.8)

set (INCLUDE_DIR ../include ../libStats ../wordsBuffer)
include_directories(${INCLUDE_DIR}  /usr/local/lib ${HDF5_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${YAMLCPP_INCLUDE_DIR})

add_executable( PezHist pezHist.cpp HDF5HistWriter.cpp ProblemConfig.cpp SignalProcessor.cpp SpikesObserver.cpp ErrorFilter.cpp BitsObserver.cpp)
target_link_libraries( PezHist ${HDF5_LIBRARIES} ${YAMLCPP_LIBRARY} wordsBuffer libStats m)

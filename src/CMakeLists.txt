cmake_minimum_required(VERSION 2.8)

set (INCLUDE_DIR ../include)
include_directories(${INCLUDE_DIR} ${INCLUDE_DIR}/libStats /usr/local/lib ${HDF5_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${YAMLCPP_INCLUDE_DIR})

add_executable( PezHist pezHist.cpp HDF5HistWriter.cpp ProblemConfig.cpp SignalProcessor.cpp SpikesObserver.cpp ErrorFilter.cpp)
target_link_libraries( PezHist ${HDF5_LIBRARIES} ${STATS} ${YAMLCPP_LIBRARY} binWordsBuffer m)

#add_executable( EntropyTempCalc entropyTempCalc.cpp ) 
#target_link_libraries( PezHist ${HDF5_LIBRARIES} ${STATS} ${YAMLCPP_LIBRARY} binWordsBuffer m)

add_executable( TransitionMatrix transitionMatrix.cpp HDF5HistWriter.cpp ProblemConfig.cpp SignalProcessor.cpp SpikesObserver.cpp ErrorFilter.cpp)
target_link_libraries( TransitionMatrix ${HDF5_LIBRARIES} ${STATS} ${YAMLCPP_LIBRARY} binWordsBuffer m)

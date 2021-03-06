cmake_minimum_required(VERSION 2.8)

set (INCLUDE_DIR ../include ../libStats ../wordsBuffer)
find_package(Sanitizers)

#-- BIO-BIN-HIST -----------------
add_executable( ${PROJECT_NAME}-hist 
    bio-hist.cpp HDF5HistWriter.cpp ProblemConfig.cpp SignalProcessor.cpp SpikesObserver.cpp ErrorFilter.cpp BitsObserver.cpp)
#add_sanitizers(${PROJECT_NAME}-hist)
target_link_libraries( ${PROJECT_NAME}-hist
    ${HDF5_LIBRARIES} ${YAMLCPP_LIBRARY} wordsBuffer libStats m)
target_include_directories(${PROJECT_NAME}-hist PUBLIC
    ${INCLUDE_DIR}  /usr/local/lib ${HDF5_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${YAMLCPP_INCLUDE_DIR})

if (MSVC)
  target_compile_options(${PROJECT_NAME}-hist PRIVATE "/IW4")
  target_compile_options(${PROJECT_NAME}-hist PRIVATE "/WX")
else()
  target_compile_options(${PROJECT_NAME}-hist PRIVATE "-Wall")
  target_compile_options(${PROJECT_NAME}-hist PRIVATE "-Wextra")
  target_compile_options(${PROJECT_NAME}-hist PRIVATE "-Werror")
  target_compile_options(${PROJECT_NAME}-hist PRIVATE "-pedantic")
endif()
#---------------------------------

#-- BIO-BIN-TRANSITIONS ----------
add_executable( ${PROJECT_NAME}-transitions 
    bio-transitions.cpp HDF5HistWriter.cpp ProblemConfig.cpp SignalProcessor.cpp SpikesObserver.cpp ErrorFilter.cpp BitsObserver.cpp)
#add_sanitizers(${PROJECT_NAME}-transitions)
target_link_libraries( ${PROJECT_NAME}-transitions 
    ${HDF5_LIBRARIES} ${YAMLCPP_LIBRARY} wordsBuffer libStats m)
target_include_directories(${PROJECT_NAME}-transitions PUBLIC
    ${INCLUDE_DIR}  /usr/local/lib ${HDF5_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${YAMLCPP_INCLUDE_DIR})

if (MSVC)
  target_compile_options(${PROJECT_NAME}-transitions PRIVATE "/IW4")
  target_compile_options(${PROJECT_NAME}-transitions PRIVATE "/WX")
else()
  target_compile_options(${PROJECT_NAME}-transitions PRIVATE "-Wall")
  target_compile_options(${PROJECT_NAME}-transitions PRIVATE "-Wextra")
  target_compile_options(${PROJECT_NAME}-transitions PRIVATE "-Werror")
  target_compile_options(${PROJECT_NAME}-transitions PRIVATE "-pedantic")
endif()
#---------------------------------


option(BUILD_TEST "Build a brute force implementation to test against")

if(BUILD_TEST)
  add_executable(bruteforcesolver bruteforcesolver.cpp)
  target_link_libraries(bruteforcesolver ${EXTRA_LIBS})
endif()

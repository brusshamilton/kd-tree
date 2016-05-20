
add_executable(build_kdtree build_kdtree.cpp)
target_link_libraries(build_kdtree ${EXTRA_LIBS})

add_executable(query_kdtree query_kdtree.cpp)
target_link_libraries(query_kdtree ${EXTRA_LIBS})

install(TARGETS build_kdtree query_kdtree DESTINATION bin)

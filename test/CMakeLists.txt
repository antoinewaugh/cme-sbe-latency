# Now simply link against gtest or gtest_main as needed. Eg
add_executable(depthlist depthlist.cpp)
target_link_libraries(depthlist gtest_main gmock_main DepthBook)
add_test(NAME depthlist_test COMMAND depthlist)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})


include(GoogleTest)

# Create the test executable
add_executable(test_my_library test_image.cpp)

# Add the 'include' directory to the include path so that we can access header files
target_include_directories(test_my_library PRIVATE ${CMAKE_SOURCE_DIR}/include ${PNG_INCLUDE_DIRS})
target_include_directories(test_my_library PRIVATE ${CMAKE_SOURCE_DIR}/src ${PNG_INCLUDE_DIRS})

# Link the test executable with the GTest library and the main library
target_link_libraries(
  test_my_library
  GTest::gtest_main
  main_library  # Make sure the main_library target is defined in the src/CMakeLists.txt
  ${PNG_LIBRARIES}
)

# Discover and run tests automatically
gtest_discover_tests(test_my_library)
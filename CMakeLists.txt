cmake_minimum_required(VERSION 3.28.0)

set(PROJECT_NAME mlib)
set(EXEC mlib)
project(PROJECT_NAME, VERSION 0.0.1)

add_executable(
    EXEC 
    main.cpp
    lib/mvector.hpp
    lib/mstack.hpp
    lib/mtest.hpp
    lib/mmap.hpp
    tests/vector/test_vector.cpp
    )



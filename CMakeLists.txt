cmake_minimum_required(VERSION 3.0...3.28.0)

set(PROJECT_NAME mlib)
set(EXEC_NAME mlib_lib)  # Renaming the executable

message("Before 'project':")
message("  C: '${CMAKE_C_COMPILER}'")
message("  C++: '${CMAKE_CXX_COMPILER}'")
message("  Source: ${PROJECT_SOURCE_DIR}")
message("  Binary: ${PROJECT_BINARY_DIR}")
message("  Version: ${PROJECT_VERSION}")
message("  Version (alt): ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

project(${PROJECT_NAME} VERSION 0.0.1 DESCRIPTION "mlib standard" LANGUAGES CXX)
include(GNUInstallDirs)

# Define the library
add_library(${PROJECT_NAME} SHARED 
    lib/mvector.hpp
    lib/mstack.hpp
    lib/mallocator.hpp
    lib/miterator.hpp
    lib/mttraits.hpp
    lib/mstring.hpp
    lib/mtest.hpp)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    LINKER_LANGUAGE CXX
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER api/mlib.hpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



target_include_directories(${PROJECT_NAME} PRIVATE .)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(FILES ${CMAKE_BINARY_DIR}/mlib.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
message("After 'project':")
message("   Description : '${PROJECT_DESCRIPTION}'" )
message("  C: '${CMAKE_C_COMPILER}'")
message("  C++: '${CMAKE_CXX_COMPILER}'")
message("  Source: ${PROJECT_SOURCE_DIR}")
message("  Binary: ${PROJECT_BINARY_DIR}")
message("  Version: ${PROJECT_VERSION}")
message("  Version (alt): ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

if(MSVC)
  target_compile_options(${EXEC_NAME} PRIVATE /W4 /WX)
endif()

# Define the executable
add_executable(${EXEC_NAME} 
    main.cpp
    lib/mvector.hpp
    lib/mstack.hpp
    lib/mallocator.hpp
    lib/miterator.hpp
    lib/mttraits.hpp
    lib/mstring.hpp
    lib/mtest.hpp
)

message("Processing CMakeLists.txt")
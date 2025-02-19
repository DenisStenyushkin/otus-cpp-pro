cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

find_package(GTest REQUIRED)

configure_file(version.h.in version.h)

add_executable(ip_filter ip_filter.cpp funcs.hpp)
add_executable(ip_filter_tests ip_filter_tests.cpp funcs.hpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(ip_filter
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(ip_filter_tests
    PRIVATE ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(
    ip_filter_tests
    ${GTEST_BOTH_LIBRARIES}
)

if (MSVC)
    target_compile_options(ip_filter PRIVATE
        /W4
    )
    target_compile_options(ip_filter_tests PRIVATE
        /W4
    )
else ()
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(ip_filter_tests PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(ip_filter_tests ip_filter_tests) 

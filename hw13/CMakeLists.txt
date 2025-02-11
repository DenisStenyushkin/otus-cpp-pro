cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(fasion_mnist VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_executable(fasion_mnist main.cpp
                             classifier.h
                             helpers.h
                             helpers.cpp
                             tf_classifier.h
                             tf_classifier.cpp)

set_target_properties(fasion_mnist PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(fasion_mnist
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(fasion_mnist tensorflow)

if (MSVC)
    target_compile_options(fasion_mnist PRIVATE
        /W4
    )
else ()
    target_compile_options(fasion_mnist PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS fasion_mnist RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

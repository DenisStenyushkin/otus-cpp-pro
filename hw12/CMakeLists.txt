cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(mapreduce VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_executable(mapreduce main.cpp
                        text_file_splitter.hpp
                        data_provider.hpp
                        map_func.hpp
                        mapper.hpp
                        list_merging.hpp)

set_target_properties(mapreduce PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(mapreduce
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if (MSVC)
    target_compile_options(mapreduce PRIVATE
        /W4
    )
else ()
    target_compile_options(mapreduce PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS mapreduce RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

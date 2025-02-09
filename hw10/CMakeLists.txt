cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulk_server VERSION ${PROJECT_VESRION})

find_package(Boost 1.86.0 COMPONENTS system REQUIRED )
find_package(Threads)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

configure_file(version.h.in version.h)

add_executable(bulk_server
                bulk_server.cpp
                BatchInputCommandProcessorState.h
                BatchInputCommandProcessorState.cpp
                CommandMetadata.h
                CommandProcessor.h
                CommandProcessor.cpp
                CommandProcessorState.h
                CommandProcessorState.cpp
                CommandProcessorStateFabric.h
                CommandProcessorStateFabric.cpp
                ScopedInputCommandProcessorState.h
                ScopedInputCommandProcessorState.cpp
                OutputHandler.hpp)

set_target_properties(bulk_server PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(bulk_server
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(bulk_server ${Boost_LIBRARIES}
                                  ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

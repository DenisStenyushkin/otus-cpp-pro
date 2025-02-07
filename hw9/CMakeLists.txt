cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(async VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_library(async SHARED
    async.h
    async.cpp
    BatchInputCommandProcessorState.h
    BatchInputCommandProcessorState.cpp
    CommandMetadata.h
    CommandProcessor.h
    CommandProcessor.cpp
    CommandProcessorState.h
    CommandProcessorState.cpp
    CommandProcessorStateFabric.h
    CommandProcessorStateFabric.cpp
    ContextManager.hpp
    ScopedInputCommandProcessorState.h
    ScopedInputCommandProcessorState.cpp
    WaitingQueue.hpp)
add_executable(bulk_async_demo demo.cpp)
add_executable(bulk_async_client client.cpp)

set_target_properties(async bulk_async_demo bulk_async_client PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(bulk_async_demo
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_link_libraries(bulk_async_demo 
    PRIVATE async
)

target_include_directories(bulk_async_client
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_link_libraries(bulk_async_client 
    PRIVATE async
)

if (MSVC)
    target_compile_options(async PRIVATE
        /W4
    )
    target_compile_options(bulk_async_demo PRIVATE
        /W4
    )
    target_compile_options(bulk_async_client PRIVATE
        /W4
    )
else ()
    target_compile_options(async PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(bulk_async_demo PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(bulk_async_client PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS async RUNTIME DESTINATION bin)
install(TARGETS bulk_async_demo RUNTIME DESTINATION bin)
install(TARGETS bulk_async_client RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

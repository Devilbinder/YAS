set(SCHEDULER_SOURCES
    src/yas_scheduler.c
)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")

    message("PLATFORM: Windows")

    set(PLATFORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platform/windows)

    set(WINDOWS_SRC
        platform/windows/yas_win.c
        platform/windows/yas_win_timer.c
    )

    list(APPEND SCHEDULER_SOURCES ${WINDOWS_SRC})

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message("PLATFORM: Linux")
    
    set(PLATFORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platform/linux)
    
    set(LINUX_SRC
        platform/linux/yas_linux.c
        platform/linux/yas_linux_timer.c
    )

    list(APPEND SCHEDULER_SOURCES ${LINUX_SRC})

else()
message("PLATFORM: " ${CMAKE_SYSTEM_NAME} " Unsupported" )
endif()

add_library(scheduler ${SCHEDULER_SOURCES})

target_include_directories(scheduler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
    ${PLATFORM_DIR}
)

# Add platform-specific dependencies
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(scheduler PRIVATE Winmm)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(scheduler PRIVATE ${LIBRT})
endif()

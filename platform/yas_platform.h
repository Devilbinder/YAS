#ifndef __YAS_PLATFORM_H__
#define __YAS_PLATFORM_H__


#if defined(__MINGW32__)

#include "yas_win.h"

#elif  defined(__linux__)
#include "yas_linux.h"
#else
#error "platform not supported"
#endif

/**
 * @brief Get the current system time in ms
 * 
 * @return int64_t current system ticks in ms
 */
int64_t yas_platform_current_timestamp();

/**
 * @brief Set the sleep time of a task.
 * 
 * @note Private use. Not to be used by the main application.
 * 
 * @param time 
 */
void yas_platform_sleep(int64_t time);





#endif
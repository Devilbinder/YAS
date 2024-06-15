#ifndef __YAS_PLATFORM_H__
#define __YAS_PLATFORM_H__


#if defined(__MINGW32__)

#include "yas_win.h"

#elif  defined(__linux__)
#include "yas_linux.h"
#else
#error "platform not supported"
#endif

int64_t yas_platform_current_timestamp();
void yas_platform_sleep(int64_t time);





#endif
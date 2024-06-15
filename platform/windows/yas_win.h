#ifndef __YAS_WIN_H__
#define __YAS_WIN_H__

#include <Windows.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

typedef struct{
    UINT_PTR id;
    uint32_t handle;
}yas_timer_t;

int yas_platform_init(void);

#endif
#ifndef __YAS_LINUX_H__
#define __YAS_LINUX_H__

#include <stdint.h>
#include <time.h>

typedef struct{
    timer_t id;
    uint32_t handle;
}yas_timer_t;


int yas_platform_init(void);
int yas_timer_platform_init(void);


#endif
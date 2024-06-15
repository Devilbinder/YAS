#ifndef __YAS_TIMER_H__
#define __YAS_TIMER_H__

#include "yas_platform.h"


typedef void (*yas_timer_callback)(void *context);

typedef enum{
    YAS_TIMER_RELOAD,
    YAS_TIMER_ONESHOT,
}yas_timer_type_t;


int yas_create_timer(yas_timer_t *timer, yas_timer_callback cb, void *context);
int yas_start_timer(yas_timer_t *timer, yas_timer_type_t type, uint32_t timeout_ms);
int yas_free_timer(yas_timer_t *timer);


#endif
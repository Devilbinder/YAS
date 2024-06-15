#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "yas_error.h"
#include "yas_platform.h"

#define YAS_STOP_FLAG (1 << 0)

typedef struct yas_task_link_t yas_task_link_t;
typedef struct yas_task_t yas_task_t;
typedef void (*yas_task)(yas_task_t *task, void *context);

typedef void (*yas_idle_hook_t)(int64_t idle_time_ms);

struct yas_task_t{
    uint8_t id;
    int64_t delay_time;
    void *context;
    yas_task task;
};

struct yas_task_link_t{
    yas_task_t task;
    yas_task_link_t *next;
    yas_task_link_t *prev;
};

typedef struct{
    yas_task_link_t *start;
    yas_task_link_t *end;
}yas_task_list_t;


int yas_init(void);
int yas_create_task(yas_task func, yas_task_t *task);
int yas_start(void);
void yas_stop(void);
int yas_deinit(void);
void yas_set_idle_hook(yas_idle_hook_t hook);

void yas_task_sleep(yas_task_t *task, uint32_t delay);

#endif
#include "yas_scheduler.h"

static yas_task_list_t task_list;
static yas_idle_hook_t yas_idle_hook;
uint8_t flags;

int yas_init(void){
    yas_idle_hook = NULL;
    memset(&task_list, 0, sizeof(task_list));
    return yas_platform_init();
}

int yas_create_task(yas_task func, yas_task_t *task){

    static uint8_t id = 0;

    yas_task_link_t *new_task = NULL;

    if(func == NULL){
        return YAS_INVALID_PARMA_ERROR;
    }

    new_task = (yas_task_link_t*)malloc(sizeof(yas_task_link_t));


    if(new_task == NULL){
        return YAS_MEM_ERROR;
    }

    new_task->task.id = id++;
    new_task->task.task = func;
    new_task->task.delay_time = 0;
    
    if(task != NULL){
        task = &new_task->task;
    }

    if(task_list.start == NULL){
        
        task_list.start = new_task;
        task_list.end = new_task;
        new_task->next = NULL;
        new_task->prev = NULL;
        

        return YAS_OK;
    }


    yas_task_link_t *current = task_list.start;

    while(current->next != NULL){
        current = current->next;
    }

    current->next = new_task;
    new_task->prev = current;
    new_task->next = NULL;
    task_list.end = new_task;
    
    return YAS_OK;
}

static void yas_sleep_idle(int64_t time){
    if(yas_idle_hook != NULL){
        yas_idle_hook(time);
    }
    yas_platform_sleep(time);
}

int yas_start(void){

    int64_t now = 0;
    
    flags &= ~(YAS_STOP_FLAG);

    while((flags & YAS_STOP_FLAG) == 0){

        yas_task_link_t *current = task_list.start;
        yas_task_link_t *next = NULL;
        int64_t sleep_time = INT64_MAX;

        while(current != NULL){

            now = yas_platform_current_timestamp();

            int64_t time_diff = current->task.delay_time - now;

            if(time_diff <= 0){
               current->task.task(&current->task, current->task.context); 
            }else{
                if(time_diff < sleep_time){
                    sleep_time = time_diff;
                }
            }

            current = current->next;
        }

        if((sleep_time > 0) && (sleep_time != INT64_MAX)){
            yas_sleep_idle(sleep_time);
        }
        
    }

    return YAS_OK;

}

void yas_stop(void){
    flags |= (YAS_STOP_FLAG);
}

int yas_deinit(void){

    yas_task_link_t *current = task_list.start;
    yas_task_link_t *next = NULL;


    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }

    return YAS_OK;
}

void yas_set_idle_hook(yas_idle_hook_t hook){
    yas_idle_hook = hook;
}

void yas_task_sleep(yas_task_t *task, uint32_t delay){
    task->delay_time = yas_platform_current_timestamp() + delay;
}


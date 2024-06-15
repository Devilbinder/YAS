#include "yas_win.h"
#include "yas_error.h"
#include "yas_timer.h"

typedef struct win_timer_list_t win_timer_list_t;

struct win_timer_list_t{
    UINT_PTR id;
    uint32_t handle;
    yas_timer_callback callback;
    void *context;
    win_timer_list_t *next;
    win_timer_list_t *prev;
};

static win_timer_list_t *start = NULL;
static win_timer_list_t *timer_list;

VOID CALLBACK TimerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw) {
    win_timer_list_t *now = start;

    while(now != NULL){

        if(now->id == uTimerID){
            if(now->callback != NULL){
                now->callback(now->context);
                break;
            }
        }
        now = now->next;
    }
}

int yas_create_timer(yas_timer_t *timer, yas_timer_callback cb, void *context){

    static uint32_t handle = 0;

    win_timer_list_t *new_timer = (win_timer_list_t*)malloc(sizeof(win_timer_list_t));

    if(new_timer == NULL){
       return YAS_MEM_ERROR; 
    }

    timer->id = 0;
    timer->handle = handle;
    new_timer->handle = handle;
    new_timer->callback = cb;
    new_timer->context = context;

    ++handle;

    if(start == NULL){
        start = new_timer;
        new_timer->next = NULL;
        new_timer->prev = NULL;
        return YAS_OK;
    }

    win_timer_list_t *end = start;

    while(end->next != NULL){
        end = end->next;
    }

    end->next = new_timer;
    new_timer->prev = end;

    return YAS_OK;
}

int yas_start_timer(yas_timer_t *timer, yas_timer_type_t type, uint32_t timeout_ms){

    win_timer_list_t *current = start;

    while(current != NULL){
        if(current->handle == timer->handle){
            break;
        }
        current = current->next;
    }

    if(current == NULL){
        return YAS_NOT_FOUND;
    }

    if(type == YAS_TIMER_RELOAD){
        timer->id = timeSetEvent(timeout_ms, 1, TimerCallback, 0, TIME_PERIODIC);
    }else{
        timer->id = timeSetEvent(timeout_ms, 1, TimerCallback, 0, TIME_ONESHOT);
    }

    if (timer->id == 0) {
        return YAS_CREATE_ERROR;
    }

    current->id = timer->id;

    return YAS_OK;
}


int yas_free_timer(yas_timer_t *timer){

    win_timer_list_t *prev = NULL;
    win_timer_list_t *next = NULL;

    win_timer_list_t *current = start;

    while(current != NULL){
        if(current->id == timer->id){
            break;
        }
        current = current->next;
    }

    if(current == NULL){
        return YAS_NOT_FOUND;
    }

    next = current->next;
    prev = current->prev;

    if(prev != NULL){
       prev->next = next; 
    }

    if(next != NULL){
        next->prev = prev;
    }

    if(current == start){
        start = next;
    }
    
    KillTimer(NULL, timer->id);

    free(current);

    return YAS_OK;
}
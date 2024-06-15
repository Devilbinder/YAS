#include "yas_error.h"
#include "yas_timer.h"
#include "yas_linux.h"
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct linux_timer_list_t linux_timer_list_t;

struct linux_timer_list_t{
    timer_t id;
    uint32_t handle;
    yas_timer_callback callback;
    void *context;
    linux_timer_list_t *next;
    linux_timer_list_t *prev;
};

static linux_timer_list_t *start = NULL;
static linux_timer_list_t *timer_list;

void TimerCallback(int sig, siginfo_t *si, void *uc) {
    linux_timer_list_t *now = start;
    uint64_t expirations;

    while (now != NULL) {
        if (&now->id == si->si_value.sival_ptr) {
            if (now->callback != NULL) {
                now->callback(now->context);
            }
            break;
        }
        now = now->next;
    }
}

int yas_timer_platform_init(void){

    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = TimerCallback;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        return YAS_START_ERROR;
    }

    return YAS_OK;

}


int yas_create_timer(yas_timer_t *timer, yas_timer_callback cb, void *context){

    static uint32_t handle = 0;
    struct sigevent sev;

    linux_timer_list_t *new_timer = (linux_timer_list_t*)malloc(sizeof(linux_timer_list_t));

    if(new_timer == NULL){
       return YAS_MEM_ERROR; 
    }

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &new_timer->id;

    if (timer_create(CLOCK_REALTIME, &sev, &new_timer->id) == -1) {
        free(new_timer);
        return YAS_CREATE_ERROR;
    }

    timer->id = new_timer->id;
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

    linux_timer_list_t *end = start;

    while(end->next != NULL){
        end = end->next;
    }

    end->next = new_timer;
    new_timer->prev = end;

    return YAS_OK;
}

int yas_start_timer(yas_timer_t *timer, yas_timer_type_t type, uint32_t timeout_ms){
    struct itimerspec timeout;

    linux_timer_list_t *current = start;

    while(current != NULL){
        if(current->handle == timer->handle){
            break;
        }
        current = current->next;
    }

    if(current == NULL){
        return YAS_NOT_FOUND;
    }

    timeout.it_value.tv_sec = timeout_ms / 1000;
    timeout.it_value.tv_nsec = (timeout_ms % 1000) * 1000000;

    if(type == YAS_TIMER_RELOAD){
        timeout.it_interval.tv_sec = timeout_ms / 1000;
        timeout.it_interval.tv_nsec = (timeout_ms % 1000) * 1000000;
    }else{
        timeout.it_interval.tv_sec = 0;
        timeout.it_interval.tv_nsec = 0;
    }

    if (timer_settime(current->id, 0, &timeout, NULL) == -1) {
        return YAS_START_ERROR;
    }

    return YAS_OK;
}

int yas_free_timer(yas_timer_t *timer){

    linux_timer_list_t *current = start;
    linux_timer_list_t *prev = NULL;
    linux_timer_list_t *next = NULL;

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

    free(current);

    return YAS_OK;
}
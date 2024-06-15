#include "yas_win.h"
#include "yas_error.h"

int64_t yas_platform_current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    return te.tv_sec * 1000LL + te.tv_usec / 1000;
}


void yas_platform_sleep(int64_t time){
    usleep(time * 1000);
}

int yas_platform_init(void){
    return YAS_OK;
}
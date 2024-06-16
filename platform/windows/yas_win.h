#ifndef __YAS_WIN_H__
#define __YAS_WIN_H__

#include <Windows.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

typedef struct{
    UINT_PTR id; /**< handle of the platform timer */
    uint32_t handle; /**< YAS handle for the timer */
}yas_timer_t;

/**
 * @brief Windows setup platform platform specific requirements
 * 
 * @note Private use. Not to be used by the main application
 * 
 * @return YAS_OK : All good.
 */
int yas_platform_init(void);

#endif
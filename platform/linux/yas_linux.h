#ifndef __YAS_LINUX_H__
#define __YAS_LINUX_H__

#include <stdint.h>
#include <time.h>

typedef struct{
    timer_t id; /**< handle of the platform timer */
    uint32_t handle; /**< YAS handle for the timer */
}yas_timer_t;

/**
 * @brief Linux setup platform platform specific requirements
 * 
 * @note Private use. Not to be used by the main application
 * 
 * @return YAS_OK : All good.
 * @return YAS_START_ERROR : Failed to start scheduler
 */
int yas_platform_init(void);

/**
 * @brief Setup Linux specific timer requirements.
 * 
 * @note Private use. Not to be used by the main application
 * 
 * @return YAS_OK : All good.
 * @return YAS_START_ERROR : Failed to start scheduler
 */
int yas_timer_platform_init(void);


#endif
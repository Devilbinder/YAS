#ifndef __YAS_TIMER_H__
#define __YAS_TIMER_H__

#include "yas_platform.h"


typedef void (*yas_timer_callback)(void *context); /**< Callback definition of a timer */

typedef enum{
    YAS_TIMER_RELOAD,  /**< Timer will run periodically */
    YAS_TIMER_ONESHOT, /**< Timer will only run once */
}yas_timer_type_t;

/**
 * @brief Crate a instance of a timer
 * 
 * @param timer     Handle to a timer
 * @param cb        Callback when the timer expires
 * @param context   Context for the timer to use
 * @return YAS_OK: All is good
 * @return YAS_MEM_ERROR: Failed to allocate the timer
 * @return YAS_CREATE_ERROR: Failed to crate the timer
 */
int yas_create_timer(yas_timer_t *timer, yas_timer_callback cb, void *context);

/**
 * @brief Start a timer
 * 
 * @param timer         Handle to a timer
 * @param type          Will the timer be YAS_TIMER_RELOAD or YAS_TIMER_ONESHOT
 * @param timeout_ms    Period the timer will run for in ms
 * 
 * @return YAS_OK: All is good
 * @return YAS_NOT_FOUND: Timer not found
 * @return YAS_START_ERROR: Failed to start the timer
 */
int yas_start_timer(yas_timer_t *timer, yas_timer_type_t type, uint32_t timeout_ms);

/**
 * @brief Free a timer and remove it.
 * 
 * @param timer         Handle to a timer
 * 
 * @return YAS_OK: All is good
 * @return YAS_NOT_FOUND: Timer not found
 */
int yas_free_timer(yas_timer_t *timer);


#endif
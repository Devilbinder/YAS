#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "yas_error.h"
#include "yas_platform.h"

#define YAS_STOP_FLAG (1 << 0) requirements

typedef struct yas_task_link_t yas_task_link_t;
typedef struct yas_task_t yas_task_t;

typedef void (*yas_task)(yas_task_t *task, void *context); /**< Callback definition of a task */

typedef void (*yas_idle_hook_t)(int64_t idle_time_ms); /**< Callback definition of the idle time hook */

/**
 * @brief Definition of a task handle.
 */
struct yas_task_t{
    uint8_t id; /**< id of the task */
    int64_t delay_time; /**< Time the task will wait before executing again */
    void *context; /**< Pointer for the a context item to be passed to a task*/
    yas_task task; /**< Function pointer for the task to be exacted */
};

/**
 * @brief Definition of a task list.
 * @note Private struct not to be used in the main application.
 */
struct yas_task_link_t{
    yas_task_t task; /**< Handle of the current task in the list */
    yas_task_link_t *next; /**< Handle of the next task in the list */
    yas_task_link_t *prev; /**< Handle of the previous task in the list */
};

/**
 * @brief Definition of a task links. Holds the start and end of the tasks link list.
 * @note Private struct not to be used in the main application.
 */
typedef struct{
    yas_task_link_t *start; /**< Handle to the start of the task list */
    yas_task_link_t *end; /**< Handle to the end of the task list */
}yas_task_list_t;

/**
 * @brief   Setup the scheduler and any platform specific requirements for the scheduler to be able
 *          to run.
 *          
 * @return YAS_OK : All good.
 * @return YAS_START_ERROR : Failed to start scheduler
 */
int yas_init(void);

/**
 * @brief Create a task and add it to the scheduler to start running.
 * 
 * @note Must be called only after init. Can be called befor the scheduler is started but tasks will
 *       only start running after yas_start() is called.
 * 
 * @param func function pointer to the task
 * @param task task handle
 * 
 * @return YAS_OK : All good.
 * @return YAS_MEM_ERROR: Failed to allocate the new task.
 */
int yas_create_task(yas_task func, yas_task_t *task);

/**
 * @brief Start the scheduler. Clears the YAS_STOP_FLAG
 * 
 * @return YAS_OK : All good.
 */
int yas_start(void);

/**
 * @brief Stop the scheduler. Set the YAS_STOP_FLAG flag
 * 
 */
void yas_stop(void);

/**
 * @brief Free all resources allocated to the scheduler
 * 
 * @return YAS_OK : All good.
 */
int yas_deinit(void);

/**
 * @brief Set the idle hook function
 * @note can be set to NULL. the default internal idle function will then be used
 * 
 * @param hook function pointer to the new idle hook.
 */
void yas_set_idle_hook(yas_idle_hook_t hook);

/**
 * @brief Set the sleep time of a task. Must be called withing the task function.
 * 
 * @param task Task handle
 * @param delay Time to delay exaction in ms.
 */
void yas_task_sleep(yas_task_t *task, uint32_t delay);

#endif
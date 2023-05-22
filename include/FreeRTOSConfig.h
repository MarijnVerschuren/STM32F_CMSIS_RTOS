//
// Created by marijn on 5/15/23.
//
#ifndef STM32F_CMSIS_FREERTOSCONFIG_H
#define STM32F_CMSIS_FREERTOSCONFIG_H
#define RTOS

#include "main.h"	// stm32 cmsis header
#include "sys.h" 	// stm32 system (SYS_clock_frequency)

/* The CMSIS-RTOS V2 FreeRTOS wrapper is dependent on the heap implementation used
 * by the application thus the correct define need to be enabled below */
#define USE_FreeRTOS_HEAP_4
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS			/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
#define configPRIO_BITS        __NVIC_PRIO_BITS
#else
#define configPRIO_BITS         4
#endif


#define configUSE_PREEMPTION                     1
#define configUSE_TIME_SLICING                   1
#define configSUPPORT_STATIC_ALLOCATION          1
#define configSUPPORT_DYNAMIC_ALLOCATION         1
#define configUSE_IDLE_HOOK                      0
#define configUSE_TICK_HOOK                      0
#define configCPU_CLOCK_HZ                       ( SYS_clock_frequency )
#define configTICK_RATE_HZ                       ((TickType_t)1000)
#define configMAX_PRIORITIES                     ( 56 )
// For ES: Increase configMINIMAL_STACK_SIZE size in case more stack size for the interrupt handlers is needed, e.g. when using sprintf in an interrupt handler.
// This because the interrupt handlers reuse the Idle stack, initially used by main().
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)
// For ES: Increase configTOTAL_HEAP_SIZE size in case more threads are needed.
// Note that with FreeRTOS the thread stack is allocated from the heap and that the stack size is in words (4 bytes) and the heap size in bytes.
// Using xPortGetFreeHeapSize() one can see that with a stack size of 128 words (512 bytes), 624 bytes are taken from the heap with each new thread.
#define configTOTAL_HEAP_SIZE                    ((size_t)10000) // was 3072 bytes
// For ES: configCHECK_FOR_STACK_OVERFLOW added so that vApplicationStackOverflowHook will be called when a stack overflow occurs.
#define configMAX_TASK_NAME_LEN                  ( 16 )
#define configUSE_TRACE_FACILITY                 1
#define configUSE_16_BIT_TICKS                   0
#define configUSE_MUTEXES                        1
#define configQUEUE_REGISTRY_SIZE                8
#define configUSE_RECURSIVE_MUTEXES              1
#define configUSE_COUNTING_SEMAPHORES            1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                    0
#define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS                         1
#define configTIMER_TASK_PRIORITY                ( 2 )
#define configTIMER_QUEUE_LENGTH                 10
#define configTIMER_TASK_STACK_DEPTH             256

/* The following flag must be enabled only when using newlib */
#define configUSE_NEWLIB_REENTRANT          0

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1
#define INCLUDE_xTimerPendFunctionCall      1
#define INCLUDE_xQueueGetMutexHolder        1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define INCLUDE_eTaskGetState               1

/*
 * The CMSIS-RTOS V2 FreeRTOS wrapper is dependent on the heap implementation used
 * by the application thus the correct define need to be enabled below
 */
#define USE_FreeRTOS_HEAP_4

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
#define configPRIO_BITS         __NVIC_PRIO_BITS
#else
#define configPRIO_BITS         4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
/* USER CODE BEGIN 1 */
#define configASSERT( x ) if ((x) == 0) { taskDISABLE_INTERRUPTS(); for(;;); }
/* USER CODE END 1 */

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler

/* IMPORTANT: This define is commented when used with STM32Cube firmware, when the timebase source is SysTick,
              to prevent overwriting SysTick_Handler defined within STM32Cube HAL */

#define xPortSysTickHandler RTOS_tick_handler

/* USER CODE BEGIN Defines */
/* Section where parameter definitions can be added (for instance, to override default ones in FreeRTOS.h) */
/* USER CODE END Defines */
#endif //STM32F_CMSIS_FREERTOSCONFIG_H
//
// Created by marijn on 5/22/23.
//
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>

#include "main.h"
#include "base.h"


void vApplicationMallocFailedHook(void) {
	for(;;);
}
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName) {
	for(;;);
}
void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer, StackType_t** ppxTimerTaskStackBuffer, uint32_t* pulTimerTaskStackSize) {
	static StaticTask_t xTimerTaskTCB;						// all created buffers need to be static because they are used after function lifetime
	static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;				// pass StaticTask_t structure in which the idle task state is stored
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;			// pass pointer to the timer stack
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;	// pass the size of the timer stack in words
}
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize) {
	static StaticTask_t xIdleTaskTCB;						// all created buffers need to be static because they are used after function lifetime
	static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;					// pass StaticTask_t structure in which the idle task state is stored
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;				// pass pointer to the idle task stack
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;		// pass the size of the idle task stack in words
}
#ifndef DEBUG_H_
#define DEBUG_H_

#include "hal_variables.h"
#include <string.h>

extern char  __debug_buffer[256];

#define DEBUG(fmt, ...) snprintf(__debug_buffer, 255, fmt, ##__VA_ARGS__);HAL_UART_Transmit(&huart2, (uint8_t *) __debug_buffer, strlen(__debug_buffer),HAL_MAX_DELAY);HAL_UART_Transmit(&huart2, (uint8_t *)("\r\n"), strlen("\r\n"),HAL_MAX_DELAY);
#define DEBUG_NO_NEWLINE(fmt, ...) snprintf(__debug_buffer, 255, fmt, ##__VA_ARGS__);HAL_UART_Transmit(&huart2, (uint8_t *) __debug_buffer, strlen(__debug_buffer),HAL_MAX_DELAY);

#endif /* DEBUG_H_ */

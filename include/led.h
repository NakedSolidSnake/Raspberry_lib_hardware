#ifndef __LED_H
#define __LED_H

#include <gpio.h>

typedef struct
{
    GPIO_t gpio;    /**!< Herda Gpio */
}LED_t;

/**
 * @brief 
 * 
 * @param led 
 * @return int 
 */
int LED_init(LED_t *led);

/**
 * @brief 
 * 
 * @param led 
 * @param eState 
 * @return int 
 */
int LED_set(LED_t *led, eState_t eState);

#endif
#ifndef __GPIO_H
#define __GPIO_H

typedef enum
{
    eModeInput = 0,
    eModeOutput
}eMode_t;

typedef enum
{
    eStateLow = 0,
    eStateHigh
}eState_t;

typedef struct 
{
    int pin;
    eMode_t eMode;
}GPIO_t;

/**
 * @brief 
 * 
 * @param gpio 
 * @return int 
 */
int GPIO_init(GPIO_t *gpio);

#endif
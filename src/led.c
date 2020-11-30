#include <led.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

int LED_init(LED_t *led)
{
    if(!led)
        return EXIT_FAILURE;

    return GPIO_init((GPIO_t *)led);    
}

int LED_set(LED_t *led, eState_t eState)
{
    if(!led)
        return EXIT_FAILURE;

    digitalWrite(((GPIO_t *)led)->pin, eState);

    return EXIT_SUCCESS;
}
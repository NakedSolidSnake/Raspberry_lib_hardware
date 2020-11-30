#include <gpio.h>
#include <stdlib.h>
#include <wiringPi.h>

int GPIO_init(GPIO_t *gpio)
{
    if(!gpio)
        return EXIT_FAILURE;

    wiringPiSetup();
    pinMode(gpio->pin, gpio->eMode);

    return EXIT_SUCCESS;
}
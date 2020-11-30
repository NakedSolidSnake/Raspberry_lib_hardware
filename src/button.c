#include <button.h>
#include <stdio.h>
#include <stdlib.h>

int Button_init(Button_t *button)
{
    if(!button)
        return EXIT_FAILURE;

    GPIO_init((GPIO_t *)&button);
      
    pullUpDnControl(((GPIO_t *)button)->pin, button->ePullMode);
    if(button->cb)
        wiringPiISR(((GPIO_t *)button)->pin, button->eIntEdge, button->cb);


    return EXIT_SUCCESS;
}

int Button_read(Button_t *button)
{
    if(!button)
        return EXIT_FAILURE;

    return digitalRead(((GPIO_t *)button)->pin);
}
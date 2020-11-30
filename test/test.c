#include <button.h>
#include <led.h>
#include <unistd.h>
#include <stdlib.h>

#define _1MS    1000

static void inputHandler(void);
static int state = 0;

static LED_t led = {
        .gpio.pin = 0,
        .gpio.eMode = eModeOutput
    };

static Button_t button = {
        .gpio.pin = 7,
        .gpio.eMode = eModeInput,
        .ePullMode = ePullModePullUp,
        .eIntEdge = eIntEdgeFalling,
        .cb = inputHandler
    };

int main(int argc, char const *argv[])
{   
    eState_t eState = eStateLow;
    

    /* Initialize Led and Button */
    if(Button_init(&button))
        return EXIT_FAILURE;
    if(LED_init(&led))
        return EXIT_FAILURE;


    while(1)
    {
        if(state)
        {
            LED_set(&led, eState);
            if(eState == eStateLow)
                eState = eStateHigh;
            else
                eState = eStateLow;

            state = 0;
        }

        usleep(_1MS);
    }

    return 0;
}

static void inputHandler(void)
{
    if(!Button_read(&button)){
        usleep(_1MS * 40);
        while(!Button_read(&button));

        state = 1;
    }
}
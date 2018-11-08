
#include "gpio.h"
#include "fpioa.h"
#include "led.h"
void led_init()
{
    int i;

    gpio_init();
    gpio->interrupt_mask.u32[0] = 0;
    gpio->source.u32[0] = 0xff;
    fpioa_init();
    for (i = 4; i <= 5; i++)
        fpioa_set_function(i + 20, FUNC_GPIO0 + i); /* Bidirectional */

    gpio->direction.u32[0] = 0;
    for (i = 4; i <= 5; i++) {
        gpio->direction.u32[0] |= (1 << i);
        gpio->data_output.u32[0] |= (1 << i);
    }
}

void led_OnOff(int lednum,int OnOff)
{
    if(lednum == 0)
    {
        if(OnOff)
            gpio->data_output.u32[0] &= ~(1 << 4);
        else
            gpio->data_output.u32[0] |= (1 << 4);
    }
    else if(lednum == 1)
    {
        if(OnOff)
            gpio->data_output.u32[0] &= ~(1 << 5);
        else
            gpio->data_output.u32[0] |= (1 << 5);
    }
}

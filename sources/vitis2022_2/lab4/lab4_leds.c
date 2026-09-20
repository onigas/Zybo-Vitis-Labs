#include "board_gpio.h"
#include "led_regs.h"
#include "sleep.h"

int main(void)
{
    XGpio switches, buttons;
    u32 previous_sw = 0xFFFFFFFFU, previous_btn = 0xFFFFFFFFU;
    volatile u32 stack_probe = 0U;
    xil_printf("\r\nLab 4: switches -> custom LED IP\r\n");
    xil_printf("LED base: 0x%08x\r\n", (unsigned)LAB_LED_BASEADDR);
    xil_printf("Stack probe: 0x%08x\r\n", (unsigned)(UINTPTR)&stack_probe);
    if (lab_gpio_init(&switches, &buttons) != XST_SUCCESS) {
        return XST_FAILURE;
    }
    lab_led_write(0U);
    while (1) {
        const u32 sw = XGpio_DiscreteRead(&switches, 1U) & LAB_IO_MASK;
        const u32 btn = XGpio_DiscreteRead(&buttons, 1U) & LAB_IO_MASK;
        lab_led_write(sw);
        stack_probe = sw;
        if ((sw != previous_sw) || (btn != previous_btn)) {
            xil_printf("SW=0x%x BTN=0x%x LED=0x%x\r\n",
                       (unsigned)sw, (unsigned)btn, (unsigned)lab_led_read());
            previous_sw = sw;
            previous_btn = btn;
        }
        usleep(20000U);
    }
}

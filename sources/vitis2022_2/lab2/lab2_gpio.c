#include "board_gpio.h"
#include "sleep.h"

int main(void)
{
    XGpio switches, buttons;
    u32 previous_sw = 0xFFFFFFFFU, previous_btn = 0xFFFFFFFFU;
    xil_printf("\r\nLab 2: switches and buttons\r\n");
    if (lab_gpio_init(&switches, &buttons) != XST_SUCCESS) {
        return XST_FAILURE;
    }
    while (1) {
        const u32 sw = XGpio_DiscreteRead(&switches, 1U) & LAB_IO_MASK;
        const u32 btn = XGpio_DiscreteRead(&buttons, 1U) & LAB_IO_MASK;
        if ((sw != previous_sw) || (btn != previous_btn)) {
            xil_printf("SW=0x%x BTN=0x%x\r\n", (unsigned)sw, (unsigned)btn);
            previous_sw = sw;
            previous_btn = btn;
        }
        usleep(20000U); /* Polling interval, not a full debouncing algorithm. */
    }
}

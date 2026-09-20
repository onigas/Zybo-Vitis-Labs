/* New example for Zybo legacy / Vitis 2022.2, standalone Cortex-A9. */
#ifndef LAB_BOARD_GPIO_H
#define LAB_BOARD_GPIO_H
#include "xparameters.h"
#include "xgpio.h"
#include "xstatus.h"
#include "xil_printf.h"

#ifndef XPAR_SWITCHES_DEVICE_ID
#error "Missing switches: name the AXI GPIO instance switches and rebuild the platform."
#endif
#ifndef XPAR_BUTTONS_DEVICE_ID
#error "Missing buttons: name the AXI GPIO instance buttons and rebuild the platform."
#endif
#define LAB_IO_MASK 0x0FU
#define LAB_EXIT_BUTTON_MASK 0x01U /* BTN0, NOT a reset button. */

static inline int lab_gpio_init(XGpio *switches, XGpio *buttons)
{
    if (XGpio_Initialize(switches, XPAR_SWITCHES_DEVICE_ID) != XST_SUCCESS) {
        xil_printf("ERROR: switches GPIO initialization failed\r\n");
        return XST_FAILURE;
    }
    if (XGpio_Initialize(buttons, XPAR_BUTTONS_DEVICE_ID) != XST_SUCCESS) {
        xil_printf("ERROR: buttons GPIO initialization failed\r\n");
        return XST_FAILURE;
    }
    XGpio_SetDataDirection(switches, 1U, LAB_IO_MASK);
    XGpio_SetDataDirection(buttons, 1U, LAB_IO_MASK);
    return XST_SUCCESS;
}
#endif

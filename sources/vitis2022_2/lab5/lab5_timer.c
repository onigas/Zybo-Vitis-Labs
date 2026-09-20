#include "board_gpio.h"
#include "led_regs.h"
#include "timer_math.h"
#include "xscutimer.h"
#include <stddef.h>

#if defined(XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ)
#define LAB_CPU_HZ XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ
#elif defined(XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ)
#define LAB_CPU_HZ XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ
#else
#error "Missing CPU frequency: inspect xparameters.h; do not guess the clock."
#endif
#if defined(XPAR_PS7_SCUTIMER_0_DEVICE_ID)
#define LAB_TIMER_DEVICE_ID XPAR_PS7_SCUTIMER_0_DEVICE_ID
#elif defined(XPAR_XSCUTIMER_0_DEVICE_ID)
#define LAB_TIMER_DEVICE_ID XPAR_XSCUTIMER_0_DEVICE_ID
#else
#error "Missing private timer: verify the standalone domain and the scutimer driver."
#endif

static int timer_set_interval(XScuTimer *timer, u32 switches, u32 *reload_value)
{
    *reload_value = lab_timer_reload((uint32_t)LAB_CPU_HZ, switches);
    if (*reload_value == 0U) {
        xil_printf("ERROR: invalid timer reload\r\n");
        return XST_FAILURE;
    }
    /* Restart from a full new interval; discard a stale event flag. */
    XScuTimer_Stop(timer);
    XScuTimer_ClearInterruptStatus(timer);
    XScuTimer_LoadTimer(timer, *reload_value);
    XScuTimer_Start(timer);
    return XST_SUCCESS;
}

int main(void)
{
    XGpio switches, buttons;
    XScuTimer timer;
    XScuTimer_Config *config;
    u32 switch_value, previous_switch, reload_value = 0U, count = 0U;
    int status;
    xil_printf("\r\nLab 5: private timer polling; BTN0 exits\r\n");
    if (lab_gpio_init(&switches, &buttons) != XST_SUCCESS) {
        return XST_FAILURE;
    }
    config = XScuTimer_LookupConfig(LAB_TIMER_DEVICE_ID);
    if (config == NULL) {
        xil_printf("ERROR: timer configuration not found\r\n");
        return XST_FAILURE;
    }
    status = XScuTimer_CfgInitialize(&timer, config, config->BaseAddr);
    if (status != XST_SUCCESS) {
        xil_printf("ERROR: timer initialization failed\r\n");
        return XST_FAILURE;
    }
    XScuTimer_Stop(&timer);
    XScuTimer_DisableInterrupt(&timer); /* Polling: no GIC/ISR setup required. */
    XScuTimer_SetPrescaler(&timer, 0U);
    XScuTimer_EnableAutoReload(&timer);
    previous_switch = XGpio_DiscreteRead(&switches, 1U) & LAB_IO_MASK;
    lab_led_write(0U);
    if (timer_set_interval(&timer, previous_switch, &reload_value) != XST_SUCCESS) {
        return XST_FAILURE;
    }
    xil_printf("CPU=%u Hz, SW=0x%x, LOAD=%u\r\n",
               (unsigned)LAB_CPU_HZ, (unsigned)previous_switch, (unsigned)reload_value);
    while (1) {
        if ((XGpio_DiscreteRead(&buttons, 1U) & LAB_EXIT_BUTTON_MASK) != 0U) {
            break;
        }
        switch_value = XGpio_DiscreteRead(&switches, 1U) & LAB_IO_MASK;
        if (switch_value != previous_switch) {
            previous_switch = switch_value;
            count = 0U;
            lab_led_write(count);
            if (timer_set_interval(&timer, switch_value, &reload_value) != XST_SUCCESS) {
                XScuTimer_Stop(&timer);
                return XST_FAILURE;
            }
            xil_printf("SW=0x%x, LOAD=%u\r\n", (unsigned)switch_value,
                       (unsigned)reload_value);
        }
        if (XScuTimer_IsExpired(&timer)) {
            XScuTimer_ClearInterruptStatus(&timer);
            count = (count + 1U) & LAB_IO_MASK;
            lab_led_write(count); /* Set a breakpoint here. */
        }
    }
    XScuTimer_Stop(&timer);
    XScuTimer_ClearInterruptStatus(&timer);
    lab_led_write(0U);
    xil_printf("BTN0 pressed; timer stopped.\r\n");
    return XST_SUCCESS;
}

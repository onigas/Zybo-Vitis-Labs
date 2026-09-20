/* Register access for the four-register AXI4-Lite LED IP from lab 3. */
#ifndef LAB_LED_REGS_H
#define LAB_LED_REGS_H
#include "xparameters.h"
#include "xil_io.h"

#if defined(XPAR_LED_IP_S_AXI_BASEADDR)
#define LAB_LED_BASEADDR XPAR_LED_IP_S_AXI_BASEADDR
#elif defined(XPAR_LED_IP_0_S_AXI_BASEADDR)
#define LAB_LED_BASEADDR XPAR_LED_IP_0_S_AXI_BASEADDR
#else
#error "Missing LED IP: check instance/interface names and the current XSA/platform."
#endif
#define LAB_LED_DATA_OFFSET 0x00U
static inline void lab_led_write(u32 value)
{
    Xil_Out32((UINTPTR)LAB_LED_BASEADDR + LAB_LED_DATA_OFFSET, value & 0x0FU);
}
static inline u32 lab_led_read(void)
{
    return Xil_In32((UINTPTR)LAB_LED_BASEADDR + LAB_LED_DATA_OFFSET) & 0x0FU;
}
#endif

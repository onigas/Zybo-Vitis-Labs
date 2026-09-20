# Software Writing for Timer and Debugging

> **Target:** original Digilent Zybo (legacy), Vivado 2022.2 and Vitis 2022.2 (Eclipse-based IDE).
> Use the **Zybo** board preset, not Zybo Z7. The original workbook figures are retained where they illustrate the hardware, code or results; their appearance and example values can differ from your project.
> **Paths:** `{labs}` is your working directory; `{sources}` is the original repository's `sources` directory. Keep this Markdown file in the repository root, alongside `pics/` and `sources/`.
> Follow the original hardware sequence below. Where a figure shows another board, use the Zybo-specific settings in the text. Continue from your own project built with Vivado 2022.2. An older solution project must first be copied, upgraded and rebuilt for the correct board.

## Objectives

After completing this lab, you will be able to:
* Use the CPU's private timer in polled mode.
* Complete an application using the timer driver API.
* Use the Vitis debugger to set breakpoints and inspect variables and memory.

## Steps

### Open the Project in Vivado

1. Open the **lab4** project from the previous lab, or a prepared lab4 project from **{labsolutions}**, and use **File > Save Project As...** to save it as **lab5** under **{labs}**.

    The CPU's private timer is already present. Do not add a timer IP block or change the block design for this lab.

1. Open the Block Design. Saving the project under a new name may mark synthesis and implementation as out of date. Because the hardware is unchanged, use the **system_wrapper.xsa** exported in Lab 4, which already contains the matching bitstream. Do not ignore an out-of-date warning if you have actually changed the hardware; regenerate the bitstream and export a new XSA in that case.

### Create the Application in Vitis

1. Open **Vitis 2022.2** and select **{labs}/lab5/vitis_workspace** as the workspace. Click **Launch**.
1. Select **File > New > Application Project**. Click **Next** on the welcome page, if it appears.[^application]
1. On the **Platform** page, select **Create a new platform from hardware (XSA)**. Browse to **{labs}/lab4/system_wrapper.xsa** and name the platform **lab5_platform**. Clear **Generate boot components**, if offered; this lab uses JTAG and the PS initialization script. Click **Next**.
1. Set the application name to **lab5**, keep **lab5_system** as the system-project name, and select **ps7_cortexa9_0**. Click **Next**.
1. On the **Domain** page, select **standalone**, **32-bit** if shown, and language **C**. Keep the proposed domain name and click **Next**.
1. Select **Empty Application (C)** and click **Finish**. This one wizard creates the platform, standalone domain/BSP, system project and application.
1. Open **lab5_platform > platform.spr**, select the application's standalone domain, and open **Board Support Package > Modify BSP Settings**. Under **standalone**, set **stdin** and **stdout** to **ps7_uart_1**. Apply the settings.[^bsp]

### Import the Source and Enable the LED Driver

1. In Vitis, open **Xilinx > Software Repositories**. Under **Local Repositories**, add the same custom-IP software repository used in Lab 4, normally **{labs}/led_ip/ip_repo/led_ip_1.0**, and click **Apply and Close**. This is a new workspace, so its local repository list must be configured again. Select the directory containing **drivers**, not the driver's **src** directory.[^repository]
1. Open the platform's **Board Support Package > Modify BSP Settings > drivers**. Assign **led_ip** to the LED hardware instance if necessary. Verify that the GPIO instances use **gpio** and the private timer uses **scutimer**. Apply the settings and build **lab5_platform**.
1. Right-click **lab5 > src > Import...**. Select **General > File System**, browse to **{sources}/lab5**, select **lab5.c**, and click **Finish**.

    The supplied source deliberately contains missing includes and incomplete timer operations. Complete it in the following sections rather than replacing the exercise with another application.

### Refer to the Scutimer API Documentation

1. Open the **scutimer** driver documentation from the platform's BSP settings, or open **xscutimer.h** from the application's BSP include files. The versioned [driver header][timer-header] and [polled example][timer-example] are also useful references.
1. Find **XScuTimer_LookupConfig** and **XScuTimer_CfgInitialize**. They obtain the timer configuration and initialize the driver instance before it is used.
1. Examine the timer load, auto-reload, start, stop, expiration-test and status-clear operations.[^timer]

    <p align="center">
    <img src="./pics/lab 5/1usefulfn.jpg" width="70%" alt="Useful private-timer API operations"/>
    </p>
    <p align="center"><i>Useful private-timer API operations</i></p>

    <p align="center">
    <img src="./pics/lab 5/2usefulfn.jpg" width="70%" alt="Additional private-timer API operations"/>
    </p>
    <p align="center"><i>Additional private-timer API operations</i></p>

### Correct the Errors and Complete the Code

1. In **Problems**, double-click the unknown-type error associated with **XScuTimer** to open the source at the relevant location.

    <p align="center">
    <img src="./pics/lab 5/3fsterr.jpg" width="80%" alt="Example missing-header errors in the incomplete source"/>
    </p>
    <p align="center"><i>Example missing-header errors in the incomplete source</i></p>

1. Add the missing timer header with its exact lowercase filename. Also include the declarations used for terminal output and status codes:

    ```c
    #include "xscutimer.h"
    #include "xil_printf.h"
    #include "xstatus.h"
    ```

1. Check **xparameters.h** for **XPAR_SWITCHES_DEVICE_ID**, **XPAR_BUTTONS_DEVICE_ID**, **XPAR_PS7_SCUTIMER_0_DEVICE_ID**, and **XPAR_LED_IP_S_AXI_BASEADDR**. Use your generated names if the hardware instances were named differently. Do not edit the generated header.
1. Find the blank lines after the timer-initialization comments.

    <p align="center">
    <img src="./pics/lab 5/4fillin.jpg" width="80%" alt="Fill in timer initialization, loading, auto-reload and start operations"/>
    </p>
    <p align="center"><i>Fill in timer initialization, loading, auto-reload and start operations</i></p>

1. Complete the sequence: look up the configuration, check it, initialize the timer, read the switches, load **ONE_TENTH * dip_check_prev**, enable auto-reload, and start the timer. Use the following APIs:

    ```c
    XScuTimer_LookupConfig(...);
    XScuTimer_CfgInitialize(...);
    XScuTimer_LoadTimer(...);
    XScuTimer_EnableAutoReload(...);
    XScuTimer_Start(...);
    ```

1. Set the prescaler to **0** and leave timer interrupts disabled: this is a **polled** application. Clear any stale expiration status before starting. The completed example below shows these setup calls.[^timer-example]
1. Calculate **ONE_TENTH** from the CPU frequency generated for your hardware. With prescaler zero, the private timer clock is half the CPU clock, so one tenth of a second is approximately **CPU clock / 20** timer ticks.[^timer-clock] Use:

    ```c
    #define ONE_TENTH (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 20U)
    ```

    For a 650 MHz CPU this gives **32,500,000** ticks. Do not assume that frequency when your generated hardware parameters specify a different value. For this exercise, use switch settings **0001 through 1111**; **0000** produces a zero load and is not a meaningful delay setting.

1. Locate the remaining blanks in the polling loop.

    <p align="center">
    <img src="./pics/lab 5/5morecd.jpg" width="80%" alt="Complete the switch-change and timer-expiration operations"/>
    </p>
    <p align="center"><i>Complete the switch-change and timer-expiration operations</i></p>

1. When the switch value changes, load the new timer value and reset **count** to zero. When the timer expires, clear its status, write **count** to LED register offset **0**, and increment **count**. Use **XScuTimer_LoadTimer**, **XScuTimer_ClearInterruptStatus**, and **LED_IP_mWriteReg**.
1. Compare your work with this completed version. It retains the timer, switches, buttons and binary-counter exercise, with explicit initialization checks and clock-derived timing:

```c
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "xstatus.h"
#include "led_ip.h"
#include "xscutimer.h"

/* With prescaler 0, the private timer runs at half the CPU clock. */
#define ONE_TENTH (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 20U)

XScuTimer Timer;

int main(void)
{
    XGpio dip, push;
    u32 psb_check, dip_check, dip_check_prev, count;
    int Status;
    XScuTimer_Config *ConfigPtr;
    XScuTimer *TimerInstancePtr = &Timer;

    xil_printf("-- Start of the Program --\r\n");

    if (XGpio_Initialize(&dip, XPAR_SWITCHES_DEVICE_ID) != XST_SUCCESS ||
        XGpio_Initialize(&push, XPAR_BUTTONS_DEVICE_ID) != XST_SUCCESS) {
        xil_printf("GPIO initialization failed\r\n");
        return XST_FAILURE;
    }
    XGpio_SetDataDirection(&dip, 1, 0xFFFFFFFFU);
    XGpio_SetDataDirection(&push, 1, 0xFFFFFFFFU);

    count = 0;
    ConfigPtr = XScuTimer_LookupConfig(XPAR_PS7_SCUTIMER_0_DEVICE_ID);
    if (ConfigPtr == 0) {
        xil_printf("Timer configuration not found\r\n");
        return XST_FAILURE;
    }
    Status = XScuTimer_CfgInitialize(TimerInstancePtr, ConfigPtr,
                                   ConfigPtr->BaseAddr);
    if (Status != XST_SUCCESS) {
        xil_printf("Timer init() failed\r\n");
        return XST_FAILURE;
    }

    XScuTimer_Stop(TimerInstancePtr);
    XScuTimer_SetPrescaler(TimerInstancePtr, 0);
    XScuTimer_DisableInterrupt(TimerInstancePtr);
    XScuTimer_ClearInterruptStatus(TimerInstancePtr);

    /* Use a nonzero switch value (1 to 15) for this exercise. */
    dip_check_prev = XGpio_DiscreteRead(&dip, 1);
    XScuTimer_LoadTimer(TimerInstancePtr, ONE_TENTH * dip_check_prev);
    XScuTimer_EnableAutoReload(TimerInstancePtr);
    XScuTimer_Start(TimerInstancePtr);

    while (1) {
        /* Any of the four user buttons stops the program. */
        psb_check = XGpio_DiscreteRead(&push, 1);
        if (psb_check != 0U) {
            xil_printf("Push button pressed: Exiting\r\n");
            XScuTimer_Stop(TimerInstancePtr);
            break;
        }

        dip_check = XGpio_DiscreteRead(&dip, 1);
        if (dip_check != dip_check_prev) {
            xil_printf("DIP Switch Status %x, %x\r\n",
                       (unsigned int)dip_check_prev, (unsigned int)dip_check);
            dip_check_prev = dip_check;
            XScuTimer_LoadTimer(TimerInstancePtr, ONE_TENTH * dip_check);
            XScuTimer_ClearInterruptStatus(TimerInstancePtr);
            count = 0;
        }

        if (XScuTimer_IsExpired(TimerInstancePtr)) {
            XScuTimer_ClearInterruptStatus(TimerInstancePtr);
            LED_IP_mWriteReg(XPAR_LED_IP_S_AXI_BASEADDR, 0, count);
            count++;
        }
    }
    return XST_SUCCESS;
}
```

1. Set **lab5 > Properties > C/C++ Build > Settings** to the **Debug** configuration, **None (-O0)** optimization, with debug information enabled.[^flags]
1. Use **Generate Linker Script...** for **lab5** and select the DDR region for **Code**, **Data**, and **Heap and Stack**. This new application has its own linker script; it does not automatically inherit the modified Lab 4 script.[^linker]
1. Select **Project > Build All**. Resolve all build errors and confirm that **lab5/Debug/lab5.elf** exists.

### Verify Operation in Hardware

1. Set the four switches to **0001**, with all user buttons released.
1. With the board powered off, set **JP5 to JTAG** and, for USB power, **JP7 to USB**. Connect a micro-USB cable to **JTAG PROG**, then turn on the board. See [Hardware Setup](./README.md#hardware-setup).
1. Open a serial terminal from **Window > Show View > Other...** (search for **Terminal**), or use an external serial terminal. Connect to the board's COM port using **115200 baud, 8 data bits, no parity, 1 stop bit, no flow control**. Connect before running the application.
1. Select **lab5**, then open **Run > Run Configurations...**. Create a **Single Application Debug** configuration named **lab5_hw**. Select the local hardware-server connection, **lab5_platform**, **ps7_cortexa9_0**, and **lab5/Debug/lab5.elf**, with application download enabled.[^run]
1. On **Target Setup**, enable **Reset entire system**. Enable **Program FPGA** and select the bitstream supplied by **lab5_platform**. Use the bitstream from this XSA, not from an earlier hardware design.
1. Clear **Use FSBL flow for initialization** and select this platform's **ps7_init.tcl** as the initialization file, normally under **lab5_platform/hw**. Keep PS initialization enabled; where shown, select both **Run ps7_init** and **Run ps7_post_config**.[^target]
1. Click **Apply > Run**. The launch initializes the board and downloads the application. Reuse this configuration for subsequent runs. If execution stops at `main`, click **Resume**.

1. Observe the LEDs counting in binary. The four LEDs show the four least significant bits of **count**, so the visible pattern repeats every 16 increments.
1. Change between nonzero switch settings and observe the delay change. A switch value of **1** gives approximately **0.1 seconds** per timer expiration; **2** gives **0.2 seconds**, and **15** gives **1.5 seconds**. The terminal prints the previous and current switch values in hexadecimal.

    <p align="center">
    <img src="./pics/lab 5/7termop.jpg" width="30%" alt="Example terminal output when the switch setting changes"/>
    </p>
    <p align="center"><i>Example terminal output when the switch setting changes</i></p>

1. Press any user button. The program prints its exit message and stops the private timer. Release the button before the next launch.

### Launch the Debugger

1. Terminate the previous launch connection. Open **Run > Debug Configurations...**, select the **lab5_hw** configuration created above, verify the application ELF, and click **Debug**. Enable **Stop at main** on the application's debug settings if it is not already enabled.[^debug]
1. Accept the switch to the **Debug** perspective. After download, execution should stop at **main**. Open **Variables**, **Breakpoints**, and **Memory** from **Window > Show View > Other...** as needed.
1. Set five breakpoints by double-clicking the editor's left margin beside the following executable statements. Use the statements, not the line numbers in the example figures:

    | Breakpoint | Statement or location |
    | --- | --- |
    | 1 | The first `count = 0;`, before timer initialization. |
    | 2 | The `xil_printf` inside the timer-initialization failure branch. |
    | 3 | The initial `dip_check_prev = XGpio_DiscreteRead(&dip, 1);`. |
    | 4 | The `xil_printf` inside the user-button exit branch. |
    | 5 | The `LED_IP_mWriteReg(...)` in the timer-expired branch. |

    <p align="center">
    <img src="./pics/lab 5/8bp.jpg" width="80%" alt="Example breakpoint locations around initialization"/>
    </p>
    <p align="center"><i>Example breakpoint locations around initialization</i></p>

    <p align="center">
    <img src="./pics/lab 5/9bp.jpg" width="80%" alt="Example breakpoint locations in the main loop"/>
    </p>
    <p align="center"><i>Example breakpoint locations in the main loop</i></p>

1. Click **Resume (F8)** to reach breakpoint 1. In **Variables**, **count** has not yet been initialized; its displayed value is not meaningful.
1. Click **Step Over (F6)**. The assignment executes and **count** becomes **0**.
1. Click **Resume**. A successful timer initialization skips breakpoint 2 and stops at breakpoint 3. If the failure breakpoint is reached, inspect **Status** and the platform configuration before continuing.
1. Keep the switches at **0001** and click **Step Over**. Verify that **dip_check_prev** becomes **1**.
1. Open **Memory** and click **Add Memory Monitor** (the plus button).[^memory]

    <p align="center">
    <img src="./pics/lab 5/cmemlocn.jpg" width="30%" alt="Add a memory monitor"/>
    </p>
    <p align="center"><i>Add a memory monitor</i></p>

1. Enter **0xF8F00600**, the private timer's load-register address, and select a rendering that shows 32-bit hexadecimal values. Inspect the following four registers.[^timer-registers]

    <p align="center">
    <img src="./pics/lab 5/dmonitormem.jpg" width="30%" alt="Monitor the private timer register block"/>
    </p>
    <p align="center"><i>Monitor the private timer register block</i></p>

    | Address | Offset | Register |
    | --- | --- | --- |
    | `0xF8F00600` | `0x00` | Load value. |
    | `0xF8F00604` | `0x04` | Current countdown value. |
    | `0xF8F00608` | `0x08` | Control register. |
    | `0xF8F0060C` | `0x0C` | Expiration status. |

    Check **XPAR_PS7_SCUTIMER_0_BASEADDR** in **xparameters.h** and the offsets in **xscutimer_hw.h**. The load and current-counter registers are separate registers.

1. Step over **XScuTimer_LoadTimer**. Confirm that the load register contains **ONE_TENTH * dip_check_prev**. For a 650 MHz CPU and switch value **1**, the value is **0x01EFE920**. In a byte-oriented little-endian view, the bytes appear as **20 E9 EF 01**; this is not a different 32-bit value.
1. Click **Resume**. Execution passes the auto-reload and start calls and stops at breakpoint 5 when the timer has expired. Inspect the current-count register, which is different from the reload value during counting. Values observed while the CPU is halted are not a measurement of the program's real-time interval.
1. Click **Step Over** to write zero to the LED register. All four LEDs should turn off because **count** is initially **0**. Step over **count++** and check that it becomes **1**.
1. Disable breakpoint 5 in the **Breakpoints** view and click **Resume**. Leave the button-exit breakpoint enabled. The LEDs now count without stopping at each timer expiration.
1. Change between nonzero switch values and observe the rate change.
1. Press a user button. Execution stops at breakpoint 4 **before** the exit message and timer-stop call. Step over the message and **XScuTimer_Stop**. Refresh the memory view and verify that the timer-enable bit (**bit 0**) in the control register is clear. The timer is not stopped merely by arriving at the earlier breakpoint.
1. Terminate the debug session, exit Vitis and Vivado, and power off the board.

## Conclusion

The application uses the CPU's private timer in polled auto-reload mode to control a binary LED counter. The debugger exercise follows initialization and expiry, inspects local variables and distinguishes the timer's load, counter, control and status registers.

[timer-header]: https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/XilinxProcessorIPLib/drivers/scutimer/src/xscutimer.h
[timer-example]: https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/XilinxProcessorIPLib/drivers/scutimer/examples/xscutimer_polled_example.c

## References and Validation

Original exercise and figures: [XUP - Lab 5](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab5.md). Image paths remain relative to the original `pics/lab 5/` directory. Source exercises use the original `sources/` tree.

**Validation:** Documentation and static checks only. This adaptation has not been built in Vivado/Vitis or tested on a physical Zybo board. The retained figures are illustrations from the original workbook, not a new test record.

[^application]: AMD/Xilinx, [Application project wizard](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Standalone-Application-Project).
[^bsp]: AMD/Xilinx, [Board Support Package settings](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Board-Support-Package-Settings-Page).
[^repository]: AMD/Xilinx, [Adding a software repository](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Adding-the-Software-Repository).
[^timer]: AMD/Xilinx, [SCU timer driver, 2022.2](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/XilinxProcessorIPLib/drivers/scutimer/src/xscutimer.h).
[^timer-example]: AMD/Xilinx, [Polled SCU timer example, 2022.2](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/XilinxProcessorIPLib/drivers/scutimer/examples/xscutimer_polled_example.c).
[^timer-clock]: AMD/Xilinx, [Zynq-7000 private-timer clocking, UG585](https://docs.amd.com/r/en-US/ug585-zynq-7000-SoC-TRM/Clocking?contentId=6Os6wbxQry_sl2Tfx4OevA).
[^flags]: AMD/Xilinx, [Debug and optimization settings](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Specifying-Debug-and-Optimization-Compiler-Flags).
[^linker]: AMD/Xilinx, [Generating an application linker script](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Generating-a-Linker-Script-for-an-Application).
[^run]: AMD/Xilinx, [Launch configurations](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Launch-Configurations).
[^target]: AMD/Xilinx, [Target setup and PS initialization](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Target-Setup-Page).
[^debug]: AMD/Xilinx, [Standalone application debugging](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Standalone-Application-Debug-Using-Xilinx-System-Debugger).
[^memory]: AMD/Xilinx, [Viewing memory contents](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Viewing-Memory-Contents).
[^timer-registers]: AMD/Xilinx, [SCU timer register definitions, 2022.2](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/XilinxProcessorIPLib/drivers/scutimer/src/xscutimer_hw.h).

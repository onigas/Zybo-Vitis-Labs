# Writing Basic Software Application

> **Target:** original Digilent Zybo (legacy), Vivado 2022.2 and Vitis 2022.2 (Eclipse-based IDE).
> Use the **Zybo** board preset, not Zybo Z7. The original workbook figures are retained where they illustrate the hardware, code or results; their appearance and example values can differ from your project.
> **Paths:** `{labs}` is your working directory; `{sources}` is the original repository's `sources` directory. Keep this Markdown file in the repository root, alongside `pics/` and `sources/`.
> Follow the original hardware sequence below. Where a figure shows another board, use the Zybo-specific settings in the text. Continue from your own project built with Vivado 2022.2. An older solution project must first be copied, upgraded and rebuilt for the correct board.

## Objectives

After completing this lab, you will be able to:
* Write a Vitis application that accesses a custom peripheral through its driver.
* Develop a linker script and partition the executable between DDR3 and BRAM.
* Generate an ELF executable, download the design, and test it on Zybo.

## Steps

### Opening the Project

1. Start Vivado and open the **lab3.xpr** project from the previous laboratory, or a prepared lab3 solution.
1. Select **File > Save Project As...**. Enter **lab4**, check **Create Project Subdirectory**, set the project directory to **{labs}**, and click **OK**.

    This creates a lab4 copy of the project and its associated files. No new peripheral is added in this laboratory.

### Export the Hardware to Vitis

1. Confirm that bitstream generation completed successfully for this design. If the copied project has no current bitstream, run **Generate Bitstream** and wait for synthesis and implementation to finish.
1. Select **File > Export > Export Hardware**. Choose **Fixed** if a platform-type page appears, and enable **Include bitstream**.[^xsa]
1. Export **system_wrapper.xsa** to **{labs}/lab4**, review the summary and click **Finish**. This XSA must describe this lab's hardware and include its matching bitstream.

### Create the Application in Vitis

1. Open **Vitis 2022.2** and select **{labs}/lab4/vitis_workspace** as the workspace. Click **Launch**.
1. Select **File > New > Application Project**. Click **Next** on the welcome page, if it appears.[^application]
1. On the **Platform** page, select **Create a new platform from hardware (XSA)**. Browse to **{labs}/lab4/system_wrapper.xsa** and name the platform **lab4_platform**. Clear **Generate boot components**, if offered; this lab uses JTAG and the PS initialization script. Click **Next**.
1. Set the application name to **lab4**, keep **lab4_system** as the system-project name, and select **ps7_cortexa9_0**. Click **Next**.
1. On the **Domain** page, select **standalone**, **32-bit** if shown, and language **C**. Keep the proposed domain name and click **Next**.
1. Select **Empty Application (C)** and click **Finish**. This one wizard creates the platform, standalone domain/BSP, system project and application.
1. Open **lab4_platform > platform.spr**, select the application's standalone domain, and open **Board Support Package > Modify BSP Settings**. Under **standalone**, set **stdin** and **stdout** to **ps7_uart_1**. Apply the settings.[^bsp]

### Import the Application and Examine the GPIO Driver

1. Expand **lab4**, right-click **src**, and select **Import > General > File System**.
1. Browse to **{sources}/lab4**, select **lab4.c**, and click **Finish**. The starter file deliberately contains device-name placeholders; complete the following steps before expecting a successful application build.
1. Right-click **lab4_platform** and select **Build Project** to generate the BSP headers and drivers.
1. Open **lab4.c**. Use **Open Declaration** on the `xgpio.h` include to inspect the generated GPIO driver header. The Outline view helps navigate the source. Review the GPIO API declarations and comments; the versioned driver source is also available in the references.[^gpio]

    <p align="center">
    <img src="./pics/lab 4/2outlinevw.JPG" width="40%" alt="Outline view for the application source"/>
    </p>
    <p align="center"><i>Outline view for the application source</i></p>

1. Find the functions used to initialize a GPIO, set its direction and read its input:

    ```c
    int XGpio_Initialize(XGpio *InstancePtr, u16 DeviceId);
    void XGpio_SetDataDirection(XGpio *InstancePtr,
                               unsigned Channel, u32 DirectionMask);
    u32 XGpio_DiscreteRead(XGpio *InstancePtr, unsigned Channel);
    ```

    **InstancePtr** identifies the driver's state structure, allocated by your application. **DeviceId** selects one hardware instance. **Channel** is 1 or 2; these single-channel peripherals use **1**. In the direction mask, a **1** selects input and a **0** selects output.

1. Open **xparameters.h** through its include in the application. Find **XPAR_SWITCHES_DEVICE_ID**, **XPAR_BUTTONS_DEVICE_ID** and the corresponding base-address macros. The numerical IDs are generated from the hardware and need not match another project.
1. Replace **XPAR_DIP_DEVICE_ID** in **lab4.c** with **XPAR_SWITCHES_DEVICE_ID**.
1. Replace **XPAR_PUSH_DEVICE_ID** with **XPAR_BUTTONS_DEVICE_ID**. Add `#include "xil_printf.h"` for the serial printing calls, then save the file. Do not modify **xparameters.h**.

### Assign the Custom LED Driver

1. In Windows Explorer, locate the **drivers** directory inside the packaged IP created in Lab 3. With the original naming, it is under **{labs}/led_ip/ip_repo/led_ip_1.0**. The actual directory is the IP output location shown by **Review and Package** in Lab 3.
1. In Vitis, select **Xilinx > Software Repositories**. Under **Local Repositories**, add the directory that **contains** `drivers`, for example **{labs}/led_ip/ip_repo/led_ip_1.0**. Click **Apply and Close**.[^repository]

    The expected structure is:

    ```text
    led_ip_1.0/                  <- add this directory
      drivers/
        led_ip_v1_0/
          data/
            led_ip.mdd
            led_ip.tcl
          src/
            led_ip.h
            led_ip.c
            ...
    ```

    The directory names can include a different generated version suffix. Select the parent of **drivers**, not its **src** subdirectory. The source and driver-description files are both needed for driver discovery.[^repository-layout]

1. Open **lab4_platform > platform.spr**, select the application's standalone domain, and open **Board Support Package > Modify BSP Settings > drivers**.
1. For the **led_ip** hardware instance, select the **led_ip** driver if it is not already assigned. Leave the **switches** and **buttons** instances assigned to **gpio**. Apply the settings and rebuild **lab4_platform**.[^bsp]

    <p align="center">
    <img src="./pics/lab 4/3assigndrv.JPG" width="80%" alt="Example led_ip driver assignment; instance suffixes and versions can differ"/>
    </p>
    <p align="center"><i>Example led_ip driver assignment; instance suffixes and versions can differ</i></p>

    If **led_ip** is missing from the driver list, first check the software-repository directory and that its `data` and `src` files were generated. Adding the IP repository in Vivado does not by itself configure a Vitis software repository.

### Examine the Driver Code

1. Open **drivers/led_ip_v1_0/src/led_ip.c** in the packaged IP. Inspect the implementation supplied by the wizard.
1. Open **led_ip.h** and find **LED_IP_mWriteReg** and **LED_IP_mReadReg**. They perform 32-bit accesses using **Xil_Out32** and **Xil_In32**. The arguments are the peripheral base address, register offset in bytes and, for a write, the data value.
1. Return to **lab4.c**. Add the LED header:

    ```c
    #include "led_ip.h"
    ```

1. Open **xparameters.h** and locate the base-address macro for **led_ip/S_AXI**. With the instance name used in Lab 3 it is **XPAR_LED_IP_S_AXI_BASEADDR**. Use the exact generated name if you named the instance differently.
1. At the LED-output comment, before the delay loop, add:

    ```c
    LED_IP_mWriteReg(XPAR_LED_IP_S_AXI_BASEADDR, 0, dip_check);
    ```

    Register offset **0** drives the LED user logic in Lab 3. On Zybo, the four least significant bits are visible on the four user LEDs.

1. Compare your completed program with the following. It keeps the original polling and delay-loop exercise, with explicit headers and initialization checks:

```c
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "xstatus.h"
#include "led_ip.h"

int main(void)
{
    XGpio dip, push;
    int i;
    u32 psb_check, dip_check;

    xil_printf("-- Start of the Program --\r\n");

    if (XGpio_Initialize(&dip, XPAR_SWITCHES_DEVICE_ID) != XST_SUCCESS ||
        XGpio_Initialize(&push, XPAR_BUTTONS_DEVICE_ID) != XST_SUCCESS) {
        xil_printf("GPIO initialization failed\r\n");
        return XST_FAILURE;
    }
    XGpio_SetDataDirection(&dip, 1, 0xFFFFFFFFU);
    XGpio_SetDataDirection(&push, 1, 0xFFFFFFFFU);

    while (1) {
        psb_check = XGpio_DiscreteRead(&push, 1);
        xil_printf("Push Buttons Status %x\r\n", (unsigned)psb_check);
        dip_check = XGpio_DiscreteRead(&dip, 1);
        xil_printf("DIP Switch Status %x\r\n", (unsigned)dip_check);

        LED_IP_mWriteReg(XPAR_LED_IP_S_AXI_BASEADDR, 0, dip_check);

        /* Keep -O0 for this original software-delay exercise. */
        for (i = 0; i < 9999999; ++i) {
        }
    }
}
```

### Build and Analyze the Executable

1. Right-click **lab4 > Properties > C/C++ Build > Settings**. In the **Debug** configuration, set compiler optimization to **None (-O0)** and retain debug information. Apply the changes. The original empty delay loop is not a calibrated timer and may be removed by optimization.[^flags]
1. Right-click **lab4 > Generate Linker Script...**. For this first build, explicitly place **Code**, **Data**, and **Heap and Stack** in the **DDR** region listed in your hardware map, usually **ps7_ddr_0**. Generate the script and confirm overwriting **src/lscript.ld** if prompted.[^linker]
1. Select **Project > Build All** and verify **lab4/Debug/lab4.elf** is generated without errors.
1. Open **Vitis Shell** from the **Xilinx** menu. Change to **{labs}/lab4/vitis_workspace/lab4/Debug**. Run:

    ```sh
    arm-none-eabi-objdump -h lab4.elf
    ```

    Use the Vitis 2022.2 toolchain shell, not an unrelated system compiler. The option is an ordinary ASCII **-h**.[^shell]

1. Examine the section sizes and addresses. Confirm that the allocated **.text**, **.heap**, and **.stack** sections are in DDR. Debug-information sections with zero addresses are not memory allocations on the board.

    <p align="center">
    <img src="./pics/lab 4/4shellop.JPG" width="80%" alt="Example object dump with application, heap and stack in DDR3"/>
    </p>
    <p align="center"><i>Example object dump with application, heap and stack in DDR3</i></p>

### Verify in Hardware

1. With the board powered off, set **JP5 to JTAG** and, for USB power, **JP7 to USB**. Connect a micro-USB cable to **JTAG PROG**, then turn on the board. See [Hardware Setup](./README.md#hardware-setup).
1. Open a serial terminal from **Window > Show View > Other...** (search for **Terminal**), or use an external serial terminal. Connect to the board's COM port using **115200 baud, 8 data bits, no parity, 1 stop bit, no flow control**. Connect before running the application.
1. Select **lab4**, then open **Run > Run Configurations...**. Create a **Single Application Debug** configuration named **lab4_hw**. Select the local hardware-server connection, **lab4_platform**, **ps7_cortexa9_0**, and **lab4/Debug/lab4.elf**, with application download enabled.[^run]
1. On **Target Setup**, enable **Reset entire system**. Enable **Program FPGA** and select the bitstream supplied by **lab4_platform**. Use the bitstream from this XSA, not from an earlier hardware design.
1. Clear **Use FSBL flow for initialization** and select this platform's **ps7_init.tcl** as the initialization file, normally under **lab4_platform/hw**. Keep PS initialization enabled; where shown, select both **Run ps7_init** and **Run ps7_post_config**.[^target]
1. Click **Apply > Run**. The launch initializes the board and downloads the application. Reuse this configuration for subsequent runs. If execution stops at `main`, click **Resume**.

1. Change the DIP switches. Their four-bit value should appear on the user LEDs. Press the buttons and check that both switch and button values are printed in the serial terminal.

    <p align="center">
    <img src="./pics/lab 4/5sdkop.JPG" width="30%" alt="Example switch and push-button values in the serial terminal"/>
    </p>
    <p align="center"><i>Example switch and push-button values in the serial terminal</i></p>

### Place the Heap and Stack in BRAM

1. Suspend and terminate the current launch connection before replacing the executable.
1. Right-click **lab4 > Generate Linker Script...** and select the **Basic** page.[^linker]
1. Keep **Code** and **Data** in **ps7_ddr_0**. Set **Heap and Stack** to the memory region belonging to **axi_bram_ctrl_0**, for example **axi_bram_ctrl_0_Mem0**. Select the region from the hardware-map list; a C base-address macro is not a linker-region name.
1. Set **Heap Size** to **1 KB (0x400)** and **Stack Size** to **1 KB (0x400)**, as in the original exercise. Generate the script and confirm the overwrite.

    <p align="center">
    <img src="./pics/lab 4/6bram.JPG" width="71%" alt="Example DDR code and data with BRAM heap and stack; use your generated region names"/>
    </p>
    <p align="center"><i>Example DDR code and data with BRAM heap and stack; use your generated region names</i></p>

1. Build again and inspect **lscript.ld** and the linker map. The BRAM region must be **8 KiB**, and all allocations into it, including any processor-mode stacks emitted by the linker script, must fit. Do not ignore a region-overflow error or enlarge the declared memory beyond the hardware size.
1. Run the same object-dump command again:

    ```sh
    arm-none-eabi-objdump -h lab4.elf
    ```

1. Confirm that **.heap** and **.stack** now lie in the BRAM address range, while **.text** and application data remain in DDR. Use your own Address Editor and linker map to identify the range; do not assume the example addresses below match your project.

    <p align="center">
    <img src="./pics/lab 4/7shellop.jpg" width="60%" alt="Example object dump after moving heap and stack to BRAM"/>
    </p>
    <p align="center"><i>Example object dump after moving heap and stack to BRAM</i></p>

1. Relaunch **lab4_hw** to download the newly linked **lab4.elf**. Check again that the switches drive the LEDs and that the terminal reports switch and button values.
1. Compare the response with the all-DDR build. This exercise demonstrates memory placement, not a controlled performance benchmark; the delay loop, UART output, optimization and memory attributes all affect the observed rate.
1. Suspend the target and terminate the launch connection. Exit Vitis and Vivado, then power off the board.

## Conclusion

The application uses generated hardware identifiers and a custom peripheral driver to read the GPIOs and update the LED register. The linker script first places the program in DDR, then moves the heap and stack into the 8 KiB BRAM while leaving code and data in DDR. Both builds are inspected before they are tested on the board.

## References and Validation

Original exercise and figures: [XUP - Lab 4](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab4.md). Image paths remain relative to the original `pics/lab 4/` directory. Source exercises use the original `sources/` tree.

**Validation:** Documentation and static checks only. This adaptation has not been built in Vivado/Vitis or tested on a physical Zybo board. The retained figures are illustrations from the original workbook, not a new test record.

[^xsa]: AMD/Xilinx, [Hardware export](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Hardware-Design-XSA-File).
[^application]: AMD/Xilinx, [Application project wizard](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Standalone-Application-Project).
[^bsp]: AMD/Xilinx, [Board Support Package settings](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Board-Support-Package-Settings-Page).
[^gpio]: AMD/Xilinx, [AXI GPIO driver, 2022.2](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/XilinxProcessorIPLib/drivers/gpio/src/xgpio.h).
[^repository]: AMD/Xilinx, [Adding a software repository](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Adding-the-Software-Repository).
[^repository-layout]: AMD/Xilinx, [Software repository structure](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Software-Repository).
[^flags]: AMD/Xilinx, [Debug and optimization settings](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Specifying-Debug-and-Optimization-Compiler-Flags).
[^linker]: AMD/Xilinx, [Generating an application linker script](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Generating-a-Linker-Script-for-an-Application).
[^shell]: AMD/Xilinx, [Vitis Shell](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Vitis-Shell).
[^run]: AMD/Xilinx, [Launch configurations](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Launch-Configurations).
[^target]: AMD/Xilinx, [Target setup and PS initialization](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Target-Setup-Page).

# Adding IP cores in PL

> **Target:** original Digilent Zybo (legacy), Vivado 2022.2 and Vitis 2022.2 (Eclipse-based IDE).
> Use the **Zybo** board preset, not Zybo Z7. The original workbook figures are retained where they illustrate the hardware, code or results; their appearance and example values can differ from your project.
> **Paths:** `{labs}` is your working directory; `{sources}` is the original repository's `sources` directory. Keep this Markdown file in the repository root, alongside `pics/` and `sources/`.
> Follow the original hardware sequence below. Where a figure shows another board, use the Zybo-specific settings in the text. Continue from your own project built with Vivado 2022.2. An older solution project must first be copied, upgraded and rebuilt for the correct board.

## Objectives

After completing this lab, you will be able to:
* Configure the GP Master port of the PS to connect to IP in the PL.
* Add additional IP to a hardware design.
* Set up compiler settings and run a GPIO application in Vitis.

## Steps

### Open the Project

1. Open the previous project, or a prepared lab1 project from **{labsolutions}**, and save it as lab2. Open the Block Design.
1. Start Vivado, if necessary, and open **lab1.xpr** using **Open Project** on the Getting Started page.
1. Select **File > Save Project As...**. Enter **lab2**, keep **Create Project Subdirectory** checked, set the project directory to **{labs}**, and click **OK**.

    This creates the lab2 directory and saves the project and associated files with the new name.

### Add Two Instances of GPIO

1. In Sources, expand **system_wrapper** and double-click **system.bd (system_i)** to open IP Integrator.
1. Double-click the Zynq block to open its configuration window.
1. Select **PS-PL Configuration**.
1. Expand **AXI Non Secure Enablement > GP Master AXI Interfaces** and enable **M_AXI_GP0**.
1. Expand **General > Enable Clock Resets** and select **FCLK_RESET0_N**.
1. Select **Clock Configuration > PL Fabric Clocks**. Enable **FCLK_CLK0**, with requested frequency **100.000000 MHz**, and click **OK**.
1. Observe the added **M_AXI_GP0**, **M_AXI_GP0_ACLK**, **FCLK_CLK0**, and **FCLK_RESET0_N** connections. Click **Regenerate Layout** to redraw the diagram.

    <p align="center">
    <img src="./pics/lab 2/1BlockAXI.JPG" width="40%" alt="Zynq system with AXI and clock interfaces"/>
    </p>
    <p align="center"><i>Zynq system with AXI and clock interfaces</i></p>

1. Right-click in the Diagram, select **Add IP**, and search for **AXI GPIO**.
1. Double-click **AXI GPIO** to add the core.
1. Select the core and change its instance name to **switches** in Properties.

    <p align="center">
    <img src="./pics/lab 2/2AXIblockadd.JPG" width="50%" alt="Change AXI GPIO default name"/>
    </p>
    <p align="center"><i>Change AXI GPIO default name</i></p>

1. Double-click **switches** to open its customization window.
1. On **Board Interface**, select **sws_4bits** for Zybo. The original workbook also lists eight switches for ZedBoard and two for PYNQ-Z2; those are not the settings for this board.
1. Open **IP Configuration** and check that the GPIO width is **4**. Leave **Enable Dual Channel** and **Enable Interrupt** unchecked.
1. Click **OK**.
1. Click **Run Connection Automation** and select **/switches/S_AXI**.
1. Click **OK** to connect the master and slave interfaces.

    <p align="center">
    <img src="./pics/lab 2/3Dsgnswauto.JPG" width="60%" alt="Design with switches automatically connected"/>
    </p>
    <p align="center"><i>Design with switches automatically connected</i></p>

    **Processor System Reset** and **AXI Interconnect** are added automatically. Rearrange the blocks or regenerate the layout as needed.

1. Add another **AXI GPIO** and name it **buttons**.
1. Double-click **buttons**, select the **btns_4bits** board interface for Zybo, and click **OK**. Connect this instance manually in the following steps.
1. Double-click **AXI Interconnect**, named **ps7_0_axi_periph**, set **Number of Master Interfaces** to **2**, and click **OK**.

    <p align="center">
    <img src="./pics/lab 2/4AXIrecust.JPG" width="60%" alt="Add master port to AXI Interconnect"/>
    </p>
    <p align="center"><i>Add master port to AXI Interconnect</i></p>

1. Drag **buttons/S_AXI** to **ps7_0_axi_periph/M01_AXI**. Release when the valid-connection indicator appears.
1. Connect the remaining ports:

    ```text
    buttons/s_axi_aclk              -> processing_system7_0/FCLK_CLK0
    buttons/s_axi_aresetn           -> Processor System Reset/peripheral_aresetn
    ps7_0_axi_periph/M01_ACLK       -> processing_system7_0/FCLK_CLK0
    ps7_0_axi_periph/M01_ARESETN    -> Processor System Reset/peripheral_aresetn
    ```

    The diagram should now resemble the following figure.

    <p align="center">
    <img src="./pics/lab 2/5AXIbuttsw.JPG" width="70%" alt="System Assembly View after Adding the Peripherals"/>
    </p>
    <p align="center"><i>System Assembly View after Adding the Peripherals</i></p>

1. Select **Address Editor** and expand **processing_system7_0 > Data > Unmapped Slaves**, if necessary.
1. The switches peripheral already has an address. Right-click the unmapped **buttons** register segment and select **Assign Address**.

    Both peripherals occupy the GP0 address window, **0x40000000 to 0x7FFFFFFF**. Use the addresses assigned in your project.

    <p align="center">
    <img src="./pics/lab 2/6AXIAdd.jpg" width="70%" alt="Peripherals Memory Map"/>
    </p>
    <p align="center"><i>Peripherals Memory Map</i></p>

### Make GPIO Peripheral Connections External

1. Return to Diagram. The switches connection will be made manually.
1. Right-click the **GPIO** interface of **switches** and select **Make External**. The board interface associates the port with the board pin constraints.
1. Select the created port and rename it **switches**. Its width follows the GPIO configuration.
1. For **buttons**, click **Run Connection Automation**.
1. Select **btns_4bits** for Zybo in the options.
1. Click **OK**.
1. Rename the resulting external port **buttons**.
1. Run **Tools > Validate Design** and resolve any errors.

    <p align="center">
    <img src="./pics/lab 2/7Finaldsgn.JPG" width="80%" alt="Completed design"/>
    </p>
    <p align="center"><i>Completed design</i></p>

1. Click **Run Synthesis** in Flow Navigator. Save if prompted. After synthesis completes, select **Open Synthesized Design** and click **OK**.
1. Select **I/O Planning** from the Layout menu.

    <p align="center">
    <img src="./pics/lab 2/8iop.jpg" width="30%" alt="Switch to the IO planning view"/>
    </p>
    <p align="center"><i>Switch to the IO planning view</i></p>

1. In **I/O Ports**, expand **buttons_tri_i** and **switches_tri_i**. Check that the four inputs in each group have package pins assigned and that their I/O standard is **LVCMOS33** for Zybo. If board automation has not supplied the constraints, correct the board-interface assignment or add the corresponding legacy Zybo constraints before generating the bitstream.

### Generate Bitstream and Export to Vitis

1. Click **Generate Bitstream**. Accept the prompts to save the design and run implementation.
1. Wait for successful completion, then click **Cancel** in the completion dialog.
1. Select **File > Export > Export Hardware**. Choose **Fixed** if asked, and select **Include bitstream**. This design contains PL logic, so the bitstream is required.[^xsa]
1. Export **system_wrapper.xsa** to **{labs}/lab2**, then click **Finish**.

### Create the Application in Vitis

1. Open **Vitis 2022.2** and select **{labs}/lab2/vitis_workspace** as the workspace. Click **Launch**.
1. Select **File > New > Application Project**. Click **Next** on the welcome page, if it appears.[^application]
1. On the **Platform** page, select **Create a new platform from hardware (XSA)**. Browse to **{labs}/lab2/system_wrapper.xsa** and name the platform **lab2_platform**. Clear **Generate boot components**, if offered; this lab uses JTAG and the PS initialization script. Click **Next**.
1. Set the application name to **TestApp**, keep **TestApp_system** as the system-project name, and select **ps7_cortexa9_0**. Click **Next**.
1. On the **Domain** page, select **standalone**, **32-bit** if shown, and language **C**. Keep the proposed domain name and click **Next**.
1. Select **Empty Application (C)** and click **Finish**. This one wizard creates the platform, standalone domain/BSP, system project and application.
1. Open **lab2_platform > platform.spr**, select the application's standalone domain, and open **Board Support Package > Modify BSP Settings**. Under **standalone**, set **stdin** and **stdout** to **ps7_uart_1**. Apply the settings.[^bsp]

### Import and Build TestApp

1. Expand **TestApp**, right-click **src**, and select **Import > General > File System**.
1. Browse to **{sources}/lab2**, select **lab2.c**, and click **Finish**. Keep this original GPIO polling exercise.
1. Open **lab2.c** and add the declarations for the functions it already uses:

    ```c
    #include "xil_printf.h"
    #include "sleep.h"
    ```

1. Build the platform using **lab2_platform > Build Project**, then open **xparameters.h** from the application's include line using **Open Declaration**. Check that **XPAR_SWITCHES_DEVICE_ID** and **XPAR_BUTTONS_DEVICE_ID** are defined. Do not replace device IDs with base addresses or edit the generated header.
1. Right-click **TestApp > Properties > C/C++ Build > Settings**. For the **Debug** configuration, select the compiler's **Optimization** page and use **None (-O0)**. On **Debugging**, retain debug information. Apply the settings.[^flags]
1. Select **Project > Build All**. Verify that **TestApp/Debug/TestApp.elf** is produced without build errors.

### Test in Hardware

1. With the board powered off, set **JP5 to JTAG** and, for USB power, **JP7 to USB**. Connect a micro-USB cable to **JTAG PROG**, then turn on the board. See [Hardware Setup](./README.md#hardware-setup).
1. Open a serial terminal from **Window > Show View > Other...** (search for **Terminal**), or use an external serial terminal. Connect to the board's COM port using **115200 baud, 8 data bits, no parity, 1 stop bit, no flow control**. Connect before running the application.
1. Select **TestApp**, then open **Run > Run Configurations...**. Create a **Single Application Debug** configuration named **TestApp_hw**. Select the local hardware-server connection, **lab2_platform**, **ps7_cortexa9_0**, and **TestApp/Debug/TestApp.elf**, with application download enabled.[^run]
1. On **Target Setup**, enable **Reset entire system**. Enable **Program FPGA** and select the bitstream supplied by **lab2_platform**. Use the bitstream from this XSA, not from an earlier hardware design.
1. Clear **Use FSBL flow for initialization** and select this platform's **ps7_init.tcl** as the initialization file, normally under **lab2_platform/hw**. Keep PS initialization enabled; where shown, select both **Run ps7_init** and **Run ps7_post_config**.[^target]
1. Click **Apply > Run**. The launch initializes the board and downloads the application. Reuse this configuration for subsequent runs. If execution stops at `main`, click **Resume**.

1. Change the switches and press the user buttons. The serial terminal should display the switch and button values in hexadecimal. With four inputs in each group, the input values range from **0x0** to **0xF**. The existing `sleep(1)` call spaces the reports by approximately one second.

    <p align="center">
    <img src="./pics/lab 2/aop.jpg" width="30%" alt="Example GPIO application output"/>
    </p>
    <p align="center"><i>Example GPIO application output</i></p>

1. Suspend the target when finished, then terminate the launch connection. Close Vitis and Vivado and power off the board. Disconnecting a debug session alone is not a substitute for stopping a running target.

## Conclusion

Two AXI GPIO peripherals have been connected to the Zynq Processing System through the GP0 interface. The exported hardware is used by a Vitis application that reads the board's switches and buttons and reports their state through UART.

## References and Validation

Original exercise and figures: [XUP - Lab 2](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab2.md). Image paths remain relative to the original `pics/lab 2/` directory. Source exercises use the original `sources/` tree.

**Validation:** Documentation and static checks only. This adaptation has not been built in Vivado/Vitis or tested on a physical Zybo board. The retained figures are illustrations from the original workbook, not a new test record.

[^xsa]: AMD/Xilinx, [Hardware export](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Hardware-Design-XSA-File).
[^application]: AMD/Xilinx, [Application project wizard](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Standalone-Application-Project).
[^bsp]: AMD/Xilinx, [Board Support Package settings](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Board-Support-Package-Settings-Page).
[^flags]: AMD/Xilinx, [Debug and optimization settings](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Specifying-Debug-and-Optimization-Compiler-Flags).
[^run]: AMD/Xilinx, [Launch configurations](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Launch-Configurations).
[^target]: AMD/Xilinx, [Target setup and PS initialization](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Target-Setup-Page).

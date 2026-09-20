# Use Vivado to build an Embedded System


> **Target:** original Digilent Zybo (legacy), Vivado 2022.2 and Vitis 2022.2 (the Eclipse-based IDE).
> The original Vivado procedure and figures are retained. The screenshots were taken with an older release and may show another board; select **Zybo**, not Zybo Z7 or PYNQ, and use its board preset. Make sure the legacy Zybo board files are installed before starting.
> In the Vivado 2022.2 New Project wizard, clear **Do not specify sources at this time** on the RTL Project page to show the source pages used below. Some empty pages may be skipped by the wizard. With Verilog selected, the wrapper is `.v`, even if an original screenshot shows `.vhd`.
> The software section uses Vitis 2022.2. The PYNQ-specific note retained below does not apply to Zybo. Keep the original `pics/` directory beside this file.

## Objectives

After completing this lab, you will be able to:
*	Create a Vivado project for a Zynq system.
*	Use the IP Integrator to create a hardware system
*	Use Vitis 2022.2 to create a standard memory test project
*	Run the test application on the board and hence verify hardware functionality
## Steps
### Create a Vivado Project
1.	Open Vivado by selecting **Start > All Programs > Xilinx Design Tools > Vivado 2022.2 > Vivado 2022.2**
1.	Click **Create New Project** to start the wizard. You will see the Create a New Vivado Project dialog box. Click Next.
1.	Click the Browse button of the Project Location field of the New Project and browse to **{labs}**, and click Select.
1.	Enter **lab1** in the Project Name field.  Make sure that the Create Project Subdirectory box is checked.  Click Next.
1.	In the Project Type form select **RTL Project**, and click Next
1.	In the Add Sources form, select Verilog as the Target language and **Mixed** as the Simulator language, and click Next
1.	Click Next two more times to skip Adding Existing IP and Add Constraints
1.	In the Default Part window, select the Boards tab, and depending on the board you are using, (if you can't find the board you are looking for, refer to [README.md](./README.md) for setup) and click Next.
    <p align="center">
    <img src ="./pics/lab 1/1BoardsandParts.JPG" width="60%" height="80%"/>
    </p>
    <p align = "center">
    <i>Boards and Parts Selection</i>
    </p>

1.	Check the Project Summary (should be similar to what you see below) and click Finish to create an empty Vivado project.
    <p align="center">
    <img src ="./pics/lab 1/2ProjectSummary.JPG" width="60%" height="80%"/>
    </p>
    <p align = "center">
    <i>Project Summary</i>
    </p>
### Creating the System Using the IP Integrator

1.	In the Flow Navigator, click **Create Block Design** under IP Integrator

    <p align="center">
    <img src ="./pics/lab 1/3CreateIP.JPG" width="20%" height="20%"/>
    </p>
    <p align = "center">
    <i>Create IP Integrator Block Diagram</i>
    </p>

1.	Enter **system** for the design name and click OK
1.	Right-click anywhere in the Diagram workspace and select **Add IP**.
    <p align="center">
    <img src ="./pics/lab 1/4AddIP.jpg" width="60%" height="80%"/>
    </p>
    <p align = "center">
    <i>Add IP to Block Diagram</i>
    </p>


1.	Once the **IP Catalog** opens, type “zyn” into the Search bar, find and double click on **ZYNQ7 Processing System** entry, or click on the entry and hit the Enter key to add it to the design.
1.	Notice the message at the top of the Diagram window in a green label saying that Designer Assistance available. Click **Run Block Automation**.

1.	A new window pops up called the Run Block Automation window. In it, select /processing_system7_0, leave the default settings and click OK
1. Once Block Automation has been complete, notice that ports have been automatically added for the DDR and Fixed IO, and some additional ports are now visible. The imported configuration for the Zynq related to the board has been applied which will now be modified. The block should finally look like this:

    <p align="center">
    <img src ="./pics/lab 1/5Zynqblck.JPG" width="90%" height="80%"/>
    </p>
    <p align = "center">
     <i> Zynq Block with DDR and Fixed IO ports</i>
    </p>
1. Double-click on the added block to open its **Customization** window.
   Notice now the Customization window shows selected peripherals (with tick marks). This is the default configuration for the board applied by the block automation.
###	Configure the processing block with just UART 1 peripheral enabled.

1.	A block diagram of the Zynq should now be open again, showing various configurable blocks of the **Processing System**.
1. At this stage, the designer can click on various configurable blocks (highlighted in green) and change the system configuration.
1.	Click on one of the peripherals (in green) in the **IOP Peripherals** block of the Zynq Block Design, or select the MIO Configuration tab on the left to open the configuration form
1.	Expand **I/O peripherals** if necessary, and ensure all the following I/O peripherals are deselected except UART 1.

    >**Note :** Select UART 0 for PYNQ-Z2 instead of UART 1


    i.e. Remove: &nbsp; <i>ENET

    &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; USB 0
    &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; SD 0

    &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Expand **GPIO** to deselect GPIO MIO

    &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Expand **Memory Interfaces** to deselect Quad SPI Flash

    &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Expand **Application Processor Unit** to disable Timer 0.
    </i>
    <p align="center">
    <img src ="./pics/lab 1/6UART.jpg" width="90%" height="80%"/>
    </p>
    <p align = "center">
    <i> Selecting only UART 1</i>
    </p>

1.	Select the **PS-PL Configuration** tab on the left.
1.	Expand **AXI Non Secure Enablement > GP Master AXI interface** and deselect M AXI GP0 interface.
1.	Expand **General > Enable Clock Resets** and deselect the FCLK_RESET0_N option.
1.	Select the **Clock Configuration** tab on the left. Expand the PL Fabric Clocks and deselect the FCLK_CLK0 option and click OK.
1. Click on the **Regenerate Layout button** (green arrow) shown below:
    <p align="center">
    <img src ="./pics/lab 1/7RegValid.jpg" width="90%" height="80%"/>
    </p>
    <p align = "center">
    <i> Regenerating and Validating Design</i>
    </p>

1.	Click on the **Validate Design button** (purple arrow) and make sure that there are no errors.
### Generate Top-Level and Export to Vitis
  <!--Generate IP Integrator Outputs, the top-level HDL, and prepare the hardware for Vitis.
     -->
1.	In the sources panel, right-click on **system.bd**, and select **Generate Output Products…** and click Generate to generate the Implementation, Simulation and Synthesis files for the design (You can also click on **Generate Block Design** in the Flow Navigator pane to do the same)
    <p align="center">
    <img src ="./pics/lab 1/8OP.jpg" width="40%" height="80%"/>
    </p>
    <p align = "center">
    <i> Generating output products </i>
    </p>
1.	Right-click again on system.bd, and select Create **HDL Wrapper…** to generate the top-level HDL model. Leave the **Let Vivado manage wrapper and auto-update** option selected, and click OK
    The **system_wrapper.v** file will be created and added to the project when the target language is Verilog (**system_wrapper.vhd** for VHDL).  Double-click on the file to see the content in the Auxiliary pane.
    <p align="center">
    <img src ="./pics/lab 1/9wrap.JPG" width="40%" height="80%"/>
    </p>
    <p align = "center">
    <i> The HDL Wrapper file generated and added to the project </i>
    </p>
1.	Notice that the wrapper file is already Set As the Top module in the design, indicated by the icon

1. Select **File > Export > Export Hardware**. Save the project if prompted, then click **Next**.[^xsa]
1. Choose **Fixed** if a platform-type page appears. Select **Pre-synthesis**, or leave **Include bitstream** cleared if a checkbox is shown. This design uses only the PS, DDR and MIO UART: do not run synthesis, implementation or bitstream generation for this lab.
1. Export **system_wrapper.xsa** to **{labs}/lab1**, review the summary and click **Finish**.

### Create the Application in Vitis

1. Open **Vitis 2022.2** and select **{labs}/lab1/vitis_workspace** as the workspace. Click **Launch**.
1. Select **File > New > Application Project**. Click **Next** on the welcome page, if it appears.[^application]
1. On the **Platform** page, select **Create a new platform from hardware (XSA)**. Browse to **{labs}/lab1/system_wrapper.xsa** and name the platform **lab1_platform**. Clear **Generate boot components**, if offered; this lab uses JTAG and the PS initialization script. Click **Next**.
1. Set the application name to **mem_test**, keep **mem_test_system** as the system-project name, and select **ps7_cortexa9_0**. Click **Next**.
1. On the **Domain** page, select **standalone**, **32-bit** if shown, and language **C**. Keep the proposed domain name and click **Next**.
1. Select **Memory Tests** and click **Finish**. This one wizard creates the platform, standalone domain/BSP, system project and application.
1. Open **lab1_platform > platform.spr**, select the application's standalone domain, and open **Board Support Package > Modify BSP Settings**. Under **standalone**, set **stdin** and **stdout** to **ps7_uart_1**. Apply the settings.[^bsp]

### Build and Examine the Memory Test

1. Select **Project > Build All**. Confirm that the platform and application build without errors and that **mem_test/Debug/mem_test.elf** is generated.
1. Expand **mem_test > src**. Open **memorytest.c** to examine the test calls and **memory_config_g.c** to inspect the generated memory-region list. Keep the template's generated **lscript.ld** and memory placement unchanged.[^memtemplate]

    The **Memory Tests** template performs destructive write/read tests. Its generated placement and range selection keep the test program out of the ranges it exercises. Do not move the application into a region that it tests. A displayed region size does not mean that the whole region has been tested; the 2022.2 template tests a 4 KiB block at the beginning of each selected range.[^memcode]

### Test in Hardware

1. With the board powered off, set **JP5 to JTAG** and, for USB power, **JP7 to USB**. Connect a micro-USB cable to **JTAG PROG**, then turn on the board. See [Hardware Setup](./README.md#hardware-setup).
1. Open a serial terminal from **Window > Show View > Other...** (search for **Terminal**), or use an external serial terminal. Connect to the board's COM port using **115200 baud, 8 data bits, no parity, 1 stop bit, no flow control**. Connect before running the application.
1. Select **mem_test**, then open **Run > Run Configurations...**. Create a **Single Application Debug** configuration named **mem_test_hw**. Select the local hardware-server connection, **lab1_platform**, **ps7_cortexa9_0**, and **mem_test/Debug/mem_test.elf**, with application download enabled.[^run]
1. On **Target Setup**, enable **Reset entire system**. Clear **Program FPGA**. This PS-only design has no bitstream.
1. Clear **Use FSBL flow for initialization** and select this platform's **ps7_init.tcl** as the initialization file, normally under **lab1_platform/hw**. Keep PS initialization enabled; where shown, select both **Run ps7_init** and **Run ps7_post_config**.[^target]
1. Click **Apply > Run**. The launch initializes the board and downloads the application. Reuse this configuration for subsequent runs. If execution stops at `main`, click **Resume**.

1. Observe the memory-test messages in the **serial terminal**. Verify that every reported test says **PASSED!**, with no **FAILED!** result. The completion message by itself is not a pass criterion.[^memcode]

    <p align="center">
    <img src="./pics/lab 1/etermop.jpg" width="80%" alt="Example memory-test output; regions and addresses depend on the generated platform"/>
    </p>
    <p align="center"><i>Example memory-test output; regions and addresses depend on the generated platform</i></p>

1. Terminate the launch connection and close Vitis and Vivado.

## Conclusion

The original Vivado sequence creates a PS-only Zynq system with DDR and UART. Its XSA is selected directly in the **New Application Project** wizard, which creates the Vitis platform and the **Memory Tests** application together. JTAG initialization and the serial test output are used to check the selected memory regions without a programmable-logic bitstream.

## References and Validation

Original exercise and figures: [XUP - Lab 1](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab1.md). Image paths remain relative to the original `pics/lab 1/` directory. Source exercises use the original `sources/` tree.

**Validation:** Documentation and static checks only. This adaptation has not been built in Vivado/Vitis or tested on a physical Zybo board. The retained figures are illustrations from the original workbook, not a new test record.

[^xsa]: AMD/Xilinx, [Hardware export](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Hardware-Design-XSA-File).
[^application]: AMD/Xilinx, [Application project wizard](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Standalone-Application-Project).
[^bsp]: AMD/Xilinx, [Board Support Package settings](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Board-Support-Package-Settings-Page).
[^memtemplate]: AMD/Xilinx, [Memory Tests template generation, 2022.2](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/lib/sw_apps/memory_tests/data/memory_tests.tcl).
[^memcode]: AMD/Xilinx, [Memory Tests application, 2022.2](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/lib/sw_apps/memory_tests/src/memorytest.c).
[^run]: AMD/Xilinx, [Launch configurations](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Launch-Configurations).
[^target]: AMD/Xilinx, [Target setup and PS initialization](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Target-Setup-Page).

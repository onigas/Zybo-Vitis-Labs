# Use Vivado to build an Embedded System


> **Target:** original Digilent Zybo (legacy), Vivado 2022.2 and Vitis 2022.2 (the Eclipse-based IDE).
> The original Vivado procedure and figures are retained. The screenshots were taken with an older release and may show another board; select **Zybo**, not Zybo Z7 or PYNQ, and use its board preset. Make sure the legacy Zybo board files are installed before starting.
> In the Vivado 2022.2 New Project wizard, clear **Do not specify sources at this time** on the RTL Project page to show the source pages used below. Some empty pages may be skipped by the wizard. With Verilog selected, the wrapper is `.v`, even if an original screenshot shows `.vhd`.
> Software instructions change to Vitis at the hardware export step. Original SDK screenshots in the software section are explicitly labelled as legacy references, not Vitis screenshots. The PYNQ-specific note retained below does not apply to Zybo.

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

1. Select **File > Export > Export Hardware**. Save the project if prompted, and click **Next** in the export wizard.
1. Select **Fixed** if the wizard asks for the platform type. On the output page, select **Pre-synthesis** (the option without a bitstream). If an **Include bitstream** checkbox is shown instead, leave it cleared. Do not run synthesis, implementation or bitstream generation for this PS-only lab.[^xsa]

    > **Note:** As in the original lab, this design uses the Processing System (PS), DDR and MIO UART only. There is no user logic in the Programmable Logic (PL), so there is no bitstream to include. Do not use the bitstream-inclusive export required by later labs here.

1. Set the XSA file name to **system_wrapper.xsa** and export it to **{labs}/lab1**. Click **Next**, review the summary, then click **Finish**.
1. Confirm that **{labs}/lab1/system_wrapper.xsa** exists. Vitis uses this **Xilinx Support Archive (XSA)** as the hardware description. Do not rename an old SDK `.hdf` file to `.xsa`.

### Create the Hardware Platform in Vitis

1. Open **Vitis 2022.2** from the Windows Start menu or its desktop shortcut. If **Tools > Launch Vitis IDE** is available in Vivado, it can be used instead. Select **{labs}/lab1/vitis_workspace** as a new workspace and click **Launch**. Do not reuse an old SDK workspace.[^launch]
1. Close the Welcome page, if necessary, to display the Explorer view.
1. Select **File > New > Platform Project**. Name the project **lab1_platform** and click **Next**.[^platform]
1. Select **Create from hardware specification (XSA)**, click **Browse**, and select **{labs}/lab1/system_wrapper.xsa**.
1. Select processor **ps7_cortexa9_0** and operating system **standalone**. Use the **32-bit** architecture if it is shown. Keep the generated standalone domain name.
1. Clear **Generate boot components**, if offered. This lab uses the PS initialization script over JTAG; it does not require a bootable SD-card image or a separately built FSBL.
1. Click **Finish**, then right-click **lab1_platform** and select **Build Project**. Wait for the platform build to finish without errors.
1. Open **platform.spr** under **lab1_platform**. Inspect the hardware information and the standalone domain. In Vitis, this platform and its domain/BSP replace the separate SDK hardware-platform and BSP projects.
1. In the standalone domain's **Board Support Package** settings, open **Modify BSP Settings** and select **standalone**. Confirm that **stdin** and **stdout** use **ps7_uart_1**, matching the UART 1 hardware configuration. Apply any changes and rebuild **lab1_platform**.[^bsp]

    > The hardware archive provides the processor, peripheral and memory-map information. Its generated **ps7_init.tcl** script will be used to initialize the PS, including DDR, before the application is downloaded. It must come from this lab's XSA, not from an unrelated board or an old SDK project.[^psinit]

### Generate Memory TestApp in Vitis

1. Generate the memory test application using the standard **Memory Tests** template, as in the original lab. Do not substitute a custom memory-test program.
1. In Vitis, select **File > New > Application Project**. Click **Next** on the introductory page, if shown.[^application]
1. On the platform page, select the existing **lab1_platform** and click **Next**. Do not create a second platform or choose a generic evaluation-board platform.
1. Name the application **mem_test**. Keep the generated system-project name **mem_test_system** and select **ps7_cortexa9_0** as the target processor. Click **Next**.
1. Select the existing **standalone** domain from **lab1_platform**, with the **32-bit** architecture, and select **C** as the language. Click **Next**.

    <!-- Original SDK figure retained in a comment, as in the upstream lab.
         This is a legacy SDK dialog, not the Vitis Application Project wizard.
    <p align="center">
    <img src ="./pics/lab 1/aNewSDK.jpg" width="35%" height="80%"/>
    </p>
    <p align = "center">
    <i> Legacy SDK New Project window - historical reference only </i>
    </p>
    -->

1. Select **Memory Tests** from the templates list and click **Finish**. Vitis creates the application sources and the template's linker script.[^memtemplate]
1. Select **Project > Build All** and wait for the build to complete. Confirm that **mem_test/Debug/mem_test.elf** is generated and that the Problems view contains no build errors.
1. Expand the Explorer view. The relevant projects are **lab1_platform**, **mem_test_system**, and **mem_test**. Depending on the Explorer presentation, the application may be nested under its system project. The BSP belongs to the standalone domain inside the platform; a separate **mem_test_bsp** project is not expected.

    ```text
    lab1_platform       Hardware specification and standalone domain/BSP
    mem_test_system     System project containing the application
    mem_test            Memory Tests application
      src
        memorytest.c
        memory_config_g.c
        lscript.ld
      Debug
        mem_test.elf
    ```

    <p align="center">
    <img src ="./pics/lab 1/aExplorer.jpg" width="35%" height="80%"/>
    </p>
    <p align = "center">
    <i>Original SDK Project Explorer - retained for comparison only. Use the Vitis project structure described above.</i>
    </p>

1. Open **memorytest.c** under **mem_test > src** and examine the memory-test calls. Inspect **memory_config_g.c** for the memory ranges generated from your platform. Keep the generated **lscript.ld** unchanged for this lab.[^memtemplate]

    > **Memory-test note:** This is a destructive write/read test. Keep the template's memory placement and generated range selection; do not move the application into a range that it tests. The 2022.2 template performs 32-bit, 16-bit and 8-bit tests over 4 KiB at the start of each selected range. A printed region size is not evidence that every byte of that region was tested.[^memcode]

### Test in Hardware

1. Set up the hardware as shown in **[README.md](./README.md#hardware-setup)**. For the original Zybo, set **JP5 to JTAG**; when powering from USB, set **JP7 to USB power**. Connect the micro-USB cable to the **JTAG PROG** connector. Set the jumpers with the board powered off, then switch the board on.
1. In Vitis, open **Window > Show View > Other...**, search for **terminal**, and open the serial terminal view, such as **Vitis Serial Terminal** or **Terminal**, as offered by the installation. An external serial terminal may also be used with the same settings.

    <p align="center">
    <img src ="./pics/lab 1/bTerminalwind.JPG" width="35%" height="80%"/>
    </p>
    <p align = "center">
    <i>Original SDK terminal-view selection - legacy reference. Select the corresponding serial terminal view in Vitis.</i>
    </p>

1. Open the terminal's serial connection settings. Choose the board's **COM port**, not necessarily the COM number shown in the original screenshot. You can identify it under **Ports (COM & LPT)** in Windows Device Manager.

    <p align="center">
    <img src ="./pics/lab 1/cConnect.JPG" width="35%" height="80%"/>
    </p>
    <p align = "center">
    <i>Original SDK terminal connection button - legacy reference; the Vitis toolbar can differ.</i>
    </p>

1. Configure **115200 baud**, **8 data bits**, **1 stop bit**, **no parity**, and **no flow control**, then connect the terminal **before** launching the application. Close any other program that is using the same COM port.

    <p align="center">
    <img src ="./pics/lab 1/dSetting.JPG" width="25%" height="80%"/>
    </p>
    <p align = "center">
    <i>Original serial settings - the same UART parameters apply in Vitis; select your own COM port.</i>
    </p>

1. Select **mem_test** in Explorer, right-click, and select **Run As > Run Configurations...**. Double-click **Single Application Debug** to create a hardware launch configuration, and name it **mem_test_hw**.[^run]
1. In the configuration, select the local hardware-server connection and **lab1_platform** as the hardware platform. On the application page, select **mem_test** and its **Debug/mem_test.elf** for **ps7_cortexa9_0**, with application download enabled.
1. On the **Target Setup** page, clear **Program FPGA**. There is no `.bit` file for this lab, so do not select one from another project.
1. Clear **Use FSBL flow for initialization**. Select the platform's **ps7_init.tcl** in the **Initialization File** field and leave PS initialization enabled. Where separate checkboxes are displayed, keep **Run ps7_init** and **Run ps7_post_config** enabled. Enable **Reset entire system** so that each run starts from a known state.[^target]

    > The initialization file is normally available under **lab1_platform/hw/ps7_init.tcl** in the workspace. Use **Browse** to select the file actually generated for your platform if the path differs. Disabling FPGA programming must **not** disable PS/DDR initialization. This configuration uses the Tcl initialization flow instead of the default FSBL flow.[^psinit]

1. Click **Apply**, then **Run**. Vitis connects through JTAG, resets and initializes the PS, downloads **mem_test.elf**, and runs the application. Subsequent launches can reuse this configuration. If you use **Debug** instead, resume execution if it stops at `main`.
1. Observe the memory-test output in the **serial terminal**, not just the build Console. For each reported test, verify **PASSED!** and confirm there are no **FAILED!** results. The completion message alone is not a pass criterion, because the template prints it after iterating through the tests.[^memcode]

    <p align="center">
    <img src ="./pics/lab 1/etermop.jpg" width="80%" height="80%"/>
    </p>
    <p align = "center">
    <i>Original memory-test output - an example, not a captured Vitis 2022.2 run. The region list and addresses depend on the generated platform and template.</i>
    </p>

1. Close Vitis and Vivado by selecting **File > Exit** in each program.

## Conclusion

Vivado and the IP Integrator allow a base embedded processor system to be created quickly. The hardware design and configuration sequence in this lab are the same as in the original exercise. The hardware is now exported as an **XSA**, then used to create a **Vitis platform with a standalone domain**.

Software development is performed in **Vitis 2022.2**, using its standard **Memory Tests** application template. Running the application through JTAG and checking the serial output verifies the tested memory regions without adding programmable-logic hardware or generating a bitstream.

> **Validation status:** The Vitis migration has been checked against the sources below. It has not been built with Vivado/Vitis or run on a Zybo board in this environment. The original figures are retained as reference material; they are not evidence of a new hardware test.

## References

Original lab and figures: [XUP - Zynq Design using Vivado, Lab 1](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab1.md). Keep this file in the repository root and retain the original **pics/lab 1/** directory so that the relative image links work.

[^xsa]: AMD, UG1400, 2022.2: [Creating a Hardware Design (XSA File)](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Hardware-Design-XSA-File). The no-bitstream choice follows this lab's original PS-only design.
[^launch]: AMD, UG1400, 2022.2: [Setting Up the Environment to Run the Vitis Software Platform](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Setting-Up-the-Environment-to-Run-the-Vitis-Software-Platform); [Launching Vitis IDE](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Launching-Vitis-IDE).
[^platform]: AMD, UG1400, 2022.2: [Creating a Platform Project from XSA](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Platform-Project-from-XSA).
[^bsp]: AMD, UG1400, 2022.2: [Board Support Package Settings Page](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Board-Support-Package-Settings-Page).
[^application]: AMD, UG1400, 2022.2: [Creating a Standalone Application Project](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Standalone-Application-Project).
[^memtemplate]: Xilinx/embeddedsw, `xilinx_v2022.2`: [Memory Tests template generation and linker constraints](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/lib/sw_apps/memory_tests/data/memory_tests.tcl).
[^memcode]: Xilinx/embeddedsw, `xilinx_v2022.2`: [memorytest.c](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/lib/sw_apps/memory_tests/src/memorytest.c).
[^run]: AMD, UG1400, 2022.2: [Launch Configurations](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Launch-Configurations).
[^target]: AMD, UG1400, 2022.2: [Target Setup Page](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Target-Setup-Page).
[^psinit]: AMD, UG1400, 2022.2: [Performing Standalone Application Debug](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Performing-Standalone-Application-Debug); AMD, UG821: [Zynq PS Configuration](https://docs.amd.com/r/en-US/ug821-zynq-7000-swdev/Zynq-PS-Configuration).

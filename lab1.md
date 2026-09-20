# Lab 1 - PS System and DDR Test in Vitis 2022.2

[Home](README.md) | [Next: Lab 2](lab2.md)

**Objective:** create a legacy Zybo PS system, export an XSA, create a standalone
platform and C application, and verify DDR and UART operation.
At this stage, there is no user logic in the PL.

Prerequisite: [setup guide](docs/vitis2022_2/SETUP.md).
The `{repo}` and `{work}` placeholders are defined there.

## 1. Create the Vivado project

Start **Vivado 2022.2**, register the board repository, and select
**Create Project**. Use `lab1` as the name and `{work}/vivado` as the location.
Select **RTL Project**; you do not need to add source or XDC files yet.
Set the target language to **Verilog**.

On the **Boards** tab, select the legacy **Zybo** board, not a Zybo Z7.
After creating the project, check the following in the Tcl Console:

```tcl
get_property PART [current_project]
get_property BOARD_PART [current_project]
```

The part should be `xc7z010clg400-1`, and the board should be the original
`zybo` entry. Correct DDR configuration requires the board preset, not just
the device name.

## 2. Configure the Processing System

Select **IP Integrator > Create Block Design** and name it `system`.
Add a **ZYNQ7 Processing System** IP. Keep its standard instance name,
`processing_system7_0`.

Run **Run Block Automation** with the board preset applied.
Make the `DDR` and `FIXED_IO` connections external.
Open the PS configuration and preserve the preset's DDR and PS clock settings.

For this PS-only starting point, unused I/O peripherals may be disabled, but
keep **UART1** enabled on **MIO48/49**. Do not select UART0 or EMIO UART.
The PS DDR controller must also remain enabled.

Under **PS-PL Configuration**, disable the **M_AXI_GP0** interface and the
**FCLK_RESET0_N** port for now. Under **Clock Configuration**, disable the
**FCLK_CLK0** PL clock output. Do not disable the PS operating clocks.
The Cortex-A9 private timer is not an AXI Timer IP to be added later; Lab 5
uses it through software.

Run **Validate Design**, resolve all errors, and save the block design.

## 3. Generate the wrapper and export the XSA

In the Sources view, right-click `system.bd` and select
**Generate Output Products**, then **Create HDL Wrapper**.
Choose the Vivado-managed wrapper that is updated automatically.

Because there is no user PL logic, no bitstream is required at this stage.
Select **File > Export > Export Hardware**, choose **Fixed**, and leave
**Include bitstream** unchecked. Save to `{work}/export/lab1.xsa`.

Start **Vitis 2022.2**. Do not follow the old `File > Launch SDK` or `.hdf` steps.

## 4. Create the platform and application

Follow the [common workflow](docs/vitis2022_2/VITIS_WORKFLOW.md) with these settings:

```text
Workspace:    {work}/vitis/lab1_ws
Platform:     zybo_lab1_platform
Hardware:     {work}/export/lab1.xsa
Processor:    ps7_cortexa9_0
OS:           standalone
Architecture: 32-bit
stdin/stdout: ps7_uart_1
Application:  lab1_ddr_test
Template:     Empty Application (C)
```

Import `{repo}/sources/vitis2022_2/lab1/lab1_ddr_test.c`.

In the linker settings, place code, data, `.bss`, heap and stack in DDR.
Build the platform first, then the application.

### What does this version test?

The C application tests a 16 KiB DDR buffer reserved by the linker.
It verifies that the buffer is actually within the DDR range reported by the
BSP. During the test, it flushes and disables the data cache so that reads do
not merely return cached data. It does not overwrite other live program
variables or the stack.

This is a **functional DDR check**, not a test of all 512 MiB and not a hardware
qualification test. `Xil_TestMem32` is destructive within the specified buffer,
which is why the test area must be allocated correctly.
See the [AMD memory-test API reference](docs/vitis2022_2/SOURCES.md).

## 5. Launch over JTAG

Set JP5 to JTAG. Open the board's COM port at 115200 8N1.
Start the application with **Debug As > Launch on Hardware
(Single Application Debug)**.

In this configuration, **do not request FPGA programming**, because there is
no bitstream. PS initialization is still required for DDR operation.
If the debugger stops at `main()`, select **Resume/F8**.

Expected output, with addresses depending on the linker layout:

```text
Lab 1: Zybo legacy, reserved DDR buffer test
Buffer: 0x........ - 0x........
PASS: 16 KiB reserved DDR buffer
```

This is an expected-output example, not a log captured from a hardware test in
this environment. If it fails, check the DDR preset, PS initialization, linker
regions and UART1. Do not try to fix it by disabling FPGA DRC checks.

## 6. Explore the original Memory Tests template

You can complete the original template-based exercise separately by creating
an application named `lab1_memory_tests_template` on the same platform.
Select **Memory Tests** instead of **Empty Application** in the wizard.

Review the generated `memorytest.c` and the template's memory-range data.
Before running it, check which regions it tests or skips and where the program
itself executes. Blindly testing all DDR could overwrite the application
running there. The reserved-buffer test above is therefore the main path in
this adaptation. If the template skips DDR, its success message alone does
not demonstrate that DDR was tested.

## 7. Checkpoint

Record the board part, XSA filename, PS CPU frequency, test-buffer address and
actual serial output. Explain why this lab does not need a bitstream but still
requires PS initialization.

References: [original Lab 1, AMD XSA/platform documentation and Digilent](docs/vitis2022_2/SOURCES.md).

# Common Vitis 2022.2 Workflow

[Setup](SETUP.md) | [Home](../../README.md)

## 1. What changes from SDK?

| Old SDK concept or step | This version |
|---|---|
| Vivado hardware export as `.hdf` | Vivado hardware export as `.xsa` |
| `File > Launch SDK` | Start Vitis 2022.2 separately; alternatively, use Vivado `Tools > Launch Vitis IDE` |
| SDK Hardware Platform | Vitis Platform Project created from an XSA |
| Create a separate `*_bsp` project | Standalone domain and its BSP within the platform |
| Old `.sdk` workspace | A new, separate Vitis workspace |
| `system.mss` as the main entry point | `platform.spr` -> domain -> BSP settings |
| SDK `System Debugger` menu labels | Vitis `Launch on Hardware (Single Application Debug)` |
| Copy in a new HDF | `Update Hardware Specification`, followed by rebuilding |

**Renaming an old `.hdf` to `.xsa` is not sufficient.** Export the XSA from the
current Vivado project. Do not create the new Vitis project by copying old
`.metadata` directories or SDK-generated files.

Official references for the 2022.2 concepts and steps: [AMD UG1400](SOURCES.md).

## 2. Export hardware from Vivado

Validate the block design and generate the wrapper.
For a lab that uses the PL, complete synthesis, implementation and bitstream
generation successfully before exporting. Check DRC and the timing summary.

Select **File > Export > Export Hardware** and choose a **Fixed** platform.
For the PS-only Lab 1 hardware, leave **Include bitstream** unchecked.
For Lab 2 and Lab 3 hardware, select **Include bitstream**.
Use a clear filename, such as `{work}/export/lab2.xsa`.

The export helper is an alternative to the GUI, not a project-building script:

```tcl
source C:/fpga/Zynq-Design-using-Vivado/scripts/vitis2022_2/export_xsa.tcl
export_lab_xsa C:/fpga/zybo_work/export/lab1.xsa 0
```

For hardware that uses the PL, open the freshly implemented design and use
`1` instead. The helper refuses to overwrite an existing XSA. It does not
copy the Vivado project or generate a bitstream for you.

## 3. Create a Platform Project

Start **Vitis 2022.2** and select a new workspace.
Choose **File > New > Platform Project** and enter a platform name, for example
`zybo_lab2_platform`. On the next page, select
**Create from hardware specification (XSA)** and the appropriate export.

Use **standalone**, **ps7_cortexa9_0**, and **32-bit**.
You may keep the usual **Generate boot components** setting; this may also
generate an FSBL. The lab still launches over JTAG, not from an SD card.
Do not select Linux, Cortex-A53, MicroBlaze or an acceleration platform.

Select **Finish**, then right-click the platform and choose **Build Project**.
Wait for a successful build. The platform's domain name is generated; record
the actual name rather than assuming it is identical in every installation.

Open **`platform.spr`**. Select the application's standalone domain, open its
**Board Support Package** settings, and choose **Modify BSP Settings**.
Under `standalone`, set both `stdin` and `stdout` to **`ps7_uart_1`**.
Make sure you are not editing the FSBL's separate domain instead.
Run **Build Project** on the platform again.

## 4. Create an Application Project

Choose **File > New > Application Project**, select the correct platform,
enter the application name, select **ps7_cortexa9_0**, and use the platform's
existing standalone domain. A system project may also be created for the
application; this is normal.

Select the **Empty Application (C)** or **Memory Tests** template specified
by the lab. For Empty Application, import the appropriate C file and required
common headers through:

**application > src > right-click > Import > General > File System**.

Do not import all lab C files into a single application: that would create
multiple definitions of `main()`.
You can copy the common headers into the application's `src` directory.
Preserve the exact names and capitalization shown in the examples.

| Application | Files to import from `sources/vitis2022_2` |
|---|---|
| Lab 1 | `lab1/lab1_ddr_test.c` |
| Lab 2 | `lab2/lab2_gpio.c`, `common/board_gpio.h` |
| Lab 4 | `lab4/lab4_leds.c`, `common/board_gpio.h`, `common/led_regs.h` |
| Lab 5 | `lab5/lab5_timer.c` and all three headers from `common` |

Do not manually copy the platform BSP's `xparameters.h`, `xgpio.h`,
`xscutimer.h` or related files. The selected platform/domain provides them.
Use **Open Declaration / F3** on an `#include` to inspect which BSP copy is
being used.

Run **Build Project** on the application.
Fix the first actual compiler error in the Console before later errors.
A red underline from the indexer alone does not prove that the build failed.

## 5. Default linker layout

Until the separate experiment in Lab 4, place code, data, heap and stack in
**DDR**. Right-click the application and select **Generate Linker Script...**.
Choose the actual DDR region from the memory list, with a name such as
`ps7_ddr_0` or `ps7_ddr_0_S_AXI_BASEADDR`.

Check the hardware memory range and size, not just the name suffix.
The generated `lscript.ld` is authoritative.
Rebuild after changing the linker script.

## 6. Launch and debug over JTAG

Open the serial terminal first. On the application, select
**Debug As > Launch on Hardware (Single Application Debug)**.
The debugger may stop at the start of `main()`; use **Resume/F8** to continue.

Inspect the configuration through **Run > Debug Configurations...**.
Under **Target Setup**, the target should be a Zynq/Cortex-A9 accessible through
a local hardware server, not Linux attach or QEMU.
For Lab 1, leave FPGA programming disabled.
For Labs 2-5, program the PL with the bitstream belonging to the current
platform.

The launch must initialize the PS using the `ps7_init.tcl` associated with
the selected XSA and download the application's ELF.
The usual Zynq launch configuration handles this; check the initialization
settings and application/platform association, especially after copying
projects. PL programming and the necessary PS/PL initialization must finish
before the application accesses PL registers or BRAM.

For normal execution, use the appropriate **Run As > Launch on Hardware**
launch. If the shortcut is unavailable, create a Single Application Debug
configuration in the Run/Debug Configurations window.
**Xilinx > Program Device** is a separate programming option. Do not use
MicroBlaze-specific BMM/MMI or ELF-to-bitstream association for these ARM labs.

There is no need to run a separate FSBL manually if the JTAG launch correctly
performs PS initialization. Generating boot components is not the same as
selecting the initialization procedure actually used at launch.

## 7. After changing the hardware

When an IP, address, bus connection, clock or PL logic changes:

1. In Vivado, validate, regenerate the bitstream when required, and **export a
   new XSA**.
2. In Vitis, right-click the platform, select **Update Hardware Specification**,
   choose the new XSA, and rebuild the platform.
3. Clean/build the application and check addresses, linker settings and the
   launch configuration. Download the matching new bitstream and ELF.

Regenerating the XSA **does not automatically update** the Vitis platform.
Manually editing `xparameters.h` does not fix a hardware/platform mismatch.
If only the C code changes, rebuilding and downloading the application is
usually sufficient; a new Vivado bitstream is not required.

## 8. Troubleshooting

| Symptom | First checks |
|---|---|
| Missing `xparameters.h` | Platform build, selected domain and platform association |
| Missing `XPAR_SWITCHES_DEVICE_ID` | IP instance named `switches`, current XSA/BSP |
| Missing LED base address | Lab 3 hardware, `led_ip` instance name, `S_AXI` interface |
| No terminal output | UART1, `ps7_uart_1` stdout, COM port, 115200 8N1, Resume |
| Cortex-A9 is not visible | JTAG mode, cable/driver, JP6 not set to independent JTAG |
| DDR access failure | Legacy preset, PS initialization, correct XSA; do not disable DRC |
| Program stops at LED register access | Bitstream, GP0, FCLK, reset, Address Editor |
| Program fails to start with a BRAM stack | PL programming, reset and clock before ELF execution |
| Old address macro remains after a change | Update Hardware Specification, rebuild platform |
| BRAM linker region overflows | Total heap, all ARM stacks and alignment requirements |

Background for the menu commands:
[UG1400 platform, application, debug, update and linker documentation](SOURCES.md).

# Zynq Labs - Zybo Legacy, Vivado / Vitis 2022.2

**Target hardware:** original Digilent Zybo, Zynq-7010, `xc7z010clg400-1`.  
**Tools:** Vivado 2022.2 + Vitis 2022.2, using the Eclipse-based Vitis IDE.  
**Software:** C, bare-metal `standalone`, `ps7_cortexa9_0`, 32-bit, JTAG.

This is an English-language adaptation of the five
[XUP Zynq-Design-using-Vivado](https://github.com/xupgit/Zynq-Design-using-Vivado)
labs for Vitis 2022.2. **It is not intended for the Zybo Z7-10 or Z7-20.**
The old SDK screenshots are not Vitis instructions; follow the guides below
for the updated workflow.

## Getting started

Read the [setup guide](docs/vitis2022_2/SETUP.md) first, followed by the
[common Vitis workflow](docs/vitis2022_2/VITIS_WORKFLOW.md).

| Lab | Exercise | Hardware |
|---|---|---|
| [Lab 1](lab1.md) | PS system, XSA, platform, DDR test | PS, DDR, UART1 |
| [Lab 2](lab2.md) | AXI GPIO, switches and push-buttons | Lab 1 + two GPIO peripherals |
| [Lab 3](lab3.md) | Custom AXI4-Lite LED IP, IP Packager, 8 KiB BRAM | Lab 2 + LED IP + BRAM |
| [Lab 4](lab4.md) | LED control, linker, DDR/BRAM placement | Lab 3 hardware |
| [Lab 5](lab5.md) | Cortex-A9 private timer, polling, debugging | Lab 3 hardware |

The adaptation also addresses several errors and ambiguities: a zero switch
value does not produce a zero timer reload; the exit button is explicitly
**BTN0**; and the timer frequency is derived from the BSP.
In Lab 4, **code/data remain in DDR**, while the **heap/stack move to BRAM**.
See the [migration notes](docs/vitis2022_2/MIGRATION_NOTES.md) for the changes.

## Directory layout

```text
lab1.md ... lab5.md              Updated lab instructions
sources/vitis2022_2/             New C examples, common headers, legacy Zybo XDC
scripts/vitis2022_2/             Vivado Tcl helpers
docs/vitis2022_2/                Setup, Vitis, GitHub and validation guides
tests/vitis2022_2/               Host-side tests; not FPGA simulation
```

The upstream `sources/lab*`, `labsolutions`, `board_files`, `pics` and `slides`
directories can remain in the original clone. The old projects and solutions
**have not been automatically converted into Vivado 2022.2 projects**.
For this version, use the new `sources/vitis2022_2` files and build the hardware
by following the labs.

## Validation status

This is a source- and documentation-level migration, not a prebuilt project
package. It does not contain ready-made `.bit`, `.xsa`, `.elf` or `.xpr` files.
No Vivado/Vitis 2022.2 build or physical Zybo test has been performed here.
The completed checks and remaining hardware work are listed in the
[test status](docs/vitis2022_2/TEST_STATUS.md).

To publish to your own GitHub account, follow the
[GitHub Desktop guide](docs/vitis2022_2/GITHUB_DESKTOP.md).
Technical references are listed in [SOURCES.md](docs/vitis2022_2/SOURCES.md).
Preserve the original XUP attribution and licensing terms; this addition
does not relicense the upstream repository.

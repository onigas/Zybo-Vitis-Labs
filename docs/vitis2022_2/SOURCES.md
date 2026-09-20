# Sources and Version References

These primary-source references accompany the migration.
The target version is **2022.2**, not the latest documentation release.
This English edition preserves the technical reference list from the preceding
migration package, dated **2026-09-20**; it is a language correction, not a new
online verification of every reference.
The links are references only. The package does not redistribute the vendors'
complete documentation, board files or driver sources.

## Original XUP labs

[Upstream repository](https://github.com/xupgit/Zynq-Design-using-Vivado),
[Lab 1](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab1.md),
[Lab 2](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab2.md),
[Lab 3](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab3.md),
[Lab 4](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab4.md),
[Lab 5](https://github.com/xupgit/Zynq-Design-using-Vivado/blob/master/lab5.md).

The adaptation was based on the public `master` content, not on the user's
possibly modified local copy. This package does not pin the exact upstream
commit; a local version may differ. Review existing personal modifications
before copying the update.

## Digilent - legacy Zybo only

[Zybo Rev. B reference manual, 2017-02-27](https://digilent.com/reference/_media/reference/programmable-logic/zybo/zybo_rm.pdf):
target device and DDR, power/JTAG, UART1 and MIO48/49, and board controls.

[Legacy Zybo master XDC](https://github.com/Digilent/digilent-xdc/blob/master/Zybo-Master.xdc):
physical switch, button and LED pins.

[Digilent vivado-boards](https://github.com/Digilent/vivado-boards):
the required family is `new/board_files/zybo`, not `zybo-z7-*`.
Keep the complete board definition together with its original license.

## AMD/Xilinx - Vitis 2022.2, UG1400

[Hardware design and XSA](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Hardware-Design-XSA-File)

[Creating a platform from an XSA](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Platform-Project-from-XSA)

[Standalone domain](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Adding-a-Standalone-Domain)

[BSP settings](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Board-Support-Package-Settings-Page)

[Standalone application](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Creating-a-Standalone-Application-Project)

[Updating the hardware specification](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Updating-the-Hardware-Specification)

[Single Application Debug](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Standalone-Application-Debug-Using-Xilinx-System-Debugger)

[Launch configurations](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Launch-Configurations)

[Generating a linker script](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Generating-a-Linker-Script-for-an-Application)

[Linker Basic Page](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Basic-Page)

[Program Device](https://docs.amd.com/r/2022.2-English/ug1400-vitis-embedded/Program-Device)

## Driver and memory-test sources

[XScuTimer 2022.2 header](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/XilinxProcessorIPLib/drivers/scutimer/src/xscutimer.h)

[XScuTimer hardware registers](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/XilinxProcessorIPLib/drivers/scutimer/src/xscutimer_hw.h)

[XGpio 2022.2 header](https://github.com/Xilinx/embeddedsw/blob/xilinx_v2022.2/XilinxProcessorIPLib/drivers/gpio/src/xgpio.h)

[Xil_TestMem32, UG643 2022.2](https://docs.amd.com/r/2022.2-English/oslib_rm/Xil_TestMem32)

## Vivado background and installation

In the Vivado 2022.2 **Documentation Navigator**, consult **UG1118**
(Creating and Packaging Custom IP), **UG994** (IP Integrator),
**PG078** (AXI BRAM Controller), and **PG058** (Block Memory Generator)
for the background to custom IP and BRAM configuration.
These document identifiers are provided for further verification; their full
specifications have not been verified here.

[2022.2 installer, UG973](https://docs.amd.com/r/2022.2-English/ug973-vivado-release-notes-install-license/Download-the-Installation-File)

## GitHub Desktop

[Cloning and forking repositories](https://docs.github.com/desktop/adding-and-cloning-repositories/cloning-and-forking-repositories-from-github-desktop)

This migration package does not modify upstream owners' or vendors' copyright
and license notices. The new files are not presented as an official
AMD/Xilinx or Digilent training release.

## Tcl export command syntax

[AMD UG835, Vivado 2022.2: `write_hw_platform`](https://docs.amd.com/r/2022.2-English/ug835-vivado-tcl-commands/write_hw_platform)

The included helper uses the documented positional output filename:

```tcl
write_hw_platform -fixed -include_bit output.xsa
```

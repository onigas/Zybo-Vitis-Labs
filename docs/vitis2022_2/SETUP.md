# Setup - Legacy Zybo and the 2022.2 Tools

[Back to home](../../README.md)

## 1. Check the target hardware

These labs target the original Zybo Rev. B family.
The target device is `xc7z010clg400-1`, and the board has 512 MiB of DDR3.
The exercises use four PL switches, four PL push-buttons and four PL LEDs.
Additional PS controls, reset controls and status indicators are not part of
this four-bit interface.

The `Zybo`, `Zybo Z7-10` and `Zybo Z7-20` board presets are not interchangeable.
Belonging to the same Zynq family does not imply an identical pinout or DDR
configuration. Hardware references are the
[Digilent reference manual and XDC](SOURCES.md).

## 2. Install the required tools

Install **Vitis 2022.2** alongside your existing **Vivado 2022.2**.
Check each tool's version separately in **Help > About**.
Use the Eclipse-based Vitis 2022.2 interface; instructions for the newer
component-based Unified IDE do not match this workflow.

The AMD/Xilinx installation must include Zynq-7000 device support and USB/JTAG
cable drivers. Having Vivado installed does not by itself confirm that Vitis
is installed. You do not need to open an old SDK workspace in Vitis.

## 3. Install the legacy Zybo board files

Use **`new/board_files/zybo`** from Digilent's `vivado-boards` repository,
not the `zybo-z7-*` directories. Keep the revision directories under `zybo`
and all their contents together: `board.xml`, `preset.xml`, the pin map and
other referenced files may be required. Use the board definition's
compatibility information to select the entry appropriate for your actual
PCB revision.

A convenient approach is to extract the Digilent repository separately and
register it in each new Vivado session. Example for the **Vivado Tcl Console**:

```tcl
set_param board.repoPaths [list C:/fpga/vivado-boards/new/board_files]
get_board_parts *:zybo:part0:*
```

This short command replaces the board-repository path list for the current
session. To preserve other board repositories, use the included helper:

```tcl
source C:/fpga/Zynq-Design-using-Vivado/scripts/vitis2022_2/register_board_repo.tcl
register_zybo_board_repo C:/fpga/vivado-boards/new/board_files
```

Replace the example path with your actual extraction location.
The output should include a match of the form `...:zybo:part0:...`.
Do not blindly copy a board version number from another guide.
This setting is not automatically persistent across Vivado restarts.

Alternatively, install the complete `zybo` directory under
`<Vivado>/data/boards/board_files/` and restart Vivado.
If that requires administrator privileges, using a separate board repository
is more convenient.

If the board does not appear, **do not select a Zybo Z7 instead**.
Selecting the device manually does not, by itself, apply the correct DDR/MIO
preset either.

## 4. Organize the directories

The lab instructions use these placeholders:

```text
{repo} = root of the source repository managed in GitHub Desktop
{work} = separate location for generated projects and build outputs
```

Suggested layout:

```text
C:/fpga/Zynq-Design-using-Vivado/    <- {repo}, under Git version control
C:/fpga/zybo_work/                 <- {work}, outside the repository
    vivado/lab1/
    vivado/lab2/
    vivado/lab3/
    vitis/lab1_ws/
    vitis/lab2_ws/
    vitis/lab345_ws/
    export/
```

For the first run-through, separate workspaces reduce the risk of mixing XSA
files. Labs 3-5 can use the same hardware and platform.
Use short working paths without spaces or accented characters.
Keep the English documentation and the version-controlled source files in
`{repo}`, and the generated projects in `{work}`.

## 5. Connect the board and serial port

Check jumper settings with the board powered off.
Set **JP5 to JTAG** and, when using USB power, **JP7 to USB**.
For normal cascaded JTAG access that includes the PS, do not short JP6 to
select independent JTAG. Follow your PCB labels and the reference manual,
not a photograph of a Zybo Z7.

Connect a data-capable micro-USB cable to **J11 USB-JTAG/UART**, not to the
USB-OTG connector. Power on the board and find its COM port in Device Manager.

Serial settings:

```text
115200 baud
8 data bits
No parity
1 stop bit
No flow control
```

The legacy Zybo USB-UART circuit connects to **PS UART1**, **MIO48/49**.
In the Vitis standalone BSP, set both `stdin` and `stdout` to **`ps7_uart_1`**.
Only one terminal program should open the COM port at a time.
In Vitis, search for `Terminal` or `Vitis Serial Terminal` through
**Window > Show View > Other...**. An external serial terminal is also suitable.

These JTAG labs do not require an SD card, Linux, PetaLinux or flash programming.

## 6. Before starting

Confirm that Vivado sees the legacy Zybo board, the project device is Z-7010,
Vitis is version 2022.2, the board is in JTAG mode, and the COM port is known.
Then continue with [Lab 1](../../lab1.md).

References: [Digilent board information and tool installation](SOURCES.md).

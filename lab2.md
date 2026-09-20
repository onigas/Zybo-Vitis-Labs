# Lab 2 - AXI GPIO, Switches and Push-Buttons

[Previous: Lab 1](lab1.md) | [Home](README.md) | [Next: Lab 3](lab3.md)

**Objective:** access two AXI GPIO peripherals in the PL from the PS.
Switch and button values are displayed in the serial terminal. This lab does
not drive the LEDs yet.

## 1. Copy the project and enable the PS-PL connection

Make a copy of the Lab 1 Vivado project using
**File > Project > Save As... / Save Project As...**, depending on the
project-save command shown in your installation. A plain **Save As** command
that saves only an individual file is not sufficient.
The new project should be `{work}/vivado/lab2/lab2.xpr`.
Work in the copy so that Lab 1 remains available as a known starting point.

In the PS configuration, enable the **M_AXI_GP0** master port,
**FCLK_CLK0** output and **FCLK_RESET0_N** reset output.
Set the PL output clock to **100 MHz**.
Do not change the CPU frequency, DDR settings or UART1 for this step.

## 2. Add two AXI GPIO IPs

Add two **AXI GPIO** IPs. Use these exact instance names:

```text
switches
buttons
```

Configure each for one channel, **GPIO width = 4**, **All Inputs**,
**Enable Dual Channel = off**, and **Enable Interrupt = off**.

These instructions assign the external PL pins using a dedicated XDC file.
Do not also apply automatic board GPIO pin assignments to these peripherals.
Keep the legacy board preset used for the PS.

Use **Run Connection Automation** to connect both **S_AXI** interfaces to the
PS **M_AXI_GP0** master. Vivado may also create interconnect and reset logic.
Generated block names and the interconnect type may differ from the old
screenshots.

Inspect the actual connections:

| Signal/function | Connection |
|---|---|
| PS `M_AXI_GP0_ACLK` | `FCLK_CLK0` |
| Both GPIO `s_axi_aclk` inputs | `FCLK_CLK0` |
| AXI infrastructure clocks | The same 100 MHz clock |
| GPIO `s_axi_aresetn` | Processor System Reset `peripheral_aresetn` |
| AXI infrastructure resets | Appropriate synchronized active-low reset |
| Reset block clock/input | FCLK and the PS reset, with the correct polarity |

The PS `FCLK_RESET0_N` signal is active-low. Match the reset block's external
reset polarity to the connection, and review the result of automation.
**Validate Design** must not report any unconnected required clock, reset or
bus signals.

## 3. Create external ports and apply the legacy Zybo pinout

Expand the GPIO interface on each IP block. Apply **Make External** to the
actual **`gpio_io_i`** input vector; do not export a GPIO signal with a different
direction.

Rename the resulting plain input ports as follows:

```text
switches[3:0]
buttons[3:0]
```

The `[3:0]` notation indicates width; it is not text to type into the port name.
The port names are `switches` and `buttons`, each four bits wide.
This differs from exporting an entire GPIO interface, which can produce
wrapper names such as `switches_tri_i`.

Select **Add Sources > Add or Create Constraints** and add
`sources/vitis2022_2/constraints/zybo_legacy_gpio.xdc`.

| Bit | SW pin | BTN pin |
|---|---|---|
| 0 | G15 | R18 |
| 1 | P15 | P16 |
| 2 | W13 | V16 |
| 3 | T16 | Y16 |

All PL signals used here are **LVCMOS33**.
The pin assignments come from the
[Digilent legacy Zybo master XDC](docs/vitis2022_2/SOURCES.md).
They do not describe the voltage settings of the PS MIO banks.

Check the generated wrapper's port names. If the automatically generated
names differ, correct either the port names or the XDC `get_ports` expressions.
Do not ignore `get_ports` warnings. Do not apply conflicting board-generated
and manual constraints to the same port.

## 4. Assign addresses, generate the bitstream and export the XSA

In **Address Editor**, map both peripherals into the PS Data address space.
The following layout is suitable, but the software does not hard-code these
addresses:

| Peripheral | Example base address | Range |
|---|---|---|
| `switches` | `0x41200000` | 64 KiB |
| `buttons` | `0x41210000` | 64 KiB |

Resolve any overlapping ranges or unmapped slaves. Save and validate the
design, regenerate the required output products and wrapper, and select
**Generate Bitstream**. Check DRC and implementation timing.
For UCIO/NSTD errors, correct the pin constraints; do not downgrade the errors
to force bitstream generation.

Select **Export Hardware > Fixed > Include bitstream** and save to
`{work}/export/lab2.xsa`.

## 5. Create the Vitis application

Follow the [common workflow](docs/vitis2022_2/VITIS_WORKFLOW.md):

```text
Workspace:   {work}/vitis/lab2_ws
Platform:    zybo_lab2_platform, based on lab2.xsa
Domain:      standalone / ps7_cortexa9_0 / 32-bit
Application: lab2_gpio
Template:    Empty Application (C)
```

Import `lab2/lab2_gpio.c` and `common/board_gpio.h` from
`sources/vitis2022_2` into the application's `src` directory.
Keep the linker layout entirely in DDR for now.
The platform's stdin/stdout settings must remain `ps7_uart_1`.

`board_gpio.h` uses the `XPAR_SWITCHES_DEVICE_ID` and
`XPAR_BUTTONS_DEVICE_ID` macros. Compilation deliberately fails if they are
missing. Do not replace them with guessed values such as `0` and `1`.
Check the Vivado instance names and the selected XSA.

## 6. Run the application

The Vitis launch configuration must now **program the PL as well**.
The bitstream, platform and ELF must all correspond to the same hardware.

Expected serial output format:

```text
Lab 2: switches and buttons
SW=0x0 BTN=0x0
SW=0x1 BTN=0x0
SW=0x1 BTN=0x1
```

Output is printed only when a value changes. The application samples every
20 ms; this is not a complete push-button debouncing algorithm.
The program runs continuously. Stop it with the debugger's
**Suspend/Terminate** controls.

## 7. Checkpoint

Test every SW and BTN bit individually. Compare the displayed value with the
physical board labels. Record both GPIO addresses from Address Editor and
the corresponding definitions in the generated `xparameters.h`.
Proceed to Lab 3 only after the GPIO system works reliably.

References: [original Lab 2, AXI GPIO and Digilent XDC](docs/vitis2022_2/SOURCES.md).

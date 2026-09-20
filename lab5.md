# Lab 5 - Private Timer and Debugging in Vitis

[Previous: Lab 4](lab4.md) | [Home](README.md)

**Objective:** use the Cortex-A9 private timer in polling mode, implement a
four-bit LED counter with an adjustable update interval, and explore
breakpoints and register inspection.

## 1. Reuse the hardware and create a new application

Use the platform created from the Lab 3 `lab3.xsa`.
The private timer is part of the processing system: **do not add an AXI Timer
IP** to the block design. Using it does not require a new bitstream.

In workspace `{work}/vitis/lab345_ws`, create an **Empty Application (C)**
project named **`lab5_timer`** on the same standalone domain.

Import these files from `sources/vitis2022_2`:

```text
lab5/lab5_timer.c
common/board_gpio.h
common/led_regs.h
common/timer_math.h
```

Start this lab with an **all-DDR linker configuration** again.
The BRAM stack in Lab 4 was a separate experiment; do not inherit that
configuration accidentally.

## 2. Vitis 2022.2 driver and time base

The required API header is **`xscutimer.h`**, in lowercase.
The driver and documentation can be found under the installation's
`data/embeddedsw` trees and in the AMD/Xilinx `embeddedsw`
**`xilinx_v2022.2`** branch. The `master` branch may contain newer API changes,
so it is not the reference for this 2022.2 workflow.

The application uses the following sequence:

```text
XScuTimer_LookupConfig
XScuTimer_CfgInitialize
XScuTimer_Stop
XScuTimer_DisableInterrupt
XScuTimer_SetPrescaler(..., 0)
XScuTimer_EnableAutoReload
XScuTimer_ClearInterruptStatus
XScuTimer_LoadTimer
XScuTimer_Start
```

The code checks the configuration pointer and initialization results.
Polling mode does not require GIC/ISR registration. The hardware expiration
flag must still be cleared after processing an event.

The private timer input clock is half the CPU clock; the prescaler is set to
zero in this example. The reload calculation is:

```text
steps = max(1, SW & 0xF)
LOAD  = floor(CPU_HZ * steps / 20)
T     approximately 0.1 s * steps
```

The CPU frequency comes from the generated `xparameters.h`.
The calculation uses a 64-bit intermediate value.
For example, at a 650 MHz CPU clock, the value for 0.1 s is 32,500,000,
but this value is **not hard-coded** for every project.

| Switch value | Nominal interval |
|---|---|
| `0000` | 0.1 s - deliberate protection against a zero LOAD |
| `0001` | 0.1 s |
| `0101` | 0.5 s |
| `1111` | 1.5 s |

The calculated value and actual interval depend on the configured clock.
This is a timer programming exercise, not a calibrated timing reference.

## 3. Application behavior

The counter starts at zero. It increments on each processed timer expiration
and wraps from 15 to zero. When the switches change, the counter resets and
the timer restarts for a full interval at the new setting; the previous
expiration flag is cleared.

Pressing **BTN0** stops the timer, turns off all four LEDs and exits.
The mask in the code is `0x01`. BTN1-BTN3 are not exit buttons.
The board's PS-reset or programming/reset button is not BTN0.
Do not apply the ZedBoard term "center button" to the four legacy Zybo buttons
used in this lab.

## 4. Build and run

The platform must provide the `scutimer` driver.
For debugging, select a **Debug** build with debug information and, for
example, **`-O0`** optimization. Check the application's compiler settings;
the command line shown in the build Console is the authoritative record.

The launch must load the Lab 3 bitstream, perform the matching PS
initialization, and download `lab5_timer.elf`.

Expected output for a 650 MHz CPU and a switch value of 1:

```text
Lab 5: private timer polling; BTN0 exits
CPU=650000000 Hz, SW=0x1, LOAD=32500000
```

A different CPU clock produces a different reload value.
The application prints only at startup, on switch changes and on exit;
it does not flood the serial port on every timer expiration.

## 5. Set breakpoints

Select **Debug As > Launch on Hardware (Single Application Debug)**.
When the program stops at `main()`, set breakpoints at these locations:

1. The check of the timer `CfgInitialize` result.
2. The line before loading the timer in `timer_set_interval()`.
3. The `lab_led_write(count)` call in the main loop's expiration-handling branch.

In **Variables**, watch `count`, `previous_switch`, `switch_value`,
`reload_value` and `status` while they are in scope.
Use **Step Over/F6**, **Step Into/F5** and **Resume/F8**.
How inline helpers from headers appear may depend on the debug settings.

The counter should increment only in the expiration-handling branch.
If initialization fails, the program prints an error and exits; it does not
dereference a null pointer or continue with an invalid timer instance.

## 6. Inspect timer registers in the Memory view

Open **Memory** through **Window > Show View > Other...**.
Check the private timer's base address in your own `xparameters.h`.
The usual Zynq-7000 base address is `0xF8F00600`.

| Register | Offset | Usual address |
|---|---|---|
| Load | `0x00` | `0xF8F00600` |
| Counter | `0x04` | `0xF8F00604` |
| Control | `0x08` | `0xF8F00608` |
| Interrupt status | `0x0C` | `0xF8F0060C` |

Verify the offsets against the 2022.2 **`xscutimer_hw.h`**.
Choose a 32-bit, little-endian display.
Do not confuse the Load register with the Counter register, which changes
while the timer runs.

Do not write arbitrary values to control/status registers from the debugger.
The status event bit is write-one-to-clear; the driver's clear operation
handles this behavior.

## 7. Limitations of polling and debugging

The expiration bit is not an event counter. If software does not check it
for a long time, multiple expirations may not be recoverable as separate
events. Do not interpret LED timing while halted or single-stepping as a
real-time measurement; debugger halt and peripheral behavior also matter.

To check the interval, remove frequently hit breakpoints and measure with
the application running freely. Unlike the earlier labs with a 20 ms GPIO
polling interval, this main loop continuously checks the timer flag.

## 8. Checkpoint

Test switch values 0, 1, 5 and 15. A zero switch setting must not freeze the
system, and the counter must wrap from 15 to zero.
BTN1 must not exit; BTN0 must exit.
Record the CPU frequency, Load values and addresses observed in the debugger.

`tests/vitis2022_2/test_timer_math.c` tests the interval calculation on the host,
but does not replace a platform build or a Zybo hardware test.
See the [test status](docs/vitis2022_2/TEST_STATUS.md) for the complete
validation checklist.

References: [original Lab 5, the 2022.2 scutimer driver and UG1400 debugging documentation](docs/vitis2022_2/SOURCES.md).

# Lab 4 - LED Control and DDR/BRAM Linker Configuration

[Previous: Lab 3](lab3.md) | [Home](README.md) | [Next: Lab 5](lab5.md)

**Objective:** display switch values through the custom LED IP, access its
registers, inspect ELF sections, and move the heap and stack to BRAM.

> The original repository summary describes moving `.text` to BRAM, while the
> detailed Lab 4 instructions move the heap and stack there. This adaptation
> follows the detailed exercise: **code/data in DDR; heap/stack in BRAM**.
> It does not execute application code from BRAM.

## 1. Reuse the existing hardware

Use the completed `lab3.xsa`, including its bitstream, and the
`zybo_lab3_platform` platform from Lab 3.
There is no need to create a new Vivado project or XSA merely because the
software exercise has a different lab number. If you actually change the
hardware, complete the export and platform-update workflow again.

The platform BSP must provide the `gpio` driver for the `switches` and
`buttons` peripherals. In this version, a small named MMIO helper accesses
the custom LED IP directly; the old SDK's automatic `led_ip` driver
registration is not required.

## 2. Create the C application

In the common Vitis workspace, create an application named **`lab4_leds`**
using the existing platform, its standalone domain, `ps7_cortexa9_0`, and the
**Empty Application (C)** template.

Import these files from `sources/vitis2022_2`:

```text
lab4/lab4_leds.c
common/board_gpio.h
common/led_regs.h
```

`lab_led_write()` writes register 0 using `Xil_Out32()`.
`lab_led_read()` reads the same register using `Xil_In32()`.
The base address comes from the LED macro in the generated `xparameters.h`.

This is not an attempt to initialize the LED IP with an AXI GPIO driver:
the custom IP's register map defines its behavior.
`led_regs.h` is a small application-side register-access layer.

In **Generate Linker Script**, initially place all main sections in DDR.
Build the platform, then the application.

## 3. Test basic operation

The JTAG launch must program the Lab 3 bitstream, initialize the PS and download
the new ELF. The terminal displays the LED register base address and the
address of a local variable:

```text
Lab 4: switches -> custom LED IP
LED base: 0x........
Stack probe: 0x........
SW=0x5 BTN=0x0 LED=0x5
```

For `SW=5`, the four-bit pattern is `0101`: LED0 and LED2 should be on.
The program prints the button values, but the buttons do not stop execution
in this lab. Verify this basic behavior before changing the linker layout.

Register readback demonstrates that the register is accessible over the bus.
Verify the physical LED output and pin assignments separately on the board.

## 4. Inspect the ELF sections

In a command prompt with the Vitis environment loaded, change to the
application's output directory and run:

```text
arm-none-eabi-objdump -h lab4_leds.elf
arm-none-eabi-size -A lab4_leds.elf
```

The file is typically in the application's `Debug` directory; use the actual
path shown in the Build Console. A plain Windows command prompt may not find
the toolchain. Use the installation's Vitis shell/command prompt or an
environment configured with `<Vitis>/2022.2/settings64.bat`.

Inspect the VMA/LMA addresses and sizes of `.text`, `.data`, `.bss`, `.heap`
and `.stack`. Stack and heap sections may be NOBITS/NOLOAD, so space occupied
in the ELF file is not the same as the required RAM capacity.

## 5. Move the heap and stack to the 8 KiB BRAM

Save a copy of the working `lscript.ld`.
In Vitis, select **Generate Linker Script...** on the application and use the
following basic settings:

| Section group | Memory |
|---|---|
| Code | DDR |
| Data | DDR |
| Heap | AXI BRAM |
| Stack | AXI BRAM |

Select the actual generated memory region, for example
`axi_bram_ctrl_0_S_AXI_BASEADDR`.
With the example address layout, the BRAM range is
`0x40000000-0x40001FFF`.

Keep exception vectors, code and the MMU table in the appropriate DDR region
in the generated linker script. **Do not move every ELF section into BRAM.**

### Respect the 8 KiB capacity

The total stack allocation may include separate stacks for the ARM processor
modes. Inspect the size definitions in the generated `lscript.ld`.
For this small exercise without interrupt handling, these values can serve
as a starting point:

```text
_STACK_SIZE              0x800  (2048 bytes)
_HEAP_SIZE               0x400  (1024 bytes)
_ABORT_STACK_SIZE        0x200
_SUPERVISOR_STACK_SIZE   0x200
_IRQ_STACK_SIZE          0x200
_FIQ_STACK_SIZE          0x200
_UNDEF_STACK_SIZE        0x200
```

If the generated script includes these separate ARM stacks and places them
in the same BRAM region, the total is **5632 bytes** before alignment.
Check the symbol names in your own script. You may need to edit the generated
file manually; regenerating it may overwrite those edits.

This is an allocation chosen for the lab, not a general recommendation for
all applications. Large local arrays, deep call chains or interrupt handlers
may require more stack space. If the region overflows, do not increase the
linker's BRAM length beyond the physically available 8192 bytes.

## 6. Rebuild and verify placement

Rebuild the application and run `objdump` and `size` again.
Also inspect the linker map file if the build generates one.

The `.heap` and `.stack` sections should now be in the BRAM address range.
The `.text`, `.data` and `.bss` sections should remain in DDR.
Include any stack sections that have different names in your generated script.

Restart the application with the updated ELF.
The **Stack probe** address printed in the terminal must be within BRAM.
Switch-to-LED behavior should remain unchanged.

**Important:** the PL must be operational before the ARM accesses the stack
in BRAM. A PS-only launch, missing PL clock or incorrect reset may cause the
program to stop even before `main()`.
Do not run a destructive memory test over BRAM that contains the running
application's stack.

## 7. What would be needed to execute code from BRAM?

That is a separate advanced exercise. The code must fit, and appropriate
startup code, section placement, MMU/memory attributes and cache handling may
also be required. Simply changing the destination of `.text` does not produce
a generally usable BRAM boot solution. That is not a completion requirement
for this lab.

## 8. Checkpoint

Keep the section listings for both the all-DDR and BRAM-stack configurations,
the final `lscript.ld`, and the serial address output.
Explain why changing the linker alone does not require a new bitstream, but
running the application still requires working PL hardware.

References: [original Lab 4 and Vitis linker/ELF documentation](docs/vitis2022_2/SOURCES.md).

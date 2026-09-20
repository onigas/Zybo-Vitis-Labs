# Migration Notes and Deliberate Differences

[Home](../../README.md)

## Scope

The updated guides retain the objectives of the five upstream Markdown labs
and specifically target **legacy Zybo + Vivado/Vitis 2022.2**.
This is not a global SDK-to-Vitis word replacement, nor a binary or in-place
migration of the old `.xpr` or `.hdf` files.

Existing screenshots and instructor solution projects may remain in the
repository, but they are not presented as tested 2022.2 solutions.
The updated labs do not rely on SDK screenshots.

## English documentation correction

This package replaces the earlier Hungarian-language documentation with
English instructions. All five labs, the README, setup guide, common Vitis
workflow, GitHub Desktop guide, migration notes, reference list and test-status
document are in English. Supporting guide filenames no longer use `_HU`.

The C examples, common headers, XDC constraints, Tcl helpers and host tests
are unchanged from the preceding package; their comments and messages were
already in English. This language correction does not add hardware validation
or silently alter the exercises' technical behavior.

## Technical changes from the original SDK labs

| Area | Updated approach and rationale |
|---|---|
| Boards | Legacy Zybo only, rather than multi-board instructions; avoids confusion with Z7 boards |
| Hardware handoff | XSA, including a bitstream for labs that use PL |
| Vitis projects | Platform + standalone domain + application + system project |
| Workspace | New workspace; no import of old SDK `.metadata` |
| BSP update | Update Hardware Specification, followed by rebuilding |
| Lab 1 test | Main path: allocated 16 KiB DDR buffer; Memory Tests template as a separate exercise |
| GPIO | Four input bits, explicit instance names, error handling and header includes |
| Pin constraints | Dedicated legacy XDC with exact wrapper port names |
| LED HDL | Expose `slv_reg0[3:0]`; preserve the 2022.2 AXI template's reset/strobe handling |
| LED driver | Small MMIO header; no mandatory dependency on the old SDK driver package |
| BRAM | Actual capacity of 8192 bytes; memory size and address range must agree |
| Lab 4 linker | Code/data in DDR, heap/stack in BRAM; include ARM mode stacks in the allocation |
| Timer frequency | Derived from the BSP CPU clock, not a fixed `32500000` constant |
| Zero switch value | `SW=0` maps to 0.1 s; do not deliberately load zero |
| Timer interval change | Stop, clear stale flag, load new value, start |
| Exit button | BTN0, mask `0x01`; not a "center button" and not every button |
| Debugging | 2022.2 Single Application Debug, `-O0`, variable and memory inspection |
| Validation claims | No "hardware tested" claim without actual test evidence |

## Why is the LED logic different?

The original separate user-logic module observed the bus-write event and data.
The new exercise achieves the same basic goal: the lower four bits written
to AXI register 0 control the LEDs. It exposes the template's decoded,
byte-strobed register instead of adding a parallel register-write path.
Creating, modifying, packaging and connecting a custom AXI peripheral remain
part of the lab.

## Why is the old led_ip driver optional?

Controlling the four LEDs requires a register write.
`led_regs.h` uses the AMD BSP's `Xil_Out32`/`Xil_In32` primitives and obtains
the base address from `xparameters.h`. Packaging a custom driver can be added
as a separate exercise later, but is not required for this Vitis migration.
The AXI GPIO peripherals and private timer still require the normal BSP drivers.

## Why are no prebuilt bitstream or complete linker script included?

The bitstream must match the actual hardware design, and the XSA must describe
that same configuration. Generated addresses and memory names affect the
linker script. Reusing a prebuilt file from a different configuration could
hide an incorrect board preset or XSA association.

The package therefore provides source files and creation instructions.
Generate each application's linker script from your own platform, then modify
it as described in Lab 4. You can later commit the final working linker script
to your own repository.

## API version

The code follows the **DeviceId**-based API of the classic 2022.2 standalone
BSP. It is not written for a newer Device Tree/System Device Tree flow.
Moving to a newer Vitis release requires separate verification.

References for the adaptation: [SOURCES.md](SOURCES.md).

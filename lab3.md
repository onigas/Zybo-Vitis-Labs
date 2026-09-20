# Lab 3 - Custom AXI4-Lite LED IP and BRAM

[Previous: Lab 2](lab2.md) | [Home](README.md) | [Next: Lab 4](lab4.md)

**Objective:** create a custom AXI4-Lite peripheral with a four-bit LED output
using Vivado IP Packager, then add AXI BRAM.
Labs 4 and 5 use software running on this same hardware.

## 1. Create the project and IP repository

Make a Vivado project copy named `lab3` from the working Lab 2 project.
Store your custom IP in a permanent location such as
`{repo}/ip_repo/led_ip_1.0`, not in a temporary `.cache` directory.

Select **Tools > Create and Package New IP**, then
**Create a new AXI4 peripheral**.
Use `led_ip` as the name and `1.0` as the version. You may use your own vendor
identifier. Set the packaging destination to the IP repository specified above.

Configure the interface name as **S_AXI**, type **AXI4-Lite**, role **Slave**,
data width **32 bits**, and number of registers **4**.
Select **Edit IP**. The editing project's device/compatibility settings must
include the Zynq-7000 family.

## 2. Add the LED port to the generated HDL

In this version, the LEDs display the lower four bits of **register 0**, which
the template already implements. AXI address decoding, register reset and
byte-strobe handling therefore remain in the generated AXI register logic.
Do not copy the upstream `lab3_user_logic.v` module into this version.

Locate the `Users to add parameters/ports` markers and the module definitions
rather than relying on line numbers.

### Top-level module: `led_ip_v1_0.v`

Add this to the parameter list:

```verilog
parameter integer LED_WIDTH = 4,
```

Add this to the port list:

```verilog
output wire [LED_WIDTH-1:0] LED,
```

Add this parameter association to the generated `led_ip_v1_0_S_AXI` instance:

```verilog
.LED_WIDTH(LED_WIDTH),
```

Add this port association to the same instance:

```verilog
.LED(LED),
```

Do not remove the existing parameters or AXI ports.
A comma is required between list items, but not after the final item.

### AXI submodule: `led_ip_v1_0_S_AXI.v`

Add `LED_WIDTH` to this module's parameter list and `LED` to its port list as
well. Find the `slv_reg0` register. In the module body, **outside any `always`
block**, add:

```verilog
assign LED = slv_reg0[LED_WIDTH-1:0];
```

Do not also drive `LED` from another `always` block.
For this lab, **LED_WIDTH = 4**; it must not exceed the 32-bit AXI data width.
After reset, the template sets `slv_reg0` to zero, so the LEDs are off.

Your own **Vivado 2022.2** installation generates the complete AXI template.
This package deliberately does not include an old generated bus module or an
unverified hand-written replacement for the AXI protocol logic.

## 3. Package and check the IP

Run HDL checks/synthesis in the editing project.
In the **Package IP** view, update **File Groups**, **Ports and Interfaces**,
and **Customization Parameters**, using **Merge changes** where available.

Make `LED_WIDTH` a visible customization parameter with a default value of 4.
Preserve the template's `S_AXI` clock/reset associations.
The compatible families must include **Zynq**.
Select **Review and Package > Re-Package IP / Package IP**.

In the main `lab3` project, add the `ip_repo` location under
**Settings > IP > Repository** and refresh the IP Catalog as needed.
All referenced HDL files must reside in the packaged IP or version-controlled
repository. Do not leave paths pointing to a temporary editing directory on
your computer.

## 4. Connect the LED IP

Add the new IP to the block design. Name the instance **`led_ip`** and set
**LED_WIDTH = 4**.
Use Connection Automation to connect **S_AXI** to the PS **M_AXI_GP0** path.
Use the same 100 MHz FCLK for the IP clock and the synchronized active-low
peripheral reset for its reset input.

Apply **Make External** to the `LED` output.
The external port must be named exactly **`LED`**, with a width of four bits.
Names are case-sensitive.

Add `sources/vitis2022_2/constraints/zybo_legacy_leds.xdc`.
Keep the GPIO XDC from Lab 2 in the project as well.

```text
LED[0] = M14
LED[1] = M15
LED[2] = G14
LED[3] = D18
IOSTANDARD = LVCMOS33
```

These are the pins from the [Digilent master XDC](docs/vitis2022_2/SOURCES.md),
not the separate PS LED or DONE indicator.

## 5. Add the AXI BRAM Controller

Add an **AXI BRAM Controller** IP with instance name `axi_bram_ctrl_0`,
**32-bit** AXI data width, **Single Port BRAM**, and no ECC.
The controller may use an AXI4 interface; it does not need to be restricted
to AXI4-Lite just because the LED peripheral uses Lite.

Connect **S_AXI** to the PS GP0 interconnect.
Run Connection Automation on **BRAM_PORTA** so that the resulting
**Block Memory Generator** serves the BRAM interface.
Do not leave a second, unconnected BRAM port on the controller.

In Address Editor, set the BRAM range to **8K**, meaning **8192 bytes**.
Also verify the generated memory's actual configuration: at a 32-bit word
width, this is **2048 words**, not 8192 32-bit words.

Use the BRAM-controller operating mode appropriate for the controller.
If automation generated a different capacity, correct the related settings.
The decoded address range and the physical memory capacity must agree.

## 6. Assign addresses and export the hardware

The following layout can be used:

| Peripheral | Example base address | Size |
|---|---|---|
| `switches` | `0x41200000` | 64 KiB |
| `buttons` | `0x41210000` | 64 KiB |
| `led_ip` | `0x43C00000` | 64 KiB address window; four registers |
| `axi_bram_ctrl_0` | `0x40000000` | 8 KiB |

The LED IP's 64 KiB address window does not represent 64 KiB of actual
register storage. Registers 0, 1, 2 and 3 have offsets `0x00`, `0x04`, `0x08`
and `0x0C`. The software uses the actual base address provided by the XSA/BSP.

Run **Validate Design**, generate output products, update the wrapper, and
select **Generate Bitstream**. Check DRC and timing.
Export a **Fixed XSA with the bitstream included** to
`{work}/export/lab3.xsa`.

In Vitis, create `zybo_lab3_platform` in workspace
`{work}/vitis/lab345_ws`, using standalone, `ps7_cortexa9_0`, 32-bit and UART1.
Build the platform. The application is created in the next lab.

## 7. Checkpoint

The IP package must contain the modified HDL and `component.xml`.
In the final block design, the switches, buttons, LED IP and BRAM must all be
accessible through GP0. The BRAM capacity must be 8192 bytes, and the four-bit
external LED port must connect to the correct pins.

Synthesis does not replace functional AXI simulation.
When developing the custom IP further, separately verify reset, byte strobes,
register addresses and backpressure handling.
Using the unmodified AXI template is not, by itself, evidence of a hardware test.

References: [original Lab 3, Digilent pinout, AMD IP Packager and BRAM documentation](docs/vitis2022_2/SOURCES.md).

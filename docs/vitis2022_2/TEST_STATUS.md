# Validation Status - Legacy Zybo / Vitis 2022.2

**Status: documentation- and source-level migration; not yet verified on hardware.**  
Host-check date for this English revision: **2026-09-20**.

Vivado, Vitis, an ARM cross-compiler and a connected Zybo were not available
in this environment. The PASS results below **do not** demonstrate a successful
Vitis build, synthesis, timing verification or operation on a physical board.

## Completed host-side checks

| Check | Result | What it demonstrates, and what it does not |
|---|---|---|
| Timer calculations: all 16 switch values, zero protection, masking and non-integer division | PASS | Pure C arithmetic behavior; not the actual hardware clock. |
| Four C applications checked with `gcc -std=c11 -Wall -Wextra -Werror -pedantic -fsyntax-only` | PASS | Syntax checks using substitute BSP declarations; not ARM compilation or linking. |
| Alternate LED/CPU/timer macro names and missing-GPIO diagnostic | PASS | The tested conditional-compilation paths work. The real `xparameters.h` must be generated from the XSA. |
| Tcl helper control flow with substitute Vivado commands | PASS | Bitstream option, positional XSA filename, version/open-project/extension/overwrite checks. Not a real XSA export. |
| Board-repository helper: preserve existing paths, avoid duplicates, report errors | PASS | Tcl control flow, not loading real board files. |
| Twelve port-to-pin assignments in the two XDC files | PASS | Agreement with the reference fixture derived from the Digilent legacy Zybo master XDC. Not an electrical measurement or implementation. |

Version-pinned GPIO and SCU timer driver references from AMD/Xilinx
`embeddedsw`, branch **`xilinx_v2022.2`**, are listed in [SOURCES.md](SOURCES.md).
The substitute test headers contain no real drivers:
**never copy them into a Vitis application**.

The `Exported:` messages printed by the Tcl test are test output; no real XSA
is created. The tests use a temporary directory and remove it afterward.

## English revision checks

The host checks above were rerun for this English package.
All 15 non-Markdown files, including C sources, headers, constraints, scripts,
tests and the `.gitignore` fragment, are byte-for-byte identical to the previous
package. The Tcl and Verilog code snippets within the five labs are also
unchanged.

All five labs and the supporting documentation are in English. Internal
Markdown file links were checked after renaming the supporting guides.
The old `_HU.md` filenames appear only in the instructions for removing
obsolete copies; those files are not included in this package.
External URLs were preserved as references, not revalidated online during
this language correction.

## Repeat the host tests

Requirements: Python 3.9 or newer, GCC and `tclsh`.
On Linux or WSL, run from the repository root:

```bash
python3 tests/vitis2022_2/run_host_checks.py
```

The tests neither require nor launch Vivado or Vitis.
Override the compiler using the `CC` environment variable if necessary.
The runner exits with an error if any check produces an unexpected result.

## Checks to perform on your computer and board

| Step | Required verification | Current status |
|---|---|---|
| Setup | Actual legacy Zybo revision, matching board preset, JTAG, UART1 | Not hardware-verified |
| Lab 1 | PS/DDR initialization, actual Vitis build, UART, PASS from the reserved 16 KiB DDR test | Not performed |
| Lab 2 | Synthesis, implementation, timing, bitstream; correct bits for all four switches and buttons | Not performed |
| Lab 3 | Generated and modified HDL, AXI reset/byte-strobe/readback behavior, addressing, actual 8 KiB BRAM | Not performed |
| Lab 4 | LED behavior, linker/map file, stack-probe address in BRAM, all stacks and heap fit | Not performed |
| Lab 5 | Actual CPU frequency, switch-dependent interval, interval restart, BTN0 exit, debugging | Not performed |
| Reproducibility | Recreate the same hardware and software from a clean clone and fresh workspace | Not performed |

Lab 3 has no prepackaged HDL IP: your own Vivado 2022.2 wizard generates the
AXI template, and the lab explains how to modify it.
The previous `.xpr` and SDK solution projects have not been converted or rebuilt.

## Record results in your own repository

After testing on hardware, record the exact Vivado and Vitis versions/builds,
board-file revision, physical board revision, repository commit,
XSA/bitstream filenames and actual console output.
Mark only the checks you actually completed as verified.

Technical references: [SOURCES.md](SOURCES.md).  
Back to [README](../../README.md).

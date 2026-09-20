#!/usr/bin/env python3
"""Limited host-only checks. NOT a Vitis build, BSP test or HDL simulation.

Run with Python 3.9+, GCC and tclsh on PATH (Linux, WSL, or equivalent).
The mock declarations below are test fixtures, never application include files.
"""
from pathlib import Path
import os
import re
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / 'sources' / 'vitis2022_2'
TESTS = ROOT / 'tests' / 'vitis2022_2'
CC = os.environ.get('CC', 'gcc')

def run(args, *, expect_ok=True):
    result = subprocess.run([str(a) for a in args], capture_output=True, text=True)
    if (result.returncode == 0) != expect_ok:
        raise RuntimeError('Unexpected command result:\n' + ' '.join(map(str, args))
                           + '\n' + result.stdout + result.stderr)
    return result

MOCKS = {
'xil_types.h': '''#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uintptr_t UINTPTR;
''',
'xstatus.h': '#define XST_SUCCESS 0\n#define XST_FAILURE 1\n',
'xil_printf.h': 'void xil_printf(const char *format, ...);\n',
'xil_cache.h': '''void Xil_DCacheFlush(void);
void Xil_DCacheDisable(void);
void Xil_DCacheEnable(void);
''',
'xil_testmem.h': '''#include "xil_types.h"
#define XIL_TESTMEM_ALLMEMTESTS 0U
s32 Xil_TestMem32(u32 *address, u32 words, u32 pattern, u8 subtest);
''',
'xil_io.h': '''#include "xil_types.h"
void Xil_Out32(UINTPTR address, u32 value);
u32 Xil_In32(UINTPTR address);
''',
'sleep.h': 'int usleep(unsigned long useconds);\n',
'xgpio.h': '''#include "xil_types.h"
typedef struct { UINTPTR BaseAddress; u32 IsReady; int InterruptPresent; int IsDual; } XGpio;
int XGpio_Initialize(XGpio *instance, u16 device_id);
void XGpio_SetDataDirection(XGpio *instance, unsigned channel, u32 mask);
u32 XGpio_DiscreteRead(XGpio *instance, unsigned channel);
''',
'xscutimer.h': '''#include "xil_types.h"
typedef struct { u16 DeviceId; u32 BaseAddr; } XScuTimer_Config;
typedef struct { XScuTimer_Config Config; u32 IsReady; u32 IsStarted; } XScuTimer;
XScuTimer_Config *XScuTimer_LookupConfig(u16 device_id);
s32 XScuTimer_CfgInitialize(XScuTimer *instance, XScuTimer_Config *config, u32 address);
void XScuTimer_Stop(XScuTimer *instance);
void XScuTimer_Start(XScuTimer *instance);
void XScuTimer_DisableInterrupt(XScuTimer *instance);
void XScuTimer_SetPrescaler(XScuTimer *instance, u8 value);
void XScuTimer_EnableAutoReload(XScuTimer *instance);
void XScuTimer_ClearInterruptStatus(XScuTimer *instance);
void XScuTimer_LoadTimer(XScuTimer *instance, u32 value);
int XScuTimer_IsExpired(XScuTimer *instance);
'''
}
PARAMS = '''#define XPAR_SWITCHES_DEVICE_ID 0U
#define XPAR_BUTTONS_DEVICE_ID 1U
#define XPAR_LED_IP_S_AXI_BASEADDR 0x43C00000U
#define XPAR_PS7_DDR_0_S_AXI_BASEADDR 0x00100000U
#define XPAR_PS7_DDR_0_S_AXI_HIGHADDR 0x1FFFFFFFU
#define XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ 650000000U
#define XPAR_PS7_SCUTIMER_0_DEVICE_ID 0U
'''

def main():
    if shutil.which(CC) is None or shutil.which('tclsh') is None:
        raise SystemExit('Install GCC and tclsh or set CC to a supported C compiler.')
    with tempfile.TemporaryDirectory(prefix='zybo_host_checks_') as tmp:
        temp = Path(tmp)
        binary = temp / ('test_timer_math.exe' if os.name == 'nt' else 'test_timer_math')
        flags = [CC, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic']
        run(flags + ['-I', SRC / 'common', TESTS / 'test_timer_math.c', '-o', binary])
        print(run([binary]).stdout.strip())
        mocks = temp / 'mock_bsp'
        mocks.mkdir()
        for name, body in MOCKS.items():
            guard = 'TEST_MOCK_' + name.upper().replace('.', '_')
            (mocks / name).write_text('#ifndef ' + guard + '\n#define ' + guard + '\n'
                                      + body + '#endif\n', encoding='utf-8')
        parameters = mocks / 'xparameters.h'
        parameters.write_text(PARAMS, encoding='utf-8')
        sources = [SRC / folder / name for folder, name in [
            ('lab1', 'lab1_ddr_test.c'), ('lab2', 'lab2_gpio.c'),
            ('lab4', 'lab4_leds.c'), ('lab5', 'lab5_timer.c')]]
        for path in sources:
            run(flags + ['-fsyntax-only', '-I', mocks, '-I', SRC / 'common', path])
        print('PASS: C syntax for four applications with mock BSP declarations (NOT cross-compilation).')
        # Check supported alternate macro spelling, then a deliberate error.
        alternate = PARAMS.replace('XPAR_LED_IP_S_AXI_BASEADDR', 'XPAR_LED_IP_0_S_AXI_BASEADDR')
        alternate = alternate.replace('XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ', 'XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ')
        alternate = alternate.replace('XPAR_PS7_SCUTIMER_0_DEVICE_ID', 'XPAR_XSCUTIMER_0_DEVICE_ID')
        parameters.write_text(alternate, encoding='utf-8')
        run(flags + ['-fsyntax-only', '-I', mocks, '-I', SRC / 'common', sources[-1]])
        parameters.write_text(PARAMS.replace('#define XPAR_SWITCHES_DEVICE_ID 0U\n', ''), encoding='utf-8')
        result = run(flags + ['-fsyntax-only', '-I', mocks, '-I', SRC / 'common', sources[1]], expect_ok=False)
        if 'Missing switches' not in result.stderr:
            raise RuntimeError('Expected missing-instance diagnostic was not produced.')
        print('PASS: alternate macro names and missing-switches fail-fast diagnostic.')
        print(run(['tclsh', TESTS / 'test_tcl_helpers.tcl', ROOT, temp]).stdout.strip())
    pins = {}
    for f in (SRC / 'constraints').glob('*.xdc'):
        text = f.read_text(encoding='utf-8')
        for pin, port in re.findall(r'set_property -dict \{PACKAGE_PIN (\w+) IOSTANDARD LVCMOS33\} \[get_ports \{([^}]+)\}\]', text):
            if port in pins:
                raise RuntimeError('Duplicate constrained port: ' + port)
            pins[port] = pin
    expected = dict(zip(
        ['switches[%d]' % i for i in range(4)] + ['buttons[%d]' % i for i in range(4)] + ['LED[%d]' % i for i in range(4)],
        ['G15', 'P15', 'W13', 'T16', 'R18', 'P16', 'V16', 'Y16', 'M14', 'M15', 'G14', 'D18']))
    if pins != expected:
        raise RuntimeError('XDC pin/port mismatch: ' + repr(pins))
    print('PASS: 12 legacy-Zybo pin/port assignments match the reference fixture.')
    print('ALL HOST CHECKS PASSED. Vivado, Vitis, timing, DDR and board behavior remain UNTESTED.')

if __name__ == '__main__':
    main()

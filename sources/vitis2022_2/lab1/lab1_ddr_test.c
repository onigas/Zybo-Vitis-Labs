/* Test only a buffer reserved by the linker, never arbitrary live memory.
 * This is a 16 KiB functional check, NOT a full DDR qualification test.
 */
#include "xparameters.h"
#include "xil_types.h"
#include "xil_printf.h"
#include "xil_cache.h"
#include "xil_testmem.h"
#include "xstatus.h"

#if !defined(XPAR_PS7_DDR_0_S_AXI_BASEADDR) || !defined(XPAR_PS7_DDR_0_S_AXI_HIGHADDR)
#error "DDR memory range is missing. Verify the Zybo PS preset and rebuild the platform."
#endif
#define LAB_TEST_WORDS 4096U
static u32 test_buffer[LAB_TEST_WORDS] __attribute__((aligned(64)));

int main(void)
{
    const UINTPTR begin = (UINTPTR)&test_buffer[0];
    const UINTPTR end = begin + sizeof(test_buffer) - 1U;
    s32 status;
    xil_printf("\r\nLab 1: Zybo legacy, reserved DDR buffer test\r\n");
    xil_printf("Buffer: 0x%08x - 0x%08x\r\n", (unsigned)begin, (unsigned)end);
    if ((begin < (UINTPTR)XPAR_PS7_DDR_0_S_AXI_BASEADDR) ||
        (end > (UINTPTR)XPAR_PS7_DDR_0_S_AXI_HIGHADDR)) {
        xil_printf("ERROR: place .bss in DDR, then rebuild. Test not run.\r\n");
        return XST_FAILURE;
    }
    /* Cache lines must not hide actual DDR accesses in this test. */
    Xil_DCacheFlush();
    Xil_DCacheDisable();
    status = Xil_TestMem32(test_buffer, LAB_TEST_WORDS, 0xA5A5A5A5U,
                           XIL_TESTMEM_ALLMEMTESTS);
    Xil_DCacheEnable();
    if (status != 0) {
        xil_printf("FAIL: reserved DDR buffer test\r\n");
        return XST_FAILURE;
    }
    xil_printf("PASS: 16 KiB reserved DDR buffer\r\n");
    return XST_SUCCESS;
}

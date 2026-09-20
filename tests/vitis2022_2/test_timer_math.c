#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "timer_math.h"
int main(void)
{
    uint32_t sw;
    assert(lab_timer_reload(650000000U, 0U) == 32500000U);
    assert(lab_timer_reload(650000000U, 1U) == 32500000U);
    assert(lab_timer_reload(650000000U, 5U) == 162500000U);
    assert(lab_timer_reload(650000000U, 15U) == 487500000U);
    assert(lab_timer_reload(666666666U, 1U) == 33333333U);
    assert(lab_timer_reload(0U, 1U) == 0U);
    assert(lab_timer_reload(1U, 1U) == 0U);
    for (sw = 0U; sw < 16U; ++sw) {
        const uint32_t steps = sw ? sw : 1U;
        const uint64_t expected = UINT64_C(650000000) * steps / 20U;
        assert(lab_timer_reload(650000000U, sw) == expected);
        assert(lab_timer_reload(650000000U, sw) != 0U);
        assert(lab_timer_reload(650000000U, sw | 0xF0U) == expected);
    }
    puts("PASS: timer reload arithmetic, all 16 switch values, zero protection, mask.");
    return 0;
}

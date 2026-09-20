/* Pure arithmetic: independently host-testable, no Xilinx headers. */
#ifndef LAB_TIMER_MATH_H
#define LAB_TIMER_MATH_H
#include <stdint.h>
/* A9 private timer input = CPU / 2; prescaler is explicitly set to zero.
 * SW = 0 is mapped to 1, so a zero reload is never deliberately requested.
 * The division rounds down; this is not a precision time reference.
 * Return zero only when parameters cannot be represented safely.
 */
static inline uint32_t lab_timer_reload(uint32_t cpu_hz, uint32_t sw)
{
    uint32_t steps = sw & UINT32_C(15);
    uint64_t ticks;
    if (steps == 0U) {
        steps = 1U;
    }
    ticks = ((uint64_t)cpu_hz * steps) / UINT64_C(20);
    if ((ticks == 0U) || (ticks > UINT32_MAX)) {
        return 0U;
    }
    return (uint32_t)ticks;
}
#endif

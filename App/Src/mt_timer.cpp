#include "mt_timer.hpp"

extern volatile uint32_t h{0}, m{0}, s{0};

extern "C" {
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    static uint64_t tk = 0;

    if (htim->Instance == TIM1) {
        // 50ms it
        if (++tk == 20) {
            tk = 0;
            ++s;
        }
        if (s == 60) {
            s = 0;
            ++m;
        }
        if (m == 60) {
            m = 0;
            ++h;
        }
    }
}
}

#include <functional>
#include <vector>

#include "pch.h"
std::vector<uint32_t> v;
uint64_t* p = nullptr;
void cppfn(std::function<void(uint64_t)> callback) {
    LED1_ON;
    HAL_Delay(200);
    LED1_OFF;
    HAL_Delay(200);
    callback(514);

    if (v.size() > 15) {
        LED1_ON;
        LED1_OFF;
        callback(514);
        for (auto num : v) {
            auto n = num + 1;
        }
        v.clear();
    }
    v.push_back((*p)++);

    if (!p)
        p = new uint64_t{0};
}

extern "C" {
void maMain(void) {
    static uint64_t hey = 114514;
    auto f = [&](auto i) { hey += i; };

    if (hey > 114514 + 1919) {
        hey = 114514;
        HAL_Delay(1000);
    } else
        cppfn(f);
}
}
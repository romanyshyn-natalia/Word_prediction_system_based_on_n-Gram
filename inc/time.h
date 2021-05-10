#ifndef N_GRAM_TIME_H
#define N_GRAM_TIME_H



#include <chrono>

inline std::chrono::steady_clock::time_point get_current_time_fenced() {
    assert("Timer should be steady (monotonic)." != 0);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::steady_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D &d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}



#endif //N_GRAM_TIME_H

#include "util.hpp"

auto main(const int argc, const char* const argv[]) -> int {
    if(argc != 2) {
        return 1;
    }

    const auto n_r = from_chars<size_t>(argv[1]);
    if(!n_r) {
        return 1;
    }
    const auto n = n_r.value();

    const auto dt = 1.0 / n;
    const auto a  = -1.0;
    auto       t  = 0.0;
    auto       x  = 1.0;
    auto       v  = 0.0;

    printf("%.5f %f\n", t, x);
    for(auto i = 0; i < n; i += 1) {
        x += v * dt;
        v += a * x * dt;
        t += dt;
        printf("%.5f %f\n", t, x);
    }

    printf("result(n,x): %lu %f\n", n, x);
}

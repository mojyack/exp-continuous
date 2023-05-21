#include "util.hpp"

struct Planet {
    double m;
    double px;
    double py;
    double vx;
    double vy;
};

class Simulator {
  private:
    static constexpr auto g = 4.0;

    double dt;
    Planet o1;
    Planet o2;

    auto force() -> std::array<double, 2> {
        const auto dx  = (o1.px - o2.px);
        const auto dy  = (o1.py - o2.py);
        const auto r   = std::sqrt(dx * dx + dy * dy);
        const auto mmg = o1.m * o2.m * g;
        const auto fx  = -mmg * dx / (r * r * r);
        const auto fy  = -mmg * dy / (r * r * r);
        return {double(fx), double(fy)};
    }

  public:
    auto update() -> void {
        const auto [fx, fy] = force();

        o1.vx += fx * dt / o1.m;
        o1.vy += fy * dt / o1.m;
        o1.px += o1.vx * dt;
        o1.py += o1.vy * dt;

        o2.vx += -fx * dt / o2.m;
        o2.vy += -fy * dt / o2.m;
        o2.px += o2.vx * dt;
        o2.py += o2.vy * dt;
    }

    auto get_planets() const -> std::array<const Planet*, 2> {
        return {&o1, &o2};
    }

    Simulator(const double dt) : dt(dt) {
        o1 = Planet{1, 1, 0, 0, 1};
        o2 = Planet{1, -1, 0, 0, -1};
    }
};

auto main(const int argc, const char* argv[]) -> int {
    if(argc != 3) {
        return 1;
    }

    const auto dt_r = from_chars<double>(argv[1]);
    if(!dt_r) {
        return 2;
    }
    const auto dt = dt_r.value();

    const auto limit_r = from_chars<size_t>(argv[2]);
    if(!limit_r) {
        return 2;
    }
    const auto limit = limit_r.value();

    auto simulator = Simulator(dt);
    for(auto i = 0; i < limit; i += 1) {
        const auto [p1, p2] = simulator.get_planets();
        printf("%d %lf %lf %lf %lf\n", i, p1->px, p1->py, p2->px, p2->py);
        simulator.update();
    }

    return 0;
}

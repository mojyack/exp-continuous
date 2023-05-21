#include "util.hpp"

struct Vec2 {
    double x;
    double y;
};

struct Planet {
    std::array<Vec2, 3> p; // [0] position, [1] velocity, [2] acceleration
    double              m;

    static auto from_stdin() -> Planet {
        const auto m  = read_stdin<double>("m? ");
        const auto px = read_stdin<double>("px? ");
        const auto py = read_stdin<double>("py? ");
        const auto vx = read_stdin<double>("vx? ");
        const auto vy = read_stdin<double>("vy? ");
        return Planet{{px, py, vx, vy, 0, 0}, m};
    }
};

enum class Method {
    Euler,
    MEuler,
    Runge,
};

class Simulator {
  private:
    static constexpr auto g = 4.0;

    Method method;
    double dt;
    Planet o1;
    Planet o2;

    // update acceleration
    static auto kepler(Planet& o1, Planet& o2) -> void {
        const auto dx = (o1.p[0].x - o2.p[0].x);
        const auto dy = (o1.p[0].y - o2.p[0].y);
        const auto r  = std::sqrt(dx * dx + dy * dy);

        o1.p[2].x = -g * o2.m * dx / (r * r * r);
        o1.p[2].y = -g * o2.m * dy / (r * r * r);
        o2.p[2].x = +g * o1.m * dx / (r * r * r);
        o2.p[2].y = +g * o1.m * dy / (r * r * r);
    }

    static auto update_euler(Planet& o1, Planet& o2, const double dt) -> void {
        kepler(o1, o2);

        for(auto i = 0; i <= 2; i += 1) {
            o1.p[i].x += dt * o1.p[i + 1].x;
            o1.p[i].y += dt * o1.p[i + 1].y;

            o2.p[i].x += dt * o2.p[i + 1].x;
            o2.p[i].y += dt * o2.p[i + 1].y;
        }
    }

    static auto update_meuler(Planet& o1, Planet& o2, const double dt) -> void {
        const auto do_meuler = [dt](Planet o1, Planet o2) -> Planet {
            kepler(o1, o2);
            auto k1 = o1;
            auto k2 = o1;
            for(auto i = 0; i < 2; i += 1) {
                k1.p[i].x = dt * k1.p[i + 1].x;
                k1.p[i].y = dt * k1.p[i + 1].y;
                k2.p[i].x += k1.p[i].x;
                k2.p[i].y += k1.p[i].y;
            }
            for(auto i = 0; i < 2; i += 1) {
                k2.p[i].x = dt * k2.p[i + 1].x;
                k2.p[i].y = dt * k2.p[i + 1].y;
                o1.p[i].x += (k1.p[i].x + k2.p[i].x) / 2.0;
                o1.p[i].y += (k1.p[i].y + k2.p[i].y) / 2.0;
            }

            return o1;
        };

        const auto t1 = do_meuler(o1, o2);
        const auto t2 = do_meuler(o2, o1);

        o1 = t1;
        o2 = t2;
    }

    static auto update_runge(Planet& o1, Planet& o2, const double dt) -> void {
        const auto do_runge = [dt](Planet o1, Planet o2) -> Planet {
            auto k1 = o1;
            auto k2 = o1;
            auto k3 = o1;
            auto k4 = o1;
            kepler(k1, o2);
            for(auto i = 0; i < 2; i += 1) {
                k1.p[i].x = dt * k1.p[i + 1].x;
                k1.p[i].y = dt * k1.p[i + 1].y;
                k2.p[i].x += k1.p[i].x / 2.0;
                k2.p[i].y += k1.p[i].y / 2.0;
            }
            kepler(k2, o2);
            for(auto i = 0; i < 2; i += 1) {
                k2.p[i].x = dt * k2.p[i + 1].x;
                k2.p[i].y = dt * k2.p[i + 1].y;
                k3.p[i].x += k2.p[i].x / 2.0;
                k3.p[i].y += k2.p[i].y / 2.0;
            }
            kepler(k3, o2);
            for(auto i = 0; i < 2; i += 1) {
                k3.p[i].x = dt * k3.p[i + 1].x;
                k3.p[i].y = dt * k3.p[i + 1].y;
                k4.p[i].x += k3.p[i].x;
                k4.p[i].y += k3.p[i].y;
            }
            kepler(k4, o2);
            for(auto i = 0; i < 2; i += 1) {
                k4.p[i].x = dt * k4.p[i + 1].x;
                k4.p[i].y = dt * k4.p[i + 1].y;

                o1.p[i].x += (k1.p[i].x + 2.0 * k2.p[i].x + 2.0 * k3.p[i].x + k4.p[i].x) / 6.0;
                o1.p[i].y += (k1.p[i].y + 2.0 * k2.p[i].y + 2.0 * k3.p[i].y + k4.p[i].y) / 6.0;
            }

            return o1;
        };

        const auto t1 = do_runge(o1, o2);
        const auto t2 = do_runge(o2, o1);

        o1 = t1;
        o2 = t2;
    }

  public:
    auto update() -> void {
        switch(method) {
        case Method::Euler:
            update_euler(o1, o2, dt);
            break;
        case Method::MEuler:
            update_meuler(o1, o2, dt);
            break;
        case Method::Runge:
            update_runge(o1, o2, dt);
            break;
        }
    }

    auto get_planets() const -> std::array<const Planet*, 2> {
        return {&o1, &o2};
    }

    Simulator(const Method method, const double dt)
        : method(method),
          dt(dt) {
        print("planet 1: ");
        o1 = Planet::from_stdin();
        print("planet 2: ");
        o2 = Planet::from_stdin();
        std::cout << std::endl;
    }
};

auto main(const int argc, const char* argv[]) -> int {
    if(argc != 4) {
        return 1;
    }

    auto method = Method();
    if(std::strcmp(argv[1], "euler") == 0) {
        method = Method::Euler;
    } else if(std::strcmp(argv[1], "meuler") == 0) {
        method = Method::MEuler;
    } else if(std::strcmp(argv[1], "runge") == 0) {
        method = Method::Runge;
    } else {
        puts("unknown method");
        return 1;
    }

    const auto dt_r = from_chars<double>(argv[2]);
    if(!dt_r) {
        return 2;
    }
    const auto dt = dt_r.value();

    const auto limit_r = from_chars<size_t>(argv[3]);
    if(!limit_r) {
        return 2;
    }
    const auto limit = limit_r.value();

    auto simulator = Simulator(method, dt);
    for(auto i = 0; i < limit; i += 1) {
        const auto [p1, p2] = simulator.get_planets();
        printf("%d %lf %lf %lf %lf\n", i, p1->p[0].x, p1->p[0].y, p2->p[0].x, p2->p[0].y);
        simulator.update();
    }

    return 0;
}

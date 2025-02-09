/* external standard headers */
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>  
#include <thread>

/* internal headers */
#include <gen.h>
#include <tools.h>

gen::gen(int n, int w, int h, int body_radius, float speed_rate)
{
    srand(time(0));

    max_x = w;
    max_y = h;
    srate = speed_rate;
    r = body_radius;

    for (int i = 0; i < n; i++) 
    {
        body b;
        b.x = rand() % w;
        b.y = rand() % h;
        b.color = rand() % NLABEL;
        b.sx = 0.1f + static_cast<float>(rand()) / (RAND_MAX / 0.9f);
        b.sy = 0.1f + static_cast<float>(rand()) / (RAND_MAX / 0.9f);
        bodies.push_back(b);
    }
}

vector<body> gen::currents()
{
    lock_guard<mutex> lock(simsync);
    return bodies;
}

void gen::set_speed(float new_speed)
{
    lock_guard<mutex> lock(simsync);
    srate = new_speed;
}

void gen::set_size(int W, int H)
{
    max_x = W;
    max_y = H;
}

void gen::start(std::atomic<bool>& run_simulation)
{
    std::cout << std::endl;

    while (run_simulation)
    {
        this_thread::sleep_for(chrono::milliseconds(30));

        lock_guard<mutex> lock(simsync);
        for (auto &b : bodies)
        {
            b.x = b.x + static_cast<int>(b.sx*srate);
            b.y = b.y + static_cast<int>(b.sy*srate);
            /* body (ball) bounces */
            if (b.x - r <= 0 || b.x + r >= max_x)
                b.sx *= -1;
            if (b.y - r <= 0 || b.y + r >= max_y)
                b.sy *= -1;
        }
    }
}

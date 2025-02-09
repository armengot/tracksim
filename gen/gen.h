#ifndef GEN_H
#define GEN_H

/* external standard libs */
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

using namespace std;
using namespace cv;

typedef struct
{
    int x;
    int y;
    int color;
    float sx;
    float sy;
} body;

/* class generator of moving bodies */
class gen
{
    public:
        gen(int n, int w, int h, int body_radius, float speed_rate);
        void start(std::atomic<bool>& run_simulation);
        vector<body> currents();
        void set_speed(float new_speed);
        void set_size(int W, int H);

    private:
        vector<body> bodies;
        int max_x = 0;
        int max_y = 0;
        int r = 0;
        float srate;
        mutex simsync;
};




#endif

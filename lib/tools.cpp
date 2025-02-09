/* external standard libs */
#include <sstream>
#include <atomic>
#include <thread>
#include <opencv2/opencv.hpp>

/* internal headers */
#include <tools.h>
#include <gen.h>

int x11colors[NLABEL][3] =
{
    {255, 0, 0},       // color 00: Red
    {0, 255, 0},       // color 01: Green
    {0, 0, 255},       // color 02: Blue
    {255, 255, 0},     // color 03: Yellow
    {255, 0, 255},     // color 04: Magenta
    {0, 255, 255},     // color 05: Cyan
    {255, 128, 0},     // color 06: Orange
    {128, 0, 255},     // color 07: Violet
    {128, 255, 0},     // color 08: Lime
    {255, 0, 128},     // color 09: Pink
    {0, 128, 255},     // color 10: Sky Blue
    {128, 0, 128},     // color 11: Purple
    {0, 128, 128},     // color 12: Teal
    {128, 128, 0},     // color 13: Lime Green
    {128, 128, 128},   // color 14: Gray
    {255, 255, 255},   // color 15: White
    {0, 255, 0},       // color 16: Green2
    {0, 0, 128},       // color 17: Blue2
    {255, 255, 0},     // color 18: Yellow2
    {255, 0, 128},     // color 19: Magenta2
    {0, 255, 128},     // color 20: Cyan2
    {128, 255, 0},     // color 21: Lime2
    {255, 128, 128},   // color 22: Pink2
    {128, 128, 255},   // color 23: Violet2
    {128, 255, 128},   // color 24: Lime Green2
    {128, 128, 0},     // color 25: Olive
    {255, 128, 0},     // color 26: Orange2
    {0, 128, 128},     // color 27: Teal2
    {128, 0, 128},     // color 28: Purple2
    {128, 128, 64},    // color 29: Olive Green
    {128, 64, 128},    // color 30: Plum
    {64, 128, 128},    // color 31: Light Teal
    {192, 192, 128},   // color 32: Khaki
    {128, 192, 192},   // color 33: Light Cyan
    {192, 128, 192},   // color 34: Orchid
    {192, 192, 192},   // color 35: Light Gray
    {0, 0, 192},       // color 36: Dark Blue
    {192, 0, 0},       // color 37: Dark Red
    {0, 192, 0},       // color 38: Dark Green
    {0, 192, 192},     // color 39: Dark Cyan
    {192, 0, 192},     // color 40: Dark Magenta
    {192, 192, 0},     // color 41: Dark Yellow
    {128, 0, 0},       // color 42: Maroon
    {0, 128, 0},       // color 43: Dark Green2
    {0, 0, 64},        // color 44: Navy
    {128, 128, 0},     // color 45: Olive2
    {0, 128, 64},      // color 46: Forest Green
    {128, 0, 128},     // color 47: Purple3
    {128, 64, 0},      // color 48: Brown
    {128, 128, 64},    // color 49: Olive Green2
    {128, 0, 64},      // color 50: Plum2
    {64, 128, 128},    // color 51: Light Teal2
    {192, 128, 0},     // color 52: Orange3
    {0, 128, 192},     // color 53: Light Blue
    {192, 0, 128},     // color 54: Orchid2
    {192, 192, 0},     // color 55: Dark Yellow2
    {0, 192, 128},     // color 56: Medium Aquamarine
    {192, 128, 192},   // color 57: Orchid3
    {128, 192, 128},   // color 58: Pale Green
    {192, 128, 128},   // color 59: Rosy Brown
    {128, 64, 64},     // color 60: Brown2
    {128, 128, 192},   // color 61: Light Steel Blue
    {64, 128, 192},    // color 62: Steel Blue
    {192, 64, 128},    // color 63: Orchid4
    {128, 64, 192},    // color 64: Purple4
    {128, 192, 64},    // color 65: Dark Green3
    {192, 64, 64},     // color 66: Maroon2
    {64, 192, 64},     // color 67: Olive Green3
    {64, 64, 192},     // color 68: Dark Blue2
    {192, 64, 64},     // color 69: Dark Red2
    {64, 192, 192},    // color 70: Teal
    {192, 192, 64},    // color 71: Dark Olive Green
    {192, 64, 192},    // color 72: Plum3
    {64, 192, 192},    // color 73: Light Teal3
    {192, 192, 128},   // color 74: Khaki2
    {128, 192, 64},    // color 75: Olive Green4
    {64, 128, 192},    // color 76: Light Blue2
    {192, 64, 64},     // color 77: Dark Red3
    {192, 128, 64},    // color 78: Orange4
    {64, 192, 128},    // color 79: Pale Turquoise
};

/* globals */
float GLOBAL_SPEED_FACTOR = 5.0;
int userbanner = 0;
bool det_enabled = 0;
std::string winbanner = "";
int N = 10;
int W = 800;
int H = 800;
int r = 20;

void user_listener(std::atomic<bool>& run_simulation, std::atomic<char>& last_key)
{
    while (run_simulation)
    {
        if (last_key!=-1)
        {
            userbanner = 200;
        }
        else
        {
            userbanner--;
        }
        if (last_key == 27 || last_key == 'q' || last_key == 'Q')
        {
            run_simulation.store(false);
            break;
        }
        if (last_key == 'W')
        {
            W = W + 100;
        }
        if (last_key == 'w')
        {
            W = W - 100;
        }
        if (last_key == 'H')
        {
            H = H + 100;
        }
        if (last_key == 'h')
        {
            H = H - 100;
        }
        if (last_key == 'R')
        {
            r = r + 1;
        }
        if (last_key == 'r')
        {
            r = r - 1;
        }                                     
        if (last_key == '+')
        {
            GLOBAL_SPEED_FACTOR = GLOBAL_SPEED_FACTOR + 1.0;
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(1) << std::setw(6) << std::setfill(' ') << GLOBAL_SPEED_FACTOR << "        ";
            winbanner = "SPEED: " + ss.str();
        }
        if (last_key == '-')
        {
            GLOBAL_SPEED_FACTOR = GLOBAL_SPEED_FACTOR - 1.0;
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(1) << std::setw(6) << std::setfill(' ') << GLOBAL_SPEED_FACTOR << "        ";
            winbanner = "SPEED: " + ss.str(); 
        }
        if (last_key == 'd' || last_key == 'D')
        {
            det_enabled = !det_enabled;
            if (det_enabled)
                winbanner = "DETECTOR ENABLED  ";
            else
                winbanner = "DETECTOR DISABLED ";
        }        
        last_key.store(-1);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));          
        if (userbanner<0)
        {
            winbanner = "";
        }
    }
}


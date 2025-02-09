#ifndef TOOLS_H
#define TOOLS_H

/* external standard */
#include <atomic>

/* interna headers */
#include <gen.h>

/* globals */
extern float GLOBAL_SPEED_FACTOR;
extern int userbanner;
const int NLABEL = 80;
extern int x11colors[NLABEL][3];
extern bool det_enabled;
extern std::string winbanner;

/* configuration */
extern int N;
extern int W;
extern int H;
extern int r;

/* debug colors */
#define DEBUG_REDTEXT "\033[31m"
#define DEBUG_GREENXT "\033[92m"
#define DEBUG_BLUETXT "\033[94m"
#define DEBUG_RESETXT "\033[39m"

/* tools */
void user_listener(std::atomic<bool>& run_simulation, std::atomic<char>& last_key);

#endif

/* external standard libs */
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

/* internal libs */
#include <tools.h>

using namespace std;
using namespace cv;

typedef struct
{
    int x;
    int y;
    int w;
    int h;
    int sx;
    int sy;
    int n;
}det;

/* class consumer of movement */
class con
{
    public:
        con();
        vector<det> detect(cv::Mat& frame);
        void reboot();
        
    private:
        vector<det> detections;
        vector<det> previous_detections;
        int frame_h;
        int frame_w;
        int detection_counter = 1;
};


/* external standard libs */
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include <iomanip>
#include <sstream> 

/* internal standard libs */
#include <tools.h>
#include <gen.h>
#include <con.h>

using namespace std;

void start_sim(gen* generator, atomic<bool>& run_simulation)
{
    generator->start(run_simulation);
}

int main()
{
    /* init */
    string winame = "TV-SIM";
    unsigned int nframe = 0;

    /* main objects and threads */
    gen generator(N, W, H, r, GLOBAL_SPEED_FACTOR);
    con detector;
    vector<std::thread> multi_th_stack;

    /* shared multi threads*/
    std::atomic<bool> run_simulation(true);
    std::atomic<char> last_key(-1);

    /* tv start */
    cv::namedWindow(winame, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(winame,400,200);
    
    /* coffee break */
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    /* threads running */
    multi_th_stack.push_back(std::thread(start_sim, &generator, std::ref(run_simulation)));
    multi_th_stack.push_back(std::thread(user_listener, ref(run_simulation), ref(last_key)));

    while (run_simulation)
    {
        cv::Mat frame = cv::Mat::zeros(H+r, W+r, CV_8UC3);

        /* synthetic generation */
        vector<body> blobs = generator.currents();
        for (const auto& b : blobs)
        {
            cv::Scalar body_color = cv::Scalar(x11colors[b.color][0], x11colors[b.color][1], x11colors[b.color][2]);
            cv::circle(frame, cv::Point(b.x, b.y), r, body_color, -1);
        }

        /* detection consumer */
        if (det_enabled)
        {
            vector<det> detections = detector.detect(frame);
            for (const auto& d : detections)
            {
                cv::rectangle(frame, cv::Point(d.x, d.y),cv::Point(d.x + d.w, d.y + d.h),cv::Scalar(255, 255, 255), 1);
                cv::putText(frame, std::to_string(d.n),cv::Point(d.x, d.y),cv::FONT_HERSHEY_SIMPLEX, r/20.0,cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
            }
        }
        /* banner */
        float base_font_scale = 1.0f;
        int base_font_thickness = 2;
        float font_scale = base_font_scale * (W / 1500.0f);
        int font_thickness = std::max(1, static_cast<int>(base_font_thickness * (W / 1500.0f)));        
        cv::Size ltext = cv::getTextSize(winbanner, cv::FONT_HERSHEY_SIMPLEX, font_scale, font_thickness, nullptr);
        int txt_x = W - ltext.width - 20;
        int txt_y = 50;                
        cv::putText(frame, winbanner, cv::Point(txt_x, txt_y), cv::FONT_HERSHEY_SIMPLEX, font_scale, cv::Scalar(255, 255, 255), font_thickness, cv::LINE_AA);

        /* results */
        cv::imshow(winame, frame);
        char key = -1;
        key = cv::waitKey(1);
        last_key.store(key);
        generator.set_speed(GLOBAL_SPEED_FACTOR);
        generator.set_size(W,H);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        
        if (userbanner > 0)
            std::cout << "\rSPEED RATE: " << std::fixed << std::setprecision(1) << std::setw(6) << std::setfill(' ') << GLOBAL_SPEED_FACTOR << " FRAME: " << std::setw(10) << std::setfill('0') << nframe << " --- KEY PRESSED --- " << userbanner;
        else
            std::cout << "\rSPEED RATE: " << std::fixed << std::setprecision(1) << std::setw(6) << std::setfill(' ') << GLOBAL_SPEED_FACTOR << " FRAME: " << std::setw(10) << std::setfill('0') << nframe << " ------------------- " << userbanner;
        fflush(stdout);
        nframe++;
    }

    for (auto& thread : multi_th_stack)
    {
        thread.join();
    }

    return(0);
}

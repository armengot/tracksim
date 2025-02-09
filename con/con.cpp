/* external standard headers */
#include <string>
#include <vector>

/* internal headers */
#include <con.h>

con::con()
{

}

void con::reboot()
{
    detections.clear();
    previous_detections.clear();
}

vector<det> con::detect(cv::Mat& frame)
{
    detections.clear();

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::Mat thresholded;
    cv::threshold(gray, thresholded, 1, 255, cv::THRESH_BINARY);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    cv::findContours(thresholded, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<det> new_detections;
    for (const auto& contour : contours)
    {
        Rect bbox = boundingRect(contour);
        det detection;
        detection.x = bbox.x;
        detection.y = bbox.y;
        detection.w = bbox.width;
        detection.h = bbox.height;
        detection.sx = 0;
        detection.sy = 0;
        detection.n = -1;

        new_detections.push_back(detection);
    }

    for (auto& new_det : new_detections)
    {
        int best_id = -1;
        double min_distance = 50.0;

        for (const auto& old_det : previous_detections)
        {
            double distance = std::sqrt(std::pow(new_det.x - old_det.x, 2) + std::pow(new_det.y - old_det.y, 2));
            
            if (distance < min_distance)
            {
                min_distance = distance;
                best_id = old_det.n;
            }
        }

        if (best_id != -1)
        {
            new_det.n = best_id;
        }
        else
        {
            new_det.n = detection_counter++;
        }

        detections.push_back(new_det);
    }

    previous_detections = detections;

    return detections;
}

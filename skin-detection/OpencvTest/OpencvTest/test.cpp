#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video.hpp>
using namespace cv;

int main(int argc, char *argv[]) {
	cv::Mat src, srcdown, bg, frMOG2, erframe, dlframe, thrframe;
	namedWindow("windows 1", CV_WINDOW_AUTOSIZE);
	cv::VideoCapture cap(0);
	cv::BackgroundSubtractorMOG bgMOG2 = cv::BackgroundSubtractorMOG(3000, 16.0, false);
	if (!cap.isOpened()){ printf("--(!)no frame from camera\n"); return -1; };
	for (;;) {
		cap >> src;
		bgMOG2.operator()(src, frMOG2);
		cv::threshold(frMOG2, thrframe, 127, 255, cv::THRESH_BINARY);
		
		/*cv::dilate(thrframe, dlframe, cv::mat());
		cv::erode(dlframe, erframe, cv::mat());
		std::vector<std::vector<cv::point>>contours;
		cv::findcontours(erframe, contours, cv_retr_external, cv_chain_approx_none);
		for (int i = 0; i < contours.size(); i++) {
			if (contourarea(contours[i]) >= 500) {
				vector<vector<point> > tcontours;
				tcontours.push_back(contours[i]);
				cv::drawcontours(srcdown, tcontours, -1, cv::scalar(0, 0, 255), 2);
			}
		}*/
		cv::imshow("windows 1", frMOG2);
		if (waitKey(30) >= 0) break;
	}
	return(0);
}
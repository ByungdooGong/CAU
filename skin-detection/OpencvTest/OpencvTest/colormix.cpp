#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int minH = 0, maxH = 20, minS = 30, maxS = 150, minV = 60, maxV = 255;
cv::Mat frame;
int count = 0;

float innerAngle(float px1, float py1, float px2, float py2, float cx1, float cy1)
{

	float dist1 = std::sqrt((px1 - cx1)*(px1 - cx1) + (py1 - cy1)*(py1 - cy1));
	float dist2 = std::sqrt((px2 - cx1)*(px2 - cx1) + (py2 - cy1)*(py2 - cy1));

	float Ax, Ay;
	float Bx, By;
	float Cx, Cy;

	//find closest point to C  
	//printf("dist = %lf %lf\n", dist1, dist2);  

	Cx = cx1;
	Cy = cy1;
	if (dist1 < dist2)
	{
		Bx = px1;
		By = py1;
		Ax = px2;
		Ay = py2;


	}
	else{
		Bx = px2;
		By = py2;
		Ax = px1;
		Ay = py1;
	}


	float Q1 = Cx - Ax;
	float Q2 = Cy - Ay;
	float P1 = Bx - Ax;
	float P2 = By - Ay;


	float A = std::acos((P1*Q1 + P2*Q2) / (std::sqrt(P1*P1 + P2*P2) * std::sqrt(Q1*Q1 + Q2*Q2)));

	A = A * 180 / CV_PI;

	return A;
}

void CallbackFunc(int event, int x, int y, int flags, void* userdata)
{
	cv::Mat RGB = frame(cv::Rect(x, y, 1, 1));
	cv::Mat HSV;
	cv::cvtColor(RGB, HSV, CV_BGR2HSV);
	cv::Vec3b pixel = HSV.at<cv::Vec3b>(0, 0);
	if (event == cv::EVENT_LBUTTONDBLCLK) // on double left clcik
	{
		std::cout << "Click" << std::endl;
		int h = pixel.val[0];
		int s = pixel.val[1];
		int v = pixel.val[2];
		if (count == 0)
		{
			minH = h;
			maxH = h;
			minS = s;
			maxS = s;
			minV = v;
			maxV = v;
		}
		else
		{
			if (h < minH)
			{
				minH = h;
			}
			else if (h > maxH)
			{
				maxH = h;
			}
			if (s < minS)
			{
				minS = s;
			}
			else if (s > maxS)
			{
				maxS = s;
			}
			if (v < minV)
			{
				minV = v;
			}
			else if (v > maxV)
			{
				maxV = v;
			}

		}
		count++;
	}
	std::cout << pixel << std::endl;
}

int main()
{
	cv::VideoCapture cap(0);

	const char* windowName = "Fingertip detection";
	int minH = 0, maxH = 42, minS = 62, maxS = 168, minV = 60, maxV = 255;
	int minthresh = 135; int maxthresh = 225;
	int thresh = 150;
	cv::namedWindow(windowName);
	/*cv::createTrackbar("MinH", windowName, &minH, 180);
	cv::createTrackbar("MaxH", windowName, &maxH, 180);
	cv::createTrackbar("MinS", windowName, &minS, 255);
	cv::createTrackbar("MaxS", windowName, &maxS, 255);
	cv::createTrackbar("MinV", windowName, &minV, 255);
	cv::createTrackbar("MaxV", windowName, &maxV, 255);
	*/
	createTrackbar("minthreshold", windowName, &minthresh, 255);
	createTrackbar("maxthreshold", windowName, &maxthresh, 255);
	createTrackbar("threshold", windowName, &thresh, 255);
	while (1)
	{
		cap >> frame;
		Mat imageRoi = frame(Rect(100, 50, 350, 350));
				
		Mat hsv;
		Mat ycb;
		Mat img(imageRoi.rows, imageRoi.cols, CV_8UC1);
		
		cvtColor(imageRoi, hsv, CV_BGR2HSV);
		cvtColor(imageRoi, ycb, CV_BGR2HSV);
		
		for (int j = 0; j < imageRoi.rows; j++)
		{
			uchar* data = imageRoi.ptr<uchar>(j);
			uchar* ycbData = ycb.ptr<uchar>(j);
			uchar* hsvData = hsv.ptr<uchar>(j);
			uchar* imgData = img.ptr<uchar>(j);
			for (int i = 0; i < imageRoi.cols*imageRoi.channels(); i+=3)
			{
				if (data[i + 2] > 20 && data[i + 1] > 20 && data[i] > 20 && abs(data[i + 2] - data[i + 1]) > 15 && data[i + 2] > data[i] && data[i + 2] > data[i+1] 
					&& hsvData[i] >= 0.0 && hsvData[i] <= 80.0 && hsvData[i + 1] >= 80)
				{
					imgData[i/3] = 255;
					
				}
				else
					imgData[i/3] = 0;
			}
		}
		imshow("img", img);
		
		// median blur
		Mat median;
		//int elementSize = 5;
		cv::medianBlur(img, median, 5);
		//cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * elementSize + 1, 2 * elementSize + 1), cv::Point(elementSize, elementSize));
		//cv::dilate(nolight, median, element);
		imshow("median", median);
		/*
		//gaussian blur
		Mat gaussian;
		GaussianBlur(nolight, gaussian, Size(5, 5), 3, 3);
		imshow("gaussian", gaussian);

		//bilateralfilter
		Mat bilateral;
		bilateralFilter(nolight, bilateral, 5, 250, 10);
		imshow("bilateral", bilateral);*/
		// Contour detection
		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(median, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));  //contour 찾고( image, contours, hierarchy, 윤곽 검색모드, 윤곽 근사방법, offset)
		size_t largestContour = 0;
		for (size_t i = 1; i < contours.size(); i++)
		{
			if (cv::contourArea(contours[i]) > cv::contourArea(contours[largestContour]))  //contour 면적 = 최대면적 
				largestContour = i;
		}
		cv::drawContours(imageRoi, contours, largestContour, cv::Scalar(0, 0, 255), 2);  //contour 그린다(빨간색으로)(result, contours, 최대contours, 색, 두께)
		imshow(windowName, imageRoi);
		if (waitKey(30) >= 0) break;
	}
	return 0;
}
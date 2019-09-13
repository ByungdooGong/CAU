#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video.hpp>
using namespace cv;

int minH = 0, maxH = 20, minS = 30, maxS = 150, minV = 60, maxV = 255;
cv::Mat frame; 
Mat frMOG2;
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
	namedWindow(windowName);
	setMouseCallback(windowName, CallbackFunc, NULL);
	
	int minH = 0, maxH = 42, minS = 62, maxS = 168, minV = 60, maxV = 255;
	int minthresh = 132; int maxthresh = 225;
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
		//cvtColor(imageRoi, CV_BGR2RGB);
		vector<Mat> cha;
		split(imageRoi, cha);
		
		imshow("B", cha[0]);
		imshow("G", cha[1]);
		imshow("R", cha[2]);
		

		//inRange(cha[0], minthresh, maxthresh, cha[0]);
		//inRange(cha[1], minthresh, maxthresh, cha[1]);
		//inRange(cha[2], minthresh, maxthresh, cha[2]);
		//imshow("cha[0]", cha[0]);
		//imshow("cha[1]", cha[1]);
		//imshow("cha[2]", cha[2]);
		Mat binary;
		//cvtColor(imageRoi, binary, CV_BGR2GRAY);
		//adaptiveThreshold(binary, binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, thresh);
		//threshold(binary, binary, 200, 255, THRESH_BINARY|THRESH_OTSU);
		//imshow("output", binary);
				
		cv::Mat hsv;
		cv::cvtColor(imageRoi, hsv, CV_BGR2HSV);
		/*vector<Mat> channels;
		split(hsv, channels);
		imshow("H", channels[0]);
		imshow("s", channels[1]);
		imshow("v", channels[2]);
		*/
					
		Mat lab;
		cvtColor(imageRoi, lab, CV_BGR2Lab);
		vector<Mat> lchannels;
		split(lab, lchannels);
		//imshow("L", lchannels[0]);
		//imshow("a", lchannels[1]);
		//imshow("b", lchannels[2]);
		Mat nolight = 0.5*lchannels[1] + 0.5*lchannels[2];
		//imshow("nolight", nolight);
		/*threshold(nolight, nolight, 132, 255, THRESH_BINARY | THRESH_OTSU);
		dilate(nolight, nolight, Mat());
		erode(nolight, nolight, Mat());
		*/
		//imshow("otsu", nolight);
		//bgMOG2.operator()(nolight, nolight);
		for (int i = 0; i < lab.rows; i++){
			for (int j = 0; j < lab.cols ; j++ )
			{
				if (lab.at<Vec3b>(j, i)[0] > 160 && lab.at<Vec3b>(j, i)[0] < 200)
					nolight.at<uchar>(j, i) = 255;
			}
		}
		//imshow("nl", nolight);

		
		//cv::inRange(nolight, minthresh, maxthresh, nolight);
		//cvtColor(imageRoi, nolight, CV_BGR2GRAY);
		//adaptiveThreshold(nolight, nolight, 255, ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 15, 3);
		
		//threshold(nolight, nolight, thresh, 255, THRESH_BINARY);
		for (int i = 0; i < nolight.rows; i++)
		{
			for (int j = 0; j < nolight.cols; j++)
			{
				if (nolight.at<uchar>(j, i) > minthresh && nolight.at<uchar>(j, i) < maxthresh )
					//&& hsv.at<Vec3b>(j, i)[0] > 0 && hsv.at<Vec3b>(j, i)[0] < 50
					//&& hsv.at<Vec3b>(j, i)[1] >= 0.23 * 255 && hsv.at<Vec3b>(j, i)[1] <= 0.68*255)
					nolight.at<uchar>(j, i) = 255;
				else 
					nolight.at<uchar>(j, i) = 0;
			}
		}
		//imshow("binary", nolight);
		
		dilate(nolight, nolight, Mat());
		erode(nolight, nolight, Mat());
		
		dilate(nolight, nolight, Mat());
		erode(nolight, nolight, Mat());

		//dilate(nolight, nolight, Mat());
		erode(nolight, nolight, Mat());
		//imshow("eroded", nolight);

		/*Mat pa;
		cvtColor(imageRoi, pa, CV_BGR2GRAY);
		adaptiveThreshold(pa, pa, 255, ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 5, thresh);
		
		imshow("adaptive", pa);
		*/
			
		//cv::inRange(hsv, cv::Scalar(minH, minS, minV), cv::Scalar(maxH, maxS, maxV), hsv);
		
		for (int i = 0; i < 3; i++)
		{
			Mat gray, edge, draw;
			//cvtColor(imageRoi, gray, CV_BGR2GRAY);
			Mat sobelX, sobelY;
			//Sobel(cha[i], sobelX, CV_8U, 1, 0, 5, 0.8, 128);
			//Sobel(cha[i], sobelY, CV_8U, 0, 1, 5, 0.8, 128);
			//GaussianBlur(cha[i], cha[i], Size(3, 3), 0, 0, 0);
			Sobel(cha[i], sobelX, CV_16S, 1, 0, 5, 5);
			Sobel(cha[i], sobelY, CV_16S, 0, 1, 5, 5);
			Mat sobel;
			sobel = abs(sobelX) + abs(sobelY);

			double sobmin, sobmax;
			minMaxLoc(sobel, &sobmin, &sobmax);
			
			sobel.convertTo(cha[i], CV_8U, -255. / sobmax, 255);
			imshow("bgr"+i,cha[i]);
		}
		Mat img;
		//cha[2] = 0;
		
		//merge(cha, img);
		img = 0.5*cha[0] + 0.3*cha[1] + 0.4*cha[2];
		erode(img, img, Mat());
		imshow("img", img);
		Mat sobelThresholded;
		/*threshold(sobelImage, sobelThresholded, 200, 255, THRESH_BINARY);
		imshow("Sobel(low threshold)", sobelThresholded);
		adaptiveThreshold(sobelImage, sobelThresholded, 255, ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 5, thresh);
		imshow("adaptive sobel", sobelThresholded);*/
		
		//Mat canny;
		//Canny(gray, canny, 100, 200);
		//Mat contoursInv;
		//threshold(canny, contoursInv, 128, 255, THRESH_BINARY_INV);
		//imshow("Canny", contoursInv);
			
		// median blur
		Mat median;
		//int elementSize = 5;
		cv::medianBlur(nolight, median, 5);
		//cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * elementSize + 1, 2 * elementSize + 1), cv::Point(elementSize, elementSize));
		//cv::dilate(nolight, median, element);
		imshow("median", median);
		
		//gaussian blur
		Mat gaussian;
		GaussianBlur(nolight, gaussian, Size(5, 5), 3, 3);
		imshow("gaussian", gaussian);
		
		//bilateralfilter
		Mat bilateral;
		bilateralFilter(nolight, bilateral, 5, 250, 10);
		imshow("bilateral", bilateral);
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
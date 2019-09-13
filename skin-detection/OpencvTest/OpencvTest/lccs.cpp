#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int minH = 0, maxH = 20, minS = 30, maxS = 150, minV = 60, maxV = 255;
cv::Mat frame;
int count1 = 0;

int xGradient(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x) +
		2 * image.at<uchar>(y, x - 1) +
		image.at<uchar>(y + 1, x - 1) -
		image.at<uchar>(y - 1, x + 1) -
		2 * image.at<uchar>(y, x + 1) -
		image.at<uchar>(y + 1, x + 1);
}
int yGradient(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1) +
		2 * image.at<uchar>(y - 1, x) +
		image.at<uchar>(y - 1, x + 1) -
		image.at<uchar>(y + 1, x - 1) -
		2 * image.at<uchar>(y + 1, x) -
		image.at<uchar>(y + 1, x + 1);
}

//int xGradient(Mat image, int x, int y)
//{
//	return image.at<uchar>(y - 1, x - 1) +
//		2 * image.at<uchar>(y, x - 1) +
//		image.at<uchar>(y + 1, x - 1) -
//		image.at<uchar>(y - 1, x + 1) -
//		2 * image.at<uchar>(y, x + 1) -
//		image.at<uchar>(y + 1, x + 1);
//}
//int yGradient(Mat image, int x, int y)
//{
//	return image.at<uchar>(y - 1, x - 1) +
//		2 * image.at<uchar>(y - 1, x) +
//		image.at<uchar>(y - 1, x + 1) -
//		image.at<uchar>(y + 1, x - 1) -
//		2 * image.at<uchar>(y + 1, x) -
//		image.at<uchar>(y + 1, x + 1);
//}

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
	cv::Vec3b pixel = RGB.at<cv::Vec3b>(0, 0);
	if (event == cv::EVENT_LBUTTONDBLCLK) // on double left clcik
	{
		std::cout << "Click" << std::endl;
		int h = pixel.val[0];
		int s = pixel.val[1];
		int v = pixel.val[2];
		if (count1 == 0)
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
		count1++;
	}
	cout << log(double(pixel[2]) / double(pixel[1])) << endl;
	//std::cout << pixel << std::endl;
}

int main()
{
	//Mat frame;
	VideoCapture cap(0);
	const char* windowName = "Fingertip detection";
	namedWindow(windowName);
	setMouseCallback(windowName, CallbackFunc, NULL);
	
	int minthresh = 132; int maxthresh = 225;
	createTrackbar("minthreshold", windowName, &minthresh, 255);
	createTrackbar("maxthreshold", windowName, &maxthresh, 255);
	while (1)
	{
		cap >> frame;
		Mat imageRoi = frame(Rect(100, 50, 350, 350));
		
		
		Mat hsv;
		Mat ycb;
		Mat img(imageRoi.rows, imageRoi.cols, CV_8UC1);
		Mat img2(imageRoi.rows, imageRoi.cols, CV_8UC1);
		Mat rgb;
		cvtColor(imageRoi, rgb, CV_BGR2RGB);
		cvtColor(imageRoi, hsv, CV_BGR2HSV);
		cvtColor(imageRoi, ycb, CV_BGR2YCrCb);

		/*for (int j = 0; j < imageRoi.rows; j++)
		{
			uchar* data = imageRoi.ptr<uchar>(j);
			uchar* ycbData = ycb.ptr<uchar>(j);
			uchar* hsvData = hsv.ptr<uchar>(j);
			uchar* imgData = img.ptr<uchar>(j);
			for (int i = 0; i < imageRoi.cols*imageRoi.channels(); i += 3)
			{
				if (ycbData[i + 1] > 77 && ycbData[i + 1]<127 && ycbData[i + 2] >133 && ycbData[i+2] < 173)
				{
					imgData[i / 3] = 255;

				}
				else
					imgData[i / 3] = 0;
			}
		}
		imshow("cbcr", img);*/
		Mat lab;
		cvtColor(imageRoi, lab, CV_BGR2Lab);
		vector<Mat> lchannels;
		split(lab, lchannels);
		imshow("y", lchannels[0]);
		imshow("a", lchannels[1]); imshow("b", lchannels[2]);
		Mat nolight = 0.5*lchannels[1] + 0.5*lchannels[2];
		//dilate(nolight, nolight, Mat());
		//erode(nolight, nolight, Mat());
		
		Mat dst; int gx, gy, sum;
		dst = nolight.clone();
		for (int y = 0; y < nolight.rows; y++)
		for (int x = 0; x < nolight.cols; x++)
			dst.at<uchar>(y, x) = 0.0;

		for (int y = 1; y < nolight.rows - 1; y++){
			for (int x = 1; x < nolight.cols - 1; x++){
				gx = xGradient(nolight, x, y);
				gy = yGradient(nolight, x, y);
				sum = abs(gx) + abs(gy);
				if (sum > 10)
					dst.at<uchar>(y, x) = 0;
				else
					dst.at<uchar>(y, x) = 255;
				//sum = sum > 255 ? 255 : sum;
				//sum = sum < 0 ? 0 : sum;
				//dst.at<uchar>(y, x) = sum;
				
			}
		}
		
		
		
		for (int y = 1; y < nolight.rows - 1; y++){
			for (int x = 1; x < nolight.cols - 1; x++){
				if (dst.at<uchar>(y - 1, x - 1) == 255 && dst.at<uchar>(y - 1, x + 1) == 255 && dst.at<uchar>(y + 1, x - 1) == 255 && dst.at<uchar>(y + 1, x + 1) == 255)
				{
					//cout << "erase!" << endl;
					dst.at<uchar>(y, x) = 255;
				}
			}
		}
		
		imshow("test", dst);
		/*for (int i = 0; i < lab.rows; i++){
			for (int j = 0; j < lab.cols; j++)
			{
				if (lab.at<Vec3b>(j, i)[0] >minthresh && lab.at<Vec3b>(j, i)[0] < maxthresh)
					nolight.at<uchar>(j, i) = 255;
			}
		}*/
		imshow("1", nolight);
		
				
		for (int i = 0; i < nolight.rows; i++)
		{
			for (int j = 0; j < nolight.cols; j++)
			{
				if (nolight.at<uchar>(j, i) > 132 && nolight.at<uchar>(j, i) < 225)
					//&& hsv.at<Vec3b>(j, i)[0] > 0 && hsv.at<Vec3b>(j, i)[0] < 50
					//&& hsv.at<Vec3b>(j, i)[1] >= 0.23 * 255 && hsv.at<Vec3b>(j, i)[1] <= 0.68*255)
					nolight.at<uchar>(j, i) = 255;
				else
					nolight.at<uchar>(j, i) = 0;
			}
		}
		imshow("lab", nolight);
		Mat element5(5, 5, CV_8U, cv::Scalar(1));
		//morphologyEx(nolight, nolight, MORPH_OPEN, element5);
		//imshow("umm", nolight);
		medianBlur(nolight, nolight, 5);
		imshow("med0", nolight);
		//////////////hsv, rgb mix....///////
		for (int j = 0; j < imageRoi.rows; j++)
		{
			uchar* data = imageRoi.ptr<uchar>(j);
			uchar* ycbData = ycb.ptr<uchar>(j);
			uchar* hsvData = hsv.ptr<uchar>(j);
			uchar* imgData = img.ptr<uchar>(j);
			for (int i = 0; i < imageRoi.cols*imageRoi.channels(); i += 3)
			{
				if (data[i + 2] > 20 && data[i + 1] > 20 && data[i] > 20 && abs(data[i + 2] - data[i + 1]) > 15 && data[i + 2] > data[i] && data[i + 2] > data[i + 1]
					&& hsvData[i] >= 0.0 && hsvData[i] <= 80.0 && hsvData[i + 1] >= 80)
				{
					imgData[i / 3] = 255;
					
				}
				else
					imgData[i / 3] = 0;
			}
		}
		imshow("img", img);
		//medianBlur(img, img, 5);
		adaptiveBilateralFilter(nolight, img, Size(3, 3), 5);
		//bilateralFilter(nolight, img, 5, 250, 10);
		imshow("med1", img);
		////////////lccs//////////////
		for (int j = 0; j < imageRoi.rows; j++)
		{
			for (int i = 0; i < imageRoi.cols; i++)
			{
				
				//cout << log(int(imageRoi.at<Vec3b>(j, i)[2]) / int(imageRoi.at<Vec3b>(j, i)[1] + 1))<< endl;
				if (log(double(imageRoi.at<Vec3b>(j, i)[2]) / double(imageRoi.at<Vec3b>(j, i)[1])) >= 0.15 &&
					log(double(imageRoi.at<Vec3b>(j, i)[2]) / double(imageRoi.at<Vec3b>(j, i)[1])) <= 1.1 &&
					log(double(imageRoi.at<Vec3b>(j, i)[0]) / double(imageRoi.at<Vec3b>(j, i)[1])) >= -4 &&
					log(double(imageRoi.at<Vec3b>(j, i)[0]) / double(imageRoi.at<Vec3b>(j, i)[1])) <= 0.3 )
					img2.at<uchar>(j, i) = 255;

				else
					img2.at<uchar>(j, i) = 0;
			}

		}
		imshow("lccs", img2);
		//adaptiveBilateralFilter(img2, img2, Size(11, 11), 50);
		//erode(img2, img2, Mat());
		
		medianBlur(img2, img2, 5);
		//bilateralFilter(img2, img2, -1, 15, 15);
		imshow("med", img2);
		

		//vector<Mat> cha;
		//split(imageRoi, cha);
		/*for (int i = 0; i < 3; i++)
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
			//imshow("bgr" + i, cha[i]);
		}
		img = 0.2*cha[0] + 0.2*cha[1] + 0.2*cha[2];
		imshow("sobel", img);
		*/
		
		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(nolight, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));  //contour 찾고( image, contours, hierarchy, 윤곽 검색모드, 윤곽 근사방법, offset)
		size_t largestContour = 0;
		for (size_t i = 1; i < contours.size(); i++)
		{
			if (cv::contourArea(contours[i]) > cv::contourArea(contours[largestContour]))  //contour 면적 = 최대면적 
				largestContour = i;
		}
		cv::drawContours(imageRoi, contours, largestContour, cv::Scalar(0, 0, 255), -1);  //contour 그린다(빨간색으로)(result, contours, 최대contours, 색, 두께)
		
		/*if (!contours.empty())
		{
			std::vector<std::vector<cv::Point> > hull(1);
			cv::convexHull(cv::Mat(contours[largestContour]), hull[0], false);
			cv::drawContours(imageRoi, hull, 0, cv::Scalar(0, 255, 0), 3);
		}*/
		imshow(windowName, imageRoi);
		if (waitKey(30) >= 0) break;
	}
	return 0;

}


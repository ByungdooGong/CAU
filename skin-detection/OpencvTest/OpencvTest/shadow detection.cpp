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
	int thresh = 171; int thresh2 = 136;
	createTrackbar("threshold", windowName, &thresh, 255);
	createTrackbar("thresho2ld", windowName, &thresh2, 255);
	while (1)
	{
		cap >> frame;
		Mat imageRoi = frame(Rect(100, 50, 350, 350));
			
		Mat lab;
		Mat nolight;
		cvtColor(imageRoi, lab, CV_BGR2Lab);
		vector<Mat> lchannels;
		split(lab, lchannels);
		imshow("Y", lchannels[0]);
		imshow("A", lchannels[1]); 
		imshow("B", lchannels[2]);
				
		int sumL = 0  , sumA = 0, sumB = 0;
		int meanL, meanA, meanB;
		int variance = 0;
		int STD;

		for (int i = 0; i < imageRoi.rows; i++)
		{
			for (int j = 0; j < imageRoi.cols; j++)
			{
				sumL = sumL + lchannels[0].at<uchar>(j, i);
				sumA = sumA + lchannels[1].at<uchar>(j, i);
				sumB = sumB + lchannels[2].at<uchar>(j, i);
				
			}
		}
		meanL = sumL / (imageRoi.rows*imageRoi.cols);
		meanA = sumA / (imageRoi.rows*imageRoi.cols);
		meanB = sumB / (imageRoi.rows*imageRoi.cols);

		for (int i = 0; i < imageRoi.rows; i++)
		{
			for (int j = 0; j < imageRoi.cols; j++)
			{
				variance += pow(lchannels[0].at<uchar>(j, i) - meanL, 2);
			}
		}
		variance = variance / ((imageRoi.rows * imageRoi.cols)-1);
		STD = sqrt(variance);
		Mat dst;
		dst = lchannels[0].clone();
		if (meanA + meanB <= 256)
		{
			for (int i = 0; i < imageRoi.rows; i++)
			{
				for (int j = 0; j < imageRoi.cols; j++)
				{
					if (lchannels[0].at<uchar>(j, i) <= meanL - STD / 3)
					{
						dst.at<uchar>(j, i) = 255;
					}
					else
						dst.at<uchar>(j, i) = 0;
				}
			}
		}
		else
		{
			for (int i = 0; i < imageRoi.rows; i++)
			{
				for (int j = 0; j < imageRoi.cols; j++)
				{
					if (lchannels[0].at<uchar>(j, i) <= thresh && lchannels[2].at<uchar>(j,i) <= thresh2)
					{
						dst.at<uchar>(j, i) = 255;
					}
					else
						dst.at<uchar>(j, i) = 0;
				}
			}
		}
		//cout << meanA + meanB << endl;
		imshow("model", dst);
		//erode(dst, dst, Mat());
		dilate(dst, dst, Mat());
		vector<Mat> rgbs;
		split(imageRoi, rgbs);
		//imshow("original red", rgbs[2]);
		int sumRI = 0, sumGI = 0, sumPI = 0;
		int sumRO = 0, sumGO = 0, sumPO = 0;
		int meanRI, meanGI, meanPI, meanRO, meanGO, meanPO;
		int num = 0;
		for (int i = 0; i < dst.rows; i++)
		{
			for (int j = 0; j < dst.cols; j++)
			{
				if (dst.at<uchar>(j, i) == 255)
				{
					sumPI += rgbs[0].at<uchar>(j, i);
					sumGI += rgbs[1].at<uchar>(j, i);
					sumRI += rgbs[2].at<uchar>(j, i);
					num++;
				}
				else
				{
					sumPO += rgbs[0].at<uchar>(j, i);
					sumGO += rgbs[1].at<uchar>(j, i);
					sumRO += rgbs[2].at<uchar>(j, i);
				}
			}
		}
		meanPI = sumPI / num;
		meanGI = sumGI / num;
		meanRI = sumRI / num;
		
		meanPO = sumPO / ((imageRoi.rows * imageRoi.cols) - num);
		meanGO = sumGO / ((imageRoi.rows * imageRoi.cols) - num);
		meanRO = sumRO / ((imageRoi.rows * imageRoi.cols) - num);

		double ratioP, ratioG, ratioR;
		ratioP = double(meanPO) / double(meanPI);
		ratioG = double(meanGO) / double(meanGI);
		ratioR = double(meanRO) / double(meanRI);

		//cout << ratioP << endl;

		for (int i = 0; i < dst.rows; i++)
		{
			for (int j = 0; j < dst.cols; j++)
			{
				if (dst.at<uchar>(j, i) == 255)
				{
					rgbs[2].at<uchar>(j, i) *= ratioP;
					rgbs[0].at<uchar>(j, i) *= ratioG;
					rgbs[1].at<uchar>(j, i) *= ratioR;
					
				}
			}
		}
		
		//cout << ratioP << "" << ratioG << "" << ratioR << endl;
		merge(rgbs, dst);
		//medianBlur(dst, dst, 3);
		imshow("shadow removal", dst);
		//imshow("g", rgbs[1]);
		//imshow("r", rgbs[2]);

		//std::vector<std::vector<cv::Point> > contours;
		//std::vector<cv::Vec4i> hierarchy;
		//cv::findContours(nolight, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));  //contour 찾고( image, contours, hierarchy, 윤곽 검색모드, 윤곽 근사방법, offset)
		//size_t largestContour = 0;
		//for (size_t i = 1; i < contours.size(); i++)
		//{
		//	if (cv::contourArea(contours[i]) > cv::contourArea(contours[largestContour]))  //contour 면적 = 최대면적 
		//		largestContour = i;
		//}
		//cv::drawContours(imageRoi, contours, largestContour, cv::Scalar(0, 0, 255), -1);  //contour 그린다(빨간색으로)(result, contours, 최대contours, 색, 두께)

		//if (!contours.empty())
		//{
		//std::vector<std::vector<cv::Point> > hull(1);
		//cv::convexHull(cv::Mat(contours[largestContour]), hull[0], false);
		//cv::drawContours(imageRoi, hull, 0, cv::Scalar(0, 255, 0), 3);
		//}
		imshow(windowName, imageRoi);
		if (waitKey(30) >= 0) break;
	}
	return 0;

}


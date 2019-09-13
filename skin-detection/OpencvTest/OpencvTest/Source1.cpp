#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int minH = 0, maxH = 12, minS = 76, maxS = 150, minV = 60, maxV = 255;
Mat frame;
int count = 0;

float innerAngle(float px1, float py1, float px2, float py2, float cx1, float cy1)
{

	float dist1 = sqrt((px1 - cx1)*(px1 - cx1) + (py1 - cy1)*(py1 - cy1));
	float dist2 = sqrt((px2 - cx1)*(px2 - cx1) + (py2 - cy1)*(py2 - cy1));

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


	float A = acos((P1*Q1 + P2*Q2) / (sqrt(P1*P1 + P2*P2) * sqrt(Q1*Q1 + Q2*Q2)));

	A = A * 180 /CV_PI;

	return A;
}

/*void CallbackFunc(int event, int x, int y, int flags, void* userdata)
{
	Mat RGB = frame(Rect(x, y, 1, 1));
	Mat HSV;
	tColor(RGB, HSV, _BGR2HSV);
	Vec3b pixel = HSV.at<Vec3b>(0, 0);
	if (event == EVENT_LBUTTONDBLCLK) // on double left clcik
	{
		cout << "Click" << endl;
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
	cout << pixel << endl;
}*/

int main()
{
	VideoCapture cap(0);
	const char* windowName = "Fingertip detection";
	namedWindow(windowName);
	//setMouseCallback(windowName, CallbackFunc, NULL);
	int inAngleMin = 0, inAngleMax = 27, angleMin = 60, angleMax = 153, lengthMin = 120, lengthMax = 223;
	/*createTrackbar("Inner angle min", windowName, &inAngleMin, 360);
	createTrackbar("Inner angle max", windowName, &inAngleMax, 360);
	createTrackbar("Angle min", windowName, &angleMin, 360);
	createTrackbar("Angle max", windowName, &angleMax, 360);
	createTrackbar("Length min", windowName, &lengthMin, 100);
	createTrackbar("Length max", windowName, &lengthMax, 100);*/
	
	while (1)
	{
		
		cap >> frame;
		Mat imageRoi = frame(Rect(100, 50, 350, 350));
		Mat hsv;
		cvtColor(imageRoi, hsv, CV_BGR2HSV);
		inRange(hsv, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), hsv);
		// Pre processing
		int blurSize = 5;
		int elementSize = 5;
		medianBlur(hsv, hsv, blurSize);
		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2 * elementSize + 1, 2 * elementSize + 1), Point(elementSize, elementSize));
		dilate(hsv, hsv, element);
		
		// Contour detection
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		findContours(hsv, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));  //contour 찾고( image, contours, hierarchy, 윤곽 검색모드, 윤곽 근사방법, offset)
		size_t largestContour = 0;
		for (size_t i = 1; i < contours.size(); i++)
		{
			if (contourArea(contours[i]) > contourArea(contours[largestContour]))  //contour 면적 = 최대면적 
				largestContour = i;
		}
		drawContours(imageRoi, contours, largestContour, Scalar(0, 0, 255), 2);  //contour 그린다(빨간색으로)(result, contours, 최대contours, 색, 두께)
				
		// Convex hull : 여러개의 점이 주어 졌을 때, 모든 점들을 포함하는 최소 크기의 볼록 다각형
		if (!contours.empty())
		{
			vector<vector<Point> > hull(1);
			convexHull(Mat(contours[largestContour]), hull[0], false);
			drawContours(imageRoi, hull, 0, Scalar(0, 255, 0), 3);
			if (hull[0].size() > 2)
			{
				vector<int> hullIndexes;
				convexHull(Mat(contours[largestContour]), hullIndexes, true);
				vector<Vec4i> convexityDefects;
				cv::convexityDefects(Mat(contours[largestContour]), hullIndexes, convexityDefects);
				Rect boundingBox = boundingRect(hull[0]);
				rectangle(imageRoi, boundingBox, Scalar(255, 0, 0));
				Point center = Point(boundingBox.x + boundingBox.width/ 2, boundingBox.y + boundingBox.height / 2);
				vector<Point> validPoints; 
				for (size_t i = 0; i < convexityDefects.size(); i++)
				{
					Point p1 = contours[largestContour][convexityDefects[i][0]];
					Point p2 = contours[largestContour][convexityDefects[i][1]];
					Point p3 = contours[largestContour][convexityDefects[i][2]];
					//line(imageRoi, p1, p3, Scalar(255, 0, 0), 2);
					//line(imageRoi, p3, p2, Scalar(255, 0, 0), 2);
					
					double angle = atan2(center.y - p1.y, center.x - p1.x) * 180 / CV_PI;
					double inAngle = innerAngle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
					double length = sqrt(pow(p1.x - p3.x, 2) + pow(p1.y - p3.y, 2));
					if (angle>-30 && angle < 160 && abs(inAngle) > 20 && abs(inAngle) < 120 && length > 0.1 * boundingBox.height)
					{
						validPoints.push_back(p1);
					}
				}

				//printf("validPoints:%d\n", validPoints.size());
				for (size_t i = 0; i < validPoints.size(); i++)
				{
					circle(imageRoi, validPoints[i], 9, Scalar(0, 255, 0), 2);
				}
			}
		}
		frame.copyTo(imageRoi);
		
		rectangle(frame, Point(100, 50), Point(450, 400), Scalar(0, 0, 255));
		imshow(windowName, frame);
		if (waitKey(30) >= 0) break;

		
		hsv.release();
	}
	return 0;  //https://picoledelimao.github.io/blog/2015/11/15/fingertip-detection-on-open/
}
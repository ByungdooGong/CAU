#include <iomanip>
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void Printmat(const char *strName, Mat m);
int main()
{
	Mat src_Image = imread("sample5.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dst_Image;
	threshold(src_Image, dst_Image, 120, 255, CV_8U);
	imshow("threshold Image:", dst_Image);
	Mat srcImage_f32;
	src_Image.convertTo(srcImage_f32, CV_32F);

	Mat A(src_Image.rows*src_Image.cols, 3, CV_32FC1);
	Mat Z(src_Image.rows*src_Image.cols, 1, CV_32FC1);
	imshow("src_Image", src_Image);
	cout << A.rows;
	int count = 0;
	for (unsigned int i = 0; i < src_Image.rows; i++)
	{
		for (unsigned int j = 0; j < src_Image.cols; j++)
		{
			A.at<float>(count, 0) = j;
			A.at<float>(count, 1) = i;
			A.at<float>(count, 2) = 1;
			Z.at<float>(count, 0) = srcImage_f32.at<float>(i, j);
			count++;
		}
	}
	count = 0;
	float mean;
	float sum = 0;
	for (unsigned int i = 0; i < src_Image.rows; i++)
	{
		for (unsigned int j = 0; j < src_Image.cols; j++)
		{
			sum += srcImage_f32.at<float>(i, j);
		}
	}
	mean = sum / (src_Image.rows*src_Image.cols);
	cout << "mean:" << mean << endl;

	Mat X = (A.t()*A).inv()*A.t()*Z;

	float aValue, bValue, cValue;
	aValue = X.at<float>(0);
	bValue = X.at<float>(1);
	cValue = X.at<float>(2);
	for (int y = 0; y < src_Image.rows; ++y)
	{
		uchar* fgPtr = src_Image.ptr(y);
		for (int x = 0; x < src_Image.cols; ++x)
		{
			float Z1 = aValue*x + bValue*y + cValue;
			srcImage_f32.at<float>(y, x) = abs(Z1 - srcImage_f32.at<float>(y, x) + mean)>150 ? 255 : 0;
		}
	}
	Mat gray_img;
	srcImage_f32.convertTo(gray_img, CV_8U);
	imshow("result", gray_img);
	Printmat("inverted=", X);
	waitKey();
	return 0;
}
void Printmat(const char*strName, Mat m)
{
	int x, y;
	float fValue;
	cout << endl << endl << strName << endl;
	cout << setiosflags(ios::fixed);
	for (y = 0; y < m.rows; y++)
	{
		for (x = 0; x < m.cols; x++)
		{
			fValue = m.at<float>(y, x);
			cout << setprecision(2) << setw(8) << fValue;
		}
		cout << endl;
	}
	cout << endl;
}
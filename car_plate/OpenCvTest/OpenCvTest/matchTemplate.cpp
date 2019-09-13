#include <iostream>
#include <opencv2\opencv.hpp>
#include <iomanip>
#include <algorithm>
using namespace std;
using namespace cv;

void Printmat(const char *strName, Mat m);
void matchTemp(Mat inputarr, Mat filter, Mat result, int method);
int main()
{
	float dataA[] = { 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1,
					1, 1, 9, 9, 1, 1,
					1, 1, 9, 9, 1, 1,
					1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1 };
	float dataB[] = { 9, 9, 1,
					9, 9, 1,
					1, 1, 1 };
	Mat A(6, 6, CV_32F, dataA); Printmat("A=", A);
	Mat B(3, 3, CV_32F, dataB); Printmat("B=", B);
	double minVal, maxVal;
	Point minLoc, maxLoc;
	Mat result = Mat::zeros(A.rows - B.rows + 1, A.cols - B.cols + 1, CV_32F);
	
	// TM_SQDIFF
	//matchTemplate(A, B, result, TM_SQDIFF);
	
	matchTemp(A, B, result, 1);
	//Printmat("result:TM_SQDIFF =", result);
	//minMaxLoc(result, &minVal, NULL, &minLoc, NULL);
	//cout << "minVal = " << minVal << endl;
	//cout << "minLoc = " << minLoc << endl;
	// TM_SQDIFF_NORMED
	//matchTemplate(A, B, result, TM_SQDIFF_NORMED);
	Printmat("result:TM_SQDIFF_NORMED =", result);
	matchTemp(A, B, result, 2);
	Printmat("result:TM_CCORR =", result);
	matchTemp(A, B, result, 3);
	Printmat("result:TM_CCOEFF =", result);
	
	/*minMaxLoc(result, &minVal, NULL, &minLoc, NULL);
	cout << "minVal = " << minVal << endl;
	cout << "minLoc = " << minLoc << endl;
	// TM_CCORR
	matchTemplate(A, B, result, TM_CCORR);
	Printmat("result:TM_CCORR =", result);
	minMaxLoc(result, NULL, &maxVal, NULL, &maxLoc);*/
}
void matchTemp(Mat inputarr, Mat filter, Mat result, int method)
{
	for (int p = 0; p < inputarr.rows - filter.rows+1; p++)
		for (int q = 0; q < inputarr.cols - filter.cols+1; q++)
		{
			if (method == 1)
			{
				float temp = 0;
				float temp2 = 0;
				float temp3 = 0;
				for (int i = 0; i < filter.rows; i++)
					for (int j = 0; j < filter.cols; j++)
					{
						temp += pow(filter.at<float>(i, j) - inputarr.at<float>(i + p, j + q), 2);
						temp2 += pow(filter.at<float>(i, j), 2);
						temp3 += pow(inputarr.at<float>(i + p, j + q), 2);
					}
				result.at<float>(p, q) = temp / sqrt(temp2*temp3) > 1.0f ? 1.0f : temp / sqrt(temp2*temp3);
			}
			if (method == 2)
			{
				float temp = 0;
				for (int i = 0; i < filter.rows; i++)
					for (int j = 0; j < filter.cols; j++)
					{
						temp += filter.at<float>(i, j)*inputarr.at<float>(i + p, j + q);
					}
				result.at<float>(p, q) = temp;
			}
			if (method == 3)
			{
				float temp = 0;
				float temp1 = 0;
				float temp2 = 0;
				for (int i = 0; i < filter.rows; i++)
					for (int j = 0; j < filter.cols; j++)
					{
						float sum1 = 0;
						float sum2 = 0;
						for (int a = 0; a < filter.rows; a++)
							for (int b = 0; b < filter.cols; b++)
							{
								sum1 += filter.at<float>(a, b);
								sum2 += inputarr.at<float>(a + p, b + q);
							}
						temp1 = filter.at<float>(i, j) - sum1 / (filter.rows*filter.cols);
						temp2 = inputarr.at<float>(i + p, j + q) - sum2 / (filter.rows*filter.cols);
						temp += temp1*temp2;
					}
				result.at<float>(p, q) = temp;

			}
			
		}
}
void Printmat(const char *strName, Mat m)
{
	int x, y; float fValue;
	cout << endl << strName << endl;
	cout << setiosflags(ios::fixed);
	for (y = 0; y<m.rows; y++) {
		for (x = 0; x<m.cols; x++) {
			fValue = m.at<float>(y, x);
			cout << setprecision(2) << setw(8) << fValue;
		}
		cout << endl;
	}
	cout << endl;
}
/*int main()
{
	Mat srcImage = imread("alphabet.jpg", IMREAD_GRAYSCALE);
	if (srcImage.empty()) return -1;
	Mat tImage1 = imread("A.jpg", IMREAD_GRAYSCALE);
	Mat tImage2 = imread("S.jpg", IMREAD_GRAYSCALE);
	Mat tImage3 = imread("b.jpg", IMREAD_GRAYSCALE);
	Mat tImage4 = imread("m.jpg", IMREAD_GRAYSCALE);
	if (tImage1.empty() || tImage2.empty() || tImage2.empty() || tImage4.empty()) return -1;
	Mat dstImage;
	cvtColor(srcImage, dstImage, COLOR_GRAY2BGR);
	double minVal, maxVal;
	Point minLoc, maxLoc;
	Mat result;
	// TM_SQDIFF
	matchTemplate(srcImage, tImage1, result, TM_SQDIFF);
	minMaxLoc(result, &minVal, NULL, &minLoc, NULL);
	rectangle(dstImage, minLoc, Point(minLoc.x + tImage1.cols, minLoc.y + tImage1.rows), Scalar(255, 0, 0), 2);
	// TM_SQDIFF_NORMED
	matchTemplate(srcImage, tImage2, result, TM_SQDIFF_NORMED);
	minMaxLoc(result, &minVal, NULL, &minLoc, NULL);
	rectangle(dstImage, minLoc, Point(minLoc.x + tImage2.cols, minLoc.y + tImage2.rows), Scalar(0, 255, 0), 2);
	// TM_CCORR_NORMED
	matchTemplate(srcImage, tImage3, result, TM_CCORR_NORMED);
	minMaxLoc(result, NULL, &maxVal, NULL, &maxLoc);
	rectangle(dstImage, maxLoc, Point(maxLoc.x + tImage3.cols, maxLoc.y + tImage3.rows), Scalar(0, 0, 255), 2);
	// TM_CCOEFF_NORMED
	matchTemplate(srcImage, tImage4, result, TM_CCOEFF_NORMED);
	minMaxLoc(result, NULL, &maxVal, NULL, &maxLoc);
	rectangle(dstImage, maxLoc, Point(maxLoc.x + tImage4.cols, maxLoc.y + tImage4.rows), Scalar(255, 0, 255), 2);
	imshow("dstImage", dstImage);
	waitKey();
	return 0;
}*/
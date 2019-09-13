#include <iomanip>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;
void Printmat(const char *strName, Mat A);
void borderTracing(Mat A, Mat B, int n, int i, int j);
int label = 0;
bool finished;
int firsti, firstj;
int main()
{
	uchar dataA[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0 };
	Mat A(7, 9, CV_8U, dataA);
	Printmat("A=", A);
	int distanceType = CV_DIST_L1;
	int maskSize = 5;
	Mat contours = Mat::zeros(A.size(), CV_8U);
	bool flag = false;
	for (int i = 0; i < A.rows; i++)
	{
		for (int j = 0; j < A.cols; j++)
		{
			if (A.at<uchar>(i, j) == 1)
			{
				contours.at<uchar>(i, j) = 1;
				firsti = i;
				firstj = j;
				borderTracing(A, contours, 4, i, j);
				flag = true;
				break;
			}
		}
		if (flag)
			break;
	}
	//distanceTransform(A, distance, distanceType, maskSize);
	//distanceTransform(A, distance, labels, distanceType, maskSize, DIST_LABEL_CCOMP );
	//distanceTransform(A, distance, labels, distanceType, maskSize, DIST_LABEL_PIXEL );
	//Printmat("distance=", distance);
	Printmat("contours=", contours);
	waitKey(); return 0;
}
void borderTracing(Mat A, Mat B, int n, int i, int j){
	vector<int> direction[] = { { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };
	if (finished)
		return;
	
	while (1)
	{
		
		if (i + direction[n][1] >= 0 && j + direction[n][0] >= 0 && i + direction[n][1] < A.rows && j + direction[n][0] < A.cols)
		{
			if (A.at<uchar>(i + direction[n][1], j + direction[n][0]) == 1)
			{
				if (i + direction[n][1] == firsti && j + direction[n][0] == firstj)
				{
					finished = true;
				}
				B.at<uchar>(i + direction[n][1], j + direction[n][0]) = 1;
				borderTracing(A, B, (n + 5) & 7, i + direction[n][1], j + direction[n][0]);
			}
		}
		if (finished)
			return;
		++n;
		if (n > 7)
			n = 0;
	}
}

void Printmat(const char *strName, Mat A)
{
	int x, y;
	uchar uValue;
	float fValue;
	double dValue;
	int iValue;
	cout << endl << endl << strName << endl;
	cout << setiosflags(ios::fixed);
	for (y = 0; y<A.rows; y++) {
		for (x = 0; x<A.cols; x++) {
			switch (A.type()) {
			case CV_8U:
				uValue = A.at<uchar>(y, x);
				cout << setw(4) << (int)uValue;
				break;
			case CV_32S:
				iValue = A.at<int>(y, x);
				cout << setw(4) << iValue;
				break;
			case CV_32F:
				fValue = A.at<float>(y, x);
				cout << setprecision(1) << setw(5) << fValue;
				break;
			case CV_64F:
				dValue = A.at<double>(y, x);
				cout << setprecision(1) << setw(5) << dValue;
				break;
			}
		}
		cout << endl;
	}
	cout << endl;
}
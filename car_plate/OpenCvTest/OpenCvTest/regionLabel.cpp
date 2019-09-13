#include <iomanip>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
void Printmat(const char *strName, Mat A);
void regionLabel(Mat A, Mat B);
void peripherySeek(Mat A, Mat B, int i, int j);
bool visited[7][9];
int label = 0;
int main()
{
	uchar dataA[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 0, 0, 1, 1, 1, 0,
			0, 1, 1, 0, 0, 0, 1, 1, 0,
			0, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 1, 0, 0, 1, 0,
			0, 0, 1, 1, 1, 0, 0, 1, 0,
			0, 0, 1, 1, 1, 0, 1, 1, 1 };
	Mat A(7, 9, CV_8U, dataA);
	Printmat("A=", A);
	int distanceType = CV_DIST_L1;
	int maskSize = 5;
	Mat labels = Mat::zeros(A.size(), CV_8U);
	regionLabel(A, labels);
	//distanceTransform(A, distance, distanceType, maskSize);
	//distanceTransform(A, distance, labels, distanceType, maskSize, DIST_LABEL_CCOMP );
	//distanceTransform(A, distance, labels, distanceType, maskSize, DIST_LABEL_PIXEL );
	//Printmat("distance=", distance);
	Printmat("labels=", labels);
	waitKey(); return 0;
}
void regionLabel(Mat A, Mat B){
	for (int i = 0; i < A.rows; i++)
		for (int j = 0; j < A.cols; j++)
		{
			if (A.at<uchar>(i, j) == 1 && !visited[i][j])
			{
				++label;
				B.at<uchar>(i, j) = label;
				visited[i][j] = true;
				peripherySeek(A, B, i, j);
			}
		}
}
void peripherySeek(Mat A, Mat B, int i, int j){
	for (int p = -1; p <= 1; p++)
		for (int q = -1; q <= 1; q++)
			if (i + p < A.rows && j+q < A.cols && i + p >= 0 && j + q >= 0 && !visited[i + p][j + q] && A.at<uchar>(i + p, j + q) == 1)
			{
				B.at<uchar>(i+p, j+q) = label;
				visited[i+p][j+q] = true;
				peripherySeek(A,B, i+p, j+q);
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
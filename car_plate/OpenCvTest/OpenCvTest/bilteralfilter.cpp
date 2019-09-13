#include <opencv2\opencv.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <iomanip>
using namespace std;
using namespace cv;

bool isInsideBoundaries(int m, int n);
double gauss(double sigma, int x, int y);
double similarity(int p, int s);
void apply(Mat image, int i, int j);
double getSpatialWeight(int m, int n, int i, int j);
void BilateralFilter(Mat image, double simgaD, double sigmaR);
Mat runFilter(Mat image);
int getValue(Mat image, int i, int j);
void Printmat(const char *strName, Mat A);

int kernelRadius;
double**kernelID;
double* gaussSimilarity;
Mat rimage;
int height = 5;
int width = 5;

int main(){
	uchar dataA[] = {
		4, 4, 5, 27, 39,
		4, 5, 3, 0, 42,
		3, 5, 5, 23, 44,
		0, 4, 5, 21, 47,
		6, 7, 5, 26, 46
	};
	//Mat A(5, 5, CV_8U, dataA);
	Mat A = imread("lena.jpg", CV_8U);
	//Printmat("A=", A);
	//rimage = A.clone();
	BilateralFilter(A, 1, 2);
	imshow("Original", A);
	imshow("Bilatera", rimage);
	//Printmat("rimage=", rimage);
	//Printmat("original=", B);
	waitKey(0);
	return 0;
}

int getValue(Mat image, int i, int j)
{
	Scalar pixel = image.at<uchar>(i, j);
	int pixelValue = pixel.val[0];
	return pixelValue;
}
void BilateralFilter(Mat image, double sigmaC, double sigmaS)
{
	height = image.rows;
	width = image.cols;
	int sigmaMax = max(sigmaC, sigmaS);
	kernelRadius = ceil((double)2 * sigmaMax);
	int kernelSize = kernelRadius * 2 + 1;
	//커널생성
	kernelID = new double*[kernelSize];
	for (int i = 0; i < kernelSize; i++)
		kernelID[i] = new double[kernelSize];
	int center = (kernelSize - 1) / 2;
	//커널 값 대입 -> Wc 값
	for (int x = -center; x < kernelSize - center; x++)
		for (int y = -center; y < kernelSize - center; y++)
			kernelID[x + center][y + center] = gauss(sigmaC, x, y);
	
	gaussSimilarity = new double[256];
	double twoSigmaSSquared = 2 * sigmaS*sigmaS;
	for (int i = 0; i < 256; i++)
		gaussSimilarity[i] = exp((double)-((i) / twoSigmaSSquared));
	rimage = image.clone();
	runFilter(image);
	
}
Mat runFilter(Mat image)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			apply(image, i, j);
			//if (i == 0 && j == 4)
				//return rimage;
			//cout << i << "  "<< j << endl;
		}
	return rimage;
}
void apply(Mat image, int i, int j){
	if (i>=0 && j>=0 && i < height && j < width)
	{
		
		double sum = 0;
		double totalWeight = 0;
		int intensityCenter = getValue(image, i, j);  //센터 픽셀 값 받기
		
		double weight;
		for (int m = i - kernelRadius; m < i + kernelRadius; m++)
			for (int n = j - kernelRadius; n < j + kernelRadius; n++)
				if (isInsideBoundaries(m, n))
				{
					int intensityKernelPos = getValue(image, m, n); //픽셀 값 받기
					//similarity = Ws, getSpatialWeight = Wc, weight = W(s,t)
					weight = getSpatialWeight(m, n, i, j)*similarity(intensityKernelPos, intensityCenter);  
					totalWeight += weight;
					sum += (weight*intensityKernelPos);
				}
		
		int newvalue = (int)floor(sum / totalWeight);  // ->dst(s,t)
		rimage.at<uchar>(i, j) = newvalue;
 	}
}
double getSpatialWeight(int m, int n, int i, int j)
{
	return kernelID[(int)(i - m + kernelRadius)][(int)(j - n + kernelRadius)];
}
double similarity(int p, int s){
	return gaussSimilarity[abs(p - s)];
}
double gauss(double sigma, int x, int y){
	return exp(-((x*x + y*y) / (2 * sigma*sigma)));
}
bool isInsideBoundaries(int m, int n){
	if (m>-1 && n>-1 && m < height && n < width)
		return true;
	else
		return false;
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

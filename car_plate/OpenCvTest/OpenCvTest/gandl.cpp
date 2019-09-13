#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;
void ZeroCrossing(Mat &src, Mat &dst, int threshold);
int main()
{
	Mat srcImage = imread("number.jpg", IMREAD_GRAYSCALE);
	if (srcImage.empty())
		return -1;
	/*normalize(srcImage, srcImage, 0, 255, NORM_MINMAX, CV_8UC1);
	imshow("normalize", srcImage);

	Mat sobelX, sobelY;

	Sobel(srcImage, sobelX, CV_16S, 1, 0, 3);
	Sobel(srcImage, sobelY, CV_16S, 0, 1, 3);

	imshow("SobelX", sobelX);
	imshow("SobelY", sobelY);*/
	
	int ksize = 5;
	Mat blurImage;
	GaussianBlur(srcImage, blurImage, Size(ksize, ksize), 0.0);
	
	imshow("gussian", blurImage);
	Mat lapImage;
	Laplacian(blurImage, lapImage, CV_32F, ksize, 0.4, 128);
	Mat dstImage;
	ZeroCrossing(lapImage, dstImage, 10);

	imshow("dstImage", lapImage);
	imshow("zeroCrossing", dstImage);
	
	waitKey();
	return 0;
}
void ZeroCrossing(Mat &src, Mat &dst, int th)
{
	int x, y;
	double a, b;

	Mat zeroCrossH(src.size(), CV_32F, Scalar::all(0));
	Mat_<float> _src(src);
	for (y = 1; y < src.rows - 1; y++)
	{
		for (x = 1; x < src.cols - 1; x++)
		{
			a = _src(y, x);
			b = _src(y, x + 1);
			if (a == 0)
				a = _src(y, x - 1);
			if (a*b < 0)
				zeroCrossH.at<float>(y, x) = fabs(a) + fabs(b);
			else
				zeroCrossH.at<float>(y, x) = 0;

		}
	}
	Mat zeroCrossV(src.size(), CV_32F, Scalar::all(0));
	for (y = 1; y < src.rows - 1; y++)
		for (x = 1; x < src.cols - 1; x++)
		{
			a = _src(y, x);
			b = _src(y + 1, x);
			if (a == 0)
				a = _src(y - 1, x);
			if (a*b < 0)
				zeroCrossV.at<float>(y, x) = fabs(a) + fabs(b);
			else
				zeroCrossV.at<float>(y, x) = 0;

		}
	Mat zeroCross(src.size(), CV_32F, Scalar::all(0));
	add(zeroCrossH, zeroCrossV, zeroCross);
    threshold(zeroCross, dst, th, 255, THRESH_BINARY);

}
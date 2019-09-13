#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
void myThreshold(InputArray _src, OutputArray _dst, uchar thresh);
int main()
{
	Mat srcImage = imread("lena.jpg", IMREAD_GRAYSCALE);
	if (srcImage.empty()) return -1;
	Mat dstImage = srcImage;
	int N = 64; // 8, 32, 64
	int nWidth = srcImage.cols / N;
	int nHeight = srcImage.rows / N;
	int x, y; // left, top
	Rect rtROI;
	Mat roi;
	for (int i = 20; i<N-10; i++)
		for (int j = 20; j<N-10; j++) {
			x = j*nWidth;
			y = i*nHeight;
			rtROI = Rect(x, y, nWidth, nHeight);
			roi = srcImage(rtROI);
			dstImage(rtROI) = mean(roi);
		}
	//cout << roi << endl;
	imshow("dstImage", dstImage);
	waitKey();
	return 0;
}
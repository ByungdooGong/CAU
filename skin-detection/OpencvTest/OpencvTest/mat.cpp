#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

void colorReduce(Mat image, int div = 64)
{
	int nl = image.rows;

	int nc = image.cols*image.channels();

	for (int j = 0; j < nl; j++)
	{
		uchar* data = image.ptr<uchar>(j);

		for (int i = 0; i < nc; i++)
		{
			data[i] = data[i] / div*div + div / 2;
		}
	}
}
void wave(const Mat &image, Mat &result)
{
	Mat srcX(image.rows, image.cols, CV_32F);
	Mat srcY(image.rows, image.cols, CV_32F);
	printf("%d\n", image.rows);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			srcX.at<float>(i, j) = 0;
			srcY.at<float>(i, j) = 0;
		}
	}
	//remap(image, result, srcX, srcY, INTER_LINEAR);
}
int main(){
	
	Mat image = imread("hand.jpg");

	wave(image, image);

	namedWindow("Image");
	imshow("Image", image);
	waitKey(0);
	return 0;
}
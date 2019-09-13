#include <opencv2/opencv.hpp>

using namespace cv;



int main(){

	Rect roi(100, 100, 150, 150); //���ɿ��� 
	Mat original_img = imread("castle.jpg"); //���� ����

	Mat img_roi = original_img(roi); //������ ���ɿ��� 
	Mat img_roi2(original_img, roi);  //������ ���ɿ���

	imshow("original_img", original_img); //���� ���� ���

	Mat copy_roi = img_roi.clone(); //���ɿ��� ����
	Mat copy_roi2;
	img_roi.copyTo(copy_roi2); //���ɿ��� ����

	img_roi = Scalar(0, 0, 255); //���ɿ��� ������ ǥ��
	copy_roi = Scalar(0, 255, 0); //������ ���ɿ��� �ʷϻ� ǥ��

	imshow("original_img + roi", original_img); //������ ���� ���� ���
	imshow("roi", img_roi); //���ɿ��� ���
	imshow("roi2", copy_roi); //���ɿ��� ���
	imshow("roi3", copy_roi2); //���ɿ��� ���
	waitKey();
	return 0;
}



#include <opencv2/opencv.hpp>

using namespace cv;



int main(){

	Rect roi(100, 100, 150, 150); //관심영역 
	Mat original_img = imread("castle.jpg"); //원본 영상

	Mat img_roi = original_img(roi); //영상의 관심영역 
	Mat img_roi2(original_img, roi);  //영상의 관심영역

	imshow("original_img", original_img); //원본 영상 출력

	Mat copy_roi = img_roi.clone(); //관심영역 복사
	Mat copy_roi2;
	img_roi.copyTo(copy_roi2); //관심영역 복사

	img_roi = Scalar(0, 0, 255); //관심영역 빨강색 표시
	copy_roi = Scalar(0, 255, 0); //복사한 관심영역 초록색 표시

	imshow("original_img + roi", original_img); //수정한 원본 영상 출력
	imshow("roi", img_roi); //관심영역 출력
	imshow("roi2", copy_roi); //관심영역 출력
	imshow("roi3", copy_roi2); //관심영역 출력
	waitKey();
	return 0;
}



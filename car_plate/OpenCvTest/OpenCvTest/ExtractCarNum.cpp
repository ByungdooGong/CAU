#include <iostream>
#include <opencv2\opencv.hpp>
#include <iomanip>
#include <algorithm>
using namespace std;
using namespace cv;
#define PI 3.141592

void newGaussian(Mat input, Mat output, int size, double sigmaX, double sigmaY);
void d_O_Gaussian(Mat input, Mat output, int size, double sigmaX, double sigmaY);
void ZeroCrossing(Mat &src, Mat &dst, int th);

int main()
{
	vector<vector<Point> > contours;  //  Vectors for 'findContours' function.
	vector<Vec4i> hierarchy;
	
	Mat input_image = imread("number.jpg");
	Mat input_gray_image;
	cvtColor(input_image, input_gray_image, CV_BGR2GRAY);
	double ratio, delta_x, delta_y, gradient;  //  Variables for 'Snake' algorithm.
	int select, plate_width, count, friend_count = 0, refinery_count = 0;
	
	imshow("입력 이미지", input_gray_image);
	/*
	Mat gaussian;
	//Mat gaussian2 = Mat::zeros(input_gray_image.size(), CV_8U);
	Mat laplace;
	//Mat dog = Mat::zeros(input_gray_image.size(), CV_8U);
	//newGaussian(input_gray_image, gaussian, 3, 1.0, 1.0);
	GaussianBlur(input_gray_image, gaussian, Size(5,5), 0.0);
	imshow("gaussian", gaussian);
	
	Laplacian(gaussian, laplace, CV_8U, 11, 1, 128);
	imshow("gaussian->laplacian", laplace);

	Laplacian(gaussian, laplace, CV_32F, 11);
	imshow("gaussian->laplacian", laplace);*/
	
	const int ksize = 11;
	float logArr[ksize*ksize];
	int s, t, k = 0;
	float g;
	float sigma = 0.3f*(ksize / 2 - 1.0f) + 0.8f;
	for (s = -ksize / 2; s <= ksize / 2; s++)
		for (t = -ksize / 2; t <= ksize / 2; t++)
		{
			g = exp(-((float)s*s + (float)t*t) / (2 * sigma*sigma));
			g *= (1 - ((float)s*s + (float)t*t) / (2 * sigma*sigma));
			g /= (3.141592f*sigma*sigma*sigma*sigma);
			logArr[k++] = -g;
		}
	Mat logKernel(ksize, ksize, CV_32F, logArr);
	//cout << "logKernel=" << logKernel << endl;
	Mat logImage;
	filter2D(input_gray_image, logImage, CV_32F, logKernel);
	imshow("logImage", logImage);
	
	
	Mat zeroCross = Mat::zeros(input_gray_image.size(), CV_8U);
	ZeroCrossing(logImage, zeroCross, 100);
	imshow("log->zeroCrossing", zeroCross);
	
	Mat img_labels, stats, centroids;
	Mat drawing;
	input_gray_image.copyTo(drawing);

	int numOfLabels = connectedComponentsWithStats(zeroCross, img_labels, stats, centroids, 8, CV_32S);
	
	vector<int> boundRect;
	vector<int> index;
	
	int numOfratio = 0;
	for (int j = 0; j < numOfLabels; j++){

		int area = stats.at<int>(j, CC_STAT_AREA);
		int left = stats.at<int>(j, CC_STAT_LEFT);
		int top = stats.at<int>(j, CC_STAT_TOP);
		int width = stats.at<int>(j, CC_STAT_WIDTH);
		int height = stats.at<int>(j, CC_STAT_HEIGHT);

		ratio = width / height;
		if (ratio <= 0.9)
		{
			rectangle(drawing, Point(left, top), Point(left + width, top + height), Scalar(255, 0, 0), 1);
			numOfratio++;
			boundRect.push_back(left);  //left 정보
			index.push_back(j); //label 정보
		}
	}
	imshow("result", drawing);
	vector<int> order(index.size());

	//Sort
	int far_right = -1;
	int k1 = 0;
	int temp;
	for (int i = 0; i < numOfratio; i++)
	{
		for (int j = 0; j < numOfratio; j++)
		{
			if (far_right < boundRect[j])
			{
				far_right = boundRect[j];
				order[k1] = index[j];  //오른쪽에 있는 순서대로 label정보 넣기
				temp = j;
			}

		}
		boundRect[temp] = -1;
		k1++;
		far_right = -1;
	}
	
	Mat sorting = Mat::zeros(drawing.size(), CV_8UC3);
	
	int num_plate = 0;
	for (int j = 0; j < numOfratio; j++)
	{
		int area = stats.at<int>(order[j], CC_STAT_AREA);
		int left = stats.at<int>(order[j], CC_STAT_LEFT);
		int top = stats.at<int>(order[j], CC_STAT_TOP);
		int width = stats.at<int>(order[j], CC_STAT_WIDTH);
		int height = stats.at<int>(order[j], CC_STAT_HEIGHT);

		rectangle(sorting, Point(left, top), Point(left + width, top + height), Scalar(255, 0, 0), 1);
		putText(sorting, to_string(j), Point(left, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2);
		
		
	
	}


	imshow("sorting", sorting);
	int h1 = 0;
	vector<int> cand;
	//candidate[0] = boundRect[index[0]];
	for (int i = 0; i < numOfratio; i++)
	{
		for (int j = i + 1; j < numOfratio; j++)
		{
			//cout << stats.at<int>(order[i], CC_STAT_WIDTH);
			//cout<<endl;
			if (abs(stats.at<int>(order[i],CC_STAT_TOP) - stats.at<int>(order[j], CC_STAT_TOP)) < 0.2 * stats.at<int>(order[i], CC_STAT_WIDTH))
			{
				num_plate++;
				cand.push_back(j);
				//cout << "i=" << i << "j=" << j << abs(stats.at<int>(order[i], CC_STAT_TOP) - stats.at<int>(order[j], CC_STAT_TOP)) << " "<<stats.at<int>(order[i], CC_STAT_WIDTH) << endl;
			}

		}
		if (num_plate == 3)
		{
			h1 = i;
			break;
		}
		//cout << endl;
		cand.clear();
		num_plate = 0;

	}
	//cout << h1;

	Mat temp_image;
	input_image.copyTo(temp_image);
	//Mat result = Mat::zeros(drawing.size(), CV_8UC3);
	rectangle(temp_image, Point(stats.at<int>(order[h1], CC_STAT_LEFT), stats.at<int>(order[h1], CC_STAT_TOP))
		, Point(stats.at<int>(order[h1], CC_STAT_LEFT) + stats.at<int>(order[h1], CC_STAT_WIDTH),
		stats.at<int>(order[h1], CC_STAT_TOP) + stats.at<int>(order[h1], CC_STAT_HEIGHT)), Scalar(0, 0, 255), 2);
	imshow("first_number", temp_image);
	
	
	int far_left = cand[cand.size() - 1];

	int width = stats.at<int>(order[h1], CC_STAT_LEFT) + stats.at<int>(order[h1], CC_STAT_WIDTH) - stats.at<int>(order[far_left], CC_STAT_LEFT);
	int end_y= stats.at<int>(order[h1], CC_STAT_TOP) + stats.at<int>(order[h1], CC_STAT_HEIGHT);
	int end_x = stats.at<int>(order[h1], CC_STAT_LEFT) + stats.at<int>(order[h1], CC_STAT_WIDTH);
	cout << "width" << width << endl;
	cout << "end_x" << end_x - 1.5*width << "end_y" << end_y - stats.at<int>(order[h1], CC_STAT_HEIGHT)*1.8 << endl;
	Mat roi_image = input_image(Rect(end_x - 1.5*width, end_y - stats.at<int>(order[h1], CC_STAT_HEIGHT)*1.8, 1.5*width, 1.8*stats.at<int>(order[h1], CC_STAT_HEIGHT)));
	
	imshow("plate", roi_image);
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
			if (a < 0 && b > 0)
				zeroCrossH.at<float>(y, x) = -1;
			else if (a > 0 && b < 0)
				zeroCrossH.at<float>(y, x) = 1;
			else
				zeroCrossH.at<float>(y, x) = 0;
		}
	}
	int firstpoint = 10000;
	int endpoint = 10000;
	for (y = 1; y < zeroCrossH.rows - 1; y++)
	{
		for (x = 1; x < zeroCrossH.cols - 1; x++)
		{
			if (zeroCrossH.at<float>(y, x) == -1)
				firstpoint = x;
			else if (zeroCrossH.at<float>(y, x) == 1)
				endpoint = x;
			if (firstpoint != 10000 && endpoint != 10000)
			{
				for (int i = firstpoint; i <= endpoint; i++)
				{
					dst.at<uchar>(y, x) = 255;
				}
			}
		}
		firstpoint = 10000;
		endpoint = 10000;
	}
	//Mat zeroCross(src.size(), CV_32F, Scalar::all(0));
	
	//add(zeroCrossH, zeroCrossV, zeroCross);
	//threshold(dst, dst, th, 255, THRESH_BINARY);

}
void newGaussian(Mat input, Mat output, int size, double sigmaX, double sigmaY)
{
	double Gx[11], Gy[11];
	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
		{
			Gx[x] = exp(-pow(x-size/2, 2) / (2 * sigmaX)) / (sqrt(2 * PI)*sigmaX);
			Gy[y] = exp(-pow(y-size/2, 2) / (2 * sigmaY)) / (sqrt(2 * PI)*sigmaY);
			
		} //kernel
	double temp = 0;
	for (int i = size/2; i < input.rows - size/2; i++)
		for (int j = size / 2; j < input.cols - size / 2; j++)
		{
			for (int p = 0; p < size; p++)
				for (int q = 0; q < size; q++)
				{
					temp += (double)input.at<uchar>(i - size / 2 + p, j - size / 2 + q) * Gx[p] * Gy[q];
				}
			output.at<uchar>(i, j) = temp;
			temp = 0;
			
		}
	
}
void d_O_Gaussian(Mat input, Mat output, int size, double sigmaX, double sigmaY)
{
	double Gx[11], Gy[11];
	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
		{
			Gx[x] = exp(-pow(x - size / 2, 2) / (2 * sigmaX)) / (sqrt(2 * PI)*sigmaX);
			Gy[y] = exp(-pow(y - size / 2, 2) / (2 * sigmaY)) / (sqrt(2 * PI)*sigmaY);

		} //kernel
	double temp = 0;
	for (int i = size / 2; i < input.rows - size / 2; i++)
		for (int j = size / 2; j < input.cols - size / 2; j++)
		{
			for (int p = 0; p < size; p++)
				for (int q = 0; q < size; q++)
				{
					temp += (double)input.at<uchar>(i - size / 2 + p, j - size / 2 + q) * (pow(Gx[p],2) - pow(Gy[q],2));
				}
			output.at<uchar>(i, j) = temp;
			temp = 0;

		}

}
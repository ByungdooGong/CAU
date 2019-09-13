//up to shadow correction
//https://www.youtube.com/watch?v=DU9sRN1ZsUM
//gaurding  using a fixed cam:select the background
//hue covering maximum pixels is shown in detected movement


#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<stdlib.h>
#include<opencv2/core/core.hpp>
#include<windows.h>
//#include"stdafx.h"
#define CRAB

using namespace cv;
using namespace std;

bool go = false;							//confirm background
bool detect = false;						//detected object
bool track = false;						//start tracking


//////serial port
HANDLE SerialPort;
int Error;
unsigned long dwNumberOfBytesWritten;


void OpenSerial(void) { // Setup a serial COM port connection

	//	SerialPort = CreateFile(TEXT("COM16"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, 0);
	SerialPort = CreateFile(TEXT("COM7"), GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);

	// Validate the COM port connection
	if (SerialPort == INVALID_HANDLE_VALUE)
	{
		Error = GetLastError();
		printf("\nError opening serial port!\nError code: %i\n\n", Error);
		system("PAUSE");
	}
	else
		printf("\nSerial port was successfully established!\n\n");

	// Verify port settings
	DCB dcbConfig;

	if (GetCommState(SerialPort, &dcbConfig)) {
		dcbConfig.BaudRate = 9600;
		dcbConfig.ByteSize = 8;
		dcbConfig.Parity = NOPARITY;
		dcbConfig.StopBits = ONESTOPBIT;
		dcbConfig.fBinary = TRUE;
		dcbConfig.fParity = TRUE;

		SetCommState(SerialPort, &dcbConfig);
	}
}

void Send(unsigned char *data) {

	// Send data out the COM port
	if (WriteFile(SerialPort, (LPVOID)data, 1, &dwNumberOfBytesWritten, NULL) == 0) {
		Error = GetLastError();
		printf("\nWrite error!\nError code: %i\n\n", Error);
	}
}

void Send(char *data)
{

	// Send data out the COM port
	if (WriteFile(SerialPort, (LPVOID)data, 1, &dwNumberOfBytesWritten, NULL) == 0) {
		Error = GetLastError();
		printf("\nWrite error!\nError code: %i\n\n", Error);
	}
}

void CloseSerial(void) {

	if (SerialPort != INVALID_HANDLE_VALUE)
		CloseHandle(SerialPort);
}



void movement(Mat *param2, Point *param4)			//track the movement using rectangle centre
{
	Mat img2 = *param2;

	int x = 0, y = 0;

	Point rec_centre = *param4;
	x = rec_centre.x;
	y = rec_centre.y;


	char l = 0, u = 0, r = 0, d = 0, c = 0;
	int il = 100, iu = 300, ir = 500, id = 700, il2 = 200, iu2 = 400, ir2 = 600, id2 = 800, ic = 900;
	Rect check(8 * img2.cols / 20, 8 * img2.rows / 20, img2.cols / 5, img2.rows / 5);
	rectangle(img2, check, Scalar(200, 125, 200), 2, 8, 0);
	line(img2, Point((img2.cols / 2) - 100, 0), Point((img2.cols / 2) - 100, img2.rows), Scalar(255, 255, 255), 2, 8, 0);
	line(img2, Point((img2.cols / 2) + 100, 0), Point((img2.cols / 2) + 100, img2.rows), Scalar(255, 255, 255), 2, 8, 0);
	line(img2, Point(0, (img2.rows / 2) - 100), Point(img2.cols, (img2.rows / 2) - 100), Scalar(255, 255, 255), 2, 8, 0);
	line(img2, Point(0, (img2.rows / 2) + 100), Point(img2.cols, (img2.rows / 2) + 100), Scalar(255, 255, 255), 2, 8, 0);

	int dx = 0, dy = 0;
	if (x<8 * img2.cols / 20)
	{
		cout << "turn left" << endl;
		dx = (img2.rows / 2) - x;
		if (dx<100)
			l = (int)(il2 / 10);
		else
			l = (int)(il / 10);
		Send(&l);								//send 10 or 20 to turn left
		if (y<8 * img2.rows / 20)
		{
			cout << "turn up" << endl;				//send 30 or 40 to turn up
			dy = (img2.cols / 2) - y;
			if (dy<100)
				u = (int)(iu2 / 10);
			else
				u = (int)(iu / 10);
			Send(&u);
		}
		else if (y>12 * img2.rows / 20)
		{
			cout << "turn down" << endl;
			dy = y - (img2.cols / 2);
			if (dy<100)
				d = (int)(id2 / 10);
			else
				d = (int)(id / 10);
			Send(&d);
		}

	}
	else if (x>12 * img2.cols / 20)
	{
		cout << "turn right" << endl;					//send 50 to turn right
		dx = x - (img2.rows / 2);
		if (dx<100)
			r = (int)(ir2 / 10);
		else
			r = (int)(ir / 10);
		Send(&r);
		if (y<8 * img2.rows / 20)
		{
			cout << "turn up" << endl;
			dy = (img2.cols / 2) - y;
			if (dy<100)
				u = (int)(iu2 / 10);
			else
				u = (int)(iu / 10);
			Send(&u);
		}
		else if (y>12 * img2.rows / 20)
		{
			cout << "turn down" << endl;				//send 70 or 80 to turn down
			dy = y - (img2.cols / 2);
			if (dy<100)
				d = (int)(id2 / 10);
			else
				d = (int)(id / 10);
			Send(&d);
		}
	}
	else if (x<12 * img2.cols / 20 && x>8 * img2.cols / 20)
	{
		if (y<8 * img2.rows / 20)
		{
			cout << "turn up" << endl;
			dy = (img2.cols / 2) - y;
			if (dy<100)
				u = (int)(iu2 / 10);
			else
				u = (int)(iu / 10);
			Send(&u);
		}
		else if (y>11 * img2.rows / 20)
		{
			cout << "turn down" << endl;
			dy = y - (img2.cols / 2);
			if (dy<100)
				d = (int)(id2 / 10);
			else
				d = (int)(id / 10);
			Send(&d);
		}
		else
		{
			cout << "stay on course" << endl;				//send 90 to stay on course
			c = (int)(ic / 10);
			Send(&c);
		}
	}
}

int extract(Mat *param, int *psame)
{
	Mat diff2 = *param;
	int same = *psame;
	int array[180] = {};
	Mat img[3];
	int temp = 0, big = 0, k = 0;

	split(diff2, img);
	//	imshow("extracted hue",img[0]);
	for (int i = 0; i<img[0].rows; i++)								//intensities of different hue values
	{
		uchar *data = img[0].ptr<uchar>(i);
		for (int j = 0; j<img[0].cols; j++)
		{
			temp = data[j];
			array[temp]++;
		}
	}

	for (int a = 180; a>0; a--)									//sorting the intensties and getting maximum hue
	{
		if (array[a]>big)
		{
			big = array[a];
			k = a;
		}
	}
	cout << "max hue is " << k << " and no. of points are " << big << endl;
	if (big>700){
		for (int i = 0; i<img[0].rows; i++)
		{
			uchar *data = img[0].ptr<uchar>(i);
			uchar *datas = img[1].ptr<uchar>(i);
			for (int j = 0; j<img[0].cols; j++)
			{
				if (data[j]<(k + 5) && data[j]>(k - 5)){
					data[j] = 255;
					datas[j] = 255;
				}
				else{
					data[j] = 0;
					datas[j] = 0;
				}
			}
		}
		imshow("only max hue", img[0]);

		int from_to[] = { -1, 2, 1, 1, 0, 0 };
		mixChannels(img, 3, &diff2, 1, from_to, 3);
		//	imshow("mixed max hue",diff2);

		//	cout<<"previous same= "<<same<<endl;
		same = k;						//storing hue of current frame
		//	cout<<"current same= "<<same<<endl;

		return(k);
	}
	else
		return 0;
}

double con(Mat *param1, Mat *param2)
{
	Mat cframe = *param1;
	Mat out = *param2;
	Mat store;
	int count = 0;							//count second before strarting tracking
	store = Mat::zeros(out.size(), CV_8UC3);
	vector<vector<Point>> contour, contour_rec;
	vector<Vec4i> hier, hier_rec;
	//find contpour of moving objects
	findContours(out, contour, hier, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	int index = 0;
	double area = 0, big = 0;
	for (int i = 0; i<contour.size(); i++)
	{
		//		drawContours(store,contour,i,Scalar(255,125,255),2,8,hier);
		Rect rec = boundingRect(contour[i]);
		rectangle(store, rec, Scalar(0, 0, 255), 2, 8, 0);
	}
	//	imshow("result",cframe);
	//	imshow("rec",store);	
	cvtColor(store, store, CV_BGR2GRAY);
	findContours(store, contour_rec, hier_rec, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (int a = 0; a<contour_rec.size(); a++)
	{
		//		drawContours(cframe,contour_rec,a,Scalar(255,125,255),2,8,hier);
		Rect bound = boundingRect(contour_rec[a]);
		//		rectangle(cframe,bound,Scalar(0,0,255),2,8,0);
		area = contourArea(contour_rec[a]);
		if (area>big){
			big = area;
			index = a;
		}
	}

	//	drawContours(cframe,contour,index,Scalar(255,125,255),2,8,hier);

	if (contour_rec.size()>0 && big>800){
		Rect big_rec = boundingRect(contour_rec[index]);
		rectangle(cframe, big_rec, Scalar(0, 255, 0), 2, 8, 0);
		if (detect){
			Point rec_centre = Point((big_rec.x + big_rec.width / 2), (big_rec.y + big_rec.height / 2));
			circle(cframe, rec_centre, 5, Scalar(0, 0, 255), -1, 8, 0);
			movement(&cframe, &rec_centre);
		}
	}
	if (detect)
		return(area);
	else
		return 0;
}

void hsv_channel(void *param, Mat *param2, Mat *param3, Mat* param4, Mat* param5)
{
	if (detect){
		int *p = (int*)param;
		int k = *p;
		Mat hsv_d = *param2;
		Mat img[3];
		split(hsv_d, img);
		for (int i = 0; i<img[0].rows; i++)
		{
			uchar *data = img[0].ptr<uchar>(i);
			uchar *data2 = img[1].ptr<uchar>(i);
			for (int j = 0; j<img[0].cols; j++)
			{
				if (data[j]<(k + 10) && data[j]>(k - 10) && data2[j]>12){
					data[j] = 255;
					data2[j] = 255;
				}
				else{
					data[j] = 0;
					data2[j] = 0;
				}
			}
		}

		int from_to[] = { -1, 2, 1, 1, 0, 0 };
		mixChannels(img, 3, &hsv_d, 1, from_to, 3);
	}

	else{
		Mat *pdiff = (Mat*)param;
		Mat diff1 = *pdiff;						//gray channel
		Mat diff2 = *param3;						//hsv channel
		Mat gray_c = *param2;
		Mat hsv_c = *param4;
		Mat hsv_p = *param5;
		Mat img[3], h[3], p[3];
		split(diff2, img);
		split(hsv_c, h);
		split(hsv_p, p);
		//		imshow("diff hue",img[0]);
		//		imshow("current value",h[2]);
		//		imshow("set value",p[2]);
		//		imshow("diff sat",img[1]);
		//		imshow("diff value",img[2]);
		Mat cmp;
		compare(h[2], p[2], cmp, CMP_GE); imshow("h[2] > p[2]", cmp);

		for (int i = 0; i<img[0].rows; i++)
		{
			uchar *data = diff1.ptr<uchar>(i);
			uchar *data2 = diff2.ptr<uchar>(i);
			uchar *data3 = gray_c.ptr<uchar>(i);

			uchar *data4 = img[0].ptr<uchar>(i);
			uchar *data5 = img[1].ptr<uchar>(i);
			uchar *data6 = img[2].ptr<uchar>(i);

			uchar *data7 = h[0].ptr<uchar>(i);
			uchar *data8 = h[1].ptr<uchar>(i);
			uchar *data9 = h[2].ptr<uchar>(i);

			for (int j = 0; j<img[0].cols; j++)
			{
				if (data[j]>15)	//check if diff in hue is greater than 15
				{
					data[j] = data3[j];		//replace diff data with original hue data if in range
					data4[j] = data7[j];
					data5[j] = data8[j];
					data6[j] = data9[j];

					if (data6[j]>60)// && data4[j]>50 )
					{
						//						data[j]=0;
						data4[j] = 0;
						data5[j] = 0;
						data6[j] = 0;
					}
				}
				else
				{
					data[j] = 0;
					data4[j] = 0;
					data5[j] = 0;
					data6[j] = 0;
				}
			}
		}

		//		imshow("thresholded diff hue",img[0]);
		//		imshow("thresholded diff sat",img[1]);
		//		imshow("thresholded diff value",img[2]);


		dilate(img[2], img[2], Mat(), Point(-1, -1), 6, 0, morphologyDefaultBorderValue());
		//		imshow("dilated thresholded diff value",img[2]);
		dilate(cmp, cmp, Mat(), Point(-1, -1), 6, 0, morphologyDefaultBorderValue());
		imshow("dilated compare", cmp);
		add(img[2], cmp, cmp);
		//		imshow("added compare",cmp);
		threshold(cmp, cmp, 5, 255, CV_THRESH_BINARY);
		dilate(cmp, cmp, Mat(), Point(-1, -1), 20, 0, morphologyDefaultBorderValue());
		erode(cmp, cmp, Mat(), Point(-1, -1), 22, 0, morphologyDefaultBorderValue());
		imshow("shadow", cmp);
		int from_to[] = { 2, 2, 1, 1, 0, 0 };
		mixChannels(img, 3, &diff2, 1, from_to, 3);

		//		imshow("movement hue",diff2);
		imshow("movement gray", diff1);
		Mat out;
		bitwise_and(diff1, cmp, out); imshow("final", out);

	}

}

int main()
{
	//OpenSerial();
	Mat cframe, pframe, dframe, out;
	Mat gray_c, gray_p;
	Mat hsv_c, hsv_p, hsv_d;
	Mat blur_c1, blur_c2, blur_p1, blur_p2, blur_d, blur;
	Mat diff1, diff2;
	int frame = 0, count = 0, m = 0, same = 0, no_track = 0;
	double area = 0;

	VideoCapture cp;
	VideoWriter writer;

	cout << "left click on input feed to set the back ground" << endl;
	while (1)
	{
		cp.open(0);
		if (!cp.isOpened())
		{
			cout << "ERROR camera initialization failed" << endl;
			return -1;
		}

		string file_name = "E:\\new_proto.avi";
		int codec = CV_FOURCC('D', 'I', 'V', '3');
		int rate = 25;
		/*Size frame_size((cp.get(CV_CAP_PROP_FRAME_WIDTH)) / 2, (cp.get(CV_CAP_PROP_FRAME_HEIGHT)) / 2);
		writer = VideoWriter(file_name, codec, rate, frame_size, true);
		if (!writer.isOpened())
		{
			cout << "ERROR writer initialization failed" << endl;
			return -1;
		}*/

		//		while(cp.get(cp.get(CV_CAP_PROP_FRAME_COUNT)>CV_CAP_PROP_POS_FRAMES)+1)	//for a video file
		while (1)																//for web cam
		{
			if (!go && !detect)
			{
				//				cvDestroyWindow("result");
				cvDestroyWindow("Tracking window");

				cout << "frame count for background is" << frame << endl;
				if (frame == 90)			//waits 5 seconds before automatically setting background
				{
					go = true;
				}
				//				setMouseCallback("input feed",set_background,&cframe);
				cp >> pframe;
				imshow("input feed", pframe);
				cvtColor(pframe, gray_p, CV_BGR2GRAY);
				cvtColor(pframe, hsv_p, CV_BGR2HSV);
				GaussianBlur(gray_p, blur_p1, Size(5, 5), 0, 0, 4);
				GaussianBlur(hsv_p, blur_p2, Size(5, 5), 0, 0, 4);
				frame++;

			}

			if (go && !detect)
			{
				int reset = 0, stay = 0;
				char r = 0, s = 0;
				cvDestroyWindow("input feed");
				cvDestroyWindow("Tracking window");
				frame = 0;					//reset timer for background selection
				no_track = 0;					//reset timeout for object tracking
				//				setMouseCallback("result",set_background,&cframe);
				cp >> cframe;
				if (!cp.grab())
				{
					cout << "ERROR acquiring video file" << endl;
					return -1;
				}

				cvtColor(cframe, gray_c, CV_BGR2GRAY);
				cvtColor(cframe, hsv_c, CV_BGR2HSV);
				imshow("input gray", gray_c);
				imshow("input hsv", hsv_c);
				//				imshow("background",hsv_p);
				GaussianBlur(gray_c, blur_c1, Size(5, 5), 0, 0, 4);
				GaussianBlur(hsv_c, blur_c2, Size(5, 5), 0, 0, 4);
				//				imshow("blur",blur_c);				
				absdiff(blur_c1, blur_p1, diff1);
				absdiff(blur_c2, blur_p2, diff2);
				hsv_channel(&diff1, &gray_c, &diff2, &hsv_c, &blur_p2);			//replace gray image of diff with gray image of current frame
				//				imshow("no value",diff);

				//				cout<<"sending same= "<<same<<endl;
				//				cout<<"previous m= "<<m<<endl;
				m = extract(&diff2, &same);						//extract hue that max pixels have
				if (m<(same + 15) && m>(same - 15) && same != 0){
					count = count + 1;
					cout << "same hue count=" << count << endl;
				}
				else
					count = 0;
				/*				if(count==40)								//8 seconds
				detect=true;
				same=m;
				//				cout<<"got same= "<<same<<endl;
				/*
				Mat d_kernel=getStructuringElement(CV_SHAPE_CROSS,Size(7,7),Point(-1,-1));
				Mat e_kernel=getStructuringElement(CV_SHAPE_RECT,Size(5,5),Point(-1,-1));
				erode(diff,out,e_kernel,Point(-1,-1),2,0,morphologyDefaultBorderValue());
				//				imshow("eroded ",out);
				GaussianBlur(out,out,Size(7,7),0,0,4);
				//				imshow("gaussian eroded",out);
				dilate(out,out,d_kernel,Point(-1,-1),3,0,morphologyDefaultBorderValue());
				//				imshow("dilated",out);
				morphologyEx(out,out,MORPH_CLOSE,e_kernel,Point(-1,-1),1,0,morphologyDefaultBorderValue());
				//				imshow("closed",out);
				erode(out,out,e_kernel,Point(-1,-1),1,0,morphologyDefaultBorderValue());
				//				GaussianBlur(out,out,Size(3,3),0,0,4);
				//				imshow("erode 2 ",out);

				Canny(out,out,20,60,3);
				//				imshow("canny",out);
				dilate(out,out,d_kernel,Point(-1,-1),1,0,morphologyDefaultBorderValue());
				//				imshow("dilated canny",out);
				morphologyEx(out,out,MORPH_CLOSE,d_kernel,Point(-1,-1),1,0,morphologyDefaultBorderValue());
				//				imshow("close canny",out);

				area=con(&cframe,&out);
				imshow("result",cframe);

				}

				if(detect)
				{
				int reset=0,stay=0;
				char r=0,s=0;
				count=0;
				cvDestroyWindow("background");
				cvDestroyWindow("result");

				cp>>dframe;
				if(!cp.grab())
				{
				cout<<"ERROR acquiring video file"<<endl;
				return -1;
				}

				cvtColor(dframe,hsv_d,CV_BGR2HSV);
				//				imshow("input feed",dframe);

				GaussianBlur(hsv_d,blur_d,Size(7,7),0,0,4);

				hsv_channel(&m,&hsv_d);
				//				imshow("input feed",hsv_d);

				Mat d_kernel=getStructuringElement(CV_SHAPE_RECT,Size(5,5),Point(-1,-1));
				Mat e_kernel=getStructuringElement(CV_SHAPE_ELLIPSE,Size(4,4),Point(-1,-1));
				erode(hsv_d,out,e_kernel,Point(-1,-1),1,0,morphologyDefaultBorderValue());
				//				imshow("detect eroded ",out);
				//				GaussianBlur(out,out,Size(7,7),0,0,4);
				//				imshow("detect gaussian eroded",out);
				dilate(out,out,d_kernel,Point(-1,-1),2,0,morphologyDefaultBorderValue());
				//				imshow("detect dilated",out);
				erode(out,out,e_kernel,Point(-1,-1),3,0,morphologyDefaultBorderValue());
				//				imshow("detect eroded 2 ",out);
				dilate(out,out,d_kernel,Point(-1,-1),1,0,morphologyDefaultBorderValue());
				//				imshow("detect dilated 2",out);

				Canny(out,out,20,60,3);
				//				imshow("detect canny",out);
				dilate(out,out,d_kernel,Point(-1,-1),1,0,morphologyDefaultBorderValue());
				//				imshow("detect dilated canny",out);
				morphologyEx(out,out,MORPH_CLOSE,d_kernel,Point(-1,-1),2,0,morphologyDefaultBorderValue());
				//				imshow("detect close canny",out);

				area=con(&dframe,&out);
				imshow("Tracking window",dframe);

				if(area<800){
				no_track=no_track+1;
				int stay=1100;
				char s=(int)(stay/10);				//nothing is detected
				Send(&s);							//stay right there
				}
				else
				no_track=0;

				cout<<"track count="<<no_track<<endl;

				if(no_track==40){						//if nothing is tracked for 8 secondsreset the camera and background
				detect=false;
				//					go=false;							//set go,if camera cant work on previously set background
				cout<<"reset the camera position"<<endl;
				reset=1000;
				r=(int)(reset/10);
				Send(&r);
				//					waitKey(3000);
				}

				*/
			}

			//				writer.write(cframe);
			if (waitKey(30) == 27) goto end;
		}

	}


end:	cvDestroyAllWindows();
	waitKey();
	return 0;
}


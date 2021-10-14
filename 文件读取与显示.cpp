#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


void test01()//读取并显示图片
{
	string path = "Resources/shapes.png";
	//imshow  
	//cv::imshow
	Mat img = imread(path);//Mat 数据类型 是一个class
	imshow("imageName",img);
	waitKey(0);//zero for infinity
}

void test02()//读取并显示video
{
	string path = "Resources/test_video.mp4";
	VideoCapture cap(path);
	Mat img;

	while (true)
	{
		cap.read(img);//restore in img
		imshow("VideoName", img);
		waitKey(50);//等待50 mini second 读取下一帧
	}
}

void test03()//读取并显示摄像头数据
{
	VideoCapture cap(0);//0 camera id；
	Mat img;

	while (true)
	{
		cap.read(img);//restore in img
		imshow("VideoName", img);
		waitKey(50);//等待50 mini second 读取下一帧
	}
}

void main()
{
	//test01();
	//test02();
	test03();
	cout << "hello" << endl;
}

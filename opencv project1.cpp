//https://www.youtube.com/watch?v=2FYm3GOonhk
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//读取并显示图片
void test01()
{
	string path = "Resources/shapes.png";
	//imshow  
	//cv::imshow
	Mat img = imread(path);//Mat 数据类型 是一个class
	imshow("imageName", img);
	waitKey(0);//zero for infinity
}

//读取并显示video
void test02()
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

//读取并显示摄像头数据
void test03()
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

//Basic Function
void test04()
{
	string path = "Resources/shapes.png";
	Mat img = imread(path);//Mat 数据类型 是一个class
	Mat imgGray;
	Mat imgBlur;
	Mat imgCanny;
	Mat imgDil;
	Mat imgErode;



	cvtColor(img, imgGray, COLOR_BGR2GRAY);//cvtColor ::convert image
	GaussianBlur(imgGray, imgBlur, Size(7, 7), 5, 0);
	Canny(imgBlur, imgCanny, 50, 150);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	dilate(imgCanny, imgDil, kernel);
	erode(imgDil, imgErode, kernel);

	imshow("Original Image", img);//imshow 是一个函数
	imshow("Gray Image", imgGray);
	imshow("Blur Image", imgBlur);
	imshow("Canny Image", imgCanny);
	imshow("Dilation Image", imgDil);
	imshow("Dilation Errode Image", imgErode);



	waitKey(0);//zero for infinity
}

//resize and crop
void test05()
{
	string path = "Resources/shapes.png";
	Mat img = imread(path);
	Mat imgResize;
	Mat imgResize2;
	Mat imgCrop;

	cout << img.size() << endl;

	resize(img, imgResize, Size(320, 320));
	resize(img, imgResize2, Size(), 0.25, 0.25);

	Rect roi(100, 100, 300, 250);
	imgCrop = img(roi);

	imshow("Original Image", img);
	imshow("Resized Image", imgResize);
	imshow("Resized Image 2", imgResize2);
	imshow("Croped Image", imgCrop);

	waitKey(0);

}

//Drawing Shapes and Text
void test06()
{
	//Blank Image
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 0));//BGR Image

	circle(img, Point(256, 256), 155, Scalar(255, 0, 0), 10);
	rectangle(img, Point(130, 226), Point(382, 286), Scalar(255, 255, 255), FILLED);
	line(img, Point(130, 296), Point(382, 296), Scalar(255, 255, 255), 2);
	putText(img, "HiQiang", Point(165, 268), FONT_HERSHEY_DUPLEX, 1.5, Scalar(255, 0, 255), 2);

	imshow("Image", img);

	waitKey(0);
}

//Warp Perspective
void test07()
{
	float w = 250;
	float h = 350;
	Mat matrix, imgWrap;

	string path = "Resources/cards.jpg";
	Mat img = imread(path);

	Point2f src[4] = { {529,142},{771,190},{405,395},{674,457} };
	Point2f dst[4] = { {0.f,0.f},{w,0.f},{0.f,h},{w,h} };

	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWrap, matrix, Point(w, h));

	for (int i = 0; i < 4; i++)
	{
		circle(img, src[i], 10, Scalar(0, 0, 255), FILLED);
	}

	imshow("Image", img);
	imshow("Warped Image", imgWrap);
	waitKey(0);
}

//Color Detection
void test08()
{
	//string path = "Resources/lambo.png";
	string path = "Resources/shapes.png";
	Mat img = imread(path);
	Mat imgHSV;
	Mat mask;

	int hmin = 0, smin = 110, vmin = 153;
	int hmax = 19, smax = 240, vmax = 255;

	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	namedWindow("TrackbarsWindow", (200, 200));
	createTrackbar("Hue Min", "TrackbarsWindow", &hmin, 179);
	createTrackbar("Hue Max", "TrackbarsWindow", &hmax, 179);
	createTrackbar("Sat Min", "TrackbarsWindow", &smin, 255);
	createTrackbar("Sat Max", "TrackbarsWindow", &smax, 255);
	createTrackbar("Val Min", "TrackbarsWindow", &vmin, 255);
	createTrackbar("Val Max", "TrackbarsWindow", &vmax, 255);

	while (true)
	{
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);

		inRange(imgHSV, lower, upper, mask);

		imshow("Image", img);
		imshow("Image HSV", imgHSV);
		imshow("Image Mask", mask);
		waitKey(1);
	}



}

//Shapes&Contour Detection 形状和轮廓检测
void getContours(Mat imgDil, Mat img)
{
	vector < vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255),2);
	vector < vector<Point> >conPoly(contours.size());
	vector <Rect>boundRect(contours.size());
	string objectTYPE;

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;


		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			//drawContours(img, contours, i, Scalar(255, 0, 255), 2);
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
			int objCor = (int)conPoly[i].size();
			if (objCor == 3)
			{
				objectTYPE = "Tri";
			}
			else if (objCor == 4)
			{
				float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
				if (aspRatio > 0.95 && aspRatio < 1.05)
				{
					objectTYPE = "Square";
				}
				else
				{
					objectTYPE = "Rect";
				}
			}
			else
			{
				objectTYPE = "Circle";
			}
			putText(img, objectTYPE, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1);
		}
	}
}
void test09()
{
	string path = "Resources/shapes.png";

	Mat img = imread(path);

	Mat imgGray;
	Mat imgBlur;
	Mat imgCanny;
	Mat imgDil;
	Mat imgErode;


	// Preprocessing
	cvtColor(img, imgGray, COLOR_BGR2GRAY);//cvtColor ::convert image
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	dilate(imgCanny, imgDil, kernel);

	getContours(imgDil, img);

	erode(imgDil, imgErode, kernel);

	imshow("Original Image", img);//imshow 是一个函数

	//imshow("Gray Image", imgGray);
	//imshow("Blur Image", imgBlur);
	//imshow("Canny Image", imgCanny);
	//imshow("Dilation Image", imgDil);
	//imshow("Dilation Errode Image", imgErode);

	waitKey(0);
}

//face detection
void test10()
{
	//string path = "Resources/test.png";
	//Mat img = imread(path);

	VideoCapture cap(0);//0 camera id；
	Mat img;

	while (true)
	{
		cap.read(img);//restore in img
		//imshow("VideoName", img);

		CascadeClassifier faceCascade;
		faceCascade.load("Resources/haarcascade_frontalface_default.xml");

		if (faceCascade.empty())
		{
			cout << "XML file not loaded" << endl;
		}

		vector<Rect> faces;
		faceCascade.detectMultiScale(img, faces, 1.1, 10);

		for (int i = 0; i < faces.size(); i++)
		{
			rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);
		}

		imshow("Image", img);
		waitKey(100);
	}
}

void colorPicker()
{
	VideoCapture cap(0);//0 camera id；
	Mat img;
	Mat imgHSV;
	Mat mask;

	int hmin = 0, smin = 0, vmin = 0;
	int hmax = 179, smax = 255, vmax = 255;

	namedWindow("TrackbarsWindow", (200, 200));
	createTrackbar("Hue Min", "TrackbarsWindow", &hmin, 179);
	createTrackbar("Hue Max", "TrackbarsWindow", &hmax, 179);
	createTrackbar("Sat Min", "TrackbarsWindow", &smin, 255);
	createTrackbar("Sat Max", "TrackbarsWindow", &smax, 255);
	createTrackbar("Val Min", "TrackbarsWindow", &vmin, 255);
	createTrackbar("Val Max", "TrackbarsWindow", &vmax, 255);

	while (true)
	{
		cap.read(img);//restore in img
		cvtColor(img, imgHSV, COLOR_BGR2HSV);

		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);

		inRange(imgHSV, lower, upper, mask);

		imshow("Image", img);
		imshow("Image HSV", imgHSV);
		imshow("Image Mask", mask);

		imshow("VideoName", img);
		waitKey(50);//等待 mini second 读取下一帧
	}
}

//Virtual Painter
//smin vmin hmin smax vmax hmax

vector<vector<int>> newPoints;
vector<vector<int>> myColors{ {161,214,71,179,255,255},//red 瓶盖
							  {8,216,124,23,255,255},  //yellow 小黄鸭
};
vector<Scalar> myColorValues{ {0,0,255},   //red
							 {0,255,255} };//yellow，BGR顺序
Point getContours2(Mat imgDil, Mat& img);

vector<vector<int>> findColor(Mat& img)
{
	Mat imgHSV;
	Mat mask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		Point myPoint = getContours2(mask, img);
		if (myPoint.x != 0)
		{
			newPoints.push_back({ myPoint.x, myPoint.y,i });
		}
	}
	return newPoints;
}
Point getContours2(Mat imgDil, Mat& img)
{
	vector < vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255),2);
	vector < vector<Point> >conPoly(contours.size());
	vector <Rect>boundRect(contours.size());
	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		//cout << area << endl;


		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			//cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;// + boundRect[i].height / 2;

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
		}
	}
	return myPoint;
}
void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues, Mat& img)
{
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
	}
}
void test11()
{
	VideoCapture cap(0);//0 camera id；
	Mat img;

	while (true)
	{
		cap.read(img);//restore in img
		newPoints = findColor(img);
		drawOnCanvas(newPoints, myColorValues, img);
		imshow("VideoName", img);
		waitKey(50);//等待50 mini second 读取下一帧
	}
}

//Document Scanner
Mat imgOriginal, imgOriginal2, imgGray, imgCanny, imgThre, imgDil, imgBlur, imgErode, imgWarp,imgCrop;
vector<Point> initialPoints, docPoints;
float w = 420, h = 596;

Mat preProcessing(Mat& img)
{
	cvtColor(img, imgGray, COLOR_BGR2GRAY);//cvtColor ::convert image
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	//erode(imgDil, imgErode, kernel);
	return imgDil;
};
vector<Point> getContours(Mat imgDil)
{
	vector < vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255),2);
	vector < vector<Point> >conPoly(contours.size());
	vector <Rect>boundRect(contours.size());
	vector<Point> biggest;
	int area;
	int maxArea = 0;

	for (int i = 0; i < contours.size(); i++)
	{
		area = contourArea(contours[i]);
		//cout << area << endl;
		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			if (area > maxArea && conPoly[i].size() == 4)
			{
				//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 5);
				biggest = { conPoly[i][0],conPoly[i][1], conPoly[i][2], conPoly[i][3], };
				maxArea = area;
			}
			//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 2);
			//rectangle(imgOriginal, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
		}
	}
	return biggest;
}
void drawPoints(vector<Point>points, Scalar color)
{
	for (int i = 0; i < points.size(); i++)
	{
		circle(imgOriginal, points[i], 10, color, FILLED);
		//putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 4, color, 4);
	}
}

vector<Point> reOrder(vector<Point> points)
{
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;

	for (int i = 0; i < 4; i++)
	{
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}
	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);//0

	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);//1
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);//2
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);//3

	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points, float w, float h)
{
	Point2f src[4] = { points[0], points[1],  points[2],points[3]};
	Point2f dst[4] = { {0.f,0.f},{w,0.f},{0.f,h},{w,h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));
	return imgWarp;
}

void test12()
{
	string path = "Resources\\paper.jpg";// \ 表示转义 也可以用 /
	imgOriginal = imread(path);

	resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5);
	
	imgOriginal2 = imread(path);

	resize(imgOriginal2, imgOriginal2, Size(), 0.5, 0.5);

	//preprocessing
	imgThre = preProcessing(imgOriginal);
	//Get Contours - Biggest
	initialPoints = getContours(imgThre);
	//drawPoints(initialPoints,Scalar(0,0,255));
	docPoints = reOrder(initialPoints);
	drawPoints(docPoints, Scalar(0, 255, 0));
	//Warp
	imgWarp = getWarp(imgOriginal2, docPoints, w, h);
	//crop
	int cropVal = 5;
	Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal));
	imgCrop = imgWarp(roi);
	imshow("Image", imgOriginal);
	imshow("ImgDil", imgDil);
	imshow("ImgWarp", imgWarp);
	imshow("ImgCrop", imgCrop);
	waitKey(0);
}

void test13()
{
	VideoCapture cap(0);//0 camera id；
	while (true)
	{
		cap.read(imgOriginal);
		//string path = "Resources/demo.jpg";
		//imgOriginal = imread(path);
		
		//preprocessing
		imgThre = preProcessing(imgOriginal);

		//Get Contours - Biggest
		initialPoints = getContours(imgThre);
		//drawPoints(initialPoints,Scalar(0,0,255));
		docPoints = reOrder(initialPoints);
		drawPoints(docPoints, Scalar(0, 255, 0));
		//Warp
		imgWarp = getWarp(imgOriginal, docPoints, w, h);
		//crop
		int cropVal = 20;
		Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal));
		imgCrop = imgWarp(roi);
		
		imshow("Image", imgOriginal);
		imshow("ImgDil", imgDil);
		imshow("ImgWarp", imgWarp);
		imshow("ImgCrop", imgCrop);
		waitKey(500);//等待50 mini second 读取下一帧
	}
}

//License Plate Detector
void test14()
{
	VideoCapture cap(0);//0 camera id；
	Mat img;
	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");
	if (plateCascade.empty())
	{
		cout << "XML file not loaded" << endl;
	}
	vector<Rect> plates;
	while (true)
	{
		cap.read(img);//restore in img
		plateCascade.detectMultiScale(img, plates, 1.1, 10);

		for (int i = 0; i < plates.size(); i++)
		{
			Mat imgCrop = img(plates[i]);
			imshow(to_string(i), imgCrop);
			imwrite("Resources/Plates/" + to_string(i) + ".png",imgCrop);
			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);
		
		}

		imshow("Image", img);
		waitKey(1);
	}
}

void main()
{
	//test01();
	//test02();
	//test03();
	//test04();
	//test05();
	//test06();
	//test07();
	//test08();
	//test08();
	//test09();
	//test10();
	//colorPicker();
	//test11();
	//test12();
	//test13();
	test14();
	cout << "hello" << endl;
}

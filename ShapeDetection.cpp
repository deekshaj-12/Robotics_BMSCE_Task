#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

int dist(int x1, int y1, int x2, int y2)
{
	int d = sqrt(abs(pow(abs(x1 - x2), 2) + pow(abs(y1 - y2), 2)));
	return d;
}

float slope(int x1, int y1, int x2, int y2)
{
	int s;
	if (x1 == x2)
	{
		s = 1.57;
	}
	
	else
	{
		s = atan((y2 - y1) / (x2 - x1));
	}
	
	return s;
}

void draw(Mat img, vector<vector<Point>> conPoly,int i, vector<Rect> boundRect,string objType)
{
	drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
	rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
	putText(img, objType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0, 69, 255), 2);

}

void getContours(Mat imgDil, Mat img,int a) 
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		
		string objType;
		float peri = arcLength(contours[i], true);
		approxPolyDP(contours[i], conPoly[i], 0.01 * peri, true);
		boundRect[i] = boundingRect(conPoly[i]);
		int objCor = (int)conPoly[i].size();
		float w = (float)boundRect[i].width, h = (float)boundRect[i].height;
		float aspRatio = w / h;
		float s1, s2, s3, s4, m1, m2;
		int d1, d2, r1, r2;
		if (objCor == 4)
		{
			s1 = slope(contours[i][0].x, contours[i][0].y, contours[i][1].x, contours[i][1].y);
			s2 = slope(contours[i][3].x, contours[i][3].y, contours[i][2].x, contours[i][2].y);
			s3 = slope(contours[i][0].x, contours[i][0].y, contours[i][3].x, contours[i][3].y);
			s4 = slope(contours[i][1].x, contours[i][1].y, contours[i][2].x, contours[i][2].y);
			d1 = dist(contours[i][0].x, contours[i][0].y, contours[i][1].x, contours[i][1].y);
			d2 = dist(contours[i][3].x, contours[i][3].y, contours[i][0].x, contours[i][0].y);
			r1 = dist(contours[i][0].x, contours[i][0].y, contours[i][2].x, contours[i][2].y);
			r2 = dist(contours[i][1].x, contours[i][1].y, contours[i][3].x, contours[i][3].y);
			m1 = slope(contours[i][0].x, contours[i][0].y, contours[i][1].x, contours[i][1].y);
			m2 = slope(contours[i][0].x, contours[i][0].y, contours[i][3].x, contours[i][3].y);
		}
		switch (a)
		{
		case 1:
			if (objCor == 3)
			{
				objType = "Triangle";
				draw(img, conPoly, i, boundRect, objType);

			}
			break;
		case 2:
			if (objCor == 4)
			{

				
				if (aspRatio > 0.8 && aspRatio < 1.2)
				{
					objType = "square"; cout << r1 << "  " << r2 << endl;
					draw(img, conPoly, i, boundRect, objType);
				}
				
			}
			break;
		case 3:
			if (objCor == 4)
			{
				if (s1==s2&&s3==s4)
				{
					objType = "Rectangle";
					draw(img, conPoly, i, boundRect, objType);
				}
			}
			break;
		case 4:
			if (objCor == 4)
			{
				if ((s1 == s2 || s3 == s4)&&(s1 != s2 || s3 != s4))
				{
					objType = "trapezium";
					draw(img, conPoly, i, boundRect, objType);
					
				}
			}
			break;
		case 5:
			if (objCor == 4)
			{
				objType = "Quadrilateral";
				draw(img, conPoly, i, boundRect, objType);
			}
			break;
		case 6:
			if (objCor == 5)
			{
				objType = "Pentagon";
				draw(img, conPoly, i, boundRect, objType);
			}
			break;
		case 7:
			if (objCor == 6)
			{
				objType = "Hexagon";
				draw(img, conPoly, i, boundRect, objType);
			}
			break;
		case 8:
			if (objCor == 7)
			{
				objType = "Heptagon";
				draw(img, conPoly, i, boundRect, objType);
			}
			break;
		case 9:
			if (objCor == 8)
			{
				objType = "Octagon";
				draw(img, conPoly, i, boundRect, objType);
			}
			break;
		case 10:
			if (objCor > 8)
			{
				if (aspRatio > 0.9 && aspRatio < 1.1)
				{
					objType = "Circle";
					draw(img, conPoly, i, boundRect, objType);
				}
				else
				{
					objType = "Oval";
					draw(img, conPoly, i, boundRect, objType);
				}
			}
			break;
		default: cout << "Invalid Entry" << endl;
		}
	}
}

void main() 
{
	int n;
	cout << "Enter:\n1 for Triangle\n2 for Square\n3 for Rectangle\n4 for Trapezium\n5 for General Quadrilateral\n6 for Pentagon\n7 for Hexagon\n8 for Heptagon\n9 for Octagon\n10 for circle or oval\n" << endl;
	cin >> n;
	string path;
	cout << "Enter the path of the image" << endl;
	cin >> path;

	path = "Resources/shapes1.jpeg";
	Mat img = imread(path);
	Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(1,1));
	dilate(imgCanny, imgDil, kernel);

	getContours(imgDil, img,n);

	imshow("Image", img);

	waitKey(0);

}
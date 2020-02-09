#include "stdafx.h"
#include "windows.h"
#include "tchar.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

#include "yolo_v2_class.hpp"    // imported functions from DLL

using namespace std;
using namespace cv;
#include <iostream>

Mat hwnd2mat(HWND hwnd, int x, int y, int w, int h) {

	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = windowsize.bottom / 2;  //change this to whatever size you want to resize to
	width = windowsize.right / 2;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

																									   // avoid memory leak
	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);

	Rect roi;
	roi.x = x;
	roi.y = y;
	roi.width = w;
	roi.height = h;

	src = src(roi);

	return src;
}

void main()
{
	init("yolov3-spp-copper.cfg", "yolov3-spp-copper_last.weights", 0);

	HWND desk = GetDesktopWindow();
	LPWSTR lpwstr = L"Test String";
	
	bbox_t_container container;
	int result = detect_image("16.jpg", container);
	string setStr = "result " + to_string(result) + "\n";
	cout << setStr;
	for (int i = 0; i < result; i++) {
		string setStr = to_string(container.candidates[i].x) + " " + to_string(container.candidates[i].y) + "\n";
		cout << setStr;
	}
	
	while (desk != 0)
	{
		Mat mat = hwnd2mat(desk, 0, 0, 320, 240);
		imshow("Capture", mat);
		waitKey(20); // need after imshow you can change wait time
		bbox_t_container container;
		int result = detect_matt(mat, container);
		for (int i = 0; i < result; i++) {
			string setStr = "obj: "+ to_string(container.candidates[i].obj_id) + " x: " +  to_string(container.candidates[i].x) + ", y: " + to_string(container.candidates[i].y) + "\n";
			cout << setStr;
		}
	}
	
}

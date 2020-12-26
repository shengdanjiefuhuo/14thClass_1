#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void onMatch(int, void* userdata);//匹配方法回调函数
int method = 0;
Mat tempMat, resultMat;//模板，输出矩阵

int main()
{
	VideoCapture cap(0);//打开摄像头
	if (!cap.isOpened())
	{
		cout << "不能打开摄像头" << endl;
		return -1;
		destroyAllWindows();
	}

	Mat frame;
	int cnt = 0;

	while (1)
	{
		cap >> frame;
		if (frame.empty())
		{
			cout << "读取帧错误" << endl;
			return -1;
			destroyAllWindows();
		}
		//选择模板
		if (!cnt)
		{
			Rect r = selectROI(frame, true);//返回选择区域
			tempMat = frame(r).clone();//取选择区域
			cnt = 1;
			destroyAllWindows();
		}

		else
		{
			namedWindow("显示结果");
			//创建滑动条
			createTrackbar("method", "显示结果", &method, 5, onMatch, &frame);
			//调用回调函数
			onMatch(0, &frame);
		}
		//延时30ms
		//等待键盘相应，按下ESC键退出
		if (waitKey(30) == 27)
		{
			destroyAllWindows();
			break;
		}
	}

	return 0;
}


void onMatch(int, void* userdata)
{
	double minValue, maxValue;
	Point minLoction, maxLoction, Loction;
	Mat src = *(Mat*)userdata;
	//模板匹配
	matchTemplate(src, tempMat, resultMat, method);
	normalize(resultMat, resultMat, 0, 1, NORM_MINMAX, -1);
	//寻找匹配的最大的位置
	minMaxLoc(resultMat, &minValue, &maxValue, &minLoction, &maxLoction);
	if (method == TM_SQDIFF || method == TM_SQDIFF_NORMED)
		Loction = minLoction;
	else
		Loction = maxLoction;
	//标示出匹配结果
	rectangle(src, Loction, Point(Loction.x + tempMat.cols, Loction.y + tempMat.rows), Scalar(255, 255, 0));
	imshow("显示结果", src);
}

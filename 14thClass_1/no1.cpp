#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void onMatch(int, void* userdata);//ƥ�䷽���ص�����
int method = 0;
Mat tempMat, resultMat;//ģ�壬�������

int main()
{
	VideoCapture cap(0);//������ͷ
	if (!cap.isOpened())
	{
		cout << "���ܴ�����ͷ" << endl;
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
			cout << "��ȡ֡����" << endl;
			return -1;
			destroyAllWindows();
		}
		//ѡ��ģ��
		if (!cnt)
		{
			Rect r = selectROI(frame, true);//����ѡ������
			tempMat = frame(r).clone();//ȡѡ������
			cnt = 1;
			destroyAllWindows();
		}

		else
		{
			namedWindow("��ʾ���");
			//����������
			createTrackbar("method", "��ʾ���", &method, 5, onMatch, &frame);
			//���ûص�����
			onMatch(0, &frame);
		}
		//��ʱ30ms
		//�ȴ�������Ӧ������ESC���˳�
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
	//ģ��ƥ��
	matchTemplate(src, tempMat, resultMat, method);
	normalize(resultMat, resultMat, 0, 1, NORM_MINMAX, -1);
	//Ѱ��ƥ�������λ��
	minMaxLoc(resultMat, &minValue, &maxValue, &minLoction, &maxLoction);
	if (method == TM_SQDIFF || method == TM_SQDIFF_NORMED)
		Loction = minLoction;
	else
		Loction = maxLoction;
	//��ʾ��ƥ����
	rectangle(src, Loction, Point(Loction.x + tempMat.cols, Loction.y + tempMat.rows), Scalar(255, 255, 0));
	imshow("��ʾ���", src);
}

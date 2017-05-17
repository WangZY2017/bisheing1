
/************************************************************************/
/*
Description:	�����������˲�Ŀ����٣�HSV��ɫ������ֱ��ͼģ�ͣ�
Author:			Zhao Lu
Email:			wyuzyf@126.com
Version:		2017-4-10
History:
Development environment:   VS2013+opencv3.10
*/
/************************************************************************/

#include <iostream >    //��׼IO
#include <string>      //�ַ���
#include <iomanip>    //�����ӡ���
#include <sstream>    //�ַ��������ֵ�ת��

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "pf.h"

using namespace std;
using namespace cv;


int nFrameNum;          //ȫ�ֱ���Ĭ�ϳ�ʼ��Ϊ0���ֲ����������ʼ������Ϊ�ֲ���������ڶ���
						//֡����Ŀ

bool bSelectObject = false;     //����ѡ���־
bool bTracking = false;       //��ʼ���ٱ�־

Point origin;       //��갴��ʱ���λ���п��ܺ�ʵ��ѡȡ�ĵ��λ�ò�һ��
extern Rect selection;     //����Ȥ�������С,����һ�����Ͻǵ�����Ϊ(x, y)��width*height���δ���
						//���εĴ�С������ѡ����Ĵ�С

//PARTICLE particle[Particle_num];     //����
//const int Particle_num = 25;       //���Ӹ���

extern Mat Image;        //ѡȡ��Ŀ��ͼ��

bool paused = false;    //��ͣ��

//�����Ӧ�������õ�ѡ������򣬱�����selection
void onMouse(int event, int x, int y, int, void*)
{
	if (bSelectObject)    //�ж�����ѡȡĿ������
	{
		selection.x = MIN(x,origin.x);  //ѡ��С��Ҳ�����÷�Χ�����ܴ�
		selection.y = MIN(y,origin.y);
		selection.width = std::abs(x - origin.x);    //x���ǿ�ȣ������ֵ
		selection.height = std::abs(y - origin.y);

		selection &= Rect(0, 0, Image.cols, Image.rows);  //&=��ֵ���������������
	}

	switch (event)
	{
	case CV_EVENT_FLAG_LBUTTON:   //�����Ϣ���� ������
		origin = Point(x,y);   //ѡȡ�ĵ��λ��
		selection = Rect(x,y,0,0);  //ѡȡĿ��Ĵ�С
		bSelectObject = true;   //ѡȡ�ɹ�
		bTracking = false;    //��û��ʼ����Ŀ��
		break;
	case CV_EVENT_LBUTTONUP:   //����ſ�
		bSelectObject = false;     //����ѡȡĿ��Ķ���
		bTracking = true;        //��ʼ����Ŀ��
		nFrameNum = 0;    //Ŀ��֡��0������û��ʽ��ʼ
		break;
	}

}

void pause(void)
{
	int delay = 10;   //���Ʋ����ٶ�
	char c;   //��ֵ
	c = (char)waitKey(delay);

	switch (c)
	{
	case 27:
		break;
	case 'p':
		paused = !paused;
		break;
	default:
		;
	}
}

int main(int argc, char *argv[])
{
	

	VideoCapture captRefrnc("F:\singlechip\a\bisheing\myself particle filtert\particleFilter\soccer");    //��Ƶ�ļ�

	if (!captRefrnc.isOpened())
	{
		return -1;
	}

	const char* WIN_RESULT = "Result";     //��������
	namedWindow(WIN_RESULT,CV_WINDOW_AUTOSIZE);    //��С�ɱ�

	setMouseCallback(WIN_RESULT,onMouse,0);     //�����Ӧ����
								//ѡȡ�Ĵ�����WIN_RESULT�����ص�������onMouse

	Mat frame;   //��Ƶ��ÿһ֡ͼ��

	//bool paused = false;    //��ͣ��
	//PARTICLE *pParticles = particle;
	//PARTICLE *pParticles = new PARTICLE[sizeof(PARTICLE)* Particle_num];

	while (true)    //�ظ�����ͼƬ
	{
		if (!paused)
		{
			captRefrnc >> frame;      //����ÿһ֡ͼƬ
			if (frame.empty())
				break;
		}

		frame.copyTo(Image);   //�ɼ�һ��ͼ�񵽻���Image

		//ѡ��Ŀ������
		if (bTracking == true)
		{
			if (!paused)
			{
				nFrameNum++;  //֡��+1

				//���������˲�
				particle_filter();
				
			}
		}
		imshow(WIN_RESULT, Image);
		pause();
	}
}

#ifndef __PF_H
#define __PF_H

//#include <iostream >    //��׼IO
//#include <string>      //�ַ���
//#include <iomanip>    //�����ӡ���
//#include <sstream>    //�ַ��������ֵ�ת��

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//using namespace cv;

/*���²����Խ���кܴ��Ӱ�죬������ʱ����*/
 const int Particle_num = 25;       //���Ӹ���

//���ӷ�����������
const double A1 = 2.0;
const double A2 = -1.0;
const double B0 = 1.0;

//��˹�����sigma����
const double SIGMA_X = 1.0;
const double SIGMA_Y = 0.5;
const double SIGMA_SCALE = 0.001;

extern int nFrameNum;

//���ӽṹ��
typedef struct particle{
	
	//ԭʼ��ѡ�����ģ���ǰ�ģ�Ԥ���
	double x;    //��ǰx����
	double y;    //��ǰy����
	double scale;      //���ڱ���ϵ��
	
	double xPre;      //xԤ������λ��
	double yPre;      //yԤ������λ��
	double scalePre;   //����Ԥ�����ϵ��
	
	double xOri;       //ԭʼx����
	double yOri;       //ԭʼy����
	Rect rect;      //ԭʼ�����С
	MatND hist;      //�������������ֱ��ͼ
	double weight;     //���ӵ�Ȩ��
}PARTICLE;
 PARTICLE particles[Particle_num];     //����

//ֱ��ͼ��ز�����������ѡȡҲ��Խ��Ӱ��޴�
// Quantize the hue to 30 levelsɫ��
// and the saturation to 32 levels ���Ͷ�
// value to 10 levels����ֵ
int hbins = 180, sbins = 256, vbin = 10;
int histSize[] = { hbins, sbins, vbin };

//hue varies from 0 to 170,see cvtColor
float hranges[] = { 0, 180 };
//saturation varies from 0 (black-gray-white) to 255 (pure spectrum color)
float sranges[] = { 0, 256 };
//value varies from 0 (black-gray-white) to 255 (pure spectrum color)
float vranges[] = { 0, 256 };

const float *ranges[] = { hranges, sranges, vranges };
//we compute the histogram from the 0-th and 1-st channels
int channels[] = { 0, 1, 2 };


//���ܺ���
int particle_cmp(const void *p1, const void *p2);
void particle_filter(void);






#endif



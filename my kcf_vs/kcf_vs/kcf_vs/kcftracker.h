
/*
http://blog.csdn.net/shenxiaolu1984/article/details/50905283
һ�㻯�ĸ���������Էֽ�����¼�����
1. ��It֡�У��ڵ�ǰλ��pt����������ѵ��һ���ع���������ع����ܼ���һ��С���ڲ�������Ӧ��
2. ��It+1֡�У���ǰһ֡λ��pt������������ǰ���ع����ж�ÿ����������Ӧ��
3. ��Ӧ��ǿ�Ĳ�����Ϊ��֡λ��pt+1��
*/

#pragma once

#include "tracker.h"

#ifndef _OPENCV_KCFTRACKER_HPP_
#define _OPENCV_KCFTRACKER_HPP_
#endif

//�̳�Tracker
class KCFTracker : public Tracker
{
public:
	// Constructor  ����KCF����������
	KCFTracker(bool hog = true,   //ʹ��HOG����
		bool fixed_window = true,   //ʹ�ù̶�����
		bool multiscale = true,    //ʹ�ö�߶�
		bool lab = true);    //ʹ��Lab��ɫ�ռ�����


	// Initialize tracker 
	//��ʼ����������roi��Ŀ���ʼ������ã�image�ǽ�����ٵĵ�һ��ͼ��
	virtual void init(const cv::Rect &roi, cv::Mat image);


	// Update position based on the new frame
	//������һ֡��ͼ�����Ŀ��λ�ã�image����һ֡ͼ��
	//��߶ȶ��������ͨ�����һ������һ��С��ĳ߶ȣ��Ƚ�������ֵ�Ľ�������ж�߶ȵ���Ӧ
	virtual cv::Rect update(cv::Mat image);  // cv::Rect�Ƿ�������


	float interp_factor; // linear interpolation factor for adaptation
	//����Ӧ�����Բ�ֵ���ӣ�����ΪHOG,Lab��ѡ��仯
	float sigma; // gaussian kernel bandwidth
	//��˹����˴�������ΪHOG,Lab��ѡ��仯
	float lambda; // regularization
	//���򻯲�����0.0001
	int cell_size; // HOG cell size 
	//ϸ����Ԫ�ߴ�
	int cell_sizeQ; // cell size^2, to avoid repeated operations
	//ϸ����Ԫ�����أ�����6*6��������Ŀ
	float padding; // extra area surrounding the target
	//Ŀ����չ����������
	float output_sigma_factor; // bandwidth of gaussian target
	//��˹Ŀ��Ĵ���
	int template_size; // template size
	//ģ���С���ڼ���_tmpl_szʱ��  
	// �ϴ��ɱ���һ��96������С�߳���������С 
	float scale_step; // scale step for multi-scale estimation
	//��߶ȹ��Ƶĳ߶Ȳ���
	float scale_weight;  // to downweight detection scores of other scales for added stability
	//Ϊ�����������߶ȼ��ʱ���ȶ��ԣ����������һ����˥���ȣ�Ϊԭ����0.95
protected:
	// Detect object in the current frame.
	//��⵱ǰ֡��Ŀ��
	//z��ǰһ֡��ѵ�������ǵ�һ֡�ĳ�ʼ�������x�ǵ�ǰ֡�߶��µ�������peak_value�Ǽ������ֵ
	cv::Point2f detect(cv::Mat z, cv::Mat x, float &peak_value);


	// train tracker with a single image
	//ʹ�õ�ǰͼ��ļ��������ѵ����x�ǵ�ǰ֡�߶��µ�������train_interp_factor��interp_factor
	void train(cv::Mat x, float train_interp_factor);

	// Evaluates a Gaussian kernel with bandwidth SIGMA for all relative shifts between input images X and Y, 
	//which must both be MxN. They must also be periodic (ie., pre-processed with a cosine window).
	//ʹ�ô���SIGMA�����˹�������������ͼ��x��y֮������λ�ƣ�
	//������M*N��С�����߱����������Եģ�ͨ��һ��cos(����)���ڽ���Ԥ����
	cv::Mat gaussianCorrelation(cv::Mat x1, cv::Mat x2);


	// Create Gaussian Peak. Function called only in the first frame.
	//������˹��ֵ�������˺���ֻ�ڵ�һ֡ʱ����
	cv::Mat createGaussianPeak(int sizey, int sizex);

	// Obtain sub-window from image, with replication-padding and extract features
	//��ͼ��õ�2.5���Ӵ��ڣ���������ͨ����ֵ��䲢��֤
	cv::Mat getFeatures(const cv::Mat & image, bool inithann, float scale_adjust = 1.0f);

	// Initialize Hanning window. Function called only in the first frame.
	//��ʼ��Hanning���ڣ�����
	void createHanningMats();

	// Calculate sub-pixel peak for one dimension
	//����һά�����ط�ֵ��������������֮�仹��sub-pixel
	float subPixelPeak(float left, float center, float right);

	cv::Mat _alphaf;  //��ʼ��/ѵ�����alpha,���ڼ�ⲿ���н���ļ��㣬����������֤���˰���ĵ�alpha
	cv::Mat _prob;   //��ʼ��������ʣ����ø��ģ�����ѵ������Ԥ�⵽��Ŀ��λ��
	cv::Mat _tmpl;  //��ʼ��/ѵ���Ľ��������detect��z���������е�����ͼ��z

	cv::Mat _num;   //
	cv::Mat _den;

	cv::Mat _labCentroids; //Lab��������

private:
	int size_patch[3];   //HOG������sizeY��sizeX��numFeatures��ά����
	cv::Mat hann;    //createHanningMats()�ļ�����
	cv::Size _tmpl_sz;  //HOGϸ��Ԫ��Ӧ�������С
	float _scale;  //������_tmpl_sz��ĳ߶ȴ�С���߶Ȳ���ָ����ĳߴ��С����ĳһ�����Ĳ���

	int _gaussian_size; //����õ��ĸ�˹��С

	bool _hogfeatures;   //HOG��־λ
	bool _labfeatures;  //lab��־λ
};


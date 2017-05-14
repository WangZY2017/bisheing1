
/*@file
	�й�Ŀ��ģ�͵Ķ���
   @author zhao lu
   @version 20170413
*/

#ifndef OBSERVATION_H
#define OBSERVATION_H

/**********************************Definitions******************************/

#include "defs.h"
#include "utils.h"

//��HSV�ֳ����ɵ���ɫ���䣬binԽ��ֱ���Խ��
#define NH 10
#define NS 10
#define NV 10

//HSV�����ֵ
#define H_MAX 360.0
#define S_MAX 1.0
#define V_MAX 1.0

//ֱ��ͼ�б��ͶȺ�����ֵ�������ֵ
#define S_THRESH 0.1
#define V_THRESH 0.2

//�ֲ�����??????
#define LAMBDA 20

/***********************Structures************************/

//ֱ��ͼ��NH * NS + NV����ʽ��ʾ��������ֵ������ֵ��NH * NS��ʾ������ɫ��������ֵ��NV��ʾ
typedef struct histogram{
	float histo[NH * NS + NV];    //ֱ��ͼ����
	int n; //�����ĳ���
}histogram;

/**********************Function Definitions******************/

/*
	��BGRͼƬת����HSV��ɫģ��

	@param img:��ת����ͼƬ

	@return :ת��Ϊһ��3ͨ����32bit��HSVͼ��
			 S and V values in the range [0,1] and H value in the range [0,360]
*/

IplImage* bgr2hsv(IplImage* img);

/*
	����HSVֱ��ͼ��binֵ

	@param h Hue
	@param s Saturation
	@param v Value

	@return ������HSV��Ӧ��bin������ֵ
*/
int histo_bin(float h, float s, float v);

/*
	���ڸ�����ͼ�����飬�������ۻ���ֱ��ͼ

	@param imgs an array of images over which to compute a cumulative histogram;
	each must have been converted to HSV colorspace using bgr2hsv()
	@param n the number of images in imgs

	@return ����imgs��HSVֱ��ͼ���޹�һ����
*/
histogram* calc_histogram(IplImage** imgs,int n);

/**
	Normalizes a histogram so all bins sum to 1.0

	@param histo a histogram
*/
void normalize_histogram(histogram* histo);

/**
	���ݰ��Ͼ����������ֱ��ͼ�����ƶ�

	@param h1 first histogram; should be normalized
    @param h2 second histogram; should be normalized
   
    @return Rerns a squared distance9�������ƽ���� based on the Battacharyya similarity
     coefficient between  h1 and  h2
*/
float histo_dist_sq(histogram* h1,histogram* h2);

/**
	�����������ֱ��ͼ���Ƶ�ͼ��

	@param img image that has been converted to HSV colorspace using bgr2hsv()
	@param r row location of center of window around which to compute likelihood
	@param c col location of center of window around which to compute likelihood
	@param w width of region over which to compute likelihood
	@param h height of region over which to compute likelihood
	@param ref_histo reference histogram for a player; must have been
	normalized with normalize_histogram()

	@return ��������ֵ
*/
float likelihood(IplImage* img,int r,int c,int w,int h,histogram* ref_histo);

/**
	�������Ȥ������ÿһ�����ص����ƶ�

	@param img the image for which likelihood is to be computed; should have
	been converted to HSV colorspace using bgr2hsv()
	@param w width of region over which to compute likelihood
	@param h height of region over which to compute likelihood
	@param ref_histo reference histogram for a player; must have been
	normalized with normalize_histogram()

	@return Returns a single-channel, 32-bit floating point image containing
	the likelihood of every pixel location in \a img normalized so that the
	sum of likelihoods is 1.
*/
IplImage* likelihood_image(IplImage* img, int w, int h, histogram* ref_histo);

/**
	��ֱ��ͼ�����ݵ������ļ�

	0 <h_0>
	...
	<i> <h_i>
	...
	<n> <h_n>

	Where n is the number of histogram bins and h_i, i = 1..n are
	floating point bin values

	@param histo histogram to be exported
	@param filename name of file to which histogram is to be exported

	@return Returns 1 on success or 0 on failure
*/
int export_histogram(histogram* histo, char* filename);



#endif


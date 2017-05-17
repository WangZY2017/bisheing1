/*
	Functions for object tracking with a particle filter

	@author zhao lu
	@version 20170414
*/

#include "defs.h"
#include "utils.h"
#include "particles.h"


/*************************** Function Definitions ****************************/

//��ʼ�����Ӽ�
particle* init_distribution(CvRect* regions, histogram** histos, int n, int p)
{
	particle* particles;
	int np;   
	float x, y;
	int i, j, width, height, k = 0;

	particles = (particle*)malloc(p * sizeof(particle));
	np = p / n;   //�����ܶ�=�������� / ����Ȥ����n

	//��n��regions�����Ĵ�������
	for (i = 0; i < n; i++)
	{
		//�ҵ����ε��������꣬regions[i].x�Ǿ������Ͻ�����
		width = regions[i].width;
		height = regions[i].height;
		x = regions[i].x + width / 2;
		y = regions[i].y + height / 2;

		for (j = 0; j < np; j++)
		{
			//ԭʼ = Ԥ�� =��ǰ ��original�ǲ���ģ�����ѡȡ��Ŀ������
			particles[k].x0 = particles[k].xp = particles[k].x = x;
			particles[k].y0 = particles[k].yp = particles[k].y = y;
			particles[k].sp = particles[k].s = 1.0;
			particles[k].width = width;
			particles[k].height = height;
			particles[k].histo = histos[i];
			particles[k++].w = 0;
		}
	}

	//ȷ����ÿ�������д�����׼ȷ��p������
	i = 0;
	while (k < p)
	{
		width = regions[i].width;
		height = regions[i].height;
		x = regions[i].x + width / 2;
		y = regions[i].y + height / 2;
		particles[k].x0 = particles[k].xp = particles[k].x = x;
		particles[k].y0 = particles[k].yp = particles[k].y = y;
		particles[k].sp = particles[k].s = 1.0;
		particles[k].width = width;
		particles[k].height = height;
		particles[k].histo = histos[i];
		particles[k++].w = 0;
		i = (i + 1) % n;
	}

	return particles;   //����һ���ṹ��

}

//���ݶ����Իع飬����״̬ת��ģ�ͣ�����ģ�͵õ��µ����Ӽ�
particle transition(particle p, int w, int h, RNG rng)
{
	float x, y, s;
	particle pn;
	
	/* sample new state using second-order autoregressive dynamics */
	x = A1 * (p.x - p.x0) + A2*(p.xp - p.x0) + B0 * rng.gaussian(TRANS_X_STD) + p.x0;
	pn.x = MAX(0.0,MIN((float)w - 1.0, x));

	y = A1 * (p.y - p.y0) + A2*(p.yp - p.y0) + B0 * rng.gaussian(TRANS_Y_STD) + p.y0;
	pn.x = MAX(0.0, MIN((float)w - 1.0, y));

	s = A1 * (p.s - 1.0) + A2 * (p.sp - 1.0) + B0 * rng.gaussian(TRANS_S_STD) + 1.0;
	pn.s = MAX(0.1,s);
	
	//��һ�����Ӽ���ǰ�Ĳ��� = �����Ӽ���Ԥ��
	pn.xp = p.x;
	pn.yp = p.y;
	pn.sp = p.s;
	pn.x0 = p.x0;
	pn.y0 = p.y0;
	pn.width = p.width;
	pn.height = p.height;
	pn.histo = p.histo;
	pn.w = 0;

	return pn;      //�����µ����Ӽ�
}

//��һ�����ӵ�Ȩֵ
void normalize_weights(particle* particles, int n)
{
	float sum = 0;
	int i;

	for (i = 0; i < n; i++)
		sum += particles[i].w;
	for (i = 0; i < n; i++)
		particles[i].w /= sum;
}


//��������Ȩֵ�����Ӽ� (�ز���) �õ��µ���Ȩֵ�����Ӽ�
particle* resample(particle* particles, int n)
{
	particle* new_particles;
	int i, j, np, k = 0;

	qsort(particles, n, sizeof(particle), particle_cmp);
	new_particles = (particle*)malloc(n * sizeof(particle));
	for (i = 0; i < n; i++)
	{
		//�������룬npԽ��˵��ȨֵԽ��,Ȼ���ڴ�Ȩֵ���ӵĸ���ȡ����
		np = cvRound(particles[i].w * n);   
		for (j = 0; j < np; j++)  //��֤��Ȩֵ��0�����ӿ϶���ɾ��
		{
			new_particles[k++] = particles[i];     //?????
			if (k == n)
				goto exit;
		}
	}
	while (k < n)
		new_particles[k++] = particles[0];

exit:
	return new_particles;

}

//�Ƚ��������ӵ�Ȩֵ
int particle_cmp(const void* p1, const void* p2)
{
	particle* _p1 = (particle*)p1;
	particle* _p2 = (particle*)p2;

	if (_p1->w > _p2->w)
		return -1;
	if (_p1->w < _p2->w)
		return 1;
	return 0;
}

//region�Ǹ����Σ������ӱ�ʾ��
void dispaly_particle(IplImage* img, particle p, CvScalar color)
{
	int x0, y0, x1, y1;
	x0 = cvRound(p.x - 0.5 * p.s * p.width);
	y0 = cvRound(p.y - 0.5 * p.s * p.height);
	x1 = x0 + cvRound(p.s * p.width);
	y1 = y0 + cvRound(p.s * p.height);

	cvRectangle(img, cvPoint(x0,y0), cvPoint(x1,y1),color,1,8,0);

}


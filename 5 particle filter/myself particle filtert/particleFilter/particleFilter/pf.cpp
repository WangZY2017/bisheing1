
#include "pf.h"


using namespace cv;

Mat hsv;    //hsvɫ�ʿռ������ͼ��
Mat roiImage;      //Ŀ������
Mat Image;        //ѡȡ��Ŀ��ͼ��
MatND roiHist;    //���������Ŀ������ֱ��ͼ

Rect selection;     //����Ȥ�������С


//����������������Ȩ��ֵ��С
int particle_cmp(const void *p1, const void *p2) //����������������ָ��
{
	PARTICLE* _p1 = (PARTICLE*)p1;    //ǿ��ת��ָ������
	PARTICLE* _p2 = (PARTICLE*)p2;

	if (_p1->weight < _p2->weight)
		return 1;    //����Ȩ�ؽ������򣿣�����
	if (_p1->weight > _p2->weight)
		return -1;
	return 0;
}

//�����˲��㷨ʵ��
void particle_filter(void)
{
	PARTICLE *pParticles = particles;

	cvtColor(Image, hsv, CV_BGR2HSV);   //��BGRת����HSVģ��
	Mat roiImage(hsv, selection);       //Ŀ�������ֱ��ͼ

	if (nFrameNum == 1)     //�Ƿ�Ϊ��һ֡
	{
		//step1:��ȡĿ����������
		calcHist(&roiImage,1,channels,Mat(),roiHist,3,histSize,ranges);
		normalize(roiHist,roiHist);   //��һ��ֱ��ͼ
		    //��Ϊȫ�ֱ������õ���ʱ������Ϊ��ǰֵ��������������һ������Ӱ��
		
		//step2:��ʼ��particles
		pParticles = particles;   //����
		for (int i = 0; i < Particle_num; i++)
		{
			//��Ϊ�ǳ�ʼ�������Ե�ǰ=ԭʼ=Ԥ��
			pParticles->x = selection.x + 0.5* selection.width;
			pParticles->y = selection.y + 0.5* selection.height;

			pParticles->xPre = pParticles->x;
			pParticles->yPre = pParticles->y;

			pParticles->xOri = pParticles->x;
			pParticles->yOri = pParticles->y;

			pParticles->rect = selection;
			pParticles->scale = 1.0;
			pParticles->scalePre = 1.0;
			pParticles->hist = roiHist;
			pParticles->weight = 0;
			pParticles++;

		}
	}

	else    //���ǵ�һ֡
	{
		pParticles = particles;
		RNG rng;  //���ȷֲ�������͸�˹�ֲ������
		for (int i = 0; i < Particle_num; i++)
		{
			//step3:��particles����һ�����ݣ�������
			//״̬ת�ƽ׶Σ���������һ֡�����ӵ�״̬��x,y,w,h�������Ʊ�֡�и������ӵ�״̬
			
			//Ŀǰ��û��ȫ���ԭ�����������ע�Ͷ��ǲ��룬��һ����ȷ
			
			double x, y, s;

			//��ǰ=Ԥ��,����һ֡������ǰһ֡��Ԥ�⣬Ȼ�����Ԥ����㵱ǰһ֡����ԭʼ��oriû��ϵ��
			pParticles->xPre = pParticles->x; 
			pParticles->yPre = pParticles->y;
			pParticles->scale = pParticles->scale;

			//�������ӵ�����
			x = A1 * (pParticles->x - pParticles->xOri) + A2 * (pParticles->xPre - pParticles->xOri)
				+ B0 * rng.gaussian(SIGMA_X) + pParticles->xOri;   
			//���pParticles->xPre = pParticles->x����ǰ����ʽ�ӳ����������ܲ�һ��֮�⣬����ʲô����ô����
			pParticles->x = std::max(0.0,std::min(x,Image.cols - 1.0));  

			
			y = A1 * (pParticles->y - pParticles->yOri) + A2 * (pParticles->yPre - pParticles->yOri)
				+ B0 * rng.gaussian(SIGMA_Y) + pParticles->yOri;
			pParticles->y = std::max(0.0,std::min(y,Image.rows - 1.0));

			s = A1 * (pParticles->scale - 1.0) + A2 * (pParticles->scalePre - 1.0)
				+ B0 * rng.gaussian(SIGMA_SCALE) + 1.0;
			pParticles->scale = std::max(0.1, std::min(s,3.0));

			//�����������ھ��εĲ���
			pParticles->rect.x = max(0,min(cvRound(pParticles->x - 0.5 * pParticles->rect.width * pParticles->scale),Image.cols-1));
			                                          //�����������룬����һ��������
			pParticles->rect.y = max(0, min(cvRound(pParticles->y - 0.5 * pParticles->rect.height * pParticles->scale), Image.rows - 1));
			
			pParticles->rect.width = min(cvRound(pParticles->rect.width * pParticles->scale),Image.cols - pParticles->rect.x);
			pParticles->rect.height = min(cvRound(pParticles->rect.height * pParticles->scale), Image.rows - pParticles->rect.y);


			//step 4:��ǰ�����Ӽ�����ֱ��ͼ
			Mat imgParticle(Image,pParticles->rect);
			calcHist(&imgParticle, 1, channels, Mat(),pParticles->hist,3,histSize,ranges);
			normalize(pParticles->hist,pParticles->hist);   //��һ��

			//step 5:�����ĶԱȣ�����particleȨ��
			pParticles->weight = compareHist(roiHist,pParticles->hist,CV_COMP_INTERSECT);
			                              //�������ƶȵķ�����CV_COMP_INTERSECT,���ֵ��СΪ0-1

			pParticles++;   //����Particle_num��
		}

		//step 6:��һ��Ȩ��
		double sum = 0.0;
		int i;
		
		//����ͣ�Ȼ���һ��
		pParticles = particles;
		for (i = 0; i < Particle_num; i++)
		{
			sum += pParticles->weight;
			pParticles++;
		}

		//pParticles = particles;  ����Ϊ����Ƕ���ģ���Ϊǰ���Ѿ���ֵ�ˣ�������ڼ�û�иı��
		for (i = 0; i < Particle_num; i++)
		{
			pParticles->weight /= sum;
			pParticles++;
		}


		//step 7:resamle�ز������������ӵ�Ȩ�صĺ�������ܶ��ز���
		pParticles = particles;
		PARTICLE newParticles[Particle_num];   //�µ����Ӽ�
		int np;    //��������ܶ�
		int k = 0;

		qsort(pParticles,Particle_num,sizeof(PARTICLE),&particle_cmp); //particle_cmpӦ���Ǹ��ص�����

		//���Ǻ����
		for (int i = 0; i < Particle_num; i++)
		{
			np = cvRound(particles[i].weight * Particle_num);     //����������ֵ��ֵԽ��˵��ȨֵԽ��
			for (int j = 0; j < np; j++)
			{
				newParticles[k++] = particles[i];
				if (k == Particle_num)
					goto EXITOUT;
			}
		}
		while (k<Particle_num)
		{
			newParticles[k++] = particles[0];
		}
	EXITOUT:
		for (int i = 0; i < Particle_num; i++)
		{
			particles[i] = newParticles[i];
		}
	}  //end else

	//�õ��µ����Ӽ�������ӵ�Ȩ����������
	qsort(pParticles,Particle_num,sizeof(PARTICLE),&particle_cmp); 

	//step 8:�������ӵ���������Ϊ���ٽ��
	Rect_<double> recTrackingTemp(0.0,0.0,0.0,0.0);
	pParticles = particles;
	for (int i = 0; i < Particle_num; i++)
	{
		recTrackingTemp.x += pParticles->rect.x * pParticles->weight;
		recTrackingTemp.y += pParticles->rect.y * pParticles->weight;
		recTrackingTemp.width += pParticles->rect.width * pParticles->weight;
		recTrackingTemp.height += pParticles->rect.height * pParticles->weight;

		pParticles++;
	}

	Rect recTracking(recTrackingTemp);  //���ٽ��

	//��ʾ�����ӵ��˶�
	for (int i = 0; i < Particle_num; i++)
	{
		rectangle(Image,particles[i].rect,Scalar(255,0,0));  //���öԽ��߶��㻭һ������
	}
	//��ʾ���ٽ��
	rectangle(Image, recTracking, Scalar(0,0,255),3);    //������β���������

	
}
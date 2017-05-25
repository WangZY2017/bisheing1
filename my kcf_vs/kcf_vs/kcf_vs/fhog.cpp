
#include "fhog.h"

//thread building blocks �̹߳���ģ�飬��������
#ifndef HAVE_TBB
#include <tbb/tbb.h>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

#endif 

#ifdef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//(2)http://blog.163.com/jiaqiang_wang/blog/static/1188961532013111164832236/
//(1)http://blog.csdn.net/langb2014/article/details/45674725
//(3)http://www.cnblogs.com/tornadomeet/archive/2012/08/15/2640754.html
//(4)���� http://blog.csdn.net/wutongxiyu2012/article/details/17299181
int getFeatureMaps(const IplImage* image, const int k, CvLSVMFeatureMapCaskade **map)
{
	int sizeX, sizeY;
	int p;
	int stringSize;     //X�᷽��bin����
	int height, width, numChannels;
	int i, j, kk, c, ii, jj, d;
	float *datadx,*datady;

	int ch;
	float magnitude, x, y, tx, ty;

	IplImage *dx, *dy;   //ˮƽ����X���ݶȷ���������ֱ����(Y)�ݶȷ�������
	int *nearest;
	float *w;

	/*
	float *r[] = {0};          //���ص���ݶȷ�ֵ
	int *alfa[] = {0};   //��ȡ������������ͨ��ID
	*/

	float *r;          //���ص���ݶȷ�ֵ
	int *alfa;

	float boundary_x[NUM_SECTOR + 1];  //9����������9+1���߽�
	float boundary_y[NUM_SECTOR + 1];
	float max, dotProd;
	int maxi;

	height = image->height;
	width = image->width;

	numChannels = image->nChannels;     //ͼ��ͨ����

	//cvCvtColor(img, dst, CV_BGR2GRAY);

	//���߷ֱ���RGBɫ�ʿռ��LABɫ�ʿռ��϶�ͼ����лҶȺ�٤���һ����
	//��ʵ������ʾ�������һ����Ԥ�����������Ľ��û��Ӱ�죬
	//ԭ������ǣ��ں���������Ҳ�й�һ���Ĺ��̣���Щ���̿���ȡ�����Ԥ����Ĺ� һ����
	//���ԣ���ʵ��Ӧ���У���һ������ʡ�ԡ�


	float kernel[3] = {-1.0f,0.0f,1.0f};   //�к��еľ������Ӧ�ò�һ����������һ����(�������Ǵ���ģ���ʵ��һ�����Ѿ���ת��180��)
	CvMat kernel_dx = cvMat(1,3,CV_32F,kernel);      //ˮƽ�ݶȾ�����ӣ�������
	CvMat kernel_dy = cvMat(3,1,CV_32F,kernel);   // ��ֱ�ݶȾ�����ӣ�������

	//û�н��лҶȻ�
	dx = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_32F,3);  //3ͨ������ͼ
	dy = cvCreateImage(CvSize(image->width, image->height), IPL_DEPTH_32F, 3);
	
	//��1��������ͼ��
	cvFilter2D(image,dx,&kernel_dx,cvPoint(-1,-1));
	cvFilter2D(image, dy, &kernel_dy, cvPoint(-1, -1));

	sizeX = width / k;     //ˮƽ�����ϵ�cell����������Ԫ�����Ŀ
	sizeY = height / k;    //��ֱ�����ϵ�cell����
	p = 3 * NUM_SECTOR;   //cell��Ԫ���ͨ������
	stringSize = sizeX * p;  //X�᷽��bin����
	allocFeatureMapObject(map,sizeX,sizeY,p);   //�����ڴ�ռ�

	float arg_vector;   //���� ����ֵ����Ϊ����Ҫ�����ڻ�
	for (i = 0; i < NUM_SECTOR; i++)
	{
		arg_vector = ((float)i) * (((float)PI) / (float)(NUM_SECTOR));
		boundary_x[i] = cosf(arg_vector);  //������ߣ�����Ϊ1����x����
		boundary_y[i] = sinf(arg_vector);	// ������ߣ�����Ϊ1����y����
	}

	r = (float*)malloc(sizeof(float) * (width * height));   //��ȡ�����ص��ݶȷ�ֵ
	alfa = (int*)malloc(sizeof(int) * (width * height * 2)); // ��ȡ������������ͨ��ID, 2 ��ʾ �޷��� + �з��� ���������

	for (j = 1; j < height - 1; j++)
	{
		// imageData��һ��ָ��ĳ�ڴ�ռ��ָ�룬���ڴ�ռ���������ͼ��ʵ�����ص����ɫ��Ϣ��������RGB��Ϣ)
		// widthstepͼ��һ�����ص���ռ���ֽ���
		datadx = (float*)(dx->imageData + dx->widthStep * j);   //
		datady = (float*)(dy->imageData + dy->widthStep * j);

		for (i = 1; i < width - 1; width++)
		{
			//��2�������ݶȣ�����Ƕ�ͨ��ͼ��ѡ���ݶȷ�ֵ����ͨ�����ݶ���Ϊ�����ص���ݶ�
			//http://blog.csdn.net/ttransposition/article/details/41805767
			//ȡRGB��ͨ����  �ݶȷ�ֵ������
			c = 0; // RGBͨ��ID
			x = (datadx[i * numChannels + c]);  // ���ص㴦ˮƽ�����ݶ�
			y = (datady[i * numChannels + c]);	// ���ص㴦��ֱ�����ݶ�

			r[j * width + i] = sqrtf(x * x + y * y);  // һ��ͨ�����ص��ݶȷ�ֵ
			for (ch = 1; ch < numChannels; ch++)
			{
				tx = (datadx[i * numChannels + ch]);
				ty = (datady[i * numChannels + ch]);
				magnitude = sqrtf(tx * tx + ty * ty);  // �Ƚϸ���ͨ�������ݶȷ�ֵ��ȡ���
				
				if (magnitude > r[j * width + i])
				{
					r[j * width + i] = magnitude;
					c = ch;
					x = tx;
					y = ty;
				}
			}

			//ͨ���Ƚ���ֱ���ڻ��Ĵ�С��ȷ�������ص��ݶ�����bin��λ��
			// <a,b> = ||a|| ||b|| cos(xita)���� xita ԽС��<a,b>��Խ��
			max = boundary_x[0] * x + boundary_y[0] * y;   //???????
			
			maxi = 0;
			for (kk = 0; kk < NUM_SECTOR; kk++)
			{
				dotProd = boundary_x[kk] * x + boundary_y[kk] * y;   //�ݶȷ���
				if (dotProd > max)
				{
					max = dotProd;
					maxi = kk;
				}
				else if (-dotProd > max)
				{
					max = -dotProd;
					maxi = kk + NUM_SECTOR;
				}
			}

			alfa[j * width * 2 + i * 2  ] = maxi % NUM_SECTOR;    //�޷����ݶȷ���ֱ��ͼ
			alfa[j * width * 2 + i * 2 + 1] = maxi;   // �з����ݶȷ���ֱ��ͼ
		}/*for(i = 0; i < width; i++)*/
	}/*for(j = 0; j < height; j++)*/

	//����ֻ��һ�����ص���ݶȷ�ֵ���ݶȷ�������Ҫͳ�ơ�һ��cell�������е����ص��ݶ�ֵ
	//�õ��ķ����� ��˫���Բ�ֵ��,�����ص�λ�ý������Լ�Ȩ
		
	nearest = (int*)malloc(sizeof(int) * k);    //k:cell�Ĵ�С
	w = (float*)malloc(sizeof(float) * (k*2));  //��Ҫ���������������ͶƱ��������������������Ӧ��bin����Ȩֵ

	for (i = 0; i < k / 2; i++)
	{
		nearest[i] = -1;
	}
	for (i = k / 2; i < k; i++)
	{
		nearest[i] = 1;
	}

	//http://blog.csdn.net/u011448029/article/details/11709443
	//http://blog.csdn.net/u011285477/article/details/50974230
	float temp;   //��cell�����ѡȡ��һ�����ص�
	//����temp�����cell���ĵ�ľ���
	for (j = 0; j < k / 2; j++)
	{
		temp = k / 2 + j + 0.5f;
		w[j * 2] = 1.0f * temp / (float)k;
		w[j * 2 + 1] = 1 - w[j * 2];
	}
	for (j = k / 2; j < k; j++)
	{
		temp = k - (j - k / 2 + 0.5f);
		w[j * 2] = 1.0f * temp / (float)k;
		w[j * 2 + 1] = 1 - w[j * 2];
	}/*for(j = k / 2; j < k; j++)*/

	for (i = 0; i < sizeY; i++)  //�� block�Ĵ�С
	{
		for (j = 0; j < sizeX; j++) //��	
		{
			for (ii = 0; ii < k; ii++)  //��
			{
				for (jj = 0; jj < k; jj++)  //��
				{
					// ���˵�ԭͼ�����������
					if ((i * k + ii > 0) && (i * k + ii < height - 1) && (j * k + jj > 0) && (j * k + jj < width - 1))
					{
						d = (k * i + ii) * width + (j * k + jj); // �����±� 

						// alfa[d * 2    ] �޷���bin��ID����
						// alfa[d * 2 + 1] �з���bin��ID����
						(*map)->map[i * stringSize + j * (*map)->numFeatures + alfa[d * 2]] += r[d] * w[ii * 2] * w[jj * 2];
						(*map)->map[i * stringSize + j * (*map)->numFeatures + alfa[d * 2 + 1] + NUM_SECTOR] += r[d] * w[ii * 2] * w[jj * 2];

						// nearest[0] = nearest[1] = -1, nearest[2] = nearest[3] = 1
						if ((i + nearest[ii] >= 0) && (i + nearest[ii] <= sizeY - 1))
						{
							(*map)->map[(i + nearest[ii]) * stringSize + j * (*map)->numFeatures + alfa[d * 2]] += r[d] * w[ii * 2 + 1] * w[jj * 2];
							(*map)->map[(i + nearest[ii]) * stringSize + j * (*map)->numFeatures + alfa[d * 2 + 1] + NUM_SECTOR] += r[d] * w[ii * 2 + 1] * w[jj * 2];
						}

						if ((j + nearest[jj] >= 0) && (j + nearest[jj] <= sizeX - 1))
						{
							(*map)->map[i * stringSize + (j + nearest[jj]) * (*map)->numFeatures + alfa[d * 2]] += r[d] * w[ii * 2] * w[jj * 2 + 1];
							(*map)->map[i * stringSize + (j + nearest[jj]) * (*map)->numFeatures + alfa[d * 2 + 1] + NUM_SECTOR] += r[d] * w[ii * 2] * w[jj * 2 + 1];
						}

						if ((i + nearest[ii] >= 0) && (i + nearest[ii] <= sizeY - 1) && (j + nearest[jj] >= 0) && (j + nearest[jj] <= sizeX - 1))
						{
							(*map)->map[(i + nearest[ii]) * stringSize + (j + nearest[jj]) * (*map)->numFeatures + alfa[d * 2]] += r[d] * w[ii * 2 + 1] * w[jj * 2 + 1];
							(*map)->map[(i + nearest[ii]) * stringSize + (j + nearest[jj]) * (*map)->numFeatures + alfa[d * 2 + 1] + NUM_SECTOR] += r[d] * w[ii * 2 + 1] * w[jj * 2 + 1];
						}
					}
				}/*for(jj = 0; jj < k; jj++)*/
			}/*for(ii = 0; ii < k; ii++)*/
		}/*for(j = 1; j < sizeX - 1; j++)*/
	}/*for(i = 1; i < sizeY - 1; i++)*/

	cvReleaseImage(&dx);
	cvReleaseImage(&dy);

	free(w);
	free(nearest);

	free(r);
	free(alfa);

	return LATENT_SVM_OK;

}


//�Ƚ���L2-norm���Խ�����нض̣�clipping��(��ֵ������Ϊv-0.2v ֮��)��Ȼ�������¹�һ��
int normalizeAndTruncate(CvLSVMFeatureMapCaskade *map, const float alfa)
{
	int i, j, ii;
	int sizeX, sizeY, p, pos, pp, xp, pos1, pos2;
	float *partOfNorm;   //��һ�����䣨i,j��
	float *newData; 
	float valOfNorm;     //��һ�����ֵ

	sizeX = map->sizeX;  // X�᷽��Cell�ĸ���
	sizeY = map->sizeY;	 // Y�᷽��Cell�ĸ���
	partOfNorm = (float*)malloc(sizeof(float) * (sizeX * sizeY) );  //Ϊblock�����ڴ�

	p = NUM_SECTOR; // �޷����ݶ�ֱ��ͼͨ����
	xp = NUM_SECTOR * 3;	// �޷����ݶ�ֱ��ͼͨ���� + �����ݶ�ֱ��ͼͨ���� 
	pp = NUM_SECTOR * 3 * 4;

	// ����ÿ����Ԫ��(cell)�������������޷����ݶ�ֱ��ͼ�����ڻ���2������ƽ����
	for (i = 0; i < sizeX * sizeY; i++)
	{
		valOfNorm = 0.0f;
		pos = i * (map->numFeatures);
		for (j = 0; j < p; j++)
			valOfNorm += map->map[pos + j] * map->map[pos + j];
		partOfNorm[i] = valOfNorm;
	}

	// ���˵�map->map���������
	sizeX -= 2;
	sizeY -= 2;

	newData = (float *)malloc(sizeof(float)* (sizeX * sizeY * pp)); // http://blog.csdn.net/ttransposition/article/details/41806601#t0  DMP HOG����

	//normalization ��һ��
	for (i = 1; i <= sizeY; i++)  //��
	{
		for (j = 1; j <= sizeX; j++)  //��
		{
			pos1 = (i)* (sizeX + 2) * xp + (j)* xp;
			pos2 = (i - 1) * (sizeX)* pp + (j - 1) * pp;
			
			// step 1 http://pan.baidu.com/s/1i5nSpQt
			valOfNorm = sqrtf(
				partOfNorm[(i)*(sizeX + 2) + (j)] +
				partOfNorm[(i)*(sizeX + 2) + (j + 1)] +
				partOfNorm[(i + 1)*(sizeX + 2) + (j)] +
				partOfNorm[(i + 1)*(sizeX + 2) + (j + 1)]) + FLT_EPSILON;
			for (ii = 0; ii < p; ii++)
				newData[pos2 + ii] = map->map[pos1 + ii] / valOfNorm;
			for (ii = 0; ii < 2 * p; ii++)
				newData[pos2 + ii + p * 4] = map->map[pos1 + ii + p] / valOfNorm;
			
			// step 2
			valOfNorm = sqrtf(
				partOfNorm[(i)*(sizeX + 2) + (j)] +
				partOfNorm[(i)*(sizeX + 2) + (j + 1)] +
				partOfNorm[(i - 1)*(sizeX + 2) + (j)] +
				partOfNorm[(i - 1)*(sizeX + 2) + (j + 1)]) + FLT_EPSILON;
			for (ii = 0; ii < p; ii++)
				newData[pos2 + ii + p] = map->map[pos1 + ii] / valOfNorm;
			for (ii = 0; ii < 2 * p; ii++)
				newData[pos2 + ii + p * 6] = map->map[pos1 + ii + p] / valOfNorm;
			
			// step 3
			valOfNorm = sqrtf(
				partOfNorm[(i)*(sizeX + 2) + (j)] +
				partOfNorm[(i)*(sizeX + 2) + (j - 1)] +
				partOfNorm[(i + 1)*(sizeX + 2) + (j)] +
				partOfNorm[(i + 1)*(sizeX + 2) + (j - 1)]) + FLT_EPSILON;
			for (ii = 0; ii < p; ii++)
				newData[pos2 + ii + p * 2] = map->map[pos1 + ii] / valOfNorm;
			for (ii = 0; ii < 2 * p; ii++)
				newData[pos2 + ii + p * 8] = map->map[pos1 + ii + p] / valOfNorm;
			
			// step 4
			valOfNorm = sqrtf(
				partOfNorm[(i)*(sizeX + 2) + (j)] +
				partOfNorm[(i)*(sizeX + 2) + (j - 1)] +
				partOfNorm[(i - 1)*(sizeX + 2) + (j)] +
				partOfNorm[(i - 1)*(sizeX + 2) + (j - 1)]) + FLT_EPSILON;
			for (ii = 0; ii < p; ii++)
				newData[pos2 + ii + p * 3] = map->map[pos1 + ii] / valOfNorm;
			for (ii = 0; ii < 2 * p; ii++)
				newData[pos2 + ii + p * 10] = map->map[pos1 + ii + p] / valOfNorm;
		}
	}

	//truncation ʹ�� alfa = 0.2 ���нض�
	for (i = 0; i < sizeX * sizeY * pp; i++)
	{
		if (newData[i] > alfa)
			newData[i] = alfa;
	}

	//change data
	map->numFeatures = pp;
	map->sizeX = sizeX;
	map->sizeY = sizeY;

	free(map->map);
	free(partOfNorm);

	map->map = newData;

	return LATENT_SVM_OK;
}



int PCAFeatureMaps(CvLSVMFeatureMapCaskade *map)
{
	int i, j, ii, jj, k;
	int sizeX, sizeY, p, pp, pos1, pos2; //xp, yp, 
	float *newData;
	float val;
	//float nx, ny;

	sizeX = map->sizeX;	//ˮƽ����
	sizeY = map->sizeY;	//��ֱ����
	p = map->numFeatures;
	pp = NUM_SECTOR * 3 + 4; // 31ά�������� http://blog.csdn.net/ttransposition/article/details/41806601#t0

	newData = (float *)malloc(sizeof(float)* (sizeX * sizeY * pp));

	for (i = 0; i < sizeY; i++)	// ��
	for (j = 0; j < sizeX; j++)	// ��
	{
		pos1 = ((i)*sizeX + j)*p;  // p: 9*3*4 = 108 ά
		pos2 = ((i)*sizeX + j)*pp; // pp: 9*3+4 = 31 ά
		k = 0;
		for (jj = 0; jj < NUM_SECTOR * 2; jj++)  // �з����ݶȷ���
		{
			val = 0;
			for (ii = 0; ii < 4; ii++)  // yp = 4
				val += map->map[pos1 + 4 * NUM_SECTOR + ii * NUM_SECTOR * 2 + jj]; //val += map->map[pos1 + yp * xp + ii * xp * 2 + jj];
			newData[pos2 + k] = val * 1.0f / sqrtf(4.0f);
			k++;
		}

		for (jj = 0; jj < NUM_SECTOR; jj++) // �޷����ݶȷ���
		{
			val = 0;
			for (ii = 0; ii < 4; ii++)
				val += map->map[pos1 + ii * NUM_SECTOR + jj];
			newData[pos2 + k] = val * 1.0f / sqrtf(4.0f);
			k++;
		}

		for (ii = 0; ii < 4; ii++)  // �޷����ݶȷ���
		{
			val = 0;
			//for (jj = 0; jj < 2 * NUM_SECTOR; jj++)  // original 
			//	val += map->map[pos1 + 4 * NUM_SECTOR + ii * NUM_SECTOR * 2 + jj];
			//newData[pos2 + k] = val * nx;
			for (jj = 0; jj < NUM_SECTOR; jj++)
				val += map->map[pos1 + ii * NUM_SECTOR + jj];
			newData[pos2 + k] = val * 1.0f / sqrtf((float)NUM_SECTOR);

			k++;
		}
	}

	// renew the data
	map->numFeatures = pp;

	free(map->map);

	map->map = newData;

	return LATENT_SVM_OK;

}


int allocFeatureMapObject(CvLSVMFeatureMapCaskade **obj, const int sizeX, const int sizeY, const int numFeatures)
{
	int i;
	(*obj) = (CvLSVMFeatureMapCaskade *)malloc(sizeof(CvLSVMFeatureMapCaskade));
	(*obj)->sizeX = sizeX;
	(*obj)->sizeY = sizeY;
	(*obj)->numFeatures = numFeatures;
	(*obj)->map = (float *)malloc(sizeof (float)* (sizeX * sizeY  * numFeatures));
	for (i = 0; i < sizeX * sizeY * numFeatures; i++)
		(*obj)->map[i] = 0.0f;
	return LATENT_SVM_OK;
}

int freeFeatureMapObject(CvLSVMFeatureMapCaskade **obj)
{
	if (*obj == NULL) return LATENT_SVM_MEM_NULL;
	free((*obj)->map);
	free(*obj);
	(*obj) = NULL;
	return LATENT_SVM_OK;   //LATENT_SVM_OK = 0
}

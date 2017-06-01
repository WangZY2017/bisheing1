%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all;clear all;clc;
%����ͼƬ�ļ�
imgDir='.\BlurCar3\';%ͼƬ�ļ���·����
[guroundtruth,img_path,img_files]=Load_image(imgDir);%���ú�����ȡͼƬ֡
% for frame=1:1%length(img_files)-1
    im1 = imread([img_path img_files{5}]);%��ȡĿ��֡
    im1 = rgb2gray(im1);%ת��Ϊ�Ҷ�ͼ
    im2 = imread([img_path img_files{6}]);%��ȡĿ��֡
    im2 = rgb2gray(im2);%ת��Ϊ�Ҷ�ͼ
    d_im1=double(im1);d_im2=double(im2);
    res=imfilter(d_im1, d_im2, 'corr');   
    % res=fftshift(res); 
    max1=max(res); 
    max2=max(max1); 
    scale=1.0/max2; 
    res=res.*scale; 
%     res=255.*uint8(res);
    %��ͼ
    subplot(1,3,1)
    imshow(im1);title('imag1');
    subplot(1,3,2)
    imshow(im2);title('imag2');
    subplot(1,3,3)
    imshow(res);title('filter');
    drawnow
% end
[maxrow,maxcol]=find(res==max(max(res)));


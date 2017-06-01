%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%function is correlation filter
%Visual Object Tracking using Adaptive Correlation Filters
%MOSSE
%date:2016-11-10
%author:WeiQin
%E-mail:285980893@qq.com
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all;clear all;clc;
%% ������Ƶ�ļ�
% videoDir = 'D:\ImageData\David';
% [videoData,ground_truth] = Load_video(videoDir);%���ú�����ȡ��Ƶ����ȡgroundtruth����
% img = read(videoData,1);% ��ȡ��һ֡
%% ����ͼƬ�ļ�
imgDir='D:\ImageData\Coke';%ͼƬ�ļ���·����
[ground_truth,img_path,img_files]=Load_image(imgDir);%���ú�����ȡͼƬ֡����ȡgroundtruth����
img = imread([img_path img_files{1}]);%��ȡĿ��֡
%% ��ʼ֡����ֹ֡
startFrame=1;%��ʼ֡
endFrame=length(img_files);%����ͼƬ�ļ�Load_imageʱ����ֹ֡
% endFrame=videoData.NumberOfFrames;%������Ƶ�ļ�Load_vedioʱ����ֹ֡
%% ת��Ϊ�Ҷ�ͼ��
if(size(img,3)==1) %�Ҷ�ͼ��
    im=img;
else
    im = rgb2gray(img);%ת��Ϊ�Ҷ�ͼ
end
%% ��ȡĿ��λ�úͿ��С
%% set initial position and size
target_sz = [ground_truth(1,4), ground_truth(1,3)];
pos = [ground_truth(1,2), ground_truth(1,1)] + floor(target_sz/2);
time = 0;  %to calculate FPS
positions = zeros(numel(img_files), 2);  %to calculate precision
%������˹����ģ��
F_response=templateGauss(target_sz,im);%��˹����ģ��
%% ��ѭ����ȡȫ��ͼ��֡
for frame=startFrame:endFrame
       %% trainingѵ�����ģ��
        img = imread([img_path img_files{frame}]);%��ȡĿ��֡(����ͼƬ�ļ�Load_imageʱ)
%         img = read(videoData,frame);% ��ȡĿ��֡(������Ƶ�ļ�Load_vedioʱ)
       %% ת��Ϊ�Ҷ�ͼ��
        if(size(img,3)==1) %�Ҷ�ͼ��
            im=img;
        else
            im = rgb2gray(img);%ת��Ϊ�Ҷ�ͼ
        end
        target_box=getsubbox(pos,target_sz,im);%��ȡĿ���s
        tic()
       %% ѵ��������ʼ���ٲ�����ģ��
    if frame>startFrame
        newPoint=real(ifft2(F_Template.*fft2(target_box)));%���з��任
%         newPoint=uint8(newPoint);
%         subplot(1,2,1)
%         imshow(256.*newPoint);
        [row, col,~] = find(newPoint == max(newPoint(:)), 1);
        pos = pos - target_sz/2 + [row, col]; 
    end
    %%
    %save position and calculate FPS
	positions(frame,:) = pos;
	time = time + toc();
    F_im= fft2(getsubbox(pos,target_sz,im));
%     F_Template=F_response./(F_im+eps);%CFģ�����   
    F_Template=conj(F_im.*conj(F_response)./(F_im.*conj(F_im)+eps));%mosseģ�����      
        %% ��ͼ
%         subplot(1,2,2)
        rect_position = [pos([2,1]) - (target_sz([2,1])/2), (target_sz([2,1]))]; 
    if frame == 1,  %first frame, create GUI
            figure
            im_handle = imagesc(uint8(img));
            rect_handle = rectangle('Position',rect_position,'LineWidth',2,'EdgeColor','r');
            tex_handle = text(5, 18, strcat('#',num2str(frame)), 'Color','y', 'FontWeight','bold', 'FontSize',20);
            drawnow;
    else
        try  %subsequent frames, update GUI
			set(im_handle, 'CData', img)
			set(rect_handle, 'Position', rect_position)
            set(tex_handle, 'string', strcat('#',num2str(frame)))
%             pause(0.001);
            drawnow;
		catch  % #ok, user has closed the window
			return
        end
    end
%         imagesc(uint8(img))
%         colormap(gray)
%         rect_position = [pos([2,1]) - (target_sz([2,1])/2), (target_sz([2,1]))]; 
%         rectangle('Position',rect_position,'LineWidth',4,'EdgeColor','r');
%         hold on;
%         text(5, 18, strcat('#',num2str(frame)), 'Color','y', 'FontWeight','bold', 'FontSize',20);
% %         set(gca,'position',[0 0 1 1]); 
%         pause(0.001); 
%         hold off;
%         drawnow;
end
disp(['Frames-per-second: ' num2str(numel(img_files) / time)])

%show the precisions plot
show_precision(positions, ground_truth, imgDir)


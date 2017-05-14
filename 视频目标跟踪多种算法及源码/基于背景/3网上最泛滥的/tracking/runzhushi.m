clear data                                       % clear keyword :clears the items indicated by keyword.
                                                 % ����ɹؼ��� keyword ����ʾ����Ŀ.
disp('input video');                             % ��ʾ˵���ַ���:input video.                            


avi = aviread('zipai.avi');                      % mov = aviread(filename) reads the AVI movie filename into the MATLAB movie structure mov.                      
                                                 % ��ȡһ��AVI����Ƶ�ļ���MATLAB��Ƶ������. 
                                                 % ����avi��Ƶ�ļ���Ϣ����Ϊ����avi.
                                                 
video = {avi.cdata};                             % cdata �ĺ������ҳ:http://www.baisi.net/thread-6612-1-1.html
for a = 1:length(video)                        % ��֡��������    
    imagesc(video{a});                           % imagesc(C) displays C as an image. Each element of C corresponds to a rectangular area in the image.
                                                 % The values of the elements of C are indices into the current colormap that determine the color of each patch. 
                                                 % imagesc(c):������C��ʾΪͼ���ʽ.C
                                                 % ��ÿһ��Ԫ�ض�Ӧ��ͼ���е�һ����������.C��Ԫ��
                                                 % ����д�뵱ǰ����ÿ��������ɫ��α��ɫͼ��.
                                             
                                                 % colormap--α��ɫͼ����ʾǿ����ƽ��ķֲ������Ƶ�ֵ��ͼ�����ͼ�ϵĵ�����ͼ�ȣ���������Ӧ��ɫ�ʱ��

    axis image off                               % axis off :turns off all axis lines,tick marks, and labels.
                                                 % �ر����е������ᡢ��ǩ�Ͷ�άͼ���ǩ.
    drawnow;                                     % drawnow: flushes the event queue and updates the figure window. ��������е��¼����к͸������ִ���.
end;                                             % ͼ��֡��ȡ���.            
disp('output video');                            % ��ʾ�ַ���"output video"
tracking(video);                                 % ���ú���tracking.

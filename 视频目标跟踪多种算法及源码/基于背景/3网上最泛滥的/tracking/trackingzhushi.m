                 %%%%%%%%%%   Directions:  % ��ʾ�Գ�������ע��; %%   ��ʾ�Գ���ע������ע�͵�ע��. %%%%%%%%%%
                                        

function d = tracking(video)                        % tracking��������.
if ischar(video)                                    % Determine whether item is character array��tf = ischar(A) returns logical 1 (true).
                                                    % if A is a character array and logical 0 (false) otherwise.
                                                    % �ж��ж�video�Ƿ�Ϊһ���ַ����顣�Ƿ���1�����򷵻�0.
                                                    
                                                    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  ��ȡ������Ƶ��Ϣ  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    % Load the video from an avi file.
    %%��һ��avi�ļ��ж�ȡ��Ƶ�ļ�,��ȡ�����Ƶ�ļ���Ϣ.
    
    avi = aviread(video);                           % mov = aviread(filename)  reads the AVI movie filename into the MATLAB movie structure mov��
                                                    % �ú�����avi�е���Ƶ��Ϣ���뵽MATLAB��Ӱ�����С�
    pixels = double(cat(4,avi(1:2:end).cdata))/255; % double(x) returns the double-precision value for X. If X is already a double-precision
                                                    % array, double has no effect.���X��һ����˫������ֵ��ú�������һ��˫����ֵ������ú������κ�����.
                                                    % cat��Concatenate arrays along specified dimension�ؾ���ָ��ά��avi��Ƶ���ݷֿ�Ϊ��4������Ԫ����ɵľ���
                                                    
                                                    
                                                    
    clear avi                                       % clear name :removes just the M-file or MEX-file function or variable name from the workspace.
                                                    % ��������ռ仺�档

else
    
    % Compile the pixel data into a single array
    %% ���������ݱ����Ϊһ��������4ά����
    
    pixels = double(cat(4,video{1:2:end}))/255;     % ����ֵpixels����Ϊ����
    clear video                                     % clear keyword: clears the items indicated by keyword.���ؼ���video�������Ŀ
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   ͼ��ת��   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Convert to RGB to GRAY SCALE image.
% % ��RGBת��Ϊ�Ҷ�ͼ��

nFrames = size(pixels,4);                           % m = size(X,dim) :returns the size of the dimension of X specified by scalar dim.
                                                    % �±�dimָʾpixdls�����ά��ֵdim��С������dim��ֵ��nFrames�� 
                                                    
for f = 1:nFrames                                   % ��Ƶͼ��֡ѭ������f

%     F = getframe(gcf);                            %%  F = getframe(gcf):gets a frame from the figure or axes identified by handle h.
                                                    %% ����hȷ����ͼƬ�������߻��һ֡��
%     [x,map]=frame2im(F);                          %% [X,Map] = frame2im(F) :converts the single movie frame F into the indexed image X and
                                                    %% associated colormap Map. The functions getframe and im2frame create a movie frame. 
                                                    %% If the frame contains true-color data, then Map is empty.
                                                    %% frame2im����������Ƶ֡ת��������ͼ����������õ�ͼ��֡����im2frame���������һ����Ƶ֡��
                                                    %% ���ͼ�����������ݣ���ô��Map���ǿյ��ˡ�
%     imwrite(x,'fln.jpg','jpg');                   %% ���õ���ͼ��д��Ӳ�̡�
% end                                               %% ����
    pixel(:,:,f) = (rgb2gray(pixels(:,:,:,f)));     % I = rgb2gray(RGB) converts the truecolor image RGB to the grayscale intensity image I.
                                                    % rgb2gray���������ͼƬRGBת��Ϊ�Ҷ�ͼ��I
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    �궨ͼ��    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rows=240;                                           % ����궨���ĵ��к�Ϊ240
cols=320;                                           % ����궨���ĵ��к�Ϊ320
nrames=f;                                           % ����nramesΪf

for l = 2:nrames                                    % ��ͼ����ѭ������l���ӵڶ���ͼ��ʼ����.
                                                    %Ϊͼ��궨�ĵ�һ��ѭ��.
d(:,:,l)=(abs(pixel(:,:,l)-pixel(:,:,l-1)));        % ͼ����������ֵ֮��ȡ����ֵ��������ת��Ϊ����d(:,:,l).��������

k=d(:,:,l);                                         % �������kΪ���ز�ֵ����,���ڳ����д.
% imagesc(k);                                       %% imagesc:Scale data and display image object �궨���ݲ�����ʾͼ�����
% drawnow;                                          %% ����¼����к͸������ִ���
% himage = imshow('d(:,:,l)');jii                   %% ��ʾͼ�񴢴���ͼ���ļ����ļ���
% hfigure = figure;                                 %% ��������ͼ�ζ���
% impixelregionpanel(hfigure, himage);              %% hpanel = impixelregionpanel(hparent,himage) creates a Pixel Region tool  panel associated with the image specified by the handle himage,
                                                    %% called the target image. 
                                                    %% �����������򹤾������ͼ����ָ����ν��Ŀ��ͼ��

% datar=imageinfo(imagesc(d(:,:,l)));               %% imagesc:Scale data and display image object �궨���ݲ�����ʾͼ�����
                                                    %% imageinfo: creates an Image Information tool associated with the image in the current
                                                    %% figure.
                                                    %% �ڵ�ǰ�Ĵ����д���һ��ͼ����Ϣ����.
% disp(datar);                                      %% displays an array, without printing the array name. 
                                                    %% ��ʾһ������ͬʱȱʡ��������.


   bw(:,:,l) = im2bw(k, .2);                        % �Ҷ�ͼ��ת��Ϊ��ֵͼ��,��Ϊ����bw(:,:,l)��
   
   bw1=bwlabel(bw(:,:,l));                          % L = bwlabel(BW,n) returns a matrix L,of the same size as BW, containing labels for the connected objects in BW. 
                                                    % n can have a value of either 4 or 8, where 4 specifies 4-connected objects and 8 specifies 8-connected objects;
                                                    % if the argument is omitted, it defaults to 8.
                                                    % ����һ����BW��ͬά���ľ���L�������BW�����Ӷ���ı�ǩ�� 
                                                    % n������һ��4��8
                                                    % ������ָ��4����ض����8�����;�������ȱʡ����Ĭ��ֵΪ8 
                                                    % ������������������̫����˲������á�
   imshow(bw(:,:,l))                                % displays the binary image BW. imshow displays pixels with the value 0 (zero) as black and pixels with
                                                    % the value 1 as white.
                                                    % ��ʾ��ֵͼ��BW��
                                                    % imshow��ʾ��0ֵ����Ϊ��ɫ��1ֵ����Ϊ��ɫ.
   hold on                                          % hold on :retains the current plot and certain axes properties so that subsequent graphing commands add to the
                                                    % existing graph.
                                                    % ����Ŀǰ�Ľ��̺�ĳЩ�����ԣ�ʹ����Ļ�ͼ����Ӧ�õ����е�ͼ�ε���.
   
% for h=1:rows                                      %% h--��ѭ������
%     for w=1:cols                                  %% w--��ѭ������
%                         
%             if(d(:,:,l)< 0.1)                     %% �ж�ͼ����������ֵ֮��ȡ����ֵ�Ƿ�С��0.1
%                  d(h,w,l)=0;                      %% ��,���Ӧ������ֵ��0.���򱣳�.
                                                    %% �궨�㷨.
%             end
%      end
%   
% end
   
% %  disp(d(:,:,l));                                %% ��ʾ��ֵ��ʾ
% %  size(d(:,:,l))                                 %% ��ʾ��ֵ��С
cou=1;                                              % �������cou=1.
for h=1:rows                                        % ��ѭ����ʼֵ(����Ϊȱʡֵ1)
    for w=1:cols                                    % ��ѭ����ʼֵ(����Ϊȱʡֵ1)      %% �˴���ѭ��Ϊ��ѭ��,��ѭ��Ϊ��ѭ��.
     if(bw(h,w,l)>0.5)                              % ���һ������ֵbw(h,w,l)����0.5
        
        
%          disp(d(h,w,l))��                         %% ��ʾ�任���ͼƬ����֡
      toplen = h;                                   % ������ֵh,��ֵ��toplen.
        if (cou == 1)                               % �����Ӧ����ֵΪ1.
            tpln=toplen;                            % ��ת��toplenΪtpln.
           
         end
         cou=cou+1;                                 % �ж���һ��                              
        break                                       % ִ����ֹ.(����whileѭ������)
     end                                            % if�ж���� 
    end                                             % ��ѭ����� 
end                                                 % ��ѭ�����                                              

disp(toplen);                                       % ��ʾ��������ֵ

% % ����ѭ���ṹ������ͬ,ֻ�Ƕ�����ֵ����0.5�Ĵ���.
coun=1;                                             % ���峣ֵ����coun,Ϊ����ֱ���жϽ�����һ����������.               
for w=1:cols                                        % ��ѭ������h
    for h=1:rows                                    % ��ѭ������h
     if(bw(h,w,l)>0.5)                              %���һ������ֵbw(h,w,l)����0.5
        
      leftsi = w;                                   %ת����ֵw
      
    
   if (coun == 1)                                   % ��Ϊͬһ�����򲻽��в���.(��Ϊ)
            lftln=leftsi;                           % ��ת����ֵ
            coun=coun+1;                            % ͬʱ����coun�Լ�1,������ѭ��.
   end                                              % if�жϽ���
      break                                         % ����
     end                                            % һ�����ж����
     
    end                                             % һ���ж���� 
end                                                 % һ���ж����

disp(leftsi);                                       %disp(X) displays an array, without printing the array name. If X contains a text string, the string is displayed.
                                                    % ��ʾһ��û�б�ʾ���Ƶ����顣�������X����һ���ı��ַ���������ʾ���ַ�����
                                                    % ��������ʾ�������ʼ��ֵ
disp(lftln);                                        % ��ʾ���������ֵ


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  ��ʼ��ʾͼ��  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    

 % % drawnow;                                       %% ͼ����ʾ��ʼ                           
% %    d = abs(pixel(:, :, l), pixel(:, :, l-1));   % abs(X) returns an array Y such that each element of Y is the absolute value of the corresponding
                                                    % element of X.
                                                    %% ���ض�Ӧ��ÿ��Ԫ��x�ľ���ֵ��yֵ��
                                                    %% ���ｫ���������ز�ֵ����Ϊ����d
% %    disp(d);                                     %% ��ʾ����d      
   
%    s = regionprops(bw1, 'BoundingBox');           %% STATS = regionprops(L, properties) measures a set of properties for each labeled region in the label matrix L.
                                                    %% ������ֻ��ģ��������ͼ�����������.�����������ݱ���Ϊ��������
                                                    %% �˴����ⶨbwl������޶�������,�����䱣��Ϊ����s
% %    centroids = cat(1, s.Centroid);              %% ��bwl��������ĵ����־���ת��Ϊ��һ�ľ���centroids
% 
% %    ang=s.Orientation;                           %% Orientation��ʾ:Convert viewpoint direction to VRML orientation���Ƕ�ת��VRML�ķ���
                                                    %% ��Ϊ��ת������������ݱ���Ϊ����ang
%    
% %    plot(centroids(:,1), centroids(:,2), 'r*')   %% ʹ��ͼѡ�����ռ��������ѡ���ı�����ͼ¼��

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   ��Ϊ���ɶ�����Ӿ��ε�ע��   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%    for r = 1 : length(s)                                     %% ����ͼ�������ѭ��
%    rectangle('Position',s(r).BoundingBox,'EdgeColor','r');   %% rectangle draws a rectangle with Position [0,0,1,1] and Curvature [0,0] (i.e., no curvature).
                                                               %% rectangle������[0,0,1,1]�������ο�������Ϊ0. 
                                                               
% %   plot('position',s(r).BoundingBox,'faceregion','r');      %% plot(...,'PropertyName',PropertyValue,...)  
                                                               %% sets properties to the specified property values for all lineseries graphics objects created by plot.
                                                               %% ����ͼ���߻���������ͼ��ĸ��������趨ֵ��
%    end                                                       %% �趨��������
%    

% %    disp(ang);                                              %% ��ʾͼ��������������ݾ���                                            
%  %  imaqmontage(k);                                          %% �������kΪ���ز�ֵ����  
                                                               %% iimaqmontage(frames) displays a montage of image frames in a MATLAB figure window using the imagesc 
                                                               %% function.frames can be any data set returned by getdata, peekdata,or getsnapshot.
                                                               %% �ú�������imagesc����������figure������ʾͼ��֡��ϸ��.frames����ʹ����getdata, peekdata,�� getsnapshot���ص����ݼ���

                                                               
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   ��ʾ���ͼ���С����  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

widh=leftsi-lftln;                                             % �궨����
heig=toplen-tpln;                                              % ��ֵ��Ϣ

widt=widh/2;                                                   % ȷ���궨���ĵĺᣨ�У�����.�궨����λ�ڱ궨���ο�����λ��.
disp(widt);                                                    % ��ʾ�궨���ĵ�������.
heit=heig/2;                                                   % ȷ���궨���ĵĺᣨ�У�����.
with=lftln+widt;                                               % ��ֵwith=leftsi
heth=tpln+heit;                                                % ��ֵheth=toplen
wth(l)=with;                                                   % ��ֵwth(1)=with
hth(l)=heth;                                                   % ��ֵhth(1)=heth

disp(heit);                                                    % ��ʾheit
disp(widh);                                                    % ��ʾwidh
disp(heig);                                                    % ��ʾheig
rectangle('Position',[lftln tpln widh heig],'EdgeColor','r');  % rectangle(...,'Curvature',[x,y]) specifies the curvature  of the rectangle sides,
                                                               % enabling it to vary from a rectangle to an ellipse. The horizontal curvature x is the fraction of width of
                                                               % the rectangle that is curved along the top and bottom edges. The vertical curvature y is the fraction of 
                                                               % the height of the rectangle that is curved along the left and right edges.
                                                               % ����(...,'����' �� [��x �� y ] ����ͨ��ָ�������˫�����ʣ�����ʹ�궨������ɾ��ε���Բ�α仯.��������x�Ǿ�������
                                                               % �����͵ײ������ľ��ο����ֵ.��ֱ����Y���������ұ߷��������ľ��θ߶���ֵ.
disp(with);                                                    % ��ʾ���
disp(heth);                                                    % ��ʾ�߶�

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    ������Ӿ���   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
plot(with,heth, 'r*');                                         % plots all lines defined by Xn versus Yn pairs.������Xn��Ynȷ������.
drawnow;                                                       % drawnow: flushes the event queue and updates the figure window. ��������е��¼����к͸������ִ���.
hold off                                                       % hold off ��resets axes properties to their defaults before drawing new plots. hold off is the default.
                                                               % hold off ���ڻ��µ�ͼ��ǰ������������ΪĬ��ֵ.hold off��Ϊȱʡֵ.

end;                                                           % ��֡ͼ��궨�������.
% wh=square(abs(wth(2)-wth(nrames)));
% ht=square(abs(hth(2)-hth(nrames)));
% disp(wth(1
% distan=sqrt(wh+ht);
% 
% disp(distan);



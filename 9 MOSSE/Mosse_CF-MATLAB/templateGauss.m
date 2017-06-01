%%%%%%%%%%%%%%%%%%%%������˹������Ӧ%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function F_response=templateGauss(sz,im)
    [rs, cs] = ndgrid((1:sz(1)) - floor(sz(1)/2), (1:sz(2)) - floor(sz(2)/2));
    dist = rs.^2 + cs.^2;
    conf = exp(-0.5 / (2.25) * sqrt(dist));%���ɶ�ά��˹�ֲ�
    conf = conf/sum(sum(conf));% normalization
    if(size(im,3)==1)%�Ҷ�ͼ��
        response=conf;
    else
        response(:,:,1)=conf;response(:,:,2)=conf;response(:,:,3)=conf;    
    end       
%         figure
%         imshow(256.*response);
%         mesh(response);
        F_response=fft2(response);%����Ҷ�任
end
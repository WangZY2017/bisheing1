%========PF=======
tic;
clc;clear;close all;
t=50;%length of time
N = 50;%particle
x=zeros(1,t);
PF_x=zeros(1,N);
p_xpat=PF_x;
x(1)=.6;%��ʼֵ
pred_x=2; %prediction
pred_xx=zeros(1,t);
pred_xx(1)=pred_x;
P=1;R=.002;Q=.002;
for i=1:N
    PF_x(i)=pred_x+sqrt(P)*randn;
end
for k=2:t
    x(k) = sin(x(k-1))+sqrt(Q)*randn;%state worthy
    y = x(k)^2+x(k)+sqrt(R)*randn;%measure
    % filter process
    for i=1:N
    p_xpat(i)=sin(PF_x(i))+sqrt(Q)*randn;
    Pred_y = p_xpat(i)^2+p_xpat(i)+sqrt(R)*randn;
    err_or=y-Pred_y;
    q(i) = 1/(sqrt(R*2*pi))*exp(-err_or^2/(2*R));%update weight
    end
     pf_sum=sum(q);
    q = q./pf_sum;%��һ��
%% =====resample
     for i=1:N
         PF_x(i)= p_xpat(find(rand<=cumsum(q),1));%��q=[a b c d],then cumsum(q)=[a a+b a+b+c a+b+c+d],find( ,1) �������������ĵ�һ��ֵ������ 
     end
%%
%��Ϊ�ز���֮��Ȩ�ض���1/N���������ӵļ�Ȩ�;�����ƽ��
   pred_xx(k)=mean(PF_x);
%% =================
end
Err = pred_xx-x;
figure('color','white');
subplot(211);
plot(x,'r+');
hold on;
plot(pred_xx,'y*');
legend('��ʵ����','�����˲�����ֵ','location','north');
xlabel('�۲����');
ylabel('�۲�ֵ');grid on;
subplot(212);
plot(Err,'b-','linewidth',2);
legend('���');title('�������');grid on;
time= toc;
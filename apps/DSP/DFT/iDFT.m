function X=iDFT(x)
N=length(x);%�����������еĳ���
 
for k=1:N
    for j=1:N    %��j��������
        WN(k,j)=exp(1i*2*pi*(k-1)*(j-1)/N);%����k�е�һϵ��Wnֵ
        X_reg(k,j)=WN(k,j)*x(j);%�������������
    end
    X(k)=sum(X_reg(k,:))/N;%��������˽�����
end

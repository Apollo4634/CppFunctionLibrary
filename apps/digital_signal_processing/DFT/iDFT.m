function X=iDFT(x)
N=length(x);%计算输入序列的长度
 
for k=1:N
    for j=1:N    %对j列做处理
        WN(k,j)=exp(1i*2*pi*(k-1)*(j-1)/N);%产生k行的一系列Wn值
        X_reg(k,j)=WN(k,j)*x(j);%与输入序列相乘
    end
    X(k)=sum(X_reg(k,:))/N;%对上述相乘结果求和
end

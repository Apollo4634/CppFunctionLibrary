/*-------------------------------------------------------
  程序：自动补零的按时间抽取的库利图基FFT算法

  功能：自动将序列长度不是2的次幂的序列在序列末尾添加0补偿为2的次幂的长度，
		然后使用按时间抽取（DIT）的库利图基FFT算法计算DFT变换结果

  相关：数字信号处理、二进制

  时间：2014-12-05

  修改：2014-12-27 添加程序模块分析等
-------------------------------------------------------
  程序模块：
  1、定义复数的结构体变量，声明子函数
  2、主函数：
	 2.1数据输入
	 2.2改变长度，使之对齐2的次幂，不足的补零
	 2.3初始化变换因子
	 2.4变址,倒位序
	 2.5蝶形运算
	 2.6输出结果
  3、定义子函数：
	 2.1数据输入
	 2.2补零
	 2.3初始化变换因子
	 2.4变址,倒位序
	 2.5蝶形运算
	 2.6输出结果
	 2.7复数运算、复数乘法、复数减法
  4、程序结束
--------------------------------------------------------
  主函数中的变量以及全局变量、常量命名：
  1、变量：
	 (1)size_x=0  //输入序列的大小，初始化为0;
	 (2)Len_x  //补零后的序列的长度;
	 (3)pow2_Len  //补零后的序列长度以2为底的对数;
	 (4)complex，real，img;//复数类型名，复数的实部和虚部；
  2、常量：
	 (1)N //此为宏定义，表明输入序列补零后的最大长度
--------------------------------------------------------
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define N 1024

/*定义复数类型*/
typedef struct {
	double real;
	double img;
} complex;
complex x[N], *W; /*输入序列,变换因子*/

/*子函数声明*/
int inputdata(size_x);//数据输入
void changeLen(size_x, Len_x);/*改变长度，使之对齐2的次幂，不足的补零*/
void fft(Len_x); /*快速傅里叶变换*/
void initW(Len_x);   /*初始化变换因子*/
void bit_reversed_order(Len_x); /*变址,倒位序*/
void output(Len_x);/*输出结果*/

void add(complex a, complex b, complex *c); /*复数加法*/
void multiply(complex a, complex b, complex *c); /*复数乘法*/
void sub(complex a, complex b, complex *c); /*复数减法*/

//---------------------------------------------------------
/* 主函数 */
void main() {
	int size_x = 0;/*输入序列的大小*/
	int pow2_Len, Len_x = 0;

	size_x = inputdata();

	pow2_Len = (int)(ceil(log(size_x) / log(2)) + 0.5);//补零后的序列长度以2为底的对数
	Len_x = (int)(pow(2, pow2_Len) + 0.5);//补零后的序列的长度

	if ((Len_x - size_x) != 0)changeLen((size_x, Len_x));//补零操作

	initW(Len_x);//初始化变换因子
	bit_reversed_order(Len_x);//倒位序
	fft(Len_x);//蝶形计算
	output(Len_x);//输出
	system("pause");
}

//---------------------------------------------------------
//定义各种调用的子函数

/*输入函数*/
int inputdata() {
	char c = 'y';//字符变量，初始值为y
	int size_x = 0, i = 0;
	printf("请输入原始序列x的长度：");
	scanf_s("%d", &size_x);//输入原始序列的长度
	getchar();//接收回车
	printf("\n输入序列是否为复序列?(Y/N) ");//如果是实序列，输入都是0的虚部很麻烦，这里做一个if判断
	scanf_s("%c", &c, 1);//字符c用于判断输入是否为实序列

	if ((c == 'Y') || (c == 'y')) {  //如果是复数序列
		printf("\n请依次输入原始序列x[N]的实部和虚部:\n");
		for (i = 0; i < size_x; i++)
			scanf_s("%lf%lf", &x[i].real, &x[i].img);
	}//分别输入各个数的实部和虚部
	else {  //如果是实序列
		printf("\n请依次输入原始序列x[N]的实部:\n");//分别输入各个数的实部
		for (i = 0; i < size_x; i++) {
			scanf_s("%lf", &x[i].real);//输入实部
			x[i].img = 0.0000;
		}//虚部赋值为0
	}
	return size_x;//返回原始序列的长度
}



/*补零：改变长度，使之对齐2的次幂，不足的补零*/
void changeLen(size_x, Len_x) {
	int i;
	for (i = size_x; i < Len_x; i++) { //对剩余的Len_x-size_x个元素赋值为0
		x[i].real = 0.0000;
		x[i].img = 0.0000;
	}
}

//-------------------------------------------------------
/*
蝶形运算
说明：
	  (1)层次分析：
	  将整个FFT计算分为pow2_Len级(1=0...pow2_Len-1)，
	  第i级分为(Len_x/2L)组，每一组有2L=2*(2^i)个元素，
	  每一组有L对数，即要进行L次单个蝶形运算，同时每一对下标也相差L，

	  因此在第i级的运算中，
	  我们用j=j+2L确定每一组的第一个元素的下标，用k表示该组的第k次单个蝶形运算

	  (2)其他
	  这里用到之前声明的复数运算子函数
*/
void fft(Len_x) {   //原理：改进的按时间抽取的FFT算法，见课本P444
	int i = 0, j = 0, k = 0, L = 0;
	complex up, down, product;//计算的中间值

	for (i = 0; i < log(Len_x) / log(2); i++) {   //一级蝶形运算,级数为i
		L = 1 << i;//使用L=pow(2,i)可以达到相同效果，但是结果不是整形还要转换
		for (j = 0; j < Len_x; j = j + 2 * L) {  //一组蝶形运算，j,每组有L次蝶形运算，每组对应元素相距2L
			for (k = 0; k < L; k++) {    //一个蝶形运算,蝶形输入的两个下标相距
				multiply(x[j + k + L], W[Len_x*k / 2 / L], &product);//这一部分请参考P444的图11.23进行理解
				add(x[j + k], product, &up);
				sub(x[j + k], product, &down);
				x[j + k] = up;//单个蝶形结果上面的那个，还是参考图11.23
				x[j + k + L] = down;//单个蝶形的结果下标相差L
			}
		}
	}
}

//-------------------------------------------------------
/*初始化变换因子*/
void initW(Len_x) {
	int i;
	double PI;
	PI = atan(1) * 4;//圆周率,当然也可以用宏定义，或者直接赋值
	W = (complex *)malloc(sizeof(complex) * Len_x);/*开辟大小是sizeof(complex)乘以Len_x的内存空间,
												   包含在 <stdlib.h>中*/
	for (i = 0; i < Len_x; i++) {
		W[i].real = cos(2 * PI / Len_x * i);//定义变换因子的实部
		W[i].img = -1 * sin(2 * PI / Len_x * i);//定义变换因子的虚部
	}
}

//--------------------------------------------------------
/*
功能：变址计算，将x(n)码位倒置
说明：转换为二进制后，倒位序的本质是实现按位左右镜像，
	  这里使用移位操作完成，将原下标i的二进制按位左移和二进制数1进行与运算，
	  然后将所得的数依次右移到另一个变量j中，至此下标转换完成；

	  使用上一步得到的下标进行数据交换，交换次数为补零后序列长度的一半
*/
void bit_reversed_order(Len_x) {
	complex temp;
	unsigned short i = 0, j = 0, k = 0;
	double t = 0;
	for (i = 0; i < Len_x; i++) {  //下标i转换到j
		k = i;//将i赋值给k是因为移位后k值有变化，保留i交换时可使用
		j = 0;
		t = (log(Len_x) / log(2));//下标转换成二进制后有t位，因此要进行t次移位
		while ((t--) > 0) {
			j = j << 1;//用于保留本次移位结果
			j = j | (k & 1);// 先k与1按位与运算，再j与(k & 1)按位或运算
			k = k >> 1;//依次移位以取出二进制下由低到高的各个数
		}
		if (j > i) {    //当i不等于j交换，j>i才交换是保证下标对应的元素不会被交换两次
			temp = x[i];
			x[i] = x[j];
			x[j] = temp;
		}
	}//for结束
}


//------------------------------------------------------------
/*输出傅里叶变换的结果*/
void output(Len_x) {
	int i;
	printf("\nFFT变换结果为：\n");
	for (i = 0; i < Len_x; i++) {
		printf("%.4f", x[i].real);

		if (x[i].img >= 0.0001)
			printf("+%.4fj\n", x[i].img);//如果虚部大于0.0001，则加上+输出虚部
		else
			if (fabs(x[i].img) < 0.0001)printf("\n");// fabs:求浮点数x的绝对值
			else printf("%.4fj\n", x[i].img);//虚部小于0.0001则直接输出（自带负号）
	}
}

//----------------------------------------------------------------
/*
 功能：定义复数运算
 说明： 通过指针保存计算结果，注意这里不返回任何值是因为复数是全局变量
*/

//定义复数加法的函数
void add(complex a, complex b, complex *c)
{
	c->real = a.real + b.real;
	c->img = a.img + b.img;
}

//定义复数乘法的函数
void multiply(complex a, complex b, complex *c)
{
	c->real = a.real*b.real - a.img*b.img;
	c->img = a.real*b.img + a.img*b.real;
}

//定义复数减法的函数
void sub(complex a, complex b, complex *c)
{
	c->real = a.real - b.real;
	c->img = a.img - b.img;//结构体指针中p->成员名等价于（*p）.成员名
}
#include<stdio.h>
#include<stdlib.h>
#include<string.h> 

#define MAX 10 
#define KEMU 3
#define NAMELEN 20
#define CODELEN 13
#define FNAMELEN 20
#define BUFLEN 80

/*课程列表*/
char course[KEMU][NAMELEN + 1] = { "Chinese","Mathematic","English" };

/*结构体*/
typedef struct student
{
	char name[NAMELEN + 1];
	char code[CODELEN + 1];
	char sex[MAX];
	int chinese;
	int mathematic;
	int english;
	int total;
	struct student *next;
}stu;
stu *head; /*头指针*/

FILE *stfpt;/*文件指针*/
char stufile[FNAMELEN];/*文件名*/

/*被调函数声明*/
void printmenu();      /*打印程序功能菜单的函数*/
void cin(stu *pt);     /*输入相关数据的函数*/
stu *cindata();        /*其他数据是否继续输入的函数*/
stu *lookdata(stu *pt); /*查看数据的函数*/
void insert();    /*通过比较学号来插入数据的函数*/
finddata(stu *p2);    /*通过姓名查找查看数据的函数*/
void caverage();  /*求各学生语文平均分、最高和最低分成绩的函数*/
void maverage();  /*求各学生数学平均分、最高和最低分成绩的函数*/
void eaverage();  /*求各学生英语平均分、最高和最低分成绩的函数*/
void total();/*求各学生总分的函数*/
update(stu *p2);  /*通过姓名查找来更新数据*/
update2(stu *p2);  /*通过学号查找来更新数据*/
save(stu *p2);    /*保留数据函数*/


/*定义初始密码*/
char password[7] = "123456";
/*主函数*/
void main()
{
	int choice;
	char passcode[8], c;
	int flag = 0;
	int n1 = 3;
	/*char buf[BUFLEN];*/
	stu *p2;

	/*登入验证板块*/
	do
	{
		printf("Enter password:\n");
		scanf("%s", passcode);
		if (!strcmp(passcode, password))  /*进行密码匹配验证*/
		{
			printf("PASS\n\n");
			flag = 1; break;
		}
		else
		{
			printf("Error Enter again:\n");
			n1--;
		}
	} while (n1 > 0);

	if (!flag)
	{
		printf("you have Enter 3 times!"); /*输入密码超过了3次！！*/
		exit(0); /*自动退出*/
	}/*密码验证成功后进入的界面*/


	printf("\n"); /*操作首界面*/
	printf("==============欢迎进入!!!=============\n");
	printf("\n");
	printf("作者:温悦欣 \n学号:2012191010010\n年级:大一\n学院:航空航天学院\n"); /*程序的作者，学院和号码*/
	printf("\n");
	printf("说明：输入时，若直接按回车跳过，成绩记为零。");
	printf("\n");
	printf("======================================\n");
	printf("\nDo you want to enter the system?(Y/N)\n"); /*问进入系统与否*/
	getchar();
	c = getchar();
	if (c == 'n' || c == 'N')
		exit(1);
	printf("\n");
	printmenu();
	while (1)
	{
		printf("Now please enter choice marked with number from 0 to 11\n");
		printf("But you can also enter -1 to show command menu...");
		printf("Your choise:");
		scanf("%d", &choice);
		printf("\n");
		if (choice == 0)
		{
			printf("Thank you for using!\n\n");
			break;/*程序运行结束*/
		}
		switch (choice)
		{
		case -1:printmenu();
			break;
		case 1:p2 = cindata(); /*其他数据是否继续输入的函数*/
			break;
		case 2:p2 = lookdata(head); /*查看数据的函数*/
			break;
		case 3:insert(); /*通过比较学号来插入数据的函数*/
			break;
		case 4:finddata(head); /*通过姓名查找查看数据的函数*/
			break;
		case 5:update(head); /*通过姓名查找来更新数据*/
			break;
		case 6:update2(head);   /*通过学号查找来更新数据*/
			break;
		case 7:save(head); /*保留数据函数*/
			break;
		case 8:caverage(); /*求各学生语文平均分、最高和最低分成绩的函数*/
			break;
		case 9:maverage(); /*求各学生数学平均分、最高和最低分成绩的函数*/
			break;
		case 10:eaverage(); /*求各学生英语平均分、最高和最低分成绩的函数*/
			break;
		case 11:total();/*求各学生的总分的函数*/
			break;
		case 0: /*退出*/
			break;
		default:printf("It's wrong command!"); break;
		}
	}
}








/*被调函数的定义*/
void printmenu() /*打印程序菜单的函数*/
{
	printf("\n\n");
	printf("   ***************Score Manage System*************\n");/*成绩管理系统*/
	printf("   *\t    --managing command menu--            *\n");
	printf("   *\t      <1>Enter Record                    *\n"); /*输入数据*/
	printf("   *\t      <2>Display all record              *\n"); /*显示所有数据*/
	printf("   *\t      <3>Insert                          *\n"); /*插入数据*/
	printf("   *\t      <4>Search data through name        *\n"); /*按姓名访问数据*/
	printf("   *\t      <5>Update through name             *\n"); /*通过姓名查找来更新数据*/
	printf("   *\t      <6>Updata through code             *\n"); /*通过学号查找来更新数据*/
	printf("   *\t      <7>Save data                       *\n"); /*保存数据*/
	printf("   *\t      <8>Chinese Average                 *\n"); /*语文平均成绩*/
	printf("   *\t      <9>Math Average                    *\n"); /*数学平均成绩*/
	printf("   *\t      <10>English Average                *\n"); /*英语平均成绩*/
	printf("   *\t      <11>Total mark                     *\n"); /*总成绩*/
	printf("   *\t      <0>Quit                            *\n"); /*退出*/
	printf("   ***********************************************\n");
}



void cin(stu *pt)    /*输入相关数据的函数*/
{
	printf("*****Enter Record*****\n");
	printf("Enter name:");
	scanf("%s", &pt->name);
	printf("Enter code:");
	scanf("%s", &pt->code);
	printf("Enter sex:");
	scanf("%s", &pt->sex);
	printf("Enter score:\n");
	pt->chinese = 0;
	printf("Chinese:");
	scanf("%d", &pt->chinese);
	pt->mathematic = 0;
	printf("Mathematic:");
	scanf("%d", &pt->mathematic);
	pt->english = 0;
	printf("English:");
	scanf("%d", &pt->english);
}


stu *cindata()/*其他数据是否继续输入的函数*/
{
	stu *p1, *p2;
	int i = 1;
	char ch;
	p1 = (stu *)malloc(sizeof(stu));
	head = p1;
	while (i)
	{
		cin(p1);
		printf("Do you Want to Continue?(Y/N)");   /*是否继续输入数据*/
		getchar();
		ch = getchar();
		if (ch == 'n' || ch == 'N')
		{
			i = 0;
			p1->next = NULL;
		}
		else
		{
			p2 = p1;
			p1 = (stu *)malloc(sizeof(stu));
			p2->next = p1;
		}
	}
	return(p1->next);
}


stu *lookdata(stu *pt) /*查看数据的函数*/
{
	printf("*****Display all data*****");
	while (pt != NULL)
	{
		printf("Name:%s\n", pt->name);
		printf("Code:%s\n", pt->code);
		printf("Sex:%s\n", pt->sex);
		printf("Chinese:%d\n", pt->chinese);
		printf("Math:%d\n", pt->mathematic);
		printf("English:%d\n", pt->english);
		printf("\n");
		pt = pt->next;
	}
	printf("*****That's all data******\n");
	printf("\n");
	return pt;
}


void insert()  /*通过比较学号来插入数据的函数*/
{
	stu *p1, *p3, *p2;
	p1 = head;
	p3 = (stu *)malloc(sizeof(stu));

	p3->next = NULL;
	if (head == NULL) { head = p3; return; }
	cin(p3);
	while (p1 != NULL && (p1->code < p3->code))  /*通过学号的比较来插入*/
	{
		p2 = p1; p1 = p1->next;
	}
	if (p2 == head) { p3->next = head; head = p3; return; }
	p3->next = p1;
	p2->next = p3;
}



finddata(stu *p2)    /*通过姓名查找查看数据的函数*/
{
	char name[NAMELEN];
	int flag = 0;
	printf("Enter the name of the student you want to find:");   /*通过姓名查看*/
	scanf("%s", name);
	while (p2 != NULL)
	{
		if (strcmp(name, p2->name) == 0)
		{
			printf("The data you want has been found...\n");
			printf("Name:%s \t", p2->name);
			printf("Code:%s\t", p2->code);
			printf("Sex:%s\n", p2->sex);
			printf("%s's mark:\n", p2->name);
			printf("Chinese:%d\t", p2->chinese);
			printf("Mathematic:%d\t", p2->mathematic);
			printf("English:%d\t", p2->english);
			printf("\n");
			flag = 1;
		}
		else if (flag == 0);
		p2 = p2->next;
	}
	if (flag == 1)
	{
		printmenu();
		printf("Find one\n");
	}
	else
	{
		printmenu();
		printf("Not find\n");
	}
}


void caverage()    /*求各学生语文平均分、最高和最低分成绩的函数*/
{
	stu *p1;
	int i;
	float max = 0.0, min = 200.0;
	float sum = 0.0, aver = 0.0;
	p1 = head;
	if (p1 == NULL)
		printf("not data!");
	else
	{
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
			sum += p1->chinese;
		aver = sum / i;

		p1 = head;
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
		{
			if (max < p1->chinese)
				max = p1->chinese;
		}
		p1 = head;
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
			if (min > p1->chinese)
				min = p1->chinese;
	}
	printf("*****Chinese mark*****");
	printf("\n");
	printf("Average:%f\n", aver);
	printf("Max:%f\n", max);
	printf("Min:%f\n", min);
}



void maverage()  /*求各学生数学平均分、最高和最低分成绩的函数*/
{
	stu *p1;
	int i;
	float max = 0.0, min = 200.0;
	float sum = 0.0, aver = 0.0;
	p1 = head;
	if (p1 == NULL)
		printf("not data!");
	else
	{
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
			sum += p1->mathematic;
		aver = sum / i;

		p1 = head;
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
		{
			if (max < p1->mathematic)
				max = p1->mathematic;
		}
		p1 = head;
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
			if (min > p1->mathematic)
				min = p1->mathematic;
	}
	printf("*****Mathematic mark*****");
	printf("\n");
	printf("Average:%f\n", aver);
	printf("Max:%f\n", max);
	printf("Min:%f\n", min);
}


void eaverage()  /*求各学生英语平均分、最高和最低分成绩的函数*/
{
	stu *p1;
	int i;
	float max = 0.0, min = 200.0;
	float sum = 0.0, aver = 0.0;
	p1 = head;
	if (p1 == NULL)
		printf("not data!");
	else
	{
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
			sum += p1->english;
		aver = sum / i;

		p1 = head;
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
		{
			if (max < p1->english)
				max = p1->english;
		}
		p1 = head;
		for (i = 0; p1 != NULL; i++, p1 = p1->next)

			if (min > p1->english)
				min = p1->english;
	}
	printf("*****English mark*****");
	printf("\n");
	printf("Average:%f\n", aver);
	printf("Max:%f\n", max);
	printf("Min:%f\n", min);
}


void total()  /*求各学生总分、平均分、最高和最低分成绩的函数*/
{
	stu *p1;
	int i;
	float max = 0.0, min = 200.0;
	float sum = 0.0, sum2 = 0.0, aver = 0.0;
	p1 = head;
	if (p1 == NULL) printf("Not data!");
	else
	{
		printf("*****Total mark*****");
		printf("\n");
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
		{
			sum = sum + p1->english;
			sum = sum + p1->chinese;
			sum = sum + p1->mathematic;
			printf("%s's total mark:%f\n", p1->name, sum);
			sum2 = sum2 + sum;
			p1->total = sum;
			sum = 0.0;
		}


		aver = sum2 / i;
		p1 = head;
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
		{
			if (max < p1->total)
				max = p1->total;
		}
		p1 = head;
		for (i = 0; p1 != NULL; i++, p1 = p1->next)
			if (min > p1->total)
				min = p1->total;
	}
	printf("Average:%f\n", aver);
	printf("Max:%f\n", max);
	printf("Min:%f\n", min);
	printf("\n");
}



update(stu *p2)   /*通过姓名查找来更新数据*/
{
	char name[NAMELEN];/*p2为指向结构体struct student的指针*/
	int b = 0;
	printf("Please enter The Name:"); /*输入姓名*/
	scanf("%s", name);

	while (p2 != NULL)
	{
		if (strcmp(name, p2->name) == 0)
		{
			printf("Find data...\n");
			printf("Please enter basic information:\n");
			printf("Name:");
			scanf("%s", &p2->name);
			printf("Code:");
			scanf("%s", &p2->code);
			printf("Sex:");
			scanf("%s", &p2->sex);
			printf("please enter %s's mark:\n", p2->name);
			printf("Chinese:");
			scanf("%d", &p2->chinese);
			printf("Mathematic:");
			scanf("%d", &p2->mathematic);
			printf("English:");
			scanf("%d", &p2->english);
			printf("\n");
			b = 1;
		}
		p2 = p2->next;
	}

	if (b == 0)
	{
		printmenu();
		printf("Sorry, not Find data!\n");
	}
	else
	{
		printmenu();
		printf("Finish!\n");
	}
}




update2(stu *p2)   /*通过学号查找来更新数据*/
{
	char code[CODELEN];   /*p2为指向结构体struct student的指针*/
	int b = 0;
	printf("Please enter student's code:"); /*输入姓名*/
	scanf("%s", code);

	while (p2 != NULL)
	{
		if (strcmp(code, p2->code) == 0)
		{
			printf("Find data...\n");
			printf("Please enter basic information:\n");
			printf("Name:");
			scanf("%s", &p2->name);
			printf("Code:");
			scanf("%s", &p2->code);
			printf("Sex:");
			scanf("%s", &p2->sex);
			printf("please enter %s's mark:\n", p2->name);
			printf("Chinese:");
			scanf("%d", &p2->chinese);
			printf("Mathematic:");
			scanf("%d", &p2->mathematic);
			printf("English:");
			scanf("%d", &p2->english);
			printf("\n");
			b = 1;
		}

		p2 = p2->next;
	}
	if (b == 0)
	{
		printmenu();
		printf("Sorry, not Find data!\n");
	}
	else
	{
		printmenu();
		printf("Finish!\n");
	}
}


save(stu *p2)  /*保留数据函数*/
{
	FILE *fp;
	char file[FNAMELEN];
	printf("Enter file name:");  /*输入文件名*/
	scanf("%s", file);
	fp = fopen(file, "w");
	while (p2 != NULL)
	{
		fprintf(fp, "%s", p2->name);
		fprintf(fp, "%s", p2->code);
		fprintf(fp, "%s", p2->sex);
		fprintf(fp, "%d", p2->chinese);
		fprintf(fp, "%d", p2->mathematic);
		fprintf(fp, "%d", p2->english);
		p2 = p2->next;
	}
	fclose(fp);
}

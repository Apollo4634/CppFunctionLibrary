#include<stdio.h>
#include<stdlib.h>
#include<string.h> 

#define MAX 10 
#define KEMU 3
#define NAMELEN 20
#define CODELEN 13
#define FNAMELEN 20
#define BUFLEN 80

/*�γ��б�*/
char course[KEMU][NAMELEN + 1] = { "Chinese","Mathematic","English" };

/*�ṹ��*/
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
stu *head; /*ͷָ��*/

FILE *stfpt;/*�ļ�ָ��*/
char stufile[FNAMELEN];/*�ļ���*/

/*������������*/
void printmenu();      /*��ӡ�����ܲ˵��ĺ���*/
void cin(stu *pt);     /*����������ݵĺ���*/
stu *cindata();        /*���������Ƿ��������ĺ���*/
stu *lookdata(stu *pt); /*�鿴���ݵĺ���*/
void insert();    /*ͨ���Ƚ�ѧ�����������ݵĺ���*/
finddata(stu *p2);    /*ͨ���������Ҳ鿴���ݵĺ���*/
void caverage();  /*���ѧ������ƽ���֡���ߺ���ͷֳɼ��ĺ���*/
void maverage();  /*���ѧ����ѧƽ���֡���ߺ���ͷֳɼ��ĺ���*/
void eaverage();  /*���ѧ��Ӣ��ƽ���֡���ߺ���ͷֳɼ��ĺ���*/
void total();/*���ѧ���ֵܷĺ���*/
update(stu *p2);  /*ͨ��������������������*/
update2(stu *p2);  /*ͨ��ѧ�Ų�������������*/
save(stu *p2);    /*�������ݺ���*/


/*�����ʼ����*/
char password[7] = "123456";
/*������*/
void main()
{
	int choice;
	char passcode[8], c;
	int flag = 0;
	int n1 = 3;
	/*char buf[BUFLEN];*/
	stu *p2;

	/*������֤���*/
	do
	{
		printf("Enter password:\n");
		scanf("%s", passcode);
		if (!strcmp(passcode, password))  /*��������ƥ����֤*/
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
		printf("you have Enter 3 times!"); /*�������볬����3�Σ���*/
		exit(0); /*�Զ��˳�*/
	}/*������֤�ɹ������Ľ���*/


	printf("\n"); /*�����׽���*/
	printf("==============��ӭ����!!!=============\n");
	printf("\n");
	printf("����:������ \nѧ��:2012191010010\n�꼶:��һ\nѧԺ:���պ���ѧԺ\n"); /*��������ߣ�ѧԺ�ͺ���*/
	printf("\n");
	printf("˵��������ʱ����ֱ�Ӱ��س��������ɼ���Ϊ�㡣");
	printf("\n");
	printf("======================================\n");
	printf("\nDo you want to enter the system?(Y/N)\n"); /*�ʽ���ϵͳ���*/
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
			break;/*�������н���*/
		}
		switch (choice)
		{
		case -1:printmenu();
			break;
		case 1:p2 = cindata(); /*���������Ƿ��������ĺ���*/
			break;
		case 2:p2 = lookdata(head); /*�鿴���ݵĺ���*/
			break;
		case 3:insert(); /*ͨ���Ƚ�ѧ�����������ݵĺ���*/
			break;
		case 4:finddata(head); /*ͨ���������Ҳ鿴���ݵĺ���*/
			break;
		case 5:update(head); /*ͨ��������������������*/
			break;
		case 6:update2(head);   /*ͨ��ѧ�Ų�������������*/
			break;
		case 7:save(head); /*�������ݺ���*/
			break;
		case 8:caverage(); /*���ѧ������ƽ���֡���ߺ���ͷֳɼ��ĺ���*/
			break;
		case 9:maverage(); /*���ѧ����ѧƽ���֡���ߺ���ͷֳɼ��ĺ���*/
			break;
		case 10:eaverage(); /*���ѧ��Ӣ��ƽ���֡���ߺ���ͷֳɼ��ĺ���*/
			break;
		case 11:total();/*���ѧ�����ֵܷĺ���*/
			break;
		case 0: /*�˳�*/
			break;
		default:printf("It's wrong command!"); break;
		}
	}
}








/*���������Ķ���*/
void printmenu() /*��ӡ����˵��ĺ���*/
{
	printf("\n\n");
	printf("   ***************Score Manage System*************\n");/*�ɼ�����ϵͳ*/
	printf("   *\t    --managing command menu--            *\n");
	printf("   *\t      <1>Enter Record                    *\n"); /*��������*/
	printf("   *\t      <2>Display all record              *\n"); /*��ʾ��������*/
	printf("   *\t      <3>Insert                          *\n"); /*��������*/
	printf("   *\t      <4>Search data through name        *\n"); /*��������������*/
	printf("   *\t      <5>Update through name             *\n"); /*ͨ��������������������*/
	printf("   *\t      <6>Updata through code             *\n"); /*ͨ��ѧ�Ų�������������*/
	printf("   *\t      <7>Save data                       *\n"); /*��������*/
	printf("   *\t      <8>Chinese Average                 *\n"); /*����ƽ���ɼ�*/
	printf("   *\t      <9>Math Average                    *\n"); /*��ѧƽ���ɼ�*/
	printf("   *\t      <10>English Average                *\n"); /*Ӣ��ƽ���ɼ�*/
	printf("   *\t      <11>Total mark                     *\n"); /*�ܳɼ�*/
	printf("   *\t      <0>Quit                            *\n"); /*�˳�*/
	printf("   ***********************************************\n");
}



void cin(stu *pt)    /*����������ݵĺ���*/
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


stu *cindata()/*���������Ƿ��������ĺ���*/
{
	stu *p1, *p2;
	int i = 1;
	char ch;
	p1 = (stu *)malloc(sizeof(stu));
	head = p1;
	while (i)
	{
		cin(p1);
		printf("Do you Want to Continue?(Y/N)");   /*�Ƿ������������*/
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


stu *lookdata(stu *pt) /*�鿴���ݵĺ���*/
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


void insert()  /*ͨ���Ƚ�ѧ�����������ݵĺ���*/
{
	stu *p1, *p3, *p2;
	p1 = head;
	p3 = (stu *)malloc(sizeof(stu));

	p3->next = NULL;
	if (head == NULL) { head = p3; return; }
	cin(p3);
	while (p1 != NULL && (p1->code < p3->code))  /*ͨ��ѧ�ŵıȽ�������*/
	{
		p2 = p1; p1 = p1->next;
	}
	if (p2 == head) { p3->next = head; head = p3; return; }
	p3->next = p1;
	p2->next = p3;
}



finddata(stu *p2)    /*ͨ���������Ҳ鿴���ݵĺ���*/
{
	char name[NAMELEN];
	int flag = 0;
	printf("Enter the name of the student you want to find:");   /*ͨ�������鿴*/
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


void caverage()    /*���ѧ������ƽ���֡���ߺ���ͷֳɼ��ĺ���*/
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



void maverage()  /*���ѧ����ѧƽ���֡���ߺ���ͷֳɼ��ĺ���*/
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


void eaverage()  /*���ѧ��Ӣ��ƽ���֡���ߺ���ͷֳɼ��ĺ���*/
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


void total()  /*���ѧ���ܷ֡�ƽ���֡���ߺ���ͷֳɼ��ĺ���*/
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



update(stu *p2)   /*ͨ��������������������*/
{
	char name[NAMELEN];/*p2Ϊָ��ṹ��struct student��ָ��*/
	int b = 0;
	printf("Please enter The Name:"); /*��������*/
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




update2(stu *p2)   /*ͨ��ѧ�Ų�������������*/
{
	char code[CODELEN];   /*p2Ϊָ��ṹ��struct student��ָ��*/
	int b = 0;
	printf("Please enter student's code:"); /*��������*/
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


save(stu *p2)  /*�������ݺ���*/
{
	FILE *fp;
	char file[FNAMELEN];
	printf("Enter file name:");  /*�����ļ���*/
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

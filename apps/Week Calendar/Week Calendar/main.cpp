#include<iostream>
#include<array>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
using namespace std;

const array<int,12> Days_Month_Common = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const array<int,12> Days_Month_Leap = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int getYear()
/*****************************************************
* ���ܣ���ȡ�������
* ���룺
* �����int year
*****************************************************/
{
	int year = 2017;
	cout << "Please enter a year: ";
	cin >> year;
	cin.get();
	cout << endl;

	if (year < 1)
	{
		cout << "Warning: The year entered is invalid!";
		return -1;
	}
	return year;
}

string num2str(int num_in, int flag = 1)
/*****************************************************
* ���ܣ�int����ת��Ϊstring��
* ���룺int num_in ����
* �����string str �ַ�������
*****************************************************/
{
	stringstream ss;
	ss << num_in;
	string str = ss.str();

	if (str.size() == 1)
	{
		if (flag == 0) str = "0" + str;
		if (flag == 2) str = " " + str;		
	}

	return str;
}

bool isLeapYear(int year)
/*****************************************************
* ���ܣ��ж���������Ƿ�Ϊ����
* ���룺int year
* �����bool flag_leap_year
*****************************************************/
{
	bool flag4 = (year % 4 == 0);
	bool flag100 = (year % 100 == 0);
	bool flag400 = (year % 400 == 0);

	bool flag_leap_year = false;
	if ((flag100 == false) && (flag4 == true)) flag_leap_year = true;
	if (flag400 == true) flag_leap_year = true;

	return flag_leap_year;
}

int getDay_NewYearsDay(int year)
/*****************************************************
* ���ܣ�����������ݵ�1��1�������ڼ�
* ���룺int year
* �����int w_0101
*****************************************************/
{
	const int days_400 = 365 * 400 + 97;
	const int days_100 = 365 * 100 + 24;
	const int days_004 = 365 * 004 + 01;

	int days = 0; //���빫Ԫ1��1��1���ж�����
	int years_remain = year - 1;
	int times_of_400 = floor(years_remain / 400);
	days = days + days_400 * times_of_400;

	if ((years_remain % 400) != 0)
	{
		int years_remain1 = years_remain - 400 * times_of_400;
		int times_of_100 = floor(years_remain1 / 100);
		days = days + days_100 * times_of_100;

		if ((years_remain1 % 100) != 0)
		{
			int years_remain2 = years_remain1 - 100 * times_of_100;
			int times_of_004 = floor(years_remain2 / 4);
			days = days + days_004 * times_of_004;

			if ((years_remain2 % 4) != 0)
				days = days + 365 * (years_remain2 % 4);
		}
	}

	int w_0101 = (((days + 1) % 7)); //ת��Ϊ������
	if (w_0101 == 0) w_0101 = 7;
	return w_0101;
}

int getDays_M(int month, bool flag_LeapYear)
/*****************************************************
* ���ܣ�����ĳһ����1���������·ݰ�����������
* ���룺int month ��ֹ�·�
*       bool flag_LeapYear ��Ӧ����Ƿ�Ϊ����
* �����int sum_days
*****************************************************/
{
	int sum_days = 0;
	if (flag_LeapYear)
	{
		for (int i_m = 0; i_m < month; i_m++) sum_days += Days_Month_Leap[i_m];
	}
	else
	{
		for (int i_m = 0; i_m < month; i_m++) sum_days += Days_Month_Common[i_m];
	}

	return sum_days;
}

void setDay(int month, int day, int w_0101, bool flag_LeapYear, int *w, int *W)
/*****************************************************
* ���ܣ���ȡ�������������ڼ��Լ����ڵڼ���
* ���룺int month �·�
*       int day ����
*       int w_0101 ��Ӧ��ݵ�1��1�����ܼ�
*       bool flag_LeapYear ��Ӧ����Ƿ�Ϊ����
* �����int w �������ڵ�����
*       int W ����������������
*****************************************************/
{
	int days = 0; //��������ھ�����Ӧ���1��1��֮�������

	if (flag_LeapYear == true)
		days = getDays_M(month - 1, true) + day - 1;
	else
		days = getDays_M(month - 1, false) + day - 1;

	*w = w_0101 + (days % 7);
	if (*w > 7) *w -= 7;

	if (days < (7 - w_0101))
		*W = 1;
	else
		*W = ceil(double(days - (7 - w_0101)) / 7) + 1;
}

void getCalendar(int year, array<vector<array<string, 8>>, 12> &calendar)
/*******************************************************
* ���ܣ���ȡ����
* ���룺int year ���
* �����array<vector<array<string,8>>,12>&calendar ����
*******************************************************/
{
	int w_0101 = getDay_NewYearsDay(year);
	bool flag_LeapYear = isLeapYear(year);
	array<int, 12> days_month;

	if (flag_LeapYear)
		days_month = Days_Month_Leap;
	else
		days_month = Days_Month_Common;

	for (int i_M = 0; i_M < 12; i_M++)
	{
		int w_FirstDay_M, w_LastDay_M;
		int W_FirstDay_M, W_LastDay_M;
		setDay(i_M + 1, 1, w_0101, flag_LeapYear, &w_FirstDay_M, &W_FirstDay_M);
		setDay(i_M + 1, days_month[i_M], w_0101, flag_LeapYear, &w_LastDay_M, &W_LastDay_M);

		int N_Weeks_M = W_LastDay_M - W_FirstDay_M + 1;
		calendar[i_M].resize(N_Weeks_M);

		for (int i_W = 0; i_W < N_Weeks_M; i_W++) //�洢����
			calendar[i_M][i_W][0] = num2str(W_FirstDay_M + i_W, 0);

		int counter_W = 0;
		int counter_w = w_FirstDay_M;
		for (int i_D_M = 0; i_D_M < days_month[i_M]; i_D_M++)
		{
			if (counter_W == 0)
			{
				for (int i_w_FirstW = 1; i_w_FirstW < w_FirstDay_M; i_w_FirstW++)
					calendar[i_M][0][i_w_FirstW] = "  ";
			}

			calendar[i_M][counter_W][counter_w] = num2str(i_D_M + 1, 2);
			counter_w++;

			if (counter_w > 7)
			{
				counter_w = 1;
				counter_W++;
			}
		}

		if (w_LastDay_M != 7)
		{
			for (int i_w_LastW = counter_w; i_w_LastW < 8; i_w_LastW++)
				calendar[i_M][N_Weeks_M - 1][i_w_LastW] = " ";
		}
	}
}

void showCalendar(array<vector<array<string, 8>>, 12> &calendar, const int year)
/*******************************************************
* ���ܣ���ʾ������Ϣ
* ���룺array<vector<array<string,8>>,12>&calendar ����
*       const int year ���
* �������Ļ��ʾ
*******************************************************/
{
	const array<string, 12> Month = { "January", "February", "March", "April", "May", 
		"June", "July", "August", "September", "October", "November", "December" };

	cout << year << " Week Calendar" << endl << endl;
	for (int i_M = 0; i_M < 12; i_M++)
	{
		cout << num2str(i_M + 1, 0) << " " << Month[i_M] << endl;
		cout << "     " << " 1" << "  2" << "  3" << "  4" << "  5" << "  6" << "  7" << endl;

		const size_t N_Weeks_M = calendar[i_M].size();
		for (int i_W = 0; i_W < N_Weeks_M; i_W++)
		{
			cout << "(" << calendar[i_M][i_W][0] << ") ";
			for (int i_w = 1; i_w < 8; i_w++) 
				cout << calendar[i_M][i_W][i_w] << " ";
			cout << endl;
		}
		cout << endl;
	}
}

bool CalendarWrite(array<vector<array<string, 8>>, 12> &calendar, const int year)
{
	bool flag_CalendarWrite = true;
	const array<string, 12> Month = { "January", "February", "March", "April", "May",
		"June", "July", "August", "September", "October", "November", "December" };

	// ȷ���ļ��洢·���봴��txt�ļ�
	char hdp_char[2];
	cout << "Select hard disk partition to store file: ";
	cin.getline(hdp_char, 2);
	string hdp = hdp_char;
	string filepath = hdp + ":\\" + num2str(year, 1) + " Week Calendar.txt";
	cout << "The file path is \"" << filepath << "\""<< endl;
	ofstream fout(filepath);

	// ·���Ϸ����ж����ļ�д��
	ifstream fin;
	fin.open(filepath);
	if (!(fin.is_open()))
	{
		cout << "Warning: The file path is invalid!" << endl;
		return flag_CalendarWrite;
	}
	else
	{
		fin.close();
		fout << year << " Week Calendar" << endl << endl;
		for (int i_M = 0; i_M < 12; i_M++)
		{
			fout << num2str(i_M + 1, 0) << " " << Month[i_M] << endl;
			fout << "     " << " 1" << "  2" << "  3" << "  4" << "  5" << "  6" << "  7" << endl;

			const size_t N_Weeks_M = calendar[i_M].size();
			for (int i_W = 0; i_W < N_Weeks_M; i_W++)
			{
				fout << "(" << calendar[i_M][i_W][0] << ") ";
				for (int i_w = 1; i_w < 8; i_w++)
					fout << calendar[i_M][i_W][i_w] << " ";
				fout << endl;
			}
			fout << endl;
		}
		fout.close();
		cout << "File storage has been completed!" << endl;
	}
	return flag_CalendarWrite;
}

int main(int argc, char* argv[])
{
	int year = getYear();
	array<vector<array<string, 8>>, 12> calendar;
	getCalendar(year,  calendar);
	showCalendar(calendar, year);
	CalendarWrite(calendar, year);
	system("pause");
}
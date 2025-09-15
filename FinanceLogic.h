#pragma once
#include<iostream>
#include<string>//string������
#include<vector>//��vector����
#include<fstream>
#include<sstream>//ʹ��ostringstream
#include<iomanip>//ʹ��setw,setfill

/*-----��������-----*/
enum class Basic_category {
	Income,
	Outcome
};

/*-----�ӷ���-----*/
enum class Sub_category {
	salary = 1,
	bonus,
	investment,
	food,
	shop
};

/*-----��¼����-----*/
class Record {
public:
	Record(std::string d, double a, Basic_category c, int y, int m, int day);	//���캯����ʼ��
	virtual ~Record() = default;												//����������
	virtual void show_record()const;											//���麯��

	std::string getdate()const;													//��ȡ��¼������
	double getmoney()const;														//��ȡ��¼�Ľ��
	void setmoney(double m);													//���ü�¼�Ľ��
	void setdate(std::string d);												//���ü�¼������
	Basic_category getcategory()const;											//��ü�¼�Ļ�������
	void setcategory(Basic_category cat);										//���ü�¼�Ļ�������
	int getyear()const;															//��ȡ��¼����
	int getmonth()const;														//��ȡ��¼����
	int getday()const;															//��ȡ��¼����
	int setyear(int y);															//���ü�¼����
	int setmonth(int m);														//���ü�¼����
	int setday(int d);															//���ü�¼����

private:
	std::string date;
	double money;
	Basic_category category;
	int year;
	int month;
	int day;
};

/*-----�����¼����-----*/
class IncomeRecord :public Record {
public:
	IncomeRecord(std::string, double a, Sub_category c, int y, int m, int day);	//���캯����ʼ��

	void show_record() const;								//����˼�¼
	void set_inc_category(Sub_category cat);				//������������
	Sub_category get_inc_category()const;					//��ȡ��������

private:
	Sub_category inc_category;
};

/*-----֧����¼����-----*/
class OutcomeRecord :public Record {
public:
	OutcomeRecord(std::string, double a, Sub_category c, int y, int m, int day);	//���캯����ʼ��

	void show_record() const;								//�����¼
	void set_out_category(Sub_category cat);				//����֧������
	Sub_category get_out_category()const;					//��ȡ֧������

private:
	Sub_category out_category;
};

/*-----�˻��������-----*/
class FinanceManager {
private:
	std::vector<std::unique_ptr<Record>>records;	//�����¼������
	double balance = 0.0;							//����֧�����ܶ�
	std::string filename;							//�ļ���
public:
	FinanceManager();
	void calculateBalance();						//������֧
	void modify_record();							//���Ҽ�¼
	void delete_record();							//ɾ����¼
	void add_records(std::unique_ptr<Record>r);		//��Ӽ�¼
	void update_records();							//�޸ļ�¼
	void calculate_start_records();					//������ʼ�����ڵĸ���Ľ��
	void modify_start_records(int key);					//������ʼ�����ڵļ�¼
	double getBalance();							//�������֧���ܶ�
	void addIncomeRecord();							//��������¼
	void addOutcomeRecord();						//���֧����¼
	void setFileName(std::string& filena);			//�����ļ���
	bool loadFromFile();							//���ļ�����
	bool saveToFile();								//���浽�ļ�

};

/*-----������ڹ�����-----*/
class DataUtils
{
public:
	static bool isLeapYear(int year);
	static bool isValidDate(int year, int month, int day);
	static std::string isJiegou(int year, int month, int day);
};

/*-----�˵���-----*/
class Menu
{
private:
	FinanceManager& manager;
	int choice;
public:
	Menu(FinanceManager& m_manager);
	void showMenu();			//��ӡ�˵�
	void getchoice();			//����û�ѡ��
	bool handleChoice();		//�����û�ѡ��
	void run();					//ѭ���˵�����
};

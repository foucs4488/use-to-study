#pragma once
#include<iostream>
#include<string>//string所在类
#include<vector>//用vector数组
#include<fstream>
#include<sstream>//使用ostringstream
#include<iomanip>//使用setw,setfill

/*-----基础分类-----*/
enum class Basic_category {
	Income,
	Outcome
};

/*-----子分类-----*/
enum class Sub_category {
	salary = 1,
	bonus,
	investment,
	food,
	shop
};

/*-----记录基类-----*/
class Record {
public:
	Record(std::string d, double a, Basic_category c, int y, int m, int day);	//构造函数初始化
	virtual ~Record() = default;												//虚析构函数
	virtual void show_record()const;											//纯虚函数

	std::string getdate()const;													//获取记录的日期
	double getmoney()const;														//获取记录的金额
	void setmoney(double m);													//设置记录的金额
	void setdate(std::string d);												//设置记录的日期
	Basic_category getcategory()const;											//获得记录的基础类型
	void setcategory(Basic_category cat);										//设置记录的基础类型
	int getyear()const;															//获取记录的年
	int getmonth()const;														//获取记录的月
	int getday()const;															//获取记录的日
	int setyear(int y);															//设置记录的年
	int setmonth(int m);														//设置记录的月
	int setday(int d);															//设置记录的日

private:
	std::string date;
	double money;
	Basic_category category;
	int year;
	int month;
	int day;
};

/*-----收入记录子类-----*/
class IncomeRecord :public Record {
public:
	IncomeRecord(std::string, double a, Sub_category c, int y, int m, int day);	//构造函数初始化

	void show_record() const;								//输出此记录
	void set_inc_category(Sub_category cat);				//设置收入类型
	Sub_category get_inc_category()const;					//获取收入类型

private:
	Sub_category inc_category;
};

/*-----支出记录子类-----*/
class OutcomeRecord :public Record {
public:
	OutcomeRecord(std::string, double a, Sub_category c, int y, int m, int day);	//构造函数初始化

	void show_record() const;								//输出记录
	void set_out_category(Sub_category cat);				//设置支出类型
	Sub_category get_out_category()const;					//获取支出类型

private:
	Sub_category out_category;
};

/*-----账户管理基类-----*/
class FinanceManager {
private:
	std::vector<std::unique_ptr<Record>>records;	//储存记录的数组
	double balance = 0.0;							//收入支出的总额
	std::string filename;							//文件名
public:
	FinanceManager();
	void calculateBalance();						//计算收支
	void modify_record();							//查找记录
	void delete_record();							//删除记录
	void add_records(std::unique_ptr<Record>r);		//添加记录
	void update_records();							//修改记录
	void calculate_start_records();					//计算起始日期内的各类的金额
	void modify_start_records(int key);					//查找起始日期内的记录
	double getBalance();							//获得收入支出总额
	void addIncomeRecord();							//添加收入记录
	void addOutcomeRecord();						//添加支出记录
	void setFileName(std::string& filena);			//设置文件名
	bool loadFromFile();							//从文件加载
	bool saveToFile();								//保存到文件

};

/*-----检测日期工具类-----*/
class DataUtils
{
public:
	static bool isLeapYear(int year);
	static bool isValidDate(int year, int month, int day);
	static std::string isJiegou(int year, int month, int day);
};

/*-----菜单类-----*/
class Menu
{
private:
	FinanceManager& manager;
	int choice;
public:
	Menu(FinanceManager& m_manager);
	void showMenu();			//打印菜单
	void getchoice();			//获得用户选择
	bool handleChoice();		//处理用户选择
	void run();					//循环菜单函数
};

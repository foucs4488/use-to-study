#include<iostream>
#include<utility>//使用std::move
#include<memory>//使用智能指针
#include<iomanip>
#include"FinanceLogic.h"

/*-----辅助工具-----*/

const char* Basic_to_string(Basic_category cat) {
    switch (cat) {
    case Basic_category::Income:
        return "Income";
    case Basic_category::Outcome:
        return "Outcome";
    default:
        return "Unknown";
    }
}

const char* Sub_to_string(Sub_category sc) {
    switch (sc) {
    case Sub_category::salary:
        return "Salary";
    case Sub_category::bonus:
        return "Bonus";
    case Sub_category::investment:
        return "Investment";
    case Sub_category::food:
        return "Food";
    case Sub_category::shop:
        return "Shop";
    default:
        return "Unknown";
    }
}

Basic_category intToBasic_category(int i) {
    switch (i) {
    case 0:
        return Basic_category::Income;
    case 1:
        return Basic_category::Outcome;
    default:
        std::cout << "输入不符合默认返回Income" << std::endl;
        return Basic_category::Income;
    }
}

Sub_category intToSub_category(int i) {
    switch (i) {
    case 1:
        return Sub_category::salary;
    case 2:
        return Sub_category::bonus;
    case 3:
        return Sub_category::investment;
    case 4:
        return Sub_category::food;
    case 5:
        return Sub_category::shop;
    default:
        std::cout << "输入不符合默认返回salary" << std::endl;
        return Sub_category::salary;
    }
}



/*-----Record基类-----*/
Record::Record(std::string d, double a, Basic_category c, int y, int m, int day) :date(d), money(a), category(c), year(y), month(m), day(day) {}

void Record::show_record()const {
    std::cout << Basic_to_string(category) << ": " << money << " on " << date << std::endl;
}

std::string Record::getdate()const {
    return date;
}

double Record::getmoney()const {
    return money;
}

Basic_category Record::getcategory()const {
    return category;
}

void Record::setmoney(double m) {
    money = m;
}

void Record::setdate(std::string d) {
    date = d;
}

void Record::setcategory(Basic_category cat) {
    category = cat;
}

int Record::getyear()const {
    return year;
}

int Record::getmonth()const {
    return month;
}

int Record::getday()const {
    return day;
}

int Record::setyear(int y) {
    year = y;
    return year;
}

int Record::setmonth(int m) {
    month = m;
    return month;
}

int Record::setday(int d) {
    day = d;
    return day;
}


/*-----IncomeRecord子类-----*/
IncomeRecord::IncomeRecord(std::string, double a, Sub_category c, int y, int m, int day) :Record(DataUtils::isJiegou(y, m, day), a, Basic_category::Income, y, m, day), inc_category(c) {}

void IncomeRecord::show_record() const {
    std::cout << "收入[" << Sub_to_string(inc_category) << "]: "<< getmoney() << " 元, 日期: " << getdate() << std::endl;
}

void IncomeRecord::set_inc_category(Sub_category cat) {
    inc_category = cat;
}

Sub_category IncomeRecord::get_inc_category()const {
    return inc_category;
}

/*-----OutcomeRecord子类-----*/
OutcomeRecord::OutcomeRecord(std::string, double a, Sub_category c, int y, int m, int day) :Record(DataUtils::isJiegou(y, m, day), a, Basic_category::Outcome, y, m, day), out_category(c) {}

void OutcomeRecord::show_record() const {
    std::cout << "支出[" << Sub_to_string(out_category) << "]: " << getmoney() << " 元, 日期: " << getdate() << std::endl;
}

void OutcomeRecord::set_out_category(Sub_category cat) {
    out_category = cat;
}

Sub_category OutcomeRecord::get_out_category()const {
    return out_category;
}

/*-----FinanceManager基类-----*/

FinanceManager::FinanceManager() :balance(0.0) {}

double FinanceManager::getBalance() {
    return balance;
}

void FinanceManager::calculateBalance() {

    balance = 0.0;                                                                              //总额初始化，避免之前运算的总额对此造成影响

    for (const auto& r : records) {
        balance += (r->getcategory() == Basic_category::Income ? 1 : -1) * r->getmoney();
    }                                                                                           //用const保证records中的值不会改变，用&来引用、避免拷贝，用auto来自动判断r的类型、减少操作，然后循环内运用三目运算符来计算收入支出总额

}//计算收支

void FinanceManager::add_records(std::unique_ptr<Record> r) {

    records.push_back(std::move(r));                                                            //unique_ptr不支持拷贝，所以用move

}

void FinanceManager::setFileName(std::string& filena)//设置文件名
{
    filename = filena;
}

bool FinanceManager::loadFromFile() {

    std::string ls;
    std::ifstream ifs(filename, std::ios::in);
    if (!ifs.is_open()) {
        std::cout << "文件打开失败" << filename << std::endl;
        ifs.close();
        return false;
    }
    else {
        std::string line;
        while (getline(ifs, line)) {
            std::istringstream iss(line);
            std::string date;
            int basicType, subType;
            int year, month, day;
            double amount;

            if (!(iss >> basicType >> subType >> date >> year >> month >> day >> amount))
                continue;
            Basic_category basicCat = intToBasic_category(basicType);
            Sub_category subCat = intToSub_category(subType);

            if (basicCat == Basic_category::Income) {
                records.push_back(std::make_unique<IncomeRecord>(date, amount, subCat, year, month, day));
            }
            else {
                records.push_back(std::make_unique<OutcomeRecord>(date, amount, subCat, year, month, day));
            }
        }

        ifs.close();
        if (records.empty()) {
            //std::cout << "记录为空，请添加记录" << std::endl;
            ifs.close();
        }
        calculateBalance();
        return true;
    }
}

bool FinanceManager::saveToFile() {
    std::ofstream ofs(filename, std::ios::out);
    if (!ofs.is_open())
    {
        std::cout << "文件打开失败" << std::endl;
        ofs.close();
        return false;
    }
    else
    {


        for (const auto& r : records)
        {
            int subValue = 0;
            if (auto* inc = dynamic_cast<IncomeRecord*>(r.get())) {
                subValue = static_cast<int>(inc->get_inc_category());
            }
            else if (auto* out = dynamic_cast<OutcomeRecord*>(r.get())) {
                subValue = static_cast<int>(out->get_out_category());
            }
            ofs << static_cast<int>(r->getcategory()) << "\t" << subValue << "\t" << r->getdate() << "\t" << r->getyear() << "\t" << r->getmonth() << "\t" << r->getday() << "\t" << r->getmoney() << std::endl;
        }
        ofs.close();
        return true;

    }
}

void FinanceManager::modify_record() {

    std::string target;
    int y, m, d;
    std::cout << "请输入要查找的日期" << std::endl;
    std::cout << "请输入年份：";
    std::cin >> y;
    std::cout << "请输入月份：";
    std::cin >> m;
    std::cout << "请输入天数：";
    std::cin >> d;
    if (!DataUtils::isValidDate(y, m, d))
    {
        std::cout << "日期格式输错了，请重新输入" << std::endl;
        return;
    }
    target = DataUtils::isJiegou(y, m, d);
    for (auto& r : records) {
        if (r->getdate() == target) {
            std::cout << "日期：" << target << "收支大类型：" << Basic_to_string(r->getcategory()) << std::endl;//用Basic_to_string函数来将<<无法直接输出的enum类型转换成其可以直接输出的字符型

            if (auto* inc = dynamic_cast<IncomeRecord*>(r.get())) {
                std::cout << "收入类型：" << Sub_to_string(inc->get_inc_category()) << std::endl;//
            }
            else if (auto* out = dynamic_cast<OutcomeRecord*>(r.get())) {
                std::cout << "支出类型：" << Sub_to_string(out->get_out_category()) << std::endl;
            }
            return;
        }
    }
    std::cout << "未找到该日期记录。\n";

}//根据用户输入的日期来查找记录

void FinanceManager::update_records() {

    std::string target;
    int y, m, d;
    std::cout << "请输入要修改的日期" << std::endl;
    std::cout << "请输入年份：";
    std::cin >> y;
    std::cout << "请输入月份：";
    std::cin >> m;
    std::cout << "请输入天数：";
    std::cin >> d;
    if (!DataUtils::isValidDate(y, m, d))
    {
        std::cout << "日期格式输错了，请重新输入" << std::endl;
        return;
    }
    target = DataUtils::isJiegou(y, m, d);

    for (auto& r : records) {
        if (r->getdate() == target) {
            double newMoney;
            //Basic_category cat;
            std::cout << "原金额: " << r->getmoney() << "  请输入新金额: ";
            std::cin >> newMoney;
            r->setmoney(newMoney);
            std::cout << "修改成功！" << std::endl;
            calculateBalance();//刷新余额
            return;
        }

    }
    std::cout << "未找到该日期记录。\n";

}

void FinanceManager::delete_record() {

    std::string target;
    int y, m, d;
    std::cout << "请输入要查找的日期：" << std::endl;
    std::cout << "请输入年份：";
    std::cin >> y;
    std::cout << "请输入月份：";
    std::cin >> m;
    std::cout << "请输入天数：";
    std::cin >> d;
    if (!DataUtils::isValidDate(y, m, d))
    {
        std::cout << "日期格式输错了，请重新输入" << std::endl;
        return;
    }
    target = DataUtils::isJiegou(y, m, d);

    std::vector<size_t>id_today;
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i]->getdate() == target) {
            id_today.push_back(i);
        }
        else if (records.empty()) {
            std::cout << "日期中无记录！" << std::endl;
            return;
        }
    }

    for (size_t j = 0; j < id_today.size(); ++j) {
        std::cout << "(" << j + 1 << ")  ";
        records[id_today[j]]->show_record();
    }

    size_t choice;
    std::cout << "选择要删除记录的序号：";
    std::cin >> choice;
    if (choice == 0 || choice > id_today.size()) {
        std::cout << "此序号不存在啊！" << std::endl;
        return;
    }

    records.erase(records.begin() + id_today[choice - 1]);
    std::cout << "删除成功！" << std::endl;
    calculateBalance();//刷新余额
    saveToFile();
}

void FinanceManager::calculate_start_records() {

    std::string start_target, end_target;
    std::cout << "请输入要查找的起始日期" << std::endl;
    int y, m, d;
    std::cout << "请输入年份：";
    std::cin >> y;
    std::cout << "请输入月份：";
    std::cin >> m;
    std::cout << "请输入天数：";
    std::cin >> d;
    if (!DataUtils::isValidDate(y, m, d))
    {
        std::cout << "日期格式输错了，请重新输入" << std::endl;
        return;
    }
    start_target = DataUtils::isJiegou(y, m, d);
    std::cout << "请输入要查找的终止日期" << std::endl;
    std::cout << "请输入年份：";
    std::cin >> y;
    std::cout << "请输入月份：";
    std::cin >> m;
    std::cout << "请输入天数：";
    std::cin >> d;
    if (!DataUtils::isValidDate(y, m, d))
    {
        std::cout << "日期格式输错了，请重新输入" << std::endl;
        return;
    }
    end_target = DataUtils::isJiegou(y, m, d);

    double inc_money = 0.0;
    double out_money = 0.0;
    for (auto& r : records) {
        const std::string d = r->getdate();
        if (d >= start_target && d <= end_target) {
            if (r->getcategory() == Basic_category::Income) {
                inc_money += r->getmoney();
            }
            else if (r->getcategory() == Basic_category::Outcome) {
                out_money += r->getmoney();
            }
        }
    }
    std::cout << "总收入: " << inc_money << std::endl;
    std::cout << "总支出: " << out_money << std::endl;
    calculateBalance();
}

void FinanceManager::modify_start_records(int key) {

    std::string start_target, end_target;
    std::cout << "请输入要查找的起始日期:" << std::endl;
    int y, m, d;
    std::cout << "请输入年份：";
    std::cin >> y;
    std::cout << "请输入月份：";
    std::cin >> m;
    std::cout << "请输入天数：";
    std::cin >> d;
    if (!DataUtils::isValidDate(y, m, d))
    {
        std::cout << "日期格式输错了，请重新输入" << std::endl;
        return;
    }
    start_target = DataUtils::isJiegou(y, m, d);
    std::cout << "请输入要查找的终止日期:" << std::endl;

    std::cout << "请输入年份：";
    std::cin >> y;
    std::cout << "请输入月份：";
    std::cin >> m;
    std::cout << "请输入天数：";
    std::cin >> d;
    if (!DataUtils::isValidDate(y, m, d))
    {
        std::cout << "日期格式输错了，请重新输入" << std::endl;
        return;
    }
    end_target = DataUtils::isJiegou(y, m, d);
    int abcd = 0;//临时检测记录数量的变量
    for (auto& r : records) {
        const std::string d = r->getdate();
        if (d >= start_target && d <= end_target) {
            if (key == 0 && r->getcategory() != Basic_category::Income)
                continue;
            if (key == 1 && r->getcategory() != Basic_category::Outcome)
                continue;
            abcd = 1;
            r->show_record();


        }
    }
    if (key == 0 && abcd == 0)
        std::cout << "起止日期内收入记录为空，请添加记录" << std::endl;
    if (key == 1 && abcd == 0)
        std::cout << "起止日期内支出记录为空，请添加记录" << std::endl;
}

void FinanceManager::addIncomeRecord()//添加收入记录
{
    //loadFromFile();
    Sub_category cordType;
    std::string date;

    int t;//类型
    int y;
    int m;
    int d;
    int inou = 0;
    double am;//金额
    while (true)
    {
        std::cout << "请输入具体收入类型（1-工资，2-奖金）：" << std::endl;
        std::cin >> t;
        if (t > 2 || t < 1)
        {
            std::cout << "输错类型了，请重新输入" << std::endl;
        }
        else
            break;
    }
    cordType = intToSub_category(t);
    std::cout << "请输入年份：";
    std::cin >> y;
    std::cout << "请输入月份：";
    std::cin >> m;
    std::cout << "请输入当前天数：";
    std::cin >> d;
    if (!DataUtils::isValidDate(y, m, d))
    {
        std::cout << "日期格式输错了，请重新输入" << std::endl;
        return;
    }
    std::cout << "请输入处理金额：";
    std::cin >> am;
    auto record = std::make_unique<IncomeRecord>(date, am, cordType, y, m, d);
    //records.push_back(std::move(record));
    record->setdate(DataUtils::isJiegou(y, m, d));
    record->set_inc_category(cordType);
    record->setyear(y);
    record->setmonth(m);
    record->setday(d);
    record->setmoney(am);
    records.push_back(std::move(record));
    if (!saveToFile())
    {
        std::cout << "添加记录失败" << std::endl;
    }
    else {
        std::cout << "添加记录成功" << std::endl;
    }
    calculateBalance();
}

void FinanceManager::addOutcomeRecord()//添加支出记录
{
    //loadFromFile();
    Sub_category cordType;
    std::string date;
    int t;
    int y;
    int m;
    int d;
    int inco = 1;
    double am;
    while (true)
    {
        std::cout << "请输入具体支出类型（3-餐饮，4-购物，5-交通）：" << std::endl;
        std::cin >> t;
        if (t > 5 || t < 3)
        {
            std::cout << "输错类型了，请重新输入" << std::endl;
        }
        else
            break;
    }
    cordType = intToSub_category(t);

    std::cout << "请输入年份：";
    std::cin >> y;
    std::cout << "请输入月份：";
    std::cin >> m;
    std::cout << "请输入当前天数：";
    std::cin >> d;

    if (!DataUtils::isValidDate(y, m, d))
    {
        std::cout << "日期格式输错了，请重新输入" << std::endl;
        return;
    }
    std::cout << "请输入处理金额：";
    std::cin >> am;
    auto record = std::make_unique<OutcomeRecord>(date, am, cordType, y, m, d);
    record->set_out_category(cordType);
    record->setdate(DataUtils::isJiegou(y, m, d));
    record->setyear(y);
    record->setmonth(m);
    record->setday(d);
    record->setmoney(am);
    records.push_back(std::move(record));
    if (!saveToFile())
    {
        std::cout << "添加记录失败" << std::endl;
    }
    else {
        std::cout << "添加记录成功" << std::endl;
    }
    calculateBalance();
}

/*-----检测工具类-----*/
std::string DataUtils::isJiegou(int year, int month, int day)
{
    std::ostringstream oss1, oss2, oss3;
    oss1 << std::setw(4) << std::setfill('0') << year;
    oss2 << std::setw(2) << std::setfill('0') << month;
    oss3 << std::setw(2) << std::setfill('0') << day;
    std::string ymd = oss1.str() + '-' + oss2.str() + '-' + oss3.str();
    return ymd;
}

bool DataUtils::isLeapYear(int year)
{
    if (year % 100 == 0 && year % 400 == 0)
        return true;
    if (year % 100 != 0 && year % 4 == 0)
        return true;
    return false;
}

bool DataUtils::isValidDate(int year, int month, int day)
{
    if (year < 0)
        return false;
    if (month < 1 || month>12 || day < 1 || day>31)
        return false;
    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day == 31)
            return false;
    }
    bool d = DataUtils::isLeapYear(year);
    if (d)
    {
        if (month == 2 && day > 29)
            return false;
    }
    else
    {
        if (month == 2 && day > 28)
            return false;
    }
    return true;
}

/*-----菜单类-----*/
Menu::Menu(FinanceManager& m_manager) :manager(m_manager), choice(-1) {}

void Menu::showMenu()//打印菜单
{
    std::cout << "****************************************" << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << "*****1-添加收入记录*********************" << std::endl;
    std::cout << "*****2-添加支出记录*********************" << std::endl;
    std::cout << "*****3-查看起止日期内的收入记录*********" << std::endl;
    std::cout << "*****4-查看起止日期内的支出记录*********" << std::endl;
    std::cout << "*****5-查看实时收支平衡情况*************" << std::endl;
    std::cout << "*****6-按照日期查找收支记录*************" << std::endl;
    std::cout << "*****7-按照日期修改收支记录*************" << std::endl;
    std::cout << "*****8-按照日期删除收支记录*************" << std::endl;
    std::cout << "*****9-统计起止日期内的各类收支情况*****" << std::endl;
    std::cout << "*****0-退出系统*************************" << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << "****************************************" << std::endl;
}

void Menu::getchoice()//获得用户选择
{
    int m_choice;
    std::cout << "请选择你要处理的业务" << std::endl;
    std::cin >> m_choice;
    if (m_choice <= 9 && m_choice >= 0)
    {
        choice = m_choice;
    }
}

bool Menu::handleChoice()//处理用户选择
{
    switch (choice)
    {
    case 1:
        manager.addIncomeRecord();
        break;
    case 2:
        manager.addOutcomeRecord();
        break;
    case 3:
        manager.modify_start_records(0);
        break;
    case 4:
        manager.modify_start_records(1);
        break;
    case 5:
        std::cout << "当前收支平衡为：" << manager.getBalance() << std::endl;
        break;
    case 6:
        manager.modify_record();
        break;
    case 7:
        manager.update_records();
        break;
    case 8:
        manager.delete_record();
        break;
    case 9:
        manager.calculate_start_records();
        break;
    case 0:
        std::cout << "感谢使用本系统，欢迎下次使用！" << std::endl;
        return false;
    default:
        std::cout << "输错了请重新输入" << std::endl;
        break;
    }
    return true;
}

void Menu::run()//循环菜单函数
{
    while (true)
    {
        showMenu();
        getchoice();

        if (handleChoice() == false)
        {
            break;
        }
        system("pause");
        system("cls");

    }
}

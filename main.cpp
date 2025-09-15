#include<iostream>
#include<string>
#include<vector>
#include"FinanceLogic.h"

int main()
{
    std::string name = R"(E:\ku.txt)";
    FinanceManager manager;
    manager.setFileName(name);
    manager.loadFromFile();
    Menu yonghu1(manager);
    yonghu1.run();
	manager.saveToFile();
    system("pause");
    return 0;
}

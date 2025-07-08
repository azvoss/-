#ifndef HEADDEPT_H
#define HEADDEPT_H
#include "User.h"
#include <string>
#include "Data.h"
class Headdept{
private:
	 User user;
	 std::string dept;
public:
	 Headdept(const User& u, std::string a = "");
	 //获得资料函数
	 std::string getDept() const;
	 std::string getId() const;
	 std::string getName() const;
	 std::string getPassword() const;
	 //检查密码合法性
	 bool checkPassword(const std::string& p);
	 //设置函数
	 bool setPassword(Data& db);
	 bool setName(Data& db);
	 bool setDept(Data& db);
};
#endif
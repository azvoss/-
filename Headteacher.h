#ifndef HEADTEACHER_H
#define HEADTEACHER_H
#include "User.h"
#include <string>
#include "Data.h"
class Headteacher{
private:
	 User user;
	 std::string className;
public:
	 Headteacher(const User& u, std::string a = "");
	 //获得资料函数
	 std::string getClassName() const;
	 std::string getId() const;
	 std::string getName() const;
	 std::string getPassword() const;
	 //检查密码合法性
	 bool checkPassword(const std::string& p);
	 //设置函数
	 bool setPassword(Data& db);
	 bool setName(Data& db);
	 bool setClass(Data& db);
};
#endif
#ifndef STUDENT_H
#define STUDENT_H
#include "User.h"
#include <string>
#include <vector>
#include "Data.h"
class Student{
private:
	 User user;
	 std::string className;
	 std::string dept;
public:
	 Student(const User& u, std::string a = "", std::string b = "");
	 //获得数据
	 std::string getClassName() const;
	 std::string getDept() const;
	 std::string getId() const;
	 std::string getName() const;
	 std::string getPassword() const;
	 //设置函数
	 bool setPassword(Data& db);
	 bool setClass(Data& db);
	 bool setDept(Data& db);
	 bool setName(Data& db);
	 //检查密码安全性
	 bool checkPassword(const std::string& p);
};
#endif
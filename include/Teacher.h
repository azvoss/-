#ifndef TEACHER_H
#define TEACHER_H
#include "User.h"
#include <string>
#include "Data.h"
class Teacher{
private:
	 User user;
public:
	 Teacher(const User &u);
	 //获得资料函数
	 std::string getId() const;
	 std::string getName() const;
	 std::string getPassword() const;
	 //检查密码合法性
	 bool checkPassword(const std::string& p);
	 //设置函数
	 bool setPassword(Data& db);
	 bool setName(Data& db);
};
#endif
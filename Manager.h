#ifndef MANAGER_H
#define MANAGER_H
#include "User.h"
#include "Data.h"
class Manager{
private:
	 User user;
public:
	 Manager(const User& u);
	 //获得资料函数
	 std::string getName() const;
	 std::string getId() const;
	 std::string getPassword() const;
	 //检查密码合法性
	 bool checkPassword(const std::string& p);
	 bool AddStudent(Data& db, const std::string& newid, const std::string& name, const std::string& classname, const std::string& dept);
	 bool RemoveStudent(Data& db, const std::string& id);
	 bool AddTeacher(Data& db, const std::string& newid, const std::string& name);
	 bool RemoveTeacher(Data& db, const std::string& id);
	 bool AddHeadteacher(Data& db, const std::string& newid, const std::string& name, const std::string& classname);
	 bool RemoveHeadteacher(Data& db, const std::string& id);
	 bool AddHeaddept(Data& db, const std::string& newid, const std::string& name, const std::string& dept);
	 bool RemoveHeaddept(Data& db, const std::string& id);
};
#endif
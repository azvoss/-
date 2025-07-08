#ifndef USER_H
#define USER_H
#include<string>
class User{
private:
	 std::string id;
	 std::string name;
	 std::string password;
public:
	 User(std::string a = "", std::string b = "", std::string c = "0000");
	 //获得资料函数
	 std::string getId() const;
	 std::string getName() const;
	 std::string getPassword() const;
	 //检查密码合法性
	 bool checkPassword(const std::string& p)const;	  //检查密码是否正确
	 //设置函数
	 void setPassword(const std::string& p);		  //设置密码
	 void setName(const std::string& n);
};
#endif
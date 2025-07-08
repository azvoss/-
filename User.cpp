#include "User.h"
#include <iostream>
using namespace std;
User::User(string a, string b, string c) : id(a), name(b), password(c) {}
string User::getId() const {			//获得id
	 return id;
}
string User::getName() const {		//获得名字
	 return name;
}
string User::getPassword() const {		 //获得密码
	 return password;
}
bool User::checkPassword(const string& p) const {		  //检查密码合法性
	 return password == p;
}
void User::setPassword(const string& p) {			//设置密码
	 password = p;
}
void User::setName(const string& n) {		  //设置名字
	 name = n;
}
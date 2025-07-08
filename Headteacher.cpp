#include "Headteacher.h"
#include <iostream>
using namespace std;
Headteacher::Headteacher(const User& u, string a) :user(u), className(a) {}
string Headteacher::getClassName() const {		//获得负责课室
	 return className;
}
string Headteacher::getId() const {			  //获得班主任编号
	 return user.getId();
}
string Headteacher::getName() const {		  //获得名字
	 return user.getName();
}
string Headteacher::getPassword() const {			//获得密码
	 return user.getPassword();
}
bool Headteacher::checkPassword(const string& p) {		  //检查密码
	 return user.checkPassword(p);
}
bool Headteacher::setPassword(Data& db) {		 //设置密码
	 cout << "Reset Password" << endl
		  << "Enter your new password: ";
	 string p;
	 cin >> p;
	 user.setPassword(p);
	 auto* stmt = db.prepare("UPDATE headteacher SET password = ? WHERE id = ?;");
	 if (!stmt) {
		  cerr << "Failed to prepare password-update statement." << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, p.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, user.getId().c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "Failed to update password: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "reset password successful" << endl;
	 return true;
}
bool Headteacher::setName(Data& db) {			//设置名字
	 cout << "Enter the current name: ";
	 string newname;
	 cin >> newname;
	 user.setName(newname);
	 auto* stmt = db.prepare("UPDATE headteacher SET name = ? WHERE id = ?;");
	 if (!stmt) {
		  cerr << "Failed to prepare name-update statement." << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, newname.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, user.getId().c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "Failed to update name: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "Name reset successful." << endl;
	 return true;
}
bool Headteacher::setClass(Data& db) {		//设置负责班级
	 cout << "Enter the current Class: ";
	 string newclass;
	 cin >> newclass;
	 className = newclass;
	 auto* stmt = db.prepare("UPDATE headteacher SET classname = ? WHERE id = ?;");
	 if (!stmt) {
		  cerr << "Failed to prepare class-update statement." << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, newclass.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, user.getId().c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "Failed to update class: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "Classroom reset successful." << endl;
	 return true;
}
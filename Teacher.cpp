#include "Teacher.h"
#include <iostream>
using namespace std;
Teacher::Teacher(const User& u) :user(u) {}
string Teacher::getId() const {		//获得教师编号
	 return user.getId();
}
string Teacher::getName() const {		 //获得名字
	 return user.getName();
}
string Teacher::getPassword() const {		  //获得密码
	 return user.getPassword();
}
bool Teacher::checkPassword(const std::string& p) {	  //检查密码
	 return user.checkPassword(p);
}
bool Teacher::setPassword(Data& db) {			//设置密码
	 cout << "Reset Password" << endl
		  << "Enter your new password: ";
	 string p;
	 cin >> p;
	 user.setPassword(p);
	 auto* stmt = db.prepare("UPDATE teacher SET password = ? WHERE id = ?;");
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
bool Teacher::setName(Data& db) {		  //设置名字
	 cout << "Enter the current name: ";
	 string newname;
	 cin >> newname;
	 user.setName(newname);
	 auto* stmt = db.prepare("UPDATE teacher SET name = ? WHERE id = ?;");
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
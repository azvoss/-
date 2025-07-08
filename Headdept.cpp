#include "Headdept.h"
#include <iostream>
using namespace std;
Headdept::Headdept(const User& u, std::string a) :user(u), dept(a) {}
string Headdept::getDept() const {		 //获得负责科系
	 return dept;
}
string Headdept::getId() const {			 //获得系主任编号
	 return user.getId();
}
string Headdept::getName() const {		 //获得名字
	 return user.getName();
}
string Headdept::getPassword() const {		  //获得密码
	 return user.getPassword();
}
bool Headdept::checkPassword(const string& p) {		 //检查密码合法性
	 return user.checkPassword(p);
}
bool Headdept::setPassword(Data& db) {		//设置密码
	 cout << "Reset Password" << endl
		  << "Enter your new password: ";
	 string p;
	 cin >> p;
	 user.setPassword(p);
	 auto* stmt = db.prepare("UPDATE headdept SET password = ? WHERE id = ?;");
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
bool Headdept::setName(Data& db) {		  //设置名字
	 cout << "Enter the current name: ";
	 string newname;
	 cin >> newname;
	 user.setName(newname);
	 auto* stmt = db.prepare("UPDATE headdept SET name = ? WHERE id = ?;");
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
	 cout << "\nName reset successful." << endl;
	 return true;
}
bool Headdept::setDept(Data& db) {		  //设置负责科系
	 cout << "Enter the current Dept: ";
	 string newdept;
	 cin >> newdept;
	 dept = newdept;
	 auto* stmt = db.prepare("UPDATE headdept SET dept = ? WHERE id = ?;");
	 if (!stmt) {
		  cerr << "Failed to prepare dept-update statement." << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, newdept.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, user.getId().c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "Failed to update dept: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "\nDept reset successful." << endl;
	 return true;
}
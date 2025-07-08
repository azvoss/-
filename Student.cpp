#include "Student.h"
#include <iostream>
using namespace std;
Student::Student(const User& u, string a, string b) : user(u), className(a), dept(b) {}
string Student::getClassName() const {		  //获得所在班级
	 return className;
}
string Student::getDept() const {		 //获得所在科系
	 return dept;
}
string Student::getId() const {			 //获得学号
	 return user.getId();
}
string Student::getName() const {		 //获得名字
	 return user.getName();
}
string Student::getPassword() const {		  //获得密码
	 return user.getPassword();
}
bool Student::checkPassword(const string& p) {		 //检查密码
	 return user.checkPassword(p);
}
bool Student::setPassword(Data& db) {			//设置密码
	 cout << "Reset your password.\n"
		  << "new password: ";
	 string p;
	 cin >> p;
	 user.setPassword(p);
	 auto* stmt = db.prepare("UPDATE student SET password = ? WHERE id = ?;");
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
	 cout << "reset password successful.\n" << endl;
	 return true;
}
bool Student::setClass(Data& db) {		  //设置课室
	 cout << "Enter the current class: ";
	 string newclass;
	 cin >> newclass;
	 className = newclass;
	 auto* stmt = db.prepare("UPDATE student SET classname = ? WHERE id = ?;");
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
bool Student::setDept(Data& db) {		  //设置科系
	 cout << "Enter the current dept: ";
	 string newdept;
	 cin >> newdept;
	 dept = newdept;
	 auto* stmt = db.prepare("UPDATE student SET dept = ? WHERE id= ?;");
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
	 cout << "Department reset successful." << endl;
	 return true;
}
bool Student::setName(Data& db) {		  //设置名字
	 cout << "Enter the current name: ";
	 string newname;
	 cin >> newname;
	 user.setName(newname);
	 auto* stmt = db.prepare("UPDATE student SET name = ? WHERE id = ?;");
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
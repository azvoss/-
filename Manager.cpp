#include "Manager.h"
#include <iostream>
using namespace std;
Manager::Manager(const User& u) :user(u) {}
string Manager::getId() const {		//获得id
	 return user.getId();
}
string Manager::getName() const {		 //获得名字
	 return user.getName();
}
string Manager::getPassword() const {		  //获得密码
	 return user.getPassword();
}
bool Manager::checkPassword(const string& p) {		 //检查密码合法性
	 return user.checkPassword(p);
}
bool Manager::AddStudent(Data& db, const string& newid, const string& name, const string& classname, const string& dept) {
	 auto* stmt = db.prepare("INSERT INTO student(id,name,password,classname,dept)"
		  " VALUES(?,?,?,?,?);");
	 if (!stmt) {
		  cerr << "Failed to prepare statement for adding student." << endl;
		  return false;
	 }
	 string pwd = "0000";
	 sqlite3_bind_text(stmt, 1, newid.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 3, pwd.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 4, classname.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 5, dept.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "Failed to add student to database: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "Student added ID: " << newid << ", Name: " << name << endl;
	 return true;
}
bool Manager::RemoveStudent(Data& db,const string& id) {
	 if (auto* stmt = db.prepare("DELETE FROM score WHERE student_id = ?;")) {
		  sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_step(stmt);
		  sqlite3_finalize(stmt);
	 }
	 auto* stmt2 = db.prepare("DELETE FROM student WHERE id = ?;");
	 if (!stmt2) {
		  cerr << "Failed to prepare statement for removing student." << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt2, 1, id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt2) == SQLITE_DONE);
	 sqlite3_finalize(stmt2);
	 if (!ok) {
		  cerr << "Failed to remove student from database: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "Deleted student ID: " << id << endl;
	 return true;
}
bool Manager::AddTeacher(Data& db,const string& newid,const string& name) {
	 auto* stmt = db.prepare("INSERT INTO teacher(id,name,password) VALUES(?,?,?);");
	 if (!stmt) {
		  cerr << "Failed to prepare insert teacher" << endl;
		  return false;
	 }
	 string pwd = "0000";
	 sqlite3_bind_text(stmt, 1, newid.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 3, pwd.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "DB insert teacher failed: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "Add teacher ID: " << newid << ", Name: " << name << endl;
	 return true;
}
bool Manager::RemoveTeacher(Data& db, const string& id) {
	 auto* stmt = db.prepare("DELETE FROM teacher WHERE id = ?;");
	 if (!stmt) {
		  cerr << "Failed to prepare DELETE teacher" << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 if (!ok) {
		  cerr << "DB delete teacher failed: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "Deleted teacher ID: " << id << endl;
	 return true;
}
bool Manager::AddHeadteacher(Data& db, const string& newid,const string& name, const string& classname) {
	 auto* stmt = db.prepare("INSERT INTO headteacher(id,name,password,classname) VALUES(?,?,?,?);");
	 if (!stmt) {
		  cerr << "Failed to prepare INSERT headteacher" << endl;
		  return false;
	 }
	 string pwd = "0000";
	 sqlite3_bind_text(stmt, 1, newid.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 3, pwd.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 4, classname.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "DB insert headteacher failed: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "Add Headteacher ID: " << newid << ", Name: " << name << endl;
	 return true;
}
bool Manager::RemoveHeadteacher(Data& db, const string& id) {
	 auto* stmt = db.prepare("DELETE FROM headteacher WHERE id = ?;");
	 if (!stmt) {
		  cerr << "Failed to prepare DELETE headteacher" << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "DB delete headteacher failed: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "Deleted Headteacher ID: " << id << endl;
	 return true;
}
bool Manager::AddHeaddept(Data& db, const string& newid, const string& name, const string& dept) {
	 auto* stmt = db.prepare("INSERT INTO headdept(id,name,password,dept) VALUES(?,?,?,?);");
	 if (!stmt) {
		  cerr << "Failed to prepare INSERT headdept" << endl;
		  return false;
	 }
	 string pwd = "0000";
	 sqlite3_bind_text(stmt, 1, newid.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 3, pwd.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 4, dept.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "D insert headdept failed: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "Added Headdept ID: " << newid << ", Name: " << name << endl;
	 return true;
}
bool Manager::RemoveHeaddept(Data& db, const string& id) {
	 auto* stmt = db.prepare("DELETE FROM headdept WHERE id = ?;");
	 if (!stmt) {
		  cerr << "Failed to prepare DELETE headdept" << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "DB delete headdept failed: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 cout << "Deleted Headdept ID: " << id << endl;
	 return true;
}
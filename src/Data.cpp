#include "Data.h"
#include <iostream>
using namespace std;
Data::Data(const string& filename):db_(nullptr) {		  //打开文件
	 if (sqlite3_open(filename.c_str(), &db_) != SQLITE_OK) {		  //打开数据库
		  cerr << "Can't open database: " << sqlite3_errmsg(db_) << endl;
		  sqlite3_close(db_);		  //关闭数据库
		  db_ = nullptr;
	 }
	 else {
		  cout << "Opened database successfully" << endl;
	 }
}
Data::~Data() {
	 if (db_) {
		  sqlite3_close(db_);
	 }
}
bool Data::exec(const string& sql) {		  //执行SQL语句
	 if (!db_) {
		  cerr << "Database not opened." << endl;
		  return false;
	 }
	 char* err = nullptr;
	 if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
		  cerr << "SQL error: " << err << endl;
		  sqlite3_free(err);
		  return false;
	 }
	 return true;
}
sqlite3_stmt* Data::prepare(const string& sql) {		  //准备SQL语句
	 if (!db_) {
		  cerr << "Database not opened." << endl;
		  return nullptr;
	 }
	 sqlite3_stmt* stmt = nullptr;
	 if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		  cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << endl;
	 }
	 return stmt;
}
sqlite3* Data::getDB() const { return db_; }
#ifndef DATA_H
#define DATA_H
#include "sqlite3.h"
#include <string>
class Data{
private:
	 sqlite3* db_ = nullptr; // 数据库指针
public:
	 Data(const std::string& filename);
	 ~Data();
	 bool exec(const std::string& sql);
	 sqlite3_stmt* prepare(const std::string& sql);
	 sqlite3* getDB() const;
};
#endif
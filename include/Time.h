#ifndef TIME_H
#define TIME_H
#include <string>
class Time{
private:
	 int startmin;
	 int endmin;
public:
	 Time(std::string time);
	 //辅助函数
	 int turn_min(std::string t);
	 //检查&判断函数
	 bool checkTime(const Time& t);
	 static bool conficttime(const std::string& t);
};
#endif
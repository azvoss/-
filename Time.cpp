#include "Time.h"
#include <iostream>
#include <cctype>
using namespace std;
Time::Time(string time) {
	 auto index = time.find('-');
	 startmin = turn_min(time.substr(0, index));
	 endmin = turn_min(time.substr(index + 1));
}
int Time::turn_min(string t) {		//转换成分钟
	 int temp = stoi(t);
	 int h = temp / 100;
	 int m = temp % 100;
	 return h * 60 + m;
}
bool Time::checkTime(const Time& other) {		  //检查时间冲突
	 //start=a,end=b;t.start=c,t.end=d
	 if (other.startmin<endmin && other.endmin>startmin) return false;
	 return true;
}
bool Time::conficttime(const string& t) {			//检查时间输入格式
	 if (t.size() != 9 || t[4] != '-') return false;
	  string s1 = t.substr(0, 4);
	  string s2 = t.substr(5, 4);
	  for (char c : s1 + s2) {
			if (!isdigit(c)) return false;
	  }
	  int start = stoi(s1);
	  int end = stoi(s2);
	  int sh = start / 100;
	  int sm = start % 100;
	  int eh = end / 100;
	  int em = end % 100;
	  if (sh < 0 || sh>23 || sm < 0 || sm>59) return false;
	  if (eh < 0 || eh>23 || em < 0 || em>59) return false;
	  return start < end;
}
#include "Course.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;
Course::Course(const string a, const string b, const string c, const string d, const string e, const string f):
	 course_id(a), course_name(b), course_teacher(c), course_day(d), course_time(e), course_class(f) {}
string Course::getCourseId() const {		  //获得id
	 return course_id;
}
string Course::getCourseName() const {		  //获得名字
	 return course_name;
}
string Course::getCourseTeacher() const {			//获得负责老师
	 return course_teacher;
}
string Course::getCourseDay() const {		  //获得星期几
	 return course_day;
}
string Course::getCourseTime() const {		  //获得当天时间
	 return course_time;
}
string Course::getCourseClass() const {	  //获得课室
	 return course_class;
}
bool Course::add(Data& db, const string& student_id) {		//学生添加课程 初始分数设置
	 auto* stmt = db.prepare("INSERT OR REPLACE INTO score "
		  "(student_id,course_id,score) VALUES(?,?,NULL);");
	 if (!stmt) return false;
	 sqlite3_bind_text(stmt, 1, student_id.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, course_id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = sqlite3_step(stmt) == SQLITE_DONE;
	 sqlite3_finalize(stmt);
	 if (!ok) return false;
	 student_score[student_id] = "-";
	 return true;
}
bool Course::remove(Data& db, const string& student_id) {		 //删除学生所在课程
	 auto* stmt = db.prepare("DELETE FROM score WHERE student_id=? AND course_id=?;");
	 if (!stmt) return false;
	 sqlite3_bind_text(stmt, 1, student_id.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, course_id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = sqlite3_step(stmt) == SQLITE_DONE;
	 sqlite3_finalize(stmt);
	 if (!ok) return false;
	 student_score.erase(student_id);
	 return true;
}
bool Course::Incourse(const string student_id) const {		//检查学生是否有在课程
	 return student_score.find(student_id) != student_score.end();			//存在True,不存在false
}
string Course::getScore(const string student_id) const {			 //获得学生分数（特定学生）
	 auto s = student_score.find(student_id);
	 if (s != student_score.end()) {
		  return s->second;
	 }
	 return "-";
}
const map<string, string>& Course::getStudentScore() const {		  //获得学生分数（用以直接循环整体）
	 return student_score;
}
bool Course::addCourse(Data& db, vector<Course>& course, const string& name, const string& teacher, const string& day, const string& time, const string& classroom, string& outId) {
	 int newid = 1;
	 for (auto& c : course) {
		  int num = stoi(c.getCourseId().substr(1));
		  if (num >= newid) {
				newid = num + 1;
		  }
	 }
	 ostringstream os;
	 os << 'C' << setw(3) << setfill('0') << newid;
	 outId = os.str();
	 sqlite3_stmt* stmt = db.prepare("INSERT INTO course(id,name,teacher,day,time,class) VALUES(?,?,?,?,?,?);");
	 if (!stmt) return false;
	 sqlite3_bind_text(stmt, 1, outId.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 3, teacher.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 4, day.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 5, time.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 6, classroom.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = sqlite3_step(stmt) == SQLITE_DONE;
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "BD insert course failed: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 course.emplace_back(outId, name, teacher, day, time, classroom);		//添加课程到列表
	 return true;
}
bool Course::removeCourse(Data& db, vector<Course>& course, const string& teachername) {
	 if (course_teacher != teachername) {
		  cout << "You are not the teacher of this course." << endl;
		  return false;
	 }
	 cout << "\nFind Course Info\nCourse Id: " << course_id
		  << "\nCourse Name: " << course_name
		  << "\nCourse Day: " << course_day
		  << "\nCourse Time: " << course_time
		  << "\nCourse Classroom: " << course_class
		  << "\nPlease confirm delete (yes:1 no:0): ";
	 string confirm;
	 cin >> confirm;
	 if (confirm != "1") {
		  cout << "Course deletion cancelled." << endl;
		  return false;
	 }
	 sqlite3_stmt* s1 = db.prepare("DELETE FROM score WHERE course_id = ?;");
	 if (s1) {
		  sqlite3_bind_text(s1, 1, course_id.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_step(s1);
		  sqlite3_finalize(s1);
	 }
	 sqlite3_stmt* s2 = db.prepare("DELETE FROM course WHERE id = ?;");
	 if (!s2) {
		  cerr << "Failed to prepare course deletion statement." << endl;
		  return false;
	 }
	 sqlite3_bind_text(s2, 1, course_id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = sqlite3_step(s2) == SQLITE_DONE;
	 sqlite3_finalize(s2);
	 if (!ok) return false;
	 auto it = find_if(course.begin(), course.end(), [&](const Course& c) {
		  return c.getCourseId() == course_id;
		  });
	 if (it != course.end()) {
		  course.erase(it);
	 }
	 return true;
}
bool Course::updateScoreDB(Data& db, const string& student_id, const string& score) {		  //评分到数据库
	 auto* stmt = db.prepare("UPDATE score SET score = ? WHERE student_id = ? AND course_id = ?;");
	 if (!stmt) {
		  cerr << "Failed to prepare score update statement." << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, score.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, student_id.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 3, course_id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = sqlite3_step(stmt) == SQLITE_DONE;
	 sqlite3_finalize(stmt);
	 if (!ok) {
		  cerr << "Failed to update score: " << sqlite3_errmsg(db.getDB()) << endl;
		  return false;
	 }
	 student_score[student_id] = score;		//更新分数到课程列表
	 return true;
}
void Course::setScore(const string student_id, const string score) {			 //初始评分
	 student_score[student_id] = score;
}
bool Course::setClass(Data& db, const string& newclass) {		 //设置课室
	 course_class = newclass;
	 auto* stmt = db.prepare("UPDATE course SET class = ? WHERE id = ?;");
	 if (!stmt) {
		  cerr << "Failed to prepare class update statement." << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, newclass.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, course_id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = sqlite3_step(stmt) == SQLITE_DONE;
	 if (!ok) {
		  cerr << "Failed to update class: " << sqlite3_errmsg(db.getDB()) << endl;
		  sqlite3_finalize(stmt);
		  return false;
	 }
	 sqlite3_finalize(stmt);
	 return true;
}
bool Course::setName(Data& db, const string& cn) {			//设置名字
	 course_name = cn;
	 auto* stmt = db.prepare("UPDATE course SET name = ? WHERE id = ?;");		//更新课程名字
	 if (!stmt) {
		  cerr << "Failed to prepare name update statement." << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, cn.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, course_id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = sqlite3_step(stmt) == SQLITE_DONE;
	 if (!ok) {
		  cerr << "Failed to update course name: " << sqlite3_errmsg(db.getDB()) << endl;
		  sqlite3_finalize(stmt);
		  return false;
	 }
	 sqlite3_finalize(stmt);
	 return true;
}
bool Course::setDay(Data& db, const string& d) {		  //设置星期几
	 course_day = d;
	 auto* stmt = db.prepare("UPDATE course SET day = ? WHERE id = ?;");		//更新课程星期几
	 if (!stmt) {
		  cerr << "Failed to prepare day update statement." << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, d.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, course_id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = sqlite3_step(stmt) == SQLITE_DONE;
	 if (!ok) {
		  cerr << "Failed to update course day: " << sqlite3_errmsg(db.getDB()) << endl;
		  sqlite3_finalize(stmt);
		  return false;
	 }
	 sqlite3_finalize(stmt);
	 return true;
}
bool Course::setTime(Data& db, const string& t) {	  //设置时间
	 course_time = t;
	 auto* stmt = db.prepare("UPDATE course SET time = ? WHERE id = ?;");		//更新课程时间
	 if (!stmt) {
		  cerr << "Failed to prepare time update statement." << endl;
		  return false;
	 }
	 sqlite3_bind_text(stmt, 1, t.c_str(), -1, SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt, 2, course_id.c_str(), -1, SQLITE_TRANSIENT);
	 bool ok = sqlite3_step(stmt) == SQLITE_DONE;
	 if (!ok) {
		  cerr << "Failed to update course time: " << sqlite3_errmsg(db.getDB()) << endl;
		  sqlite3_finalize(stmt);
		  return false;
	 }
	 sqlite3_finalize(stmt);
	 return true;
}
void Course::clearScore() {
	 student_score.clear();
}
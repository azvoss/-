#ifndef COURSE_H
#define COURSE_H
#include <string>
#include <map>
#include "Data.h"
#include <vector>
class Course{
private:
	 std::string course_id;
	 std::string course_name;
	 std::string course_teacher;
	 std::string course_day;
	 std::string course_time;
	 std::string course_class;
	 std::map<std::string, std::string> student_score;
public:
	 Course(const std::string a = "", const std::string b = "", const std::string c = "", const std::string d = "", const std::string e = "", const std::string f = "");
	 //获得资料
	 std::string getCourseId() const;
	 std::string getCourseName() const;
	 std::string getCourseTeacher() const;
	 std::string getCourseDay() const;
	 std::string getCourseTime() const;
	 std::string getCourseClass() const;
	 //学生
	 bool add(Data& db, const std::string& student_id);				//添加课程
	 bool remove(Data& db, const std::string& student_id);			//退课
	 bool Incourse(const std::string student_id) const;		  //检查学生是否有在课程内
	 std::string getScore(const std::string student_id) const;			 //以学生id 输出分数
	 const std::map<std::string, std::string>& getStudentScore() const;		  //根据学生id 输出分数（用来输出学生id所对应的课程分数）
	 //教师
	 static bool addCourse(Data& db,std::vector<Course>& course,const std::string& name, const std::string& teacher, const std::string& day, const std::string& time, const std::string& classroom, std::string& outId);
	 bool removeCourse(Data& db,std::vector<Course>& course,const std::string& teachername);
	 bool updateScoreDB(Data& db, const std::string& student_id, const std::string& score);
	 void setScore(const std::string student_id, const std::string score);		  //评分
	 bool setClass(Data& db, const std::string& newclass);
	 bool setName(Data& db, const std::string& cn);
	 bool setDay(Data& db, const std::string& d);
	 bool setTime(Data& db, const std::string& t);
	 void clearScore();
};
#endif
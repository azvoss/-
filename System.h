#ifndef SYSTEM_H
#define SYSTEM_H
#include "Course.h"
#include "Headdept.h"
#include "Headteacher.h"
#include "Student.h"
#include "Teacher.h"
#include "User.h"
#include "Time.h"
#include "Manager.h"
#include "Data.h"
#include <vector>
#include <string>
class System{
private:
	 //建立列表
	 std::vector<Student> student;					//学生列表
	 std::vector<Teacher> teacher;					//教师列表
	 std::vector<Headteacher> headteacher;			//班主任列表
	 std::vector<Headdept> headdept;					//系主任列表
	 std::vector<Course> course;						//课程列表
	 std::vector<Time> time;							//星期列表
	 std::vector<Manager> manager;		 			//管理者列表
	 Data db_;				//数据库对象
public:
	 System(const std::string& dbfile);
	 std::string doubleToString(double val);
	 //查找函数
	 int findStudent_Index(const std::string& id);				  //查找学生
	 int findTeacher_Index(const std::string& id);				  //查找教师
	 int findHeadteacher_Index(const std::string& id);			  //查找班主任
	 int findHeaddept_Index(const std::string& id);				  //查找系主任
	 int findCourse_Index(const std::string& id);				  //查找课程
	 int findManager_Index(const std::string& id);				  //查找管理者
	 //数据&初始界面
	 void run();
	 //加载数据
	 void loadStudentsFromDB();
	 void loadTeachersFromDB();
	 void loadHeadteachersFromDB();
	 void loadHeaddeptsFromDB();
	 void loadManagersFromDB();
	 void loadCoursesFromDB();
	 void loadScoresFromDB();
	 //迁移数据
	 void importStudentsFromFile(const std::string& txtFile);
	 void importTeacherFromFile(const std::string& txtFile);
	 void importHeadteacherFromFile(const std::string& txtFile);
	 void importHeadDeptFromFile(const std::string& txtFile);
	 void importManagerFromFile(const std::string& txtFile);
	 void importCourseFromFile(const std::string& txtFile);
	 //界面
	 void line(const std::string& content, int num = 10, char c = ' ', bool n = true);
	 void Student_Menu(Student& stu);					 //学生界面
	 void Teacher_Menu(Teacher& tea);					 //教师界面
	 void Headteacher_Menu(Headteacher& ht);			 //班主任界面
	 void Headdept_Menu(Headdept& hd);					 //系主任界面
	 void Manager_Menu(Manager& m);						 //管理者界面
};
#endif
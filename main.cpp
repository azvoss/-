#include "System.h"
#include <iostream>
using namespace std;
int main() {
	 System sys("school.db");
	 //迁移数据进去数据库
	 /*sys.importStudentsFromFile("Student List.txt");
	 sys.importTeacherFromFile("Teacher List.txt");
	 sys.importHeadteacherFromFile("Head Teacher List.txt");
	 sys.importHeadDeptFromFile("Head Dept List.txt");
	 sys.importManagerFromFile("Manager.txt");
	 sys.importCourseFromFile("Course List.txt");*/
	 sys.run();
	 return 0;
}
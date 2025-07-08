#include "System.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>
using namespace std;
System::System(const string& dbfile):db_(dbfile){
	 sqlite3_exec(db_.getDB(), "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
	 bool ok;
	 // 1) 学生表
	 ok=db_.exec(R"(
      CREATE TABLE IF NOT EXISTS student (
        id        TEXT PRIMARY KEY,
        name      TEXT,
        password  TEXT,
        classname TEXT,
        dept      TEXT
      );
    )");
	 if(!ok){
		  std::cerr << "❌ 建 student 表失败\n";
	 }
	 // 2) 普通老师表
	 ok=db_.exec(R"(
      CREATE TABLE IF NOT EXISTS teacher (
        id       TEXT PRIMARY KEY,
        name     TEXT,
        password TEXT
      );
    )");
	 if (!ok) {
		  std::cerr << "❌ 建 teacher 表失败\n";
	 }
	 // 3) 班主任表
	 ok=db_.exec(R"(
      CREATE TABLE IF NOT EXISTS headteacher (
        id        TEXT PRIMARY KEY,
        name      TEXT,
        password  TEXT,
        classname TEXT
      );
    )");
	 if (!ok) {
		  std::cerr << "❌ 建 headteacher 表失败\n";
	 }
	 // 4) 系主任表
	 ok=db_.exec(R"(
      CREATE TABLE IF NOT EXISTS headdept (
        id       TEXT PRIMARY KEY,
        name     TEXT,
        password TEXT,
        dept     TEXT
      );
    )");
	 if (!ok) {
		  std::cerr << "❌ 建 headdept 表失败\n";
	 }
	 // 5) 管理员表
	 ok=db_.exec(R"(
      CREATE TABLE IF NOT EXISTS manager (
        id       TEXT PRIMARY KEY,
        name     TEXT,
        password TEXT
      );
    )");
	 if (!ok) {
		  std::cerr << "❌ 建 manager 表失败\n";
	 }
	 ok=db_.exec(R"(
		CREATE TABLE IF NOT EXISTS course (
		  id      TEXT PRIMARY KEY,
		  name    TEXT,
		  teacher TEXT,
		  day     TEXT,
		  time    TEXT,
		  class   TEXT,
		   FOREIGN KEY(teacher) REFERENCES teacher(id) ON DELETE CASCADE
		);
	 )");
	 if (!ok) {
		  std::cerr << "❌ 建 course 表失败\n";
	 }
	 ok=db_.exec(R"(
		CREATE TABLE IF NOT EXISTS score (
		  student_id TEXT,
		  course_id  TEXT,
		  score      REAL,
		  PRIMARY KEY(student_id,course_id),
		  FOREIGN KEY(student_id) REFERENCES student(id),
		  FOREIGN KEY(course_id ) REFERENCES course(id)
		);
	 )");
	 if (!ok) {
		  std::cerr << "❌ 建 score 表失败\n";
	 }
}
string System::doubleToString(double val) {
	 std::ostringstream oss;
	 oss << std::defaultfloat << std::setprecision(15) << val;
	 return oss.str();
}
//查找index函数
int System::findStudent_Index(const string& id) {
	 for (int i = 0;i < student.size();i++) {
		  if (student[i].getId() == id) {
				return i;
		  }
	 }
	 return -1;
}
int System::findTeacher_Index(const string& id) {
	 for (int i = 0;i < teacher.size();i++) {
		  if (teacher[i].getId() == id) {
				return i;
		  }
	 }
	 return -1;
}
int System::findHeadteacher_Index(const string& id) {
	 for (int i = 0;i < headteacher.size();i++) {
		  if (headteacher[i].getId() == id) {
				return i;
		  }
	 }
	 return -1;
}
int System::findHeaddept_Index(const string& id) {
	 for (int i = 0;i < headdept.size();i++) {
		  if (headdept[i].getId() == id) {
				return i;
		  }
	 }
	 return -1;
}
int System::findCourse_Index(const string& id) {
	 for (int i = 0;i < course.size();i++) {
		  if (course[i].getCourseId() == id) {
				return i;
		  }
	 }
	 return -1;
}
int System::findManager_Index(const string& id) {
	 for (int i = 0;i < manager.size();i++) {
		  if (manager[i].getId() == id) {
				return i;
		  }
	 }
	 return -1;
}
//数据&初始界面
void System::loadStudentsFromDB() {
	 student.clear();
	 auto* stmt = db_.prepare("SELECT id,name,password,classname,dept FROM student;");
	 if (!stmt) {
		  cerr << "Error preparing student statement: " << endl;
		  return;
	 }
	 bool ok = false;
	 while (sqlite3_step(stmt) == SQLITE_ROW) {
		  string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		  string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		  string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		  string classname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		  string dept = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
		  student.emplace_back(User(id, name, password), classname, dept);
		  ok = true;
	 }
	 if (ok) {
		  cout << "Success preparing student statement" << endl;
	 }
	 sqlite3_finalize(stmt);
}
void System::loadTeachersFromDB() {
	 teacher.clear();
	 auto* stmt = db_.prepare("SELECT id,name,password FROM teacher;");
	 if (!stmt) {
		  cerr << "Error preparing teacher statement: " << endl;
		  return;
	 }
	 bool ok = false;
	 while (sqlite3_step(stmt) == SQLITE_ROW) {
		  string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		  string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		  string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		  teacher.emplace_back(User(id, name, password));
		  ok = true;
	 }
	 if (ok) {
		  cout << "Success preparing teacher statement" << endl;
	 }
	 sqlite3_finalize(stmt);
}
void System::loadHeadteachersFromDB() {
	 headteacher.clear();
	 auto* stmt = db_.prepare("SELECT id,name,password,classname FROM headteacher;");
	 if (!stmt) {
		  cerr << "Error preparing headteacher statement: " << sqlite3_errmsg(db_.getDB()) << endl;
		  return;
	 }
	 bool ok = false;
	 while (sqlite3_step(stmt) == SQLITE_ROW) {
		  const char* id_cstr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		  const char* name_cstr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		  const char* pass_cstr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		  const char* class_cstr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		  if (!id_cstr || !name_cstr || !pass_cstr || !class_cstr) {
				cerr << "find error info headteacher_info,skip." << endl;
				continue;
		  }
		  string id = id_cstr;
		  string name = name_cstr;
		  string password = pass_cstr;
		  string classname = class_cstr;
		  if (id.empty() || name.empty() || password.empty() || classname.empty()) {
				cerr << "find empty headteacher info, skip." << endl;
				continue;
		  }
		  headteacher.emplace_back(User(id, name, password), classname);
		  ok = true;
	 }
	 if (ok) {
		  cout << "Success preparing headteacher statement" << endl;
	 }
	 sqlite3_finalize(stmt);
}
void System::loadHeaddeptsFromDB() {
	 headdept.clear();
	 auto* stmt = db_.prepare("SELECT id,name,password,dept FROM headdept;");
	 if (!stmt) {
		  cerr << "Error preparing headdept statement: " << endl;
		  return;
	 }
	 bool ok = false;
	 while (sqlite3_step(stmt) == SQLITE_ROW) {
		  string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		  string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		  string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		  string dept = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		  headdept.emplace_back(User(id, name, password),dept);
		  ok = true;
	 }
	 if (ok) {
		  cout << "Success preparing headdept statement" << endl;
	 }
	 sqlite3_finalize(stmt);
}
void System::loadManagersFromDB() {
	 manager.clear();
	 auto* stmt = db_.prepare("SELECT id,name,password FROM manager;");
	 if (!stmt) {
		  cerr << "Error preparing manager statement: " << endl;
		  return;
	 }
	 bool ok = false;
	 while (sqlite3_step(stmt) == SQLITE_ROW) {
		  string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		  string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		  string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		  manager.emplace_back(User(id, name, password));
		  ok = true;
	 }
	 if (ok) {
		  cout << "Success preparing manager statement" << endl;
	 }
	 sqlite3_finalize(stmt);
}
void System::loadCoursesFromDB() {
	 course.clear();
	 auto* stmt = db_.prepare("SELECT id,name,teacher,day,time,class FROM course;");
	 if (!stmt) {
		  cerr << "Error preparing course statement: " << endl;
		  return;
	 }
	 bool ok = false;
	 while (sqlite3_step(stmt) == SQLITE_ROW) {
		  Course c(
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)),
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))
		  );
		  ok = true;
		  course.push_back(move(c));
	 }
	 if (ok) {
		  cout << "Success preparing course statement" << endl;
	 }
	 sqlite3_finalize(stmt);
}
void System::loadScoresFromDB() {
	 for (auto& c : course) {
		  c.clearScore();
	 }
	 auto* stmt = db_.prepare("SELECT student_id,course_id,score FROM score;");
	 if (!stmt) {
		  cerr << "Error preparing score statement: " << endl;
		  return;
	 }
	 bool ok = false;
	 while (sqlite3_step(stmt) == SQLITE_ROW) {
		  string sid = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		  string cid = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		  bool isNull = sqlite3_column_type(stmt, 2) == SQLITE_NULL;
		  double dscore = sqlite3_column_double(stmt, 2);
		  string sc = isNull ? "-" : doubleToString(dscore);
		  for (auto& c : course) {
				if (c.getCourseId() == cid) {
					 c.setScore(sid, sc);
					 break;
				}
		  }
		  ok = true;
	 }
	 if (ok) {
		  cout << "Success preparing score statement" << endl;
	 }
	 sqlite3_finalize(stmt);
}
void System::importStudentsFromFile(const std::string& txtFile) {
	 ifstream fin(txtFile);
	 if (!fin.is_open()) {
		  cerr << "Error opening shutdent file: " << txtFile << endl;
		  return;
	 }
	 cout << "Importing students from " << txtFile << endl;
	 db_.exec("BEGIN TRANSACTION;"); // 开始事务
	 string line;
	 while (getline(fin, line)) {
		  if (line.empty()) continue;
		  istringstream iss(line);
		  string id, name, pwd, cls, dep;
		  if (!(iss >> id >> name >> pwd >> cls >> dep)) {
				cerr << "Error reading line: " << line << endl;
				continue;
		  }
		  auto* stmt = db_.prepare("INSERT OR REPLACE INTO student(id,name,password,classname,dept)"" VALUES(?,?,?,?,?);");
		  if (!stmt) continue;
		  sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 3, pwd.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 4, cls.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 5, dep.c_str(), -1, SQLITE_TRANSIENT);
		  if (sqlite3_step(stmt) != SQLITE_DONE) {
				std::cerr << "插入失败: " << sqlite3_errmsg(db_.getDB()) << "\n";
		  }
		  sqlite3_finalize(stmt);
	 }
	 db_.exec("COMMIT;");
	 fin.close();
}
void System::importTeacherFromFile(const std::string& txtFile) {
	 std::ifstream fin(txtFile);
	 if (!fin) {
		  std::cerr << "无法打开教师文件: " << txtFile << std::endl;
		  return;
	 }
	 cout << "Importing students from " << txtFile << endl;
	 db_.exec("BEGIN TRANSACTION;");
	 std::string id, name, pwd;
	 while (fin >> id >> name >> pwd) {
		  auto* stmt = db_.prepare(
				"INSERT OR REPLACE INTO teacher(id,name,password) VALUES(?,?,?);"
		  );
		  if (!stmt) continue;
		  sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 3, pwd.c_str(), -1, SQLITE_TRANSIENT);
		  if (sqlite3_step(stmt) != SQLITE_DONE) {
				std::cerr << "插入教师失败: "
					 << sqlite3_errmsg(sqlite3_db_handle(stmt))
					 << std::endl;
		  }
		  sqlite3_finalize(stmt);
	 }
	 db_.exec("COMMIT;");
}
void System::importHeadteacherFromFile(const std::string& txtFile) {
	 std::ifstream fin(txtFile);
	 if (!fin) {
		  std::cerr << "无法打开班主任文件: " << txtFile << std::endl;
		  return;
	 }
	 cout << "Importing students from " << txtFile << endl;
	 db_.exec("BEGIN TRANSACTION;");
	 std::string id, name, pwd, cls;
	 while (fin >> id >> name >> pwd >> cls) {
		  auto* stmt = db_.prepare(
				"INSERT OR REPLACE INTO headteacher(id,name,password,classname) VALUES(?,?,?,?);"
		  );
		  if (!stmt) continue;
		  sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 3, pwd.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 4, cls.c_str(), -1, SQLITE_TRANSIENT);
		  if (sqlite3_step(stmt) != SQLITE_DONE) {
				std::cerr << "插入班主任失败: "
					 << sqlite3_errmsg(sqlite3_db_handle(stmt))
					 << std::endl;
		  }
		  sqlite3_finalize(stmt);
	 }
	 db_.exec("COMMIT;");
}
void System::importHeadDeptFromFile(const std::string& txtFile) {
	 std::ifstream fin(txtFile);
	 if (!fin) {
		  std::cerr << "无法打开系主任文件: " << txtFile << std::endl;
		  return;
	 }
	 cout << "Importing students from " << txtFile << endl;
	 db_.exec("BEGIN TRANSACTION;");
	 std::string id, name, pwd, dept;
	 while (fin >> id >> name >> pwd >> dept) {
		  auto* stmt = db_.prepare(
				"INSERT OR REPLACE INTO headdept(id,name,password,dept) VALUES(?,?,?,?);"
		  );
		  if (!stmt) continue;
		  sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 3, pwd.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 4, dept.c_str(), -1, SQLITE_TRANSIENT);
		  if (sqlite3_step(stmt) != SQLITE_DONE) {
				std::cerr << "插入系主任失败: "
					 << sqlite3_errmsg(sqlite3_db_handle(stmt))
					 << std::endl;
		  }
		  sqlite3_finalize(stmt);
	 }
	 db_.exec("COMMIT;");
}
void System::importManagerFromFile(const std::string& txtFile) {
	 std::ifstream fin(txtFile);
	 if (!fin) {
		  std::cerr << "无法打开管理员文件: " << txtFile << std::endl;
		  return;
	 }
	 cout << "Importing students from " << txtFile << endl;
	 db_.exec("BEGIN TRANSACTION;");
	 std::string id, name, pwd;
	 while (fin >> id >> name >> pwd) {
		  auto* stmt = db_.prepare(
				"INSERT OR REPLACE INTO manager(id,name,password) VALUES(?,?,?);"
		  );
		  if (!stmt) continue;
		  sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
		  sqlite3_bind_text(stmt, 3, pwd.c_str(), -1, SQLITE_TRANSIENT);
		  if (sqlite3_step(stmt) != SQLITE_DONE) {
				std::cerr << "插入管理员失败: "
					 << sqlite3_errmsg(sqlite3_db_handle(stmt))
					 << std::endl;
		  }
		  sqlite3_finalize(stmt);
	 }
	 db_.exec("COMMIT;");
}
void System::importCourseFromFile(const std::string& txtFile) {
	 std::ifstream fin(txtFile);
	 if (!fin.is_open()) {
		  std::cerr << "无法打开课程文件: " << txtFile << "\n";
		  return;
	 }
	 db_.exec("BEGIN TRANSACTION;");

	 std::string cid, cname, cteacher, cday, ctime, cclass;
	 int num_student;
	 while (fin >> cid >> cname >> cteacher >> cday >> ctime >> cclass >> num_student) {
		  // 1) 插入 course 表
		  {
				auto* stmt = db_.prepare(
					 "INSERT OR REPLACE INTO course "
					 "(id, name, teacher, day, time, class) "
					 " VALUES(?,?,?,?,?,?);"
				);
				if (stmt) {
					 sqlite3_bind_text(stmt, 1, cid.c_str(), -1, SQLITE_TRANSIENT);
					 sqlite3_bind_text(stmt, 2, cname.c_str(), -1, SQLITE_TRANSIENT);
					 sqlite3_bind_text(stmt, 3, cteacher.c_str(), -1, SQLITE_TRANSIENT);
					 sqlite3_bind_text(stmt, 4, cday.c_str(), -1, SQLITE_TRANSIENT);
					 sqlite3_bind_text(stmt, 5, ctime.c_str(), -1, SQLITE_TRANSIENT);
					 sqlite3_bind_text(stmt, 6, cclass.c_str(), -1, SQLITE_TRANSIENT);
					 sqlite3_step(stmt);
					 sqlite3_finalize(stmt);
				}
		  }

		  // 2) 读取已有学生和成绩，插入 enrollment 表
		  for (int i = 0; i < num_student; ++i) {
				std::string sid, sscore;
				fin >> sid >> sscore;
				bool isDash = (sscore == "-");
				double score = isDash ? 0.0 : std::stod(sscore);

				auto* stmt2 = db_.prepare(
					 "INSERT OR REPLACE INTO score "
					 "(student_id, course_id, score) "
					 " VALUES(?,?,?);"
				);
				if (!stmt2) continue;
				sqlite3_bind_text(stmt2, 1, sid.c_str(), -1, SQLITE_TRANSIENT);
				sqlite3_bind_text(stmt2, 2, cid.c_str(), -1, SQLITE_TRANSIENT);
				if (isDash) {
					 // 如果用 NULL 表示未评分，就这样：
					 sqlite3_bind_null(stmt2, 3);
				}
				else {
					 sqlite3_bind_double(stmt2, 3, score);
				}
				sqlite3_step(stmt2);
				sqlite3_finalize(stmt2);
		  }
	 }
	 db_.exec("COMMIT;");
	 fin.close();
}
void System::run() {
	 loadStudentsFromDB();
	 loadTeachersFromDB();
	 loadHeadteachersFromDB();
	 loadHeaddeptsFromDB();
	 loadManagersFromDB();
	 loadCoursesFromDB();
	 loadScoresFromDB();
	 while (true) {
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  line("Login Page", 0, '=', false);
		  cout << endl;
		  cout << "Enter User_Id (0 for logout): ";
		  string id;
		  cin >> id;
		  if (id == "0") break;
		  cout << "Enter Your Password: ";
		  string password;
		  bool ob = true;
		  cin >> password;
		  int index = findManager_Index(id);
		  if (index != -1) {
				ob = false;
				if (manager[index].checkPassword(password)) {
					 cout << "\nSueccessful Login manager " << manager[index].getName() << endl;
					 Manager_Menu(manager[index]);
				}
				else {
					 cout << "Password Wrong!!!" << endl;
				}
		  }
		  index = findStudent_Index(id);		//判断是否是学生
		  if (index != -1) {
				ob = false;
				if (student[index].checkPassword(password)) {
					 cout << "\nSuccesfull Login " << student[index].getName() << endl;
					 Student_Menu(student[index]);
				}
				else {
					 cout << "Password Wrong!!!" << endl;
				}
		  }
		  index = findTeacher_Index(id);		  //判断是否是教师
		  if (ob && index != -1) {
				ob = false;
				if (teacher[index].checkPassword(password)) {
					 cout << "\nSuccesfull Login " << teacher[index].getName() << endl;
					 Teacher_Menu(teacher[index]);
				}
				else {
					 cout << "Password Wrong!!!" << endl;
				}
		  }
		  index = findHeadteacher_Index(id);			//判断是否是班主任
		  if (ob && index != -1) {
				ob = false;
				if (headteacher[index].checkPassword(password)) {
					 cout << "\nSuccesfull Login " << headteacher[index].getName() << endl;
					 Headteacher_Menu(headteacher[index]);
				}
				else {
					 cout << "Password Wrong!!!" << endl;
				}
		  }
		  index = findHeaddept_Index(id);			//判断是否是系主任
		  if (ob && index != -1) {
				ob = false;
				if (headdept[index].checkPassword(password)) {
					 cout << "\nSuccesfull Login " << headdept[index].getName() << endl;
					 Headdept_Menu(headdept[index]);
				}
				else {
					 cout << "Password Wrong!!!" << endl;
				}
		  }
		  if (ob) {
				cout << "Id is Wrong, can't find this id" << endl;
		  }
		  cout << "system logout." << endl;
	 }
}
//界面
void System::line(const string& content, int num, char c, bool n) {
	 string temp = " " + content + " ";
	 int filllen = 59 - temp.length();
	 int left, right;
	 if (!n) {
		  left = filllen / 2;
		  right = filllen - left;
	 }
	 else {
		  left = num;
		  right = filllen - left;
	 }
	 cout << "|"
		  << string(left, c)
		  << temp
		  << string(right, c)
		  << "|" << endl;
}
void System::Student_Menu(Student& stu) {			//学生界面
	 while (true) {
		  string a;
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  line("Student Menu",0,'=',false);
		  line("1. Choose Course",20);		 //选课
		  line("2. Withdraw Course", 20);		 //退课
		  line("3. Check Course", 20);			 //课表
		  line("4. Check Score", 20);			 //查分
		  line("5. Reset Password", 20);		 //设置密码
		  line("6. logout", 20);
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  cout << "Make Choose: ";
		  cin >> a;
		  cout << endl;
		  for (int i = 0;i <= 60;i++) { cout << "="; }
		  cout << endl;
		  if (a == "1") {		 //选课
				line("Course List", 0, ' ', false);
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout <<"|"<< left << setw(7) << "ID" << setw(13) << "Name" << setw(7) << "Day"
					 << setw(13) << "Time" << setw(9) << "Class" << setw(10) << "Teacher" << "|";
				cout << endl;
				for (const auto& c : course) {
					 cout << "|" << left << setw(7) << c.getCourseId() << setw(13) << c.getCourseName()<< setw(7) << c.getCourseDay()
						  << setw(13) << c.getCourseTime()<< setw(9) << c.getCourseClass() << setw(10) << c.getCourseTeacher() << "|";
					 cout << endl;
				}
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout << "Choose course: ";
				string cid;
				cin >> cid;
				int cid_index = findCourse_Index(cid);		  //功能与上面重复
				if (cid_index == -1) {
					 cout << "Invalid Course Id" << endl;
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 cout << endl;
					 continue;
				}
				Course& c = course[cid_index];
				if (c.Incourse(stu.getId())) {
					 cout << "Already choose this course." << endl;
					 continue;
				}
				bool ob1 = true;		  //检查合法性
				Time t2(c.getCourseTime());	  //已有时间段
				for (const auto& newc : course) {
					 if (newc.Incourse(stu.getId()) && newc.getCourseDay() == c.getCourseDay()) {
						  Time t1(newc.getCourseTime());		  //新时间段
						  if (!t1.checkTime(t2)) {
								ob1 = false;
								cout << "Time conflict" << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								break;
						  }
					 }
				}
				if (!ob1) continue;
				if (c.add(db_,stu.getId())) {
					 cout << "Course Selection Successful.\n";
				}
				else {
					 cout << "Failed to add.\n";
				}
		  }
		  else if (a == "2") {		  //退课
				line("Your Course List", 0, ' ', false);
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout <<"|" << left << setw(10) << "ID" << setw(18) << "Name" << setw(15) << "Day" << setw(16) << "Time" << "|";
				cout << endl;
				for (const auto& c : course) {
					 if (c.Incourse(stu.getId())) {
						  cout << "|" << left << setw(10) << c.getCourseId() << setw(18) << c.getCourseName()
								<< setw(15) << c.getCourseDay() << setw(16) << c.getCourseTime() << "|";
						  cout << endl;
					 }
				}
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout << "Enter you want to withdraw course id: ";
				string cid;
				cin >> cid;
				int c_index = findCourse_Index(cid);
				if (c_index == -1) {
					 cout << "Invalid Course Id, this Course not have." << endl;
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 cout << endl;
					 continue;
				}
				Course& c = course[c_index];
				if (!c.Incourse(stu.getId())) {
					 cout << "you don't have this course" << endl;
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 cout << endl;
					 continue;
				}
				if (c.remove(db_, stu.getId())) {
					 cout << "Withdraw course successful" << endl;
				}
				else {
					 cout << "Withdraw failed." << endl;
				}
		  }
		  else if (a == "3") {		  //查课表
				cout << "your course list:" << endl;
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				line("Course List", 0, ' ', false);
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout <<"|" << left << setw(6) << "ID" << setw(13) << "Name" << setw(6) << "Day"
					 << setw(14) << "Time" << setw(9) << "Class" << setw(11) << "Teacher" << "|";
				cout << endl;
				for (const auto& c : course) {
					 if (c.Incourse(stu.getId())) {
						  cout << "|" << left << setw(6) << c.getCourseId() << setw(13) << c.getCourseName() << setw(6) << c.getCourseDay()
								<< setw(14) << c.getCourseTime() << setw(9) << c.getCourseClass() << setw(11) << c.getCourseTeacher() << "|";
						  cout << endl;
					 }
				}
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
		  }
		  else if (a == "4") {		  //查成绩
				cout << left << setw(25) << "Course Score List" << endl;
				cout << left << setw(15) << "Name"<< setw(10) << "Score";
				cout << endl;
				for (const auto& c : course) {
					 if (c.Incourse(stu.getId())) {
						  cout << left << setw(15) << c.getCourseName() << setw(10) << c.getScore(stu.getId());
						  cout << endl;
					 }
				}
		  }
		  else if (a == "5") {
				if (!stu.setPassword(db_)) {
					 cout << "Unable to reset password." << endl;
				}
		  }
		  else if (a == "6") break;		//退出学生界面
		  else {
				line("Invalid Number");
		  }
		  for (int i = 0;i <= 60;i++) { cout << "="; }
		  cout << endl;
		  cout << endl;
	 }
}
void System::Teacher_Menu(Teacher& tea) {			//教师界面
	 while (true) {
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  line("Teacher Menu", 0, '=', false);
		  line("1. Schedule course",20);			  //排课程
		  line("2. Withdraw Course", 20);			  //删课程
		  line("3. View courses taught", 20);	  //查课程
		  line("4. Grading students", 20);		  //评分
		  line("5. student achievements", 20);	  //特定课程中学生成绩
		  line("6. Reset password", 20);		 //重置密码
		  line("7. Reset Course Info", 20);	  //换课室信息
		  line("8. Check All Course", 20);
		  line("9. logout", 20);		  //退出
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  cout << "Make Choose: ";
		  string a;
		  cin >> a;
		  cout << endl;
		  for (int i = 0;i <= 60;i++) { cout << "="; }
		  cout << endl;
		  if (a == "1") {		 //排课程
				cout << "Enter Course Name: ";
				string cname;
				cin >> cname;
				for (char ch : cname) {		 //判断课程名字是否只有英文和数字
					 if (!isalnum(ch)) {
						  cout << "Invalid Class Name." << endl;
						  break;
					 }
				}
				cout << "Enter Course day of week (e.g. Mon Tue Wed Thu Fri Sat Sun): ";
				string day;
				cin >> day;//输入错误后果
				if (day != "Mon" && day != "Tue" && day != "Wed" && day != "Thu" && day != "Fri" && day != "Sat" && day != "Sun") {
					 cout << "Wrong day for week." << endl;
					 break;
				}
				cout << "Enter Course Time (e.g. 0800-0900): ";
				string time;
				cin >> time;
				if (!Time::conficttime(time)) {
					 cout << "Invalid format with time. (example: 0800-1000)" << endl;
					 continue;
				}
				cout << "Enter Course Class: ";
				string cclass;
				cin >> cclass;
				//检查冲突
				bool ob1 = true;
				Time t1(time);
				for (const auto& c : course) {
					 if (c.getCourseDay() != day) continue;
					 if (c.getCourseTeacher() == tea.getName()) {		//教师课程时间冲突
						  Time t(c.getCourseTime());
						  if (!t.checkTime(t1)) {
								cout << "Teacher time conflict\nIn " << day << " you already have this time: " 
									 << c.getCourseTime() << " for another class" << endl;
								ob1 = false;
								break;
						  }
					 }
					 if (c.getCourseClass() == cclass) {	  //课室冲突
						  Time t(c.getCourseTime());
						  if (!t.checkTime(t1)) {
								cout << "Class time conflict\nIn " << day 
									 << " already have this class use in that time: " << c.getCourseTime() << endl;
								ob1 = false;
								break;
						  }
					 }
				}
				if (ob1) {
					 string newid;
					 if (Course::addCourse(db_, course, cname, tea.getName(), day, time, cclass,newid)) {
						  cout << "Course added successfully. \nCourse Id: " << newid << endl;
					 }
					 else {
						  cout<<"Failed to add course."<<endl;
					 }
				}
		  }
		  else if (a == "2") {		  //删除课程
				line("Your Course List", 0, ' ', false);
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout << "|" << left << setw(8) << "ID" << setw(15) << "Name" << setw(10) << "Day"
					 << setw(15) << "Time" << setw(11) << "Class" << "|";
				cout << endl;
				for (const auto& c : course) {
					 if (tea.getName() == c.getCourseTeacher()) {
						  cout << "|" << left << setw(8) << c.getCourseId() << setw(15) << c.getCourseName() << setw(10)
								<< c.getCourseDay() << setw(15) << c.getCourseTime() << setw(11) << c.getCourseClass() << "|";
						  cout << endl;
					 }
				}
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout << "Enter you want delete Course Id: ";
				string delid;
				cin >> delid;
				int idx = findCourse_Index(delid);
				if (idx == -1) {
					 cout << "Course not found" << endl;
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 cout << endl;
					 continue;
				}
				Course& c = course[idx];
				if (c.removeCourse(db_, course, tea.getName())) {
					 cout << "Course deleted successfully." << endl;
				}
		  }
		  else if (a == "3") {		  //查课程
				line("Course List", 0, ' ', false);
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout << "|" << left << setw(8) << "ID" << setw(15) << "Name" << setw(10) << "Day"
					 << setw(15) << "Time" << setw(11) << "Class" << "|";
				cout << endl;
				bool ob = false;
				for (const auto& c : course) {
					 if (tea.getName() == c.getCourseTeacher()) {
						  cout << "|" << left << setw(8) << c.getCourseId() << setw(15) << c.getCourseName() << setw(10)
								<< c.getCourseDay() << setw(15) << c.getCourseTime() << setw(11) << c.getCourseClass() << "|";
						  cout << endl;
						  ob = true;
					 }
				}
				if (!ob) {
					 cout << "No courses found for you." << endl;
				}
		  }
		  else if (a == "4") {		  //评分
				cout << "Enter the course ID to be graded (enter '0' for qiut): ";
				string cid;
				cin >> cid;
				if (cid == "0") break;
				int it = findCourse_Index(cid);
				if (it == -1) {
					 cout << "Invalid Course Id" << endl;
					 continue;
				}
				Course& c = course[it];
				if (c.getCourseTeacher() != tea.getName()) {
					 cout << "You are not the teacher of this course." << endl;
					 continue;
				}
				const auto& sc = c.getStudentScore();
				if (sc.empty()) {
					 cout << "This Course don't have Student." << endl;
					 continue;
				}
				cout << "Course Name: " << c.getCourseName() << "\n";
				cout << "Student List:\n";
		   	cout << left << setw(15) << "Student ID" << setw(10) << "Score";
				cout << endl;
				for (const auto& s : c.getStudentScore()) {
				  cout << left << setw(15) << s.first << setw(10) << s.second << "\n";
				}
				while (true) {
					 cout << "Enter Student ID to set score (Enter '0' to quit): ";
					 string sid, score;
					 cin >> sid;
					 if (sid == "0") break;
					 if (!c.Incourse(sid)) {
						  cout << "Invalid Student id, this student don't have in your course." << endl;
						  continue;
					 }
					 cout << "Enter Score: ";
					 cin >> score;
					 if (c.updateScoreDB(db_, sid, score)) {
						  cout << "Score updated successfully." << endl;
					 }
					 else {
						  cout << "Failed to update score." << endl;
					 }
				}
		  }
		  else if (a == "5") {		  //查课程中学生的成绩
				cout << "Enter want to check score for course id: ";
				string cid;
				cin >> cid;
				int idt = findCourse_Index(cid);
				if (idt == -1) {
					 cout << "Invalid Course Id" << endl;
					 continue;
				}
				Course& c = course[idt];
				if (c.getCourseTeacher() != tea.getName()) {
					 cout << "You are not the teacher of this course." << endl;
					 continue;
				}
				const auto& score = c.getStudentScore();
				if (score.empty()) {
					 cout << "This Course don't have Student." << endl;
					 continue;
				}
				cout << right << setw(15) << "\nStudent Scores List";
				cout << endl;
				cout << left << setw(15) << "Student ID" << setw(20) << "Score";
				cout << endl;
				for (const auto& s : score) {
					 cout << left << setw(15) << s.first << setw(20) << s.second;
					 cout << endl;
				}
		  }
		  else if (a == "6") {
				tea.setPassword(db_);
		  }
		  else if (a == "7") {		  //换课室
				cout << "Enter course id wtih want to change class Info: ";
				string cid;
				cin >> cid;
				cout << endl;
				int idx = findCourse_Index(cid);
				if (idx == -1) {
					 cout << "Invalid Course Id" << endl;
					 continue;
				}
				Course& cou = course[idx];
				if (cou.getCourseTeacher() != tea.getName()) {
					 cout << "You are not this course teacher\ndo not have any authority" << endl;
					 continue;
				}
				while (true) {
					 cout << "1. Set Name\n2. Set Class address\n3. Set Day\n4. Set Time\nEnter Button (0 to quit): ";
					 string m;
					 cin >> m;
					 bool b = false;
					 if (m == "0") {
						  cout << "Back to Student Menu." << endl;
						  break;
					 }
					 if (m == "1") {
						  cout << "Enter the current class name: ";
						  string newname;
						  cin >> newname;
						  cou.setName(db_, newname);
						  b = true;
					 }
					 else if (m == "2") {
						  cout << "Enter the current class address: ";
						  string newclass;
						  cin >> newclass;
						  Time t1(cou.getCourseTime());
						  bool conflict = false;
						  for (const auto& c : course) {
								if (c.getCourseDay() != cou.getCourseDay()) continue;
								if (c.getCourseClass() == newclass) {
									 Time t2(c.getCourseTime());
									 if (!t2.checkTime(t1)) {
										  cout << "In " << c.getCourseDay() << " " << c.getCourseTime() << " " << c.getCourseId() << " has been used." << endl;
										  conflict = true;
										  break;
									 }
								}
						  }
						  if (!conflict) {
								b = true;
								cou.setClass(db_, newclass);
						  }
					 }
					 else if (m == "3") {
						  cout << "Enter the current class day: ";
						  string newday;
						  cin >> newday;
						  if (newday != "Mon" && newday != "Tue" && newday != "Wed" && newday != "Thu" && newday != "Fri" && newday != "Sat" && newday != "Sun") {
								cout << "Wrong day for week." << endl;
								continue;
						  }
						  Time t1(cou.getCourseTime());
						  bool conflict = false;
						  for (const auto& c : course) {
								if (c.getCourseClass() != cou.getCourseClass()) continue;
								if (c.getCourseDay() == newday) {
									 Time t2(c.getCourseTime());
									 if (!t2.checkTime(t1)) {
										  cout << "In " << c.getCourseDay() << " " << c.getCourseTime() << " " <<c.getCourseId() << " has been used." << endl;
										  conflict = true;
										  break;
									 }
								}
						  }
						  if (!conflict) {
								b = true;
								cou.setDay(db_, newday);
						  }
					 }
					 else if (m == "4") {
						  cout << "Enter the current class time: ";
						  string newtime;
						  cin >> newtime;
						  if (!Time::conficttime(newtime)) {
								cout << "Invalid format with time. (example: 0800-1000)" << endl;
								continue;
						  }
						  Time t1(newtime);
						  bool conflict = false;
						  for (const auto& c : course) {
								if (c.getCourseClass() != cou.getCourseClass() || c.getCourseDay() != cou.getCourseDay()) continue;
								Time t2(c.getCourseTime());
								if (!t2.checkTime(t1)) {
									 cout << "In " << c.getCourseDay() << " " << c.getCourseTime() << " " << c.getCourseId() << " has been used." << endl;
									 conflict = true;
									 break;
								}
						  }
						  if (!conflict) {
								b = true;
								cou.setTime(db_, newtime);
						  }
					 }
					 if (!b) {
						  cout << "settion error." << endl;
						  continue;
					 }
					 else {
						  cout << "Successful.\n" << endl;
					 }
				}
		  }
		  else if (a == "8") {
				cout << "The courses are as follows";
				cout << endl;
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				line("Course List", 0, ' ', false);
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout << "|" << left << setw(7) << "ID" << setw(13) << "Name" << setw(7) << "Day"
					 << setw(13) << "Time" << setw(9) << "Class" << setw(10) << "Teacher" << "|";
				cout << endl;
				for (const auto& c : course) {
					 cout << "|" << left << setw(7) << c.getCourseId() << setw(13) << c.getCourseName() << setw(7) << c.getCourseDay()
						  << setw(13) << c.getCourseTime() << setw(9) << c.getCourseClass() << setw(10) << c.getCourseTeacher() << "|";
					 cout << endl;
				}
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
		  }
		  else if (a == "9") break;		//退出
		  else {
				cout << "Invalid Number" << endl;
		  }
		  for (int i = 0;i <= 60;i++) { cout << "="; }
		  cout << endl;
		  cout << endl;
	 }
}
void System::Headteacher_Menu(Headteacher& ht) {		  //班主任界面
	 while (true) {
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  line("Headteacher Menu", 0, '=', false);
		  line("1. View grades of students in this class");
		  line("2. Reset Password");
		  line("3. logout");
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  cout << "Make Choose: ";
		  string n;
		  cin >> n;
		  cout << endl;
		  for (int i = 0;i <= 60;i++) { cout << "="; }
		  cout << endl;
		  if (n == "1") {
				cout << endl;
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				line("Class Student List", 0, '=', false);
				for (const auto& s : student) {
					 if (s.getClassName() != ht.getClassName()) {
						  continue;
					 }
					 cout << "Name: " << s.getName() << "\nId: " << s.getId() << endl;
					 bool ob = false;
					 cout << left << setw(15) << "Course" << setw(10) << "Score";
					 cout << endl;
					 for (const auto& c : course) {
						  if (c.getCourseClass() == ht.getClassName() && c.Incourse(s.getId())) {
								ob = true;
								cout << left << setw(15) << c.getCourseName() << setw(10) << c.getScore(s.getId());
								cout << endl;
						  }
					 }
					 cout << endl;
					 if (!ob) {
						  cout << "This Student dont select any course." << endl;
					 }
				}
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout << endl;
		  }
		  else if (n == "2") {
				ht.setPassword(db_);
		  }
		  else if (n == "3") break;
		  else {
				line("Invalid Number");
		  }
		  for (int i = 0;i <= 60;i++) { cout << "="; }
		  cout << endl;
		  cout << endl;
	 }
}
void System::Headdept_Menu(Headdept& hd) {					 //系主任界面
	 while (true) {
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  line("Headdept Menu", 0, '=', false);
		  line("1. View grades of students in this dept");
		  line("2. Reset Password");
		  line("3. logout");
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  cout << "Make Choose: ";
		  string n;
		  cin >> n;
		  cout << endl;
		  for (int i = 0;i <= 60;i++) { cout << "="; }
		  cout << endl;
		  if (n == "1") {
				cout << endl;
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				line("Dept Student List", 0, '=', false);
				for (const auto& s : student) {
					 if (s.getDept() != hd.getDept()) continue;
					 cout << "Name: " << s.getName() << "\nId: " << s.getId() << endl;
					 bool ob = false;
					 cout << left << setw(15) << "Course" << setw(10) << "Score";
					 cout << endl;
					 for (const auto& c : course) {
						  if (c.Incourse(s.getId())) {
								ob = true;
								cout << left << setw(15) << c.getCourseName() << setw(10) << c.getScore(s.getId());
								cout << endl;
						  }
					 }
					 if (!ob) {
						  cout << "This Student dont select any course." << endl;
					 }
				}
				for (int i = 0;i <= 60;i++) { cout << "-"; }
				cout << endl;
				cout << endl;
		  }
		  else if (n == "2") {
				hd.setPassword(db_);
		  }
		  else if (n == "3") break;
		  else {
				line("Invalid Number");
		  }
		  for (int i = 0;i <= 60;i++) { cout << "="; }
		  cout << endl;
		  cout << endl;
	 }
}
void System::Manager_Menu(Manager& m) {
	 while (true) {
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  line("Manager Menu", 0, '=', false);
		  line("1. Student", 20);
		  line("2. Teacher", 20);
		  line("3. Headteacher", 20);
		  line("4. Deptteacher", 20);
		  line("5. logout", 20);
		  for (int i = 0;i <= 60;i++) { cout << "-"; }
		  cout << endl;
		  cout << "Make Choose: ";
		  string n;
		  cin >> n;
		  cout << endl;
		  if (n == "1") {
				while (true) {
					 for (int i = 0;i <= 60;i++) { cout << "-"; }
					 cout << endl;
					 line("Student Manage Menu", 0, '=', false);
					 line("1. Add Student", 17);
					 line("2. Delete Student", 17);
					 line("3. Student List", 17);
					 line("4. Reset Info", 17);
					 line("5. Quit", 17);
					 for (int i = 0;i <= 60;i++) { cout << "-"; }
					 cout << endl;
					 cout << "Make Choose: ";
					 string k;
					 cin >> k;
					 cout << endl;
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 if (k == "5") {
						  cout << "back to manager menu...." << endl;
						  for (int i = 0;i <= 60;i++) { cout << "="; }
						  cout << endl;
						  cout << endl;
						  break;
					 }
					 if (k == "1") {
						  cout << "Enter Info for student\nName: ";
						  string name;
						  cin >> name;
						  cout << "Class: ";
						  string classname;
						  cin >> classname;
						  cout << "Dept: ";
						  string dept;
						  cin >> dept;
						  int id = 1;
						  for (auto& s : student) {
								int sid = stoi(s.getId());
								if (sid >= id) {
									 id = sid + 1;
								}
						  }
						  string newid = to_string(id);
						  if (!m.AddStudent(db_,newid,name, classname, dept)) {
								cout << "Failed to add student." << endl;
								continue;
						  }
						  student.emplace_back(User(newid, name, "0000"), classname, dept);
						  cout << "Student added successfully." << endl;
					 }
					 else if (k == "2") {
						  cout << "Enter student id for want delete.\nStudent Id: ";
						  string id;
						  cin >> id;
						  cout << endl;
						  int idx = findStudent_Index(id);
						  if (idx == -1) {
								cout << "Student not found." << endl;
								continue;
						  }
						  Student& s = student[idx];
						  cout << "Student Info\nName: " << s.getName()
								<< "\nClass: " << s.getClassName()
								<< "\nDept: " << s.getDept()
								<< "\nPlease confirm delete (yes:1 no:0) : ";
						  string confirm;
						  cin >> confirm;
						  if (confirm != "1") {
								cout << "Deletion cancel." << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								continue;
						  }
						  if (!m.RemoveStudent(db_,id)) {
								cout << "Deletion Fail." << endl;
								continue;
						  }
						  auto it = find_if(student.begin(), student.end(), [&](const Student& s) {
								return s.getId() == id; });
						  if (it != student.end()) {
								student.erase(it);
						  }
						  cout << "Deletion successful." << endl;
					 }
					 else if (k == "3") {
						  line("Student List", 0, ' ', false);
						  for (int i = 0;i <= 60;i++) { cout << "-"; }
						  cout << endl;
						  cout << "|"<<left << setw(5) << "No" << setw(10) << "ID" << setw(15)
								<< "Name" << setw(15) << "Class" << setw(14) << "Dept" << "|";
						  int no = 1;
						  cout << endl;
						  for (const auto& s : student) {
								cout << "|" << left << setw(5) << no++ << setw(10) << s.getId() << setw(15)
									 << s.getName() << setw(15) << s.getClassName() << setw(14) << s.getDept() << "|";
								cout << endl;
						  }
					 }
					 else if (k == "4") {
						  cout << "Enter student id for you want edit: ";
						  string sid;
						  cin >> sid;
						  int idx = findStudent_Index(sid);
						  if (idx == -1) {
								cout << "\nStudent not found." << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								continue;
						  }
						  Student& s = student[idx];
						  while (true) {
								cout << "\n1. Set Class\n2. Set Dept\n3. Set Name\nButton (0 for quit): ";
								string m;
								cin >> m;
								if (m == "0") break;
								if (m == "1") {
									 s.setClass(db_);
								}
								else if (m == "2") {
									 s.setDept(db_);
								}
								else if (m == "3") {
									 s.setName(db_);
								}
								else {
									 cout << "Invalid Button.\n" << endl;
									 continue;
								}
						  }
					 }
					 else {
						  cout << "\nInvalid Number.\n" << endl;
					 }
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 cout << endl;
				}
		  }
		  else if (n == "2") {
				while (true) {
					 for (int i = 0;i <= 60;i++) { cout << "-"; }
					 cout << endl;
					 line("Teacher Manage Menu", 0, '=', false);
					 line("1. Add Teacher", 17);
					 line("2. Delete Teacher", 17);
					 line("3. Teacher List", 17);
					 line("4. Reset Info", 17);
					 line("5. Quit", 17);
					 for (int i = 0;i <= 60;i++) { cout << "-"; }
					 cout << endl;
					 cout << "Make Choose: ";
					 string k;
					 cin >> k;
					 cout << endl;
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 if (k == "1") {
						  cout << "Enter Info for teacher\nName: ";
						  string name;
						  cin >> name;
						  int id = 1;
						  for (auto& t : teacher) {
								int num = stoi(t.getId().substr(1));
								if (num >= id) {
									 id = num + 1;
								}
						  }
						  ostringstream os;
						  os << 'T' << setw(3) << setfill('0') << id;
						  string newid = os.str();
						  if (!m.AddTeacher(db_,newid,name)) {
								cout << "Failed to add teacher." << endl;
						  }
						  else {
								teacher.push_back(User(newid, name, "0000"));
								cout << "Teacher add successful." << endl;
						  }
					 }
					 else if (k == "2") {
						  cout << "Enter teacher id for want delete.\nTeacher Id: ";
						  string id;
						  cin >> id;
						  cout << endl;
						  int idx = findTeacher_Index(id);
						  if (idx == -1) {
								cout << "Teacher not found." << endl;
								continue;
						  }
						  Teacher& t = teacher[idx];
						  cout << "Teacher Info\nName: " << t.getName()
								<< "\nPlease confirm delete (yes:1 no:0) : ";
						  string confirm;
						  cin >> confirm;
						  if (confirm != "1") {
								cout << "Deletion cancel." << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								continue;
						  }
						  if (!m.RemoveTeacher(db_,id)) {
								cout << "Failed to add Teacher." << endl;
								continue;
						  }
						  auto it = find_if(teacher.begin(), teacher.end(), [&](const Teacher& t) {
								return t.getId() == id;
								});
						  if (it != teacher.end()) {
								teacher.erase(it);
						  }
						  cout << "Teacher deleted successful." << endl;
					 }
					 else if (k == "3") {
						  line("Teacher List", 0, ' ', false);
						  for (int i = 0;i <= 60;i++) { cout << "-"; }
						  cout << endl;
						  cout << "|" << left << setw(8) << "No" << setw(20) << "ID" << setw(31) << "Name"<<"|";
						  int no = 1;
						  cout << endl;
						  for (const auto& t : teacher) {
								cout << "|" << left << setw(8) << no++ << setw(20) << t.getId() << setw(31)
									 << t.getName() << "|";
								cout << endl;
						  }
					 }
					 else if (k == "4") {
						  cout << "Enter teacher id for you want edit: ";
						  string id;
						  cin >> id;
						  int idx = findTeacher_Index(id);
						  if (idx == -1) {
								cout << "\nTeacher not found." << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								continue;
						  }
						  Teacher& t = teacher[idx];
						  cout << "1. Set Name\nButton: ";
						  string m;
						  cin >> m;
						  if (m == "1") {
								t.setName(db_);
						  }
						  else {
								cout << "Invalid Button." << endl;
						  }
					 }
					 else if (k == "5") {
						  cout << "Back to Manager Menu..." << endl;
						  for (int i = 0;i <= 60;i++) { cout << "="; }
						  cout << endl;
						  cout << endl;
						  break;
					 }
					 else cout << "\nInvalid Number.\n" << endl;
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 cout << endl;
				}
		  }
		  else if (n == "3") {
				while (true) {
					 for (int i = 0;i <= 60;i++) { cout << "-"; }
					 cout << endl;
					 line("Head Teacher Manage Menu", 0, '=', false);
					 line("1. Add Head Teacher", 14);
					 line("2. Delete Head Teacher", 14);
					 line("3. Head Teacher List", 14);
					 line("4. Reset Info", 14);
					 line("5. Quit", 14);
					 for (int i = 0;i <= 60;i++) { cout << "-"; }
					 cout << endl;
					 cout << "Make Choose: ";
					 string k;
					 cin >> k;
					 cout << endl;
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 if (k == "1") {
						  cout << "Enter Info for head teacher\nName: ";
						  string name;
						  cin >> name;
						  cout << "Class: ";
						  string classs;
						  cin >> classs;
						  int id = 1;
						  for (auto& ht : headteacher) {
								int num = stoi(ht.getId().substr(2));
								if (num >= id) {
									 id = num + 1;
								}
						  }
						  ostringstream os;
						  os << "HT" << setw(3) << setfill('0') << id;
						  string newid = os.str();
						  if (!m.AddHeadteacher(db_,newid,name, classs)) {
								cout << "Failed to add teacher." << endl;
								continue;
						  }
						  headteacher.emplace_back(User(newid, name, "0000"), classs);
						  cout << "Headteacher add successfull." << endl;
					 }
					 else if (k == "2") {
						  cout << "Enter head teacher id for want delete.\nHead Teacher Id: ";
						  string id;
						  cin >> id;
						  cout << endl;
						  int idx = findHeadteacher_Index(id);
						  if (idx == -1) {
								cout << "Head Teacher not found." << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								continue;
						  }
						  Headteacher ht = headteacher[idx];
						  cout << "Teacher Info\nName: " << ht.getName()
								<<"\nClass: "<<ht.getClassName()
								<< "\nPlease confirm delete (yes:1 no:0) : ";
						  string confirm;
						  cin >> confirm;
						  if (confirm != "1") {
								cout << "Deletion cancel." << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								continue;
						  }
						  if (!m.RemoveHeadteacher(db_,id)) {
								cout << "Failed to delete headteacher" << endl;
								continue;
						  }
						  auto it = find_if(headteacher.begin(), headteacher.end(), [&](const Headteacher& ht) {
								return ht.getId() == id;
								});
						  if (it != headteacher.end()) {
								headteacher.erase(it);
						  }
						  cout << "Headteacher delete successfull." << endl;
					 }
					 else if (k == "3") {
						  line("Head Teacher List", 0, ' ', false);
						  for (int i = 0;i <= 60;i++) { cout << "-"; }
						  cout << endl;
						  cout << "|" << left << setw(8) << "No" << setw(13) << "ID" << setw(18)
								<< "Name" << setw(20) << "Class" << "|";
						  int no = 1;
						  cout << endl;
						  for (const auto& ht : headteacher) {
								cout << "|" << left << setw(8) << no++ << setw(13) << ht.getId() << setw(18)
									 << ht.getName() << setw(20) << ht.getClassName() << "|";
								cout << endl;
						  }
					 }
					 else if (k == "4") {
						  cout << "Enter head teacher id for you want edit: ";
						  string id;
						  cin >> id;
						  int idx = findHeadteacher_Index(id);
						  if (idx == -1) {
								cout << "\nHead Teacher not found." << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								continue;
						  }
						  Headteacher& ht = headteacher[idx];
						  while (true) {
								cout << "\n1. Set Name\n2. Set Class\nButton (0 for quit): ";
								string m;
								cin >> m;
								if (m == "0") break;
								if (m == "1") {
									 ht.setName(db_);
								}
								else if (m == "2") {
									 ht.setClass(db_);
								}
								else {
									 cout << "Invalid Button." << endl;
									 continue;
								}
						  }
					 }
					 else if (k == "5") {
						  cout << "Back to Manager Menu..." << endl;
						  for (int i = 0;i <= 60;i++) { cout << "="; }
						  cout << endl;
						  cout << endl;
						  break;
					 }
					 else cout << "\nInvalid Number." << endl;
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 cout << endl;
				}
		  }
		  else if (n == "4") {
				while (true) {
					 for (int i = 0;i <= 60;i++) { cout << "-"; }
					 cout << endl;
					 line("Department Head Manage Menu", 0, '=', false);
					 line("1. Add Department Head", 13);
					 line("2. Delete Department Head", 13);
					 line("3. Department Head List", 13);
					 line("4. Reset Info", 13);
					 line("5. Quit", 13);
					 for (int i = 0;i <= 60;i++) { cout << "-"; }
					 cout << endl;
					 cout << "Make Choose: ";
					 string k;
					 cin >> k;
					 cout << endl;
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 if (k == "1") {
						  cout << "Enter Info for department head\nName: ";
						  string name;
						  cin >> name;
						  cout << "Dept: ";
						  string dept;
						  cin >> dept;
						  int id = 1;
						  for (auto& hd : headdept) {
								int num = stoi(hd.getId().substr(2));
								if (num >= id) {
									 id = num + 1;
								}
						  }
						  ostringstream os;
						  os << "HD" << setw(3) << setfill('0') << id;
						  string newid = os.str();
						  if (!m.AddHeaddept(db_,newid,name, dept)) {
								cout << "Failed to add department head." << endl;
								continue;
						  }
						  headdept.emplace_back(User(newid, name, "0000"), dept);
						  cout << "Headdept add successful." << endl;
					 }
					 else if (k == "2") {
						  cout << "Enter department head id for want delete.\nDepartment Head Id: ";
						  string id;
						  cin >> id;
						  cout << endl;
						  int idx = findHeaddept_Index(id);
						  if (idx == -1) {
								cout << "Department Head not found." << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								continue;
						  }
						  Headdept& hd = headdept[idx];
						  cout << "Department Head Info\nName: " << hd.getName()
								<< "\nDept: " << hd.getDept()
								<< "\nPlease confirm delete (yes:1 no:0) : ";
						  string confirm;
						  cin >> confirm;
						  if (confirm != "1") {
								cout << "Deletion cancel." << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								continue;
						  }
						  if (!m.RemoveHeaddept(db_,id)) {
								cout << "Deletion fail." << endl;
								continue;
						  }
						  auto it = find_if(headdept.begin(), headdept.end(), [&](const Headdept& hd) {
								return hd.getId() == id;
								});
						  if (it != headdept.end()) {
								headdept.erase(it);
						  }
						  cout << "Deletion successful." << endl;
					 }
					 else if (k == "3") {
						  line("Department Head List", 0, ' ', false);
						  for (int i = 0;i <= 60;i++) { cout << "-"; }
						  cout << endl;
						  cout << "|" << left << setw(8) << "No" << setw(12) << "ID" << setw(25)
								<< "Name" << setw(14) << "Dept" << "|";
						  int no = 1;
						  cout << endl;
						  for (const auto& hd : headdept) {
								cout << "|" << left << setw(8) << no++ << setw(12) << hd.getId() << setw(25)
									 << hd.getName() << setw(14) << hd.getDept() << "|";
								cout << endl;
						  }
					 }
					 else if (k == "4") {
						  cout << "Enter head teacher id for you want edit: ";
						  string id;
						  cin >> id;
						  int idx = findHeaddept_Index(id);
						  if (idx == -1) {
								cout << "\nDepartment Head not found." << endl;
								for (int i = 0;i <= 60;i++) { cout << "="; }
								cout << endl;
								cout << endl;
								continue;
						  }
						  Headdept& hd = headdept[idx];
						  while (true) {
								cout << "1. Set Name\n2. Set Dept\nButton (0 for quit): ";
								string m;
								cin >> m;
								if (m == "0") break;
								if (m == "1") {
									 hd.setName(db_);
								}
								else if (m == "2") {
									 hd.setDept(db_);
								}
								else {
									 cout << "Invalid Button." << endl;
									 continue;
								}
						  }
					 }
					 else if (k == "5") {
						  cout << "Back to Manager Menu..." << endl;
						  for (int i = 0;i <= 60;i++) { cout << "="; }
						  cout << endl;
						  cout << endl;
						  break;
					 }
					 else {
						  cout << "\nInvalid Number!!!\n" << endl;
					 }
					 for (int i = 0;i <= 60;i++) { cout << "="; }
					 cout << endl;
					 cout << endl;
				}
		  }
		  else if (n == "5") break;
	 }
}
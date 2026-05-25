
#ifndef _management
#define _management
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define adminPath "adminlib.txt"
#define teacherPath "teacherlib.txt"
#define studentPath "studentlib.txt"
#define teacherIDmin 1000000
#define teacherIDmax 10000000
//当前时间
typedef struct Ltime
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int weekday;
}ltime, * ltimep;
//生日结构体
typedef struct
{
	int year;
	int mouth;
	int day;
}Birthday;
//成绩结构体
typedef struct
{
	int math;
	int chinese;
	int english;
}Score;
//用户信息
typedef struct Data
{
	int permission;
	char name[20];
	char password[20];
	int ID;
	char sex[8];
	Birthday birthday;
	Score score;

}data;
/*通用结构提
permission:0管理员；1教师；2学生
*/
typedef struct User
{
	data data;
	struct User* prev;
	struct User* rear;
}usernode, * userlist;

typedef struct HeadList
{
	userlist admin;
	userlist teacher;
	userlist student;
	userlist login;
	userlist usertemp;
	int studentlistchange;
	int teacherlistchange;
	int adminlistchange;
}headNode, * headList;

//当前登录用户

void printgoback();
void sqillstrenter(char* str);
void cpu_time_u();//简单的读秒
headList headListCreate();

//userheadlist headListCreate();
ltimep readLocalTime();
userlist admincreate(headList headlist);
userlist userLogin(headList headlist);
void refresh(userlist user);

char* encryptpassword(char* p);
char* decodepassword(char* p);
void passwdGet(char* passwd);
void nameGet(char* name);
int birthValidity(Birthday bir);
void birthGet(Birthday* birthday);
void sexGet(char* sex);
void scoreGet(Score* score);
userlist createUserNode();
int insertUserList(userlist* head, userlist user);
int userListLen(userlist head);
void showUserList(userlist head, int start, int count);
userlist searchUserNodeID(userlist head, int id);
int showUserNodeName(userlist head, char* name);
void showOneUser(userlist login, int num);
void changeUserData(headList headlist, int mode);
void changeUserPasswd(headList headlist);
int deleteUserNode(userlist* head, int id);
void deleteUserlist(userlist* head);

int saveUserlist(headList headlist, int mode);

int readUserlist(headList headlist, int mode);

void saveFileAll(headList headlist);

int displayFun(userlist userlogin);

int userEntry(headList headlist, int mode);

void userlistbubble(userlist head, int mode);
#endif
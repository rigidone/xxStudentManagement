#include "fun.h"
void printgoback()
{
	printf("按任意键返回\n");
	getchar();
}

void sqillstrenter(char* str)
{
	while (*str != '\0') str++;
	if (*--str == '\n') *str = '\0';
}

headList headListCreate()
{
	headList pnew = (headList)malloc(sizeof(headNode));
	if (pnew == NULL)
	{
		printf("headlist create err\n");
		return NULL;
	}
	pnew->admin = NULL;
	pnew->teacher = NULL;
	pnew->student = NULL;
	pnew->login = NULL;
	return pnew;
}

ltimep readLocalTime()
{
	time_t now;
	struct tm* local;
	// 获取当前时间
	time(&now);
	local = localtime(&now);
	ltimep p = (ltimep)malloc(sizeof(ltime));
	if (p == NULL) return NULL;
	p->year = local->tm_year + 1900;   // 年份
	p->month = local->tm_mon + 1;       // 月份
	p->day = local->tm_mday;            // 日期
	p->hour = local->tm_hour;           // 小时（24小时制）
	p->minute = local->tm_min;          // 分钟
	p->second = local->tm_sec;          // 秒
	p->weekday = local->tm_wday;
	return p;
}
//后台独立创建管理员
userlist admincreate(headList headlist)
{
	userlist admin = (userlist)malloc(sizeof(usernode));
	if (admin == NULL)
	{
		printf("admin create err\n");
		return NULL;
	}
	admin->data.ID = 0;
	admin->data.permission = 0;
	strcpy(admin->data.password, "123");
	strcpy(admin->data.name, "admin");
	int t = 0;
	insertUserList(&headlist->admin, admin);
	saveUserlist(headlist, 0);
	return admin;
}
//用户登录
userlist userLogin(headList headlist)
{
	if (headlist->admin == NULL)
		return headlist->admin = admincreate(headlist);
	int id;
	char passwd[20];
	userlist loginuser = NULL;
	printf("请登录：\n");
	while (1)
	{
		printf("请输入ID:");
		fgets(passwd, 20, stdin);
		if (passwd[0] == '\n') return NULL;
		else sscanf(passwd, "%d", &id);
		if (id == 0)
		{
			loginuser = searchUserNodeID(headlist->admin, id);
			if (loginuser == NULL)
			{
				printf("用户ID:%-12d未找到;\n", id);
				continue;
			}
			else break;
		}
		else if (id > teacherIDmin && id < teacherIDmax)
		{
			loginuser = searchUserNodeID(headlist->teacher, id);
			if (loginuser == NULL)
			{
				printf("用户ID:%-12d未找到;\n", id);
				continue;
			}
			else break;
		}
		else
		{
			loginuser = searchUserNodeID(headlist->student, id);
			if (loginuser == NULL)
			{
				printf("用户ID:%-12d未找到;\n", id);
				continue;
			}
			else break;
		}
	}
	printf("请输入密码:");
	for (int i = 0; i < 3; i++)
	{
		scanf("%s", passwd);
		if (strcmp(loginuser->data.password, passwd) == 0)
		{
			printf("用户ID:%-12d,%s欢迎登录!\n", id, loginuser->data.name);
			return loginuser;
		}
		else if (i < 2)
		{
			printf("密码错误请重新输入:");
			continue;
		}
	}
	printf("密码输入错误超过3次！取消登录\n");
	return NULL;
}
//界面更新
void refresh(userlist userlogin)
{
	time_t now;
	time(&now);
	char name[20];
	char permission[20];
	if (userlogin != NULL)
	{
		strcpy(name, userlogin->data.name);
		strcpy(permission, userlogin->data.permission == 0 ? "管理员" : userlogin->data.permission == 1 ? "教师" : "学生");
	}
	else
	{
		strcpy(name, "");
		strcpy(permission, "");
	}

	system("cls");
	printf("|                               XX学生管理系统                               |\n");
	printf("|****************************************************************************|\n");
	printf(" 用户:%5s      身份:%5s      欢迎您         %s    \n", name, permission, ctime(&now));
}
//密码加密
char* encryptpassword(char* p)
{
	char* password = (char*)malloc(20);
	char* np = password;
	strcpy(password, p);
	while (*np != '\0')
	{
		(*np) += 11;
		np++;
	}
	np--;
	char* left = password;
	while (left < np)
	{
		char t = *left;
		*left = *np;
		*np = t;
		np--;
		left++;
	}
	return password;
}
//密码解密
char* decodepassword(char* p)
{
	char* password = (char*)malloc(20);
	char* np = password;
	strcpy(password, p);
	char* right = password + strlen(password) - 1;
	while (np < right)
	{
		char t = *right;
		*right = *np;
		*np = t;
		np++;
		right--;
	}
	np = password;
	while (*np != '\0')
	{
		(*np) -= 11;
		np++;
	}
	return password;
}
//密码获取
void passwdGet(char* passwd)
{
	char str[100];
	printf("请输入密码:");
	fgets(str, 100, stdin);
	if (str[0] != '\n')
	{
		sqillstrenter(str);
		strcpy(passwd, str);
	}
}
//姓名获取
void nameGet(char* name)
{
	char str[100];
	printf("请输入姓名:");
	fgets(str, 100, stdin);
	if (str[0] != '\n')
	{
		sqillstrenter(str);
		strcpy(name, str);
	}
}
//生日合理性判断
int birthValidity(Birthday bir)
{
	ltimep ltimep = readLocalTime();
	int isleapyear = (bir.year % 4 == 0 && bir.year % 100 != 0) || (bir.year % 400 == 0);
	if (bir.year<1900 || bir.year>ltimep->year)
	{
		printf("填写生日年不合理！\n");
		return -1;
	}
	else if (bir.mouth < 1 || bir.mouth>12)
	{
		printf("填写生日月不合理！\n");
		return -1;
	}
	else if (bir.day < 1 || bir.day>31)
	{
		printf("填写生日日不合理!\n");
		return -1;
	}
	else if ((bir.mouth == 2 || bir.mouth == 4 || bir.mouth == 6 || bir.mouth == 9 || bir.mouth == 11) && (bir.day > 30))
	{
		printf("填写生日日不合理!\n");
		return -1;
	}
	else if ((bir.mouth == 2) && ((isleapyear && bir.day > 29) || (bir.day > 28 && !isleapyear)))
	{
		printf("填写生日日不合理!\n");
		return -1;
	}
	return 0;
}
//生日获取
void birthGet(Birthday* birthday)
{
	Birthday newbirth = *birthday;
	char str[100];
	while (1)
	{
		printf("请输入出生年:");
		fgets(str, 100, stdin);
		if (str[0] != '\n')
		{
			sqillstrenter(str);
			sscanf(str, "%d", &newbirth.year);
		}
		printf("请输入出生月:");
		fgets(str, 100, stdin);
		if (str[0] != '\n')
		{
			sqillstrenter(str);
			sscanf(str, "%d", &newbirth.mouth);
		}

		printf("请输入出生日:");
		fgets(str, 100, stdin);
		if (str[0] != '\n')
		{
			sqillstrenter(str);
			sscanf(str, "%d", &newbirth.day);
		}
		if (birthValidity(newbirth))
			printf("出生年月日输入错误!\n");
		else
		{
			birthday->year = newbirth.year;
			birthday->mouth = newbirth.mouth;
			birthday->day = newbirth.day;
			break;
		}
	}
}
//性别获取
void sexGet(char* sex)
{
	char str[100];
	while (1)
	{
		printf("请输入性别(男/女):");
		fgets(str, 100, stdin);
		if (str[0] != '\n')
		{
			sqillstrenter(str);
		}
		else strcpy(str, sex);
		if (strcmp(str, "男") == 0 || strcmp(str, "女") == 0)
		{
			strcpy(sex, str);
			break;
		}
		else
			printf("性别输入错误!\n");
	}
}
//成绩获取
void scoreGet(Score* score)
{
	Score scoret = *score;
	char str[100];

	printf("请输入数学成绩:");
	fgets(str, 100, stdin);
	if (str[0] != '\n')
	{
		sqillstrenter(str);
		sscanf(str, "%d", &score->math);
	}
	printf("请输入语文成绩:");
	fgets(str, 100, stdin);
	if (str[0] != '\n')
	{
		sqillstrenter(str);
		sscanf(str, "%d", &score->chinese);
	}
	printf("请输入英语成绩:");
	fgets(str, 100, stdin);
	if (str[0] != '\n')
	{
		sqillstrenter(str);
		sscanf(str, "%d", &score->english);
	}
}
//新建数据节点
userlist createUserNode()
{

	userlist pnew = (userlist)malloc(sizeof(usernode));
	if (pnew == NULL)
	{
		printf("新用户节点创建失败!\n");
		return NULL;
	}
	pnew->prev = NULL;
	pnew->rear = NULL;
	return pnew;
}
//插入数据
int insertUserList(userlist* head, userlist user)
{
	if (user == NULL) return -1;
	if (*head == NULL)
	{
		switch (user->data.permission)
		{
		case 0:
			user->data.ID = 0;
			break;
		case 1:
			if (user->data.ID < teacherIDmin || user->data.ID >= teacherIDmax)
				user->data.ID = teacherIDmin + 1;
			break;
		default:
			if (user->data.ID < teacherIDmax)
				user->data.ID = teacherIDmax + 1;
			break;
		}
		*head = user;
		user->rear = user;
		user->prev = user;
	}
	//ID无效或大于链表末端时自动分配ID插入至链表末端
	else if (user->data.ID <= 0)
	{
		user->rear = *head;
		user->prev = (*head)->prev;
		user->prev->rear = user;
		(*head)->prev = user;
		user->data.ID = user->prev->data.ID + 1;
	}
	//ID小于头节点将新节点变为头节点
	else if (user->data.ID < (*head)->data.ID)
	{
		user->rear = *head;
		user->prev = (*head)->prev;
		user->prev->rear = user;
		(*head)->prev = user;
		*head = user;
	}
	else
	{//排除ID相同的节点
		switch (user->data.permission)
		{
		case 1:
			if (user->data.ID < teacherIDmin || user->data.ID >= teacherIDmax)
			{
				printf("用户ID:%-12d,姓名:%-10s,权限:%-8s,ID重复录入失败\n", user->data.ID, user->data.name,
					user->data.permission == 0 ? "管理员" : user->data.permission == 1 ? "教师" : "学生");
				return -1;
			}
			break;
		default:
			if (user->data.ID < teacherIDmax)
			{
				printf("用户ID:%-12d,姓名:%-10s,权限:%-8s,ID重复录入失败\n", user->data.ID, user->data.name,
					user->data.permission == 0 ? "管理员" : user->data.permission == 1 ? "教师" : "学生");
				return -1;
			}
			break;
		}

		userlist t = *head;
		do
		{

			if (t->data.ID == user->data.ID)
			{
				printf("用户ID:%-12d,姓名:%-10s,权限:%-8s,ID重复录入失败\n", user->data.ID, user->data.name,
					user->data.permission == 0 ? "管理员" : user->data.permission == 1 ? "教师" : "学生");
				free(user);
				return -1;
			}
			t = t->rear;
		} while (t != *head && t->data.ID <= user->data.ID);
		user->rear = t;
		user->prev = t->prev;
		user->prev->rear = user;
		t->prev = user;
	}
	printf("用户ID:%-12d,姓名:%-10s,权限:%-8s,录入成功\n", user->data.ID, user->data.name,
		user->data.permission == 0 ? "管理员" : user->data.permission == 1 ? "教师" : "学生");

	return 0;
}
//链表长度
int userListLen(userlist head)
{
	int num = 0;
	if (head == NULL) return num;
	userlist t = head;
	do
	{
		t = t->rear;
		num++;
	} while (t != head);
	return num;
}
//遍历整个链表
void showUserList(userlist head, int start, int count)
{
	printf("从第%d开始,至%d位置:\n", start, count);
	int num = 0;
	if (head == NULL)
	{
		printf("合计搜索出%d个结果;\n", num);
		printgoback();
		return;
	}
	userlist t = head;
	for (int i = 0; i < start; i++)
		t = t->rear;
	do {
		num++;
		showOneUser(t, num);
		t = t->rear;
		count--;
	} while (t != head && count > 0);
	printf("合计搜索出%d个结果;\n", num);
	printf("查询完成,");
	printgoback();
}
//根据ID查找节点
userlist searchUserNodeID(userlist head, int id)
{
	if (head == NULL)
	{
		printf("头节点无效\n");
		printgoback();
		return NULL;
	}
	if (id > head->prev->data.ID || id < head->data.ID)
	{
		printf("无该ID用户");
		printgoback();
		return NULL;
	}
	//int dirction = (head->prev->data.ID - id+1) > (id-head->data.ID);
	userlist t = head;
	do {
		if (t->data.ID == id)
		{
			//showOneUser(t,1);
			//printf("合计搜索出1个结果;\n");
			return t;
		}
		//if (dirction) 
		t = t->rear;
		//else t = t->prev;
	} while (t != head);
	printf("用户ID:%8d，未找到,\n");
	printgoback();
	return NULL;
}
//根据名称查找节点
int showUserNodeName(userlist head, char* name)
{
	if (head == NULL)
	{
		printf("头节点无效\n");
		return -1;
	}
	userlist t = head;
	int num = 0;
	do
	{
		if (strcmp(t->data.name, name) == 0)
		{
			num++;
			printf("%-5d  用户ID:%-12d,姓名:%-10s,权限:%-8s\n", num, t->data.ID, t->data.name,
				t->data.permission == 0 ? "管理员" : t->data.permission == 1 ? "教师" : "学生");
		}
		t = t->rear;
	} while (t != head);
	printf("合计搜索出%d个结果;\n", num);

	return 0;
}
//显示个人信息
void showOneUser(userlist login, int num)
{
	if (login == NULL) return;
	if (num <= 0) num == 1;
	if (login->data.permission == 0)
	{
		if (num == 1)
			printf("%-6s %-12s %-8s %-10s %-20s\n",
				"序号", "ID", "权限", "姓名", "密码");
		printf("%-6d %-12d %-8d %-10s %-20s\n",
			num, login->data.ID, login->data.permission, login->data.name, login->data.password);
	}
	else if (login->data.permission == 1)
	{
		if (num == 1)
			printf("%-6s %-12s %-8s %-10s %-20s %-6s %-6s %-4s %-4s\n",
				"序号", "ID", "权限", "姓名", "密码", "性别",
				"出生年", "月", "日");
		printf("%-6d %-12d %-8d %-10s %-20s %-6s %-6d %-4d %-4d\n",
			num, login->data.ID, login->data.permission, login->data.name,encryptpassword(login->data.password), login->data.sex,
			login->data.birthday.year, login->data.birthday.mouth, login->data.birthday.day);
	}
	else
	{
		if (num == 1)
			printf("%-6s %-12s %-8s %-10s %-20s %-6s %-6s %-4s %-4s %-4s %-4s %-4s\n",
				"序号", "ID", "权限", "姓名", "密码", "性别",
				"出生年", "月", "日",
				"数学", "语文", "英语");
		printf("%-6d %-12d %-8d %-10s %-20s %-6s %-6d %-4d %-4d %-4d %-4d %-4d\n",
			num, login->data.ID, login->data.permission, login->data.name, login->data.password, login->data.sex,
			login->data.birthday.year, login->data.birthday.mouth, login->data.birthday.day,
			login->data.score.math, login->data.score.chinese, login->data.score.english);
	}

}
//信息变更
void changeUserData(headList headlist, int mode)
{
	int id;
	char str[100];
	printf("***无需修改则输入回车***\n");
	printf("请输入你要修改%s的ID:", mode == 1 ? "教师" : "学生");
	fgets(str, 100, stdin);
	if (str[0] == '\n')
	{
		printgoback();
		return;
	}
	sscanf(str, "%d", &id);
	userlist usertemp;
	if (mode == 1) usertemp = searchUserNodeID(headlist->teacher, id);
	else usertemp = searchUserNodeID(headlist->student, id);
	if (usertemp == NULL) return;
	nameGet(usertemp->data.name);
	passwdGet(usertemp->data.password);
	sexGet(usertemp->data.sex);
	birthGet(&usertemp->data.birthday);
	if (mode == 0) headlist->adminlistchange = 1;
	else if (mode == 1) headlist->teacherlistchange = 1;
	else if (mode == 2)
	{
		scoreGet(&usertemp->data.score);
		headlist->studentlistchange = 1;
	}
	showOneUser(usertemp, 1);
	printgoback();
}
//密码变更
void changeUserPasswd(headList headlist)
{
	if (headlist->login->data.permission == 0) headlist->adminlistchange = 1;
	else if (headlist->login->data.permission == 1) headlist->teacherlistchange = 1;
	else if (headlist->login->data.permission == 2) headlist->studentlistchange = 1;

	passwdGet(headlist->login->data.password);
	printf("密码更改完成");
	printgoback();
}
//删除指定用户
int deleteUserNode(userlist* head, int id)
{
	userlist t = searchUserNodeID(*head, id);
	if (t == NULL)
	{
		printgoback();
		return -1;

	}
	printf("请确认是否删除(Y/N):");
	while (1)
	{
		char c = getchar();
		if (c == 'N' || c == 'n')
		{
			printf("取消删除！");
			printgoback();
			return 0;
		}
		if (c == 'Y' || c == 'y')
		{
			if ((*head)->rear == *head)
			{
				free(*head);
				*head = NULL;
			}
			else
			{
				if (t == *head) *head = t->rear;
				t->prev->rear = t->rear;
				t->rear->prev = t->prev;
				free(t);
				t = NULL;
			}
			printf("用户:%-12d删除成功！", id);
			break;
		}
	}
	printgoback();
	return 0;
}
//清空链表
void deleteUserlist(userlist* head)
{
	if (*head == NULL) return;
	//printf("请确认是否删除(Y/N):");
	while (1)
	{
		int t = 1;
		char c = ' ';
		//c= getchar();
		if (!t && c == 'N' || c == 'n')
		{
			printf("取消删除！\n");
			return;
		}
		if (c == 'Y' || c == 'y' || t)
		{
			(*head)->prev->rear = NULL;
			(*head)->prev = NULL;

			while (*head != NULL)
			{
				if ((*head)->prev != NULL && (*head)->rear != NULL) (*head)->prev->rear = (*head)->rear;
				if ((*head)->rear != NULL && (*head)->prev != NULL) (*head)->rear->prev = (*head)->prev;
				userlist t = *head;
				*head = (*head)->rear;
				free(t);
			}
			printf("删除成功！\n");
			break;
		}
	}
}
//链表保存
int saveUserlist(headList headlist, int mode)
{
	char filename[100];
	userlist head;
	if (mode == 0)
	{
		strcpy(filename, adminPath);
		head = headlist->admin;
		headlist->adminlistchange = 0;
	}
	else if (mode == 1)
	{
		strcpy(filename, teacherPath);
		head = headlist->teacher;
		headlist->teacherlistchange = 0;
	}
	else
	{
		strcpy(filename, studentPath);
		head = headlist->student;
		headlist->studentlistchange = 0;
	}
	FILE* fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("%s打开失败", filename);
		return -1;
	}
	if (head == NULL)
	{
		printf("%d表无数据,清空%s文件!", mode == 1 ? "教师" : "学生", filename);
		printgoback();
		return 0;
	}
	fprintf(fp, "%s身份管理中心\n", head->data.permission == 0 ? "管理员" : head->data.permission == 1 ? "教师" : "学生");
	if (head->data.permission == 0)
	{
		fprintf(fp, "%-6s %-12s %-8s %-10s %-20s\n",
			"序号", "ID", "权限", "姓名", "密码");
	}
	else if (head->data.permission == 1)
	{
		fprintf(fp, "%-6s %-12s %-8s %-10s %-20s %-6s %-6s %-4s %-4s\n",
			"序号", "ID", "权限", "姓名", "密码", "性别",
			"出生年", "月", "日");
	}
	else
	{
		fprintf(fp, "%-6s %-12s %-8s %-10s %-20s %-6s %-6s %-4s %-4s %-4s %-4s %-4s\n",
			"序号", "ID", "权限", "姓名", "密码", "性别",
			"出生年", "月", "日",
			"数学", "语文", "英语");
	}
	userlist t = head;
	int count = 0;
	do
	{
		count++;
		char* encrytpass = encryptpassword(t->data.password);
		if (head->data.permission == 0)
		{
			fprintf(fp, "%-6d %-12d %-8d %-10s %-20s\n",
				count, t->data.ID, t->data.permission, t->data.name, encrytpass);
		}
		else if (head->data.permission == 1)
		{
			fprintf(fp, "%-6d %-12d %-8d %-10s %-20s %-6s %-6d %-4d %-4d\n",
				count, t->data.ID, t->data.permission, t->data.name, encrytpass, t->data.sex,
				t->data.birthday.year, t->data.birthday.mouth, t->data.birthday.day);
		}
		else
		{
			fprintf(fp, "%-6d %-12d %-8d %-10s %-20s %-6s %-6d %-4d %-4d %-4d %-4d %-4d\n",
				count, t->data.ID, t->data.permission, t->data.name, encrytpass, t->data.sex,
				t->data.birthday.year, t->data.birthday.mouth, t->data.birthday.day,
				t->data.score.math, t->data.score.chinese, t->data.score.english);
		}
		free(encrytpass);
		t = t->rear;
	} while (t != head && t != NULL);
	printf("%s,文件保存成功!合计保存%d条信息;\n", filename, count);
	fclose(fp);
	return count;
}
//链表读取
int readUserlist(headList headlist, int mode)
{
	if (headlist == NULL) return -1;
	char filename[100];
	userlist* head;
	if (mode == 0)
	{
		if (headlist->admin != NULL) deleteUserlist(&headlist->admin);
		strcpy(filename, adminPath);
		head = &headlist->admin;
		headlist->adminlistchange = 0;
	}
	else if (mode == 1)
	{
		if (headlist->teacher != NULL) deleteUserlist(&headlist->teacher);
		strcpy(filename, teacherPath);
		head = &headlist->teacher;
		headlist->teacherlistchange = 0;
	}
	else
	{
		if (headlist->student != NULL) deleteUserlist(&headlist->student);
		strcpy(filename, studentPath);
		head = &headlist->student;
		headlist->studentlistchange = 0;
	}
	FILE* fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("%s打开失败", filename);
		return -1;
	}
	//count，导入数据计数，fpbytesize文件总大小，fpcurrbyte文件当前位置
	int count = 0, fpbytesize, fpcurrbyte;
	fseek(fp, 0, 2);
	fpbytesize = ftell(fp);
	if (fpbytesize < 20) return -1;
	fseek(fp, 0, 0);
	char c;
	for (int i = 0; i < 2; i++)
	{
		do
		{
			c = fgetc(fp);
			printf("%c", c);

		} while (c != '\n');
	}
	fpcurrbyte = ftell(fp);
	//循环判断下一个字符是否为回车
	while (!feof(fp) && (fpbytesize - fpcurrbyte > 20))
	{
		int t;
		char* decodepass;
		userlist newuser = createUserNode();
		if (newuser == NULL) continue;
		if (mode == 0)
		{
			fscanf(fp, "%d %d %d %s %s",
				&t, &newuser->data.ID, &newuser->data.permission, newuser->data.name, newuser->data.password);
			decodepass = decodepassword(newuser->data.password);
			strcpy(newuser->data.password, decodepass);
			newuser->data.permission = 0;
			printf("%-6d %-12d %-8d %-10s %-20s %-6s\n",
				t, newuser->data.ID, newuser->data.permission, newuser->data.name, newuser->data.password);
		}
		else if (mode == 1)
		{
			fscanf(fp, "%d %d %d %s %s %s %d %d %d",
				&t, &newuser->data.ID, &newuser->data.permission, newuser->data.name, newuser->data.password, newuser->data.sex,
				&newuser->data.birthday.year, &newuser->data.birthday.mouth, &newuser->data.birthday.day);
			decodepass = decodepassword(newuser->data.password);
			strcpy(newuser->data.password, decodepass);
			newuser->data.permission = 1;
			printf("%-6d %-12d %-8d %-10s %-20s %-6s %-6d %-4d %-4d\n",
				t, newuser->data.ID, newuser->data.permission, newuser->data.name, newuser->data.password, newuser->data.sex,
				newuser->data.birthday.year, newuser->data.birthday.mouth, newuser->data.birthday.day);

		}
		else
		{
			fscanf(fp, "%d %d %d %s %s %s %d %d %d %d %d %d",
				&t, &newuser->data.ID, &newuser->data.permission, newuser->data.name, newuser->data.password, newuser->data.sex,
				&newuser->data.birthday.year, &newuser->data.birthday.mouth, &newuser->data.birthday.day,
				&newuser->data.score.math, &newuser->data.score.chinese, &newuser->data.score.english);
			decodepass = decodepassword(newuser->data.password);
			strcpy(newuser->data.password, decodepass);
			newuser->data.permission = 2;
			printf("%-6d %-12d %-8d %-10s %-20s %-6s %-6d %-4d %-4d %-4d %-4d %-4d\n",
				t, newuser->data.ID, newuser->data.permission, newuser->data.name, newuser->data.password, newuser->data.sex,
				newuser->data.birthday.year, newuser->data.birthday.mouth, newuser->data.birthday.day,
				newuser->data.score.math, newuser->data.score.chinese, newuser->data.score.english);
		}
		free(decodepass);
		insertUserList(head, newuser);
		count++;
		fpcurrbyte = ftell(fp);
	}
	fclose(fp);
	printf("%s,共导入%d条数据\n", filename, count);
	return count;
}
//保存所有文件
void saveFileAll(headList headlist)
{
	if (headlist->adminlistchange)
		saveUserlist(headlist, 0);
	else printf("%s,未更改\n", adminPath);
	if (headlist->teacherlistchange)
		saveUserlist(headlist, 1);
	else printf("%s,未更改\n", teacherPath);
	if (headlist->studentlistchange)
		saveUserlist(headlist, 2);
	else printf("%s,未更改\n", studentPath);
	printf("文件保存完成,");
	printgoback();
}
//显示主页面功能
int displayFun(userlist userlogin)
{
	refresh(userlogin);
	int selectfun;
	if (userlogin == NULL)
	{
		printf("--------用户未登录功能如下:--------\n");
		printf("  1、切换用户;\n");
		printf("  2、退出程序;\n");
		printf("  3、读取数据文本;\n");
		printf("  4、保存数据文本;\n");
	}
	else
	{
		switch (userlogin->data.permission)
		{
		case 0:
			printf("----------管理员功能如下:----------\n");
			printf("  1、切换用户;\n");
			printf("  2、退出程序;\n");
			printf("  3、读取数据文本;\n");
			printf("  4、保存数据文本;\n");
			printf("  5、修改自身登录密码;\n");
			printf("  6、查阅自身信息;\n");
			printf("  7、添加新教师;\n");
			printf("  8、删除教师;\n");
			printf("  9、修改教师信息;\n");
			printf(" 10、查看所有教师;\n");
			break;
		case 1:
			printf("-----------教师功能如下:----------\n");
			printf("  1、切换用户;\n");
			printf("  2、退出程序;\n");
			printf("  3、读取数据文本;\n");
			printf("  4、保存数据文本;\n");
			printf("  5、修改自身登录密码;\n");
			printf("  6、查阅自身信息;\n");
			printf("  7、添加新学生;\n");
			printf("  8、删除学生;\n");
			printf("  9、修改学生信息;\n");
			printf(" 10、查阅指定学生信息;\n");
			printf(" 11、按学号从低到高查看所有学生信息;\n");
			printf(" 12、按总分从高到低查看所有学生信息;\n");
			break;
		case 2:
			printf("------------学生功能如下:-----------\n");
			printf("  1、切换用户;\n");
			printf("  2、退出程序;\n");
			printf("  3、读取数据文本;\n");
			printf("  4、保存数据文本;\n");
			printf("  5、修改自身登录密码;\n");
			printf("  6、查阅自身信息;\n");
			break;
		}

	}
	printf(" ----------------------------------\n");
	printf(" 请输入你需要的功能:");
	scanf("%d", &selectfun);
	getchar();
	if (userlogin == NULL)
	{
		if (selectfun < 1 || selectfun>4) return 0;
		else return selectfun;
	}
	else if (selectfun < 1 ||
		(userlogin->data.permission == 0 && selectfun > 10) ||
		(userlogin->data.permission == 1 && selectfun > 12) ||
		(userlogin->data.permission == 2 && selectfun > 6)) return 0;
	else if (selectfun >= 1 && selectfun <= 6) return selectfun;
	else if (userlogin->data.permission == 0 && selectfun >= 7) return selectfun + 100;
	else if (userlogin->data.permission == 1 && selectfun >= 7) return selectfun + 200;
	else return 0;
}
//个人用户添加
int userEntry(headList headlist, int mode)
{
	userlist pnew = createUserNode();
	if (pnew == NULL) return -1;
	printf("请填写需添加%s的信息:\n", mode == 1 ? "教师" : "学生");
	int t = 0;
	nameGet(pnew->data.name);
	sexGet(pnew->data.sex);
	passwdGet(pnew->data.password);
	birthGet(&pnew->data.birthday);
	pnew->data.ID = 0;
	if (mode == 1)
	{
		pnew->data.permission = 1;
		t = insertUserList(&headlist->teacher, pnew);
		headlist->teacherlistchange = 1;
	}
	else
	{
		pnew->data.permission = 2;
		scoreGet(&pnew->data.score);
		t = insertUserList(&headlist->student, pnew);
		headlist->studentlistchange = 1;
	}
	printgoback();
	return 0;
}
//排序，mode:0 学号低~高,1 总分高~低
void userlistbubble(userlist head, int mode)
{
	if (userListLen(head) <= 1) return;
	int swapped = 0;
	userlist stop = head->prev;
	data datat;
	do
	{
		userlist t = head;
		swapped = 0;
		while (t!= stop)
		{
			if (mode)
			{
				int score1 = t->data.score.math +
					t->data.score.chinese + t->data.score.english;
				int score2 = t->rear->data.score.math +
					t->rear->data.score.chinese + t->rear->data.score.english;
				if (score1 < score2)
				{
					datat = t->data;
					t->data = t->rear->data;
					t->rear->data = datat;
					swapped = 1;
				}
			}
			else
			{
				int id1 = t->data.ID;
				int id2 = t->rear->data.ID;
				if (id1 > id2)
				{
					datat = t->data;
					t->data = t->rear->data;
					t->rear->data = datat;
					swapped = 1;
				}
			}
			t = t->rear;
		}
		stop = t;
	} while (swapped);
}



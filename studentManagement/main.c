
#include "myheader.h"
int selectFun = 0;
int main()
{

	int id = 0, idt = -1000,permiss=-1;
	char str[100];
	headList headlist = headListCreate();
	if (headlist == NULL)
	{
		printf("headlist create err\n");
		return -1;
	}
	readUserlist(headlist, 0);
	readUserlist(headlist, 1);
	readUserlist(headlist, 2);
	printf("12141\n");
	while (1)
	{
		userlist newuse;
		refresh(headlist->login);
		if (headlist->login == NULL) headlist->login = userLogin(headlist);
		if (!(selectFun = displayFun(headlist->login))) continue;
		switch (selectFun)
		{
		case 1:
			headlist->login = NULL;
			break;
		case 2:
			exit(0);
			break;
		case 3:
			if (headlist->login != NULL)
			{
				idt = headlist->login->data.ID;
				permiss = headlist->login->data.permission;
			}
			readUserlist(headlist, 0);
			readUserlist(headlist, 1);
			readUserlist(headlist, 2);
			if(permiss==0) headlist->login = searchUserNodeID(headlist->admin,idt);
			else if(permiss==1) headlist->login = searchUserNodeID(headlist->teacher, idt);
			else if (permiss == 2) headlist->login = searchUserNodeID(headlist->student, idt);
			else headlist->login = NULL;
			idt = -1000; permiss = -1;
			break;
		case 4:
			saveFileAll(headlist);
			break;
		case 5:
			changeUserPasswd(headlist);
			saveFileAll(headlist);
			break;
		case 6:
			showOneUser(headlist->login, 1);
			printgoback();
			break;
		case 107:
			userlistbubble(headlist->teacher, 0);
			userEntry(headlist, 1);
			break;
		case 108:
			printf("请输入你要删除教师的ID:");
			fgets(str, 100, stdin);
			if (str[0] != '\n')
			{
				sscanf(str, "%d", &id);
				if (deleteUserNode(&headlist->teacher, id))
					headlist->teacherlistchange = 1;
			}
			printgoback();
			break;
		case 109:
			changeUserData(headlist, 1);
			break;
		case 110:
			showUserList(headlist->teacher, 0, userListLen(headlist->teacher));
			break;
		case 207:
			userlistbubble(headlist->student, 0);
			userEntry(headlist, 2);
			break;
		case 208:
			printf("请输入你要删除学生的ID:");
			fgets(str, 100, stdin);
			if (str[0] != '\n')
			{
				sscanf(str, "%d", &id);
				if (deleteUserNode(&headlist->student, id))
					headlist->studentlistchange = 1;
			}
			printgoback();
			break;
		case 209:
			changeUserData(headlist, 2);
			break;
		case 210:
			printf("请输入你要查询学生的ID:");
			fgets(str, 100, stdin);
			if (str[0] != '\n') sscanf(str, "%d", &id);
			else
				showOneUser(searchUserNodeID(headlist->student, id), 1);
			printgoback();
			break;
		case 211:
			printf("按学号从低到高查看所有学生信息如下:\n");
			userlistbubble(headlist->student, 0);
			showUserList(headlist->student, 0, userListLen(headlist->student));
			break;
		case 212:
			printf("按按总分从高到低查看所有学生信息如下:\n");
			userlistbubble(headlist->student, 1);
			showUserList(headlist->student, 0, userListLen(headlist->student));
			break;

		}



	}

	return 0;
}
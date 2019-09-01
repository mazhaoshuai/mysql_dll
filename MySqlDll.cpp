#include "MySqlDll.h"
#include <direct.h>
#include <fstream>
#include <io.h>

MYSQL * sql = NULL; //= my_init((MYSQL*) 0); 


/*
* 请free
*  char* fileName = "/mySqlConfig.ini"路径前加/；
* @return 获取config路径；
*/
int getMySqlIni(char*&dbuser, char*&dbpasswd, char*&dbip, char*&dbport, char*&dbname, char* fileName = "/mySqlConfig.ini")
{
	char*completePath = NULL;
	splicing_root_path(fileName,completePath);

	//读文件流
	std::ifstream in;
	int tempIndex = 0;
	in.open(completePath);
	if (in.fail()){ in.close(); printf("MySql配置文件打开失败!\n"); return -1; }
	if (completePath != NULL)free(completePath); completePath = NULL;
	//获取文件大小
	in.seekg(0, std::ios::end);
	int length = (int)in.tellg();
	//创建空间拷贝
	char* line = (char*)calloc(length, sizeof(char));
	in.seekg(0, std::ios::beg);
	while (in.getline(line, length))
	{
		if (!strncmp(line, "dbuser", strlen("dbuser"))){

			if (dbuser == NULL || strlen(dbuser) == 0)
			{
				tempIndex = strlen(line) - (strlen("dbuser"));
				dbuser = (char*)calloc(tempIndex, sizeof(char));
			}
			strcpy_s(dbuser, strlen(line) - strlen("dbuser"), line + (strlen("dbuser") + 1));
			continue;
		}
		if (!strncmp(line, "dbpasswd", strlen("dbpasswd"))){

			if (dbpasswd == NULL || strlen(dbpasswd) == 0)
			{
				tempIndex = strlen(line) - (strlen("dbpasswd"));
				dbpasswd = (char*)calloc(tempIndex, sizeof(char));
			}
			strcpy_s(dbpasswd, strlen(line) - strlen("dbpasswd"), line + (strlen("dbpasswd") + 1));
			continue;
		}
		if (!strncmp(line, "dbip", strlen("dbip"))){

			if (dbip == NULL || strlen(dbip) == 0)
			{
				tempIndex = strlen(line) - (strlen("dbip"));
				dbip = (char*)calloc(tempIndex, sizeof(char));
			}
			strcpy_s(dbip, strlen(line) - strlen("dbip"), line + (strlen("dbip") + 1));
			continue;
		}
		if (!strncmp(line, "dbport", strlen("dbport"))){

			if (dbport == NULL || strlen(dbport) == 0)
			{
				tempIndex = strlen(line) - (strlen("dbport"));
				dbport = (char*)calloc(tempIndex, sizeof(char));
			}
			strcpy_s(dbport, strlen(line) - strlen("dbport"), line + (strlen("dbport") + 1));
			continue;
		}
		if (!strncmp(line, "dbname", strlen("dbname"))){

			if (dbname == NULL || strlen(dbname) == 0)
			{
				tempIndex = strlen(line) - (strlen("dbname"));
				dbname = (char*)calloc(tempIndex, sizeof(char));
			}
			strcpy_s(dbname, strlen(line) - strlen("dbname"), line + (strlen("dbname") + 1));
		}
	}

	if (in.is_open()) in.close();
	if (line != NULL){ free(line); line = NULL; }

	return 0;

}

int sql_connect(char* iniFile)
{

	sql = mysql_init((MYSQL*)0);
	char *query = NULL;
	char* dbuser = NULL;
	char* dbpasswd = NULL;
	char* dbip = NULL;
	char* dbport = NULL;
	char* dbname = NULL;
	int error = 0;

	error = getMySqlIni(dbuser, dbpasswd, dbip, dbport, dbname, iniFile);
	if (error == -1)
		return error;

	if (!mysql_real_connect(sql, dbip, dbuser, dbpasswd, dbname, atoi(dbport), NULL, 0))
	{
		printf("请检查MySql是否启动，配置参数是否正确!\n");
		return -1;
	}

	if (!mysql_select_db(sql, dbname)) {
		printf("成功选择数据库:%s!", dbname);
		sql->reconnect = 1;
		query = "set names \'GBK\'";
		error = mysql_real_query(sql, query, strlen(query) + 1);
		printf("编码 %s!\n", query);
		if (error) {
			printf("Error making query: %s !!!\n", mysql_error(sql));
			return -1;
		}
	}

	if (dbip != NULL){ free(dbip); dbip = NULL; }
	if (dbuser != NULL){ free(dbuser); dbuser = NULL; }
	if (dbpasswd != NULL){ free(dbpasswd); dbpasswd = NULL; }
	if (dbname != NULL){ free(dbname); dbname = NULL; }
	if (dbport != NULL){ free(dbport); dbport = NULL; }

	return 0;
}

int sql_close()
{
	mysql_close(sql);
	return 0;
}


MYSQL * get_mysql()
{
	if (sql)
	{
		if (sql->db == NULL)
			return NULL;

		return sql;
	}
	return NULL;
}
#include "MySqlDll.h"
#include <fstream>


int frees(void** &items)
{
	if (items == NULL)
		return 1;

	int i = 0;
	for (; items[i] != NULL; i++)
	{
		free(items[i]); items[i] = NULL;
	}

	if (items != NULL)
	{
		free(items); items = NULL;
	}

	return 0;
}

int freea(void** &items)
{
	if (items == NULL)
		return 1;

	int i = 0;
	for (; items[i] != NULL; i++)
	{
		delete[] items[i]; items[i] = NULL;
	}

	if (items != NULL)
	{
		delete[] items; items = NULL;
	}

	return 0;
}

int free_field(Field** &items)
{
	if (items == NULL)
		return 1;

	int i = 0;
	for (; items[i] != NULL; i++)
	{
		delete[] items[i]->key; items[i]->key = NULL;
		delete[] items[i]->value; items[i]->value = NULL;
		delete[] items[i]; items[i] = NULL;
	}

	if (items != NULL)
	{
		delete[] items; items = NULL;
	}

	return 0;
}

int switch_type(Type t, bool b, char*&str)
{
	if (b)
	{
		switch (t)
		{
		case Int:
			str = " %s = %s";
			break;
		case Float:
		case Double:
			str = " %s = %s";
			break;
		case String:
			str = " %s = '%s'";
			break;
		case IntId:
			str = " WHERE %s = %s";
			break;
		case StringId:
			str = " WHERE %s='%s'";
			break;
		}
	}
	else
	{
		switch (t)
		{
		case Int:
			str = " ,%s = %s";
			break;
		case Float:
		case Double:
			str = " ,%s = %s";
			break;
		case String:
			str = " ,%s = '%s'";
			break;
		}
	}
	return 0;
}

int insert_type(Type t, int b, char*&str)
{
	if (b == 1)
	{
		switch (t)
		{
		case Int:
			str = " %s";
			break;
		case Float:
		case Double:
			str = " %s";
			break;
		case String:
			str = " '%s'";
			break;
		}
	}
	if (b == 2)
	{
		switch (t)
		{
		case Int:
			str = " ,%s";
			break;
		case Float:
		case Double:
			str = " ,%s";
			break;
		case String:
			str = " ,'%s'";
			break;
		}
	}
	if (b == 3)
	{
		switch (t)
		{
		case Int:
			str = " ,%s)";
			break;
		case Float:
		case Double:
			str = " ,%s)";
			break;
		case String:
			str = " ,'%s')";
			break;
		}
	}
	return 0;
}

/*创建字段对象*/
int new_field(Field** &field, int length)
{
	if (field != NULL)
		return -1;

	field = new Field*[length + 1];

	auto i = 0;
	for (; i < length; i++)
	{
		field[i] = new Field;
		field[i]->key = NULL;
		field[i]->value = NULL;
		field[i]->type = Int;
	}
	field[i] = NULL;//要多个空NULL做结尾

	return 0;
}

/*字段初始化*/
int init_field(Field* &field, char* key, void* value, Type type)
{
	unsigned long long *temp_value = NULL;
	float* float_value = NULL;
	double* double_value = NULL;
	int value_length = 0;
	int key_length = strlen(key) + 1;
	field->key = new char[key_length];
	strcpy_s(field->key, key_length, key);

	if (value != NULL)
	{
		switch (type)
		{
		case Type::String:
			value_length = strlen((char*)value) + 1;
			field->value = new char[value_length];
			strcpy_s(field->value, value_length, (char*)value);
			break;
		case Type::Int:
			temp_value = (unsigned long long*)value;
			field->value = new char[32];
			sprintf_s(field->value, 32, "%d", *temp_value);//在线
			break;
		case Type::Float:
			float_value = (double*)value;
			field->value = new char[64];
			sprintf_s(field->value, 64, "%f", *float_value);//在线
			break;
		case Type::Double:
			double_value = (double*)value;
			field->value = new char[64];
			sprintf_s(field->value, 64, "%f", *double_value);
			break;
		case Type::StringId:
			value_length = strlen((char*)value) + 1;
			field->value = new char[value_length];
			strcpy_s(field->value, value_length, (char*)value);
			break;
		case Type::IntId:
			temp_value = (unsigned long long*)value;
			field->value = new char[32];
			sprintf_s(field->value, 32, "%d", *temp_value);//在线
			break;
		}
	}

	field->type = type;

	return 0;
}

/*测试代码
Field** field = NULL;
new_field(field, 3);
init_field(field[0], table_data->getTable()->equipment_id(), s_equipment_key, String);
init_field(field[1], table_data->getTable()->transmission_time(), get_time(), String);
if (EQUAL(s_latitude, "")) {
	strcpy_s(s_latitude, BYTE_32, "39.98967762855358"); value = std::atof(s_latitude); }
init_field(field[2], table_data->getTable()->latitude(), &value, Float);
insert_data(tableName, field);
free_field(field);
*/
int insert_data(char* tableName, Field** field)
{
	MYSQL * sql = get_mysql(); //= my_init((MYSQL*) 0); 
	if (sql == NULL)
	{
		printf("请连接数据库!!!\n");
		return -1;
	}

	char* insert = "INSERT INTO %s (";
	int length = strlen(insert) + strlen(tableName) - 1;
	char*sentence = (char*)calloc(length, sizeof(char));
	sprintf_s(sentence, length, insert, tableName);
	char* bottom = "\0";
	char*bottoms = (char*)calloc(strlen("VALUES (\0") + 1, sizeof(char));
	strcpy_s(bottoms, strlen("VALUES (\0") + 1, ("VALUES (\0"));

	int count = 0;
	pointer_count((void**)field, count);
	for (int i = 0; i < count; i++)
	{
		if (field[i]->key == NULL)
			continue;
		if (field[i]->value == NULL)
		{
			printf("字段值为空了 !!!\n");
			return -1;
		}

		if (i > 0)
		{
			if (i == count - 1)
			{
				insert = ",%s)";
				insert_type(field[i]->type, 3, bottom);
			}
			else
			{
				insert = ",%s";
				insert_type(field[i]->type, 2, bottom);
			}
		}
		else
		{
			insert = "%s";
			insert_type(field[i]->type, 1, bottom);
		}

		length = strlen(sentence);
		append_new_buffer(sentence, length, insert, strlen(insert));//SELECT sn_id ,%s 
		length = strlen(sentence) + strlen(field[i]->key) - 1;//正常需要+1，那么把%s的长度二减去再加一就是-1；
		char*insert_ = (char*)calloc(length, sizeof(char));
		sprintf_s(insert_, length, sentence, field[i]->key);////SELECT sn_id ,na_id 
		if (sentence != NULL)
		{
			free(sentence); sentence = NULL;
		}
		sentence = insert_;

		length = strlen(bottoms);
		append_new_buffer(bottoms, length, bottom, strlen(bottom));//SELECT sn_id ,%s 
		length = strlen(bottoms) + strlen(field[i]->value) - 1;//正常需要+1，那么把%s的长度二减去再加一就是-1；
		char*bottom_ = (char*)calloc(length, sizeof(char));
		sprintf_s(bottom_, length, bottoms, field[i]->value);////SELECT sn_id ,na_id 
		if (bottoms != NULL)
		{
			free(bottoms); bottoms = NULL;
		}
		bottoms = bottom_;
	}

	length = strlen(sentence);
	append_new_buffer(sentence, length, bottoms, strlen(bottoms));//SELECT sn_id ,%s 

	int err = mysql_real_query(sql, sentence, strlen(sentence));
	if (sentence != NULL) { free(sentence); sentence = NULL; }
	if (err)
	{
		printf("插入时出错: %s !!!\n", mysql_error(sql));
		return -1;
	}
	return 0;
}

/*测试代码
Field** field = NULL;
new_field(field, 1);
init_field(field[0], table_data->getTable()->id(), &s_equipment_key, IntId);
delete_data(tableName,field[0]);
free_field(field);
*/
int delete_data(char* tableName, Field* field)
{
	MYSQL * sql = get_mysql();
	if (sql == NULL)
	{
		printf("请连接数据库!!!\n");
		return -1;
	}

	if (field == NULL || field->key == NULL)
	{
		printf("字段名为空!!!\n");
		return -2;
	}
	if (field->value == NULL)
	{
		printf("字段值为空了 !!!\n");
		return -1;
	}

	char* delet = "DELETE FROM %s ";
	int length = strlen(delet) + strlen(tableName) - 1;
	char*sentence = (char*)calloc(length, sizeof(char));
	sprintf_s(sentence, length, delet, tableName);

	switch_type(field->type, true, delet);
	length = strlen(sentence);
	append_new_buffer(sentence, length, delet, strlen(delet));//SELECT sn_id ,%s 
	length = strlen(sentence) + strlen(field->key) + strlen(field->value) - 1;

	char*sentence_ = (char*)calloc(length, sizeof(char));
	sprintf_s(sentence_, length, sentence, field->key, field->value);
	if (sentence != NULL) { free(sentence); sentence = NULL; }

	int err = mysql_real_query(sql, sentence_, strlen(sentence_));
	if (sentence_ != NULL) { free(sentence_); sentence_ = NULL; }
	if (err)
	{
		printf("删除时出错: %s !!!\n", mysql_error(sql));
		return -1;
	}
	return 0;
}

/*测试代码
Field** field = NULL;
new_field(field, 3);
init_field(field[0], table_equipment->getTable()->id(), s_equipment_key, StringId);
init_field(field[1], table_equipment->getTable()->sn_id(), &d_equipment_sn, Int);
value = (buf_msg[6] << 8) | buf_msg[5];
init_field(field[2], table_equipment->getTable()->model(), &value, Int);
update_data(tableName, field);
free_field(field);
*/
int update_data(char* tableName, Field**field)
{
	MYSQL * sql = get_mysql();
	if (sql == NULL)
	{
		printf("请连接数据库!!!\n");
		return -1;
	}

	/*----------------------------------------UPDATE字符串拼接start--------------------------------------*/
	char* update = "UPDATE %s SET";
	int length = strlen(update) + strlen(tableName) - 1;
	char*sentence = (char*)calloc(length, sizeof(char));
	sprintf_s(sentence, length, update, tableName);
	unsigned int index = 0;
	bool one = false;
	int count = 0;
	pointer_count((void**)field, count);
	for (int i = 0; i < count; i++)
	{
		if (field[i]->key == NULL)
			continue;
		if (field[i]->value == NULL)
		{
			printf("字段值为空了 !!!\n");
			return -1;
		}

		if (field[i]->type == StringId || field[i]->type == IntId)
		{
			index = i;
		}
		else
		{
			if (one)
			{
				switch_type(field[i]->type, false, update);
			}
			else
			{
				one = true;//首次字符串拼接没有‘，’号
				switch_type(field[i]->type, true, update);
			}

			length = strlen(sentence);
			append_new_buffer(sentence, length, update, strlen(update));//SELECT sn_id ,%s 
			length = strlen(sentence) + strlen(field[i]->key) + strlen(field[i]->value) - 1;//正常需要+1，那么把%s的长度二减去再加一就是-1；
			char*update_ = (char*)calloc(length, sizeof(char));
			sprintf_s(update_, length, sentence, field[i]->key, field[i]->value);////SELECT sn_id ,na_id 
			if (sentence != NULL)
			{
				free(sentence); sentence = NULL;
			}
			sentence = update_;
		}
	}

	switch_type(field[index]->type, true, update);
	length = strlen(sentence);
	append_new_buffer(sentence, length, update, strlen(update));//SELECT sn_id ,%s 
	length = length + strlen(field[index]->key) + strlen(field[index]->value) - 3;

	char*sentence_ = (char*)calloc(length, sizeof(char));
	sprintf_s(sentence_, length, sentence, field[index]->key, field[index]->value);
	if (sentence != NULL) { free(sentence); sentence = NULL; }
	/*-------------------------------------SELECT字符串拼接end-----------------------------------------*/

	int err = mysql_real_query(sql, sentence_, strlen(sentence_));
	if (sentence_ != NULL) { free(sentence_); sentence_ = NULL; }
	if (err)
	{
		printf("查询时出错: %s !!!\n", mysql_error(sql));
		return -1;
	}
	return 0;
}

/*测试代码
Field** field = NULL;
new_field(field, 2);
init_field(field[0], table_equipment->getTable()->sn_id(), NULL, Int);
init_field(field[1], table_equipment->getTable()->id(), s_equipment_key, StringId);
selete_data(tableName, field);
free_field(field);
*/
int selete_data(char* tableName, Field**&field)
{
	MYSQL * sql = get_mysql();
	if (sql == NULL)
	{
		printf("请连接数据库!!!\n");
		return -1;
	}

	/*----------------------------------------SELECT字符串拼接start--------------------------------------*/
	char* select = "SELECT %s ";
	int length = strlen(select) + strlen(field[0]->key) - 1;
	char*sentence = (char*)calloc(length, sizeof(char));
	sprintf_s(sentence, length, select, field[0]->key);
	unsigned int index = 0;
	int count = 0;
	pointer_count((void**)field, count);
	for (int i = 1; i < count; i++)
	{
		if (field[i]->key == NULL)
			continue;
		
		if (field[i]->type == StringId || field[i]->type == IntId)
		{
			if (field[i]->value == NULL)
			{
				printf("字段值为空了 !!!\n");
				return -1;
			}
			index = i;
		}
		else
		{
			select = ",%s ";
			length = strlen(sentence);
			append_new_buffer(sentence, length, select, strlen(select));//SELECT sn_id ,%s 
			length = strlen(sentence) + strlen(field[i]->key) - 1;
			char*select_ = (char*)calloc(length, sizeof(char));
			sprintf_s(select_, length, sentence, field[i]->key);////SELECT sn_id ,na_id 
			if (sentence != NULL)
			{
				free(sentence); sentence = NULL;
			}
			sentence = select_;
		}
	}

	select = " FROM %s";//" WHERE %s='%s'";
	length = strlen(sentence);
	append_new_buffer(sentence, length, select, strlen(select));//SELECT sn_id ,%s 
	switch_type(field[index]->type, true, select);
	length = strlen(sentence);
	append_new_buffer(sentence, length, select, strlen(select));//SELECT sn_id ,%s 
	length = length + strlen(tableName) + strlen(field[index]->key) + strlen(field[index]->value) - 5;

	char*sentence_ = (char*)calloc(length, sizeof(char));
	sprintf_s(sentence_, length, sentence, tableName, field[index]->key, field[index]->value);
	if (sentence != NULL) { free(sentence); sentence = NULL; }
	/*-------------------------------------SELECT字符串拼接end-----------------------------------------*/

	int err = mysql_real_query(sql, sentence_, length);
	if (sentence_ != NULL) { free(sentence_); sentence_ = NULL; }
	if (err)
	{
		printf("查询时出错: %s !!!\n", mysql_error(sql));
		return -1;
	}

	MYSQL_RES *m_res = NULL;
	m_res = mysql_store_result(sql);//将结果保存在res结构体中
	//循环打印每行每个字段
	MYSQL_ROW row = NULL;
	row = mysql_fetch_row(m_res);
	if (NULL == row)
	{
		printf("没有查询到对应的数据!!!\n");
		return -1;
	}

	unsigned int t = 0,j=0;
	for (; j < mysql_num_fields(m_res); t++, j++) {
		if (field[t]->type == StringId || field[t]->type == IntId)
			t++;
		if (field[t]->value == NULL)
		{
			length = strlen(row[t]) + 1;
			field[t]->value = (char*)calloc(length, sizeof(char));
		}
		strcpy_s(field[t]->value, length, row[j]);
	}
	/*for (t = 0; t < count; t++) {
	printf("'%s':%s\n", field[t], result[t]);
	}*/

	mysql_free_result(m_res);

	return 0;
}

/*测试代码
char**results;
char**field = new char*[4];
field[0] = new char[64];
field[1] = new char[64];
field[2] = new char[64];
field[3] = NULL;
char*sn = "sn_id";
char*name = "name";
char*electricity = "request_count";
strcpy_s(field[0], 64, sn);
strcpy_s(field[1], 64, name);
strcpy_s(field[2], 64, electricity);

err = seleteData(results, tableName, s_equipment_key, field);
frees(field);
frees(results);
*/
int selete_data_(char**&result, char* tableName, char* id, char**field)
{
	MYSQL * sql = get_mysql();
	if (sql == NULL)
	{
		printf("请连接数据库!!!\n");
		return NULL;
	}

	/*----------------------------------------SELECT字符串拼接start--------------------------------------*/
	char* select = "SELECT %s ";
	int length = strlen(select) + strlen(field[0]) - 1;
	char*sentence = (char*)calloc(length, sizeof(char));
	sprintf_s(sentence, length, select, field[0]);

	int count = 0;
	pointer_count((void**)field, count);
	for (int i = 1; i < count; i++)
	{
		select = ",%s ";
		length = strlen(sentence);
		append_new_buffer(sentence, length, select, strlen(select));//SELECT sn_id ,%s 
		length = strlen(sentence) + strlen(field[i]) - 1;
		char*select_ = (char*)calloc(length, sizeof(char));
		sprintf_s(select_, length, sentence, field[i]);////SELECT sn_id ,na_id 
		if (sentence != NULL)
		{
			free(sentence); sentence = NULL;
		}
		sentence = select_;
	}
	select = " FROM %s WHERE id='%s'";
	length = strlen(sentence);
	append_new_buffer(sentence, length, select, strlen(select));//SELECT sn_id ,%s 
	length = length + strlen(tableName) + strlen(id) - 3;

	char*sentence_ = (char*)calloc(length, sizeof(char));
	sprintf_s(sentence_, length, sentence, tableName, id);
	if (sentence != NULL) { free(sentence); sentence = NULL; }
	/*-------------------------------------SELECT字符串拼接end-----------------------------------------*/

	int err = mysql_real_query(sql, sentence_, length);
	if (sentence_ != NULL) { free(sentence_); sentence_ = NULL; }
	if (err)
	{
		printf("查询时出错: %s !!!\n", mysql_error(sql));
		return -1;
	}

	MYSQL_RES *m_res = NULL;
	m_res = mysql_store_result(sql);//将结果保存在res结构体中
	//循环打印每行每个字段
	MYSQL_ROW row = NULL;
	row = mysql_fetch_row(m_res);
	if (NULL == row)
	{
		printf("没有查询到对应的数据!!!\n");
		return -1;
	}

	result = (char**)calloc(count + 1, sizeof(char*));
	unsigned int t;
	for (t = 0; t < mysql_num_fields(m_res); t++) {
		int length = strlen(row[t]) + 1;
		result[t] = (char*)calloc(length, sizeof(char));
		strcpy_s(result[t], length, row[t]);
	}
	result[t] = NULL;
	/*for (t = 0; t < count; t++) {
	printf("'%s':%s\n", field[t], result[t]);
	}*/

	mysql_free_result(m_res);

	return 0;
}

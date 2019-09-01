
#include "stdio.h"
#include "MySqlDll.h"


int main()
{
	char* fileName = "/mySqlConfig.ini";
	int error = 0;
	error = sql_connect(fileName);

	//增加
	Field** field = NULL;
	new_field(field, 3);
	double value=3.12;
	char* equipment_id = "abcd";
	char* get_time="2019-06-20 16:37:51";
	init_field(field[0], "equipment_id", equipment_id, String);
	init_field(field[1], "transmission_time", get_time, String);
	init_field(field[2], "latitude", &value, Float);
	insert_data("tablue_name", field);
	free_field(field);
	
	//修改
	new_field(field, 3);
	int id = 25;
	equipment_id = "defg";
	double latitude = 3.14;
	init_field(field[0], "id", &id, IntId);
	init_field(field[1], "equipment_id", equipment_id, String);
	init_field(field[2], "latitude", &latitude, Float);
	update_data("tablue_name", field);
	free_field(field);

	//查询
	new_field(field, 4);
	init_field(field[0], "equipment_id", NULL, String);
	init_field(field[1], "id", &id, IntId);
	init_field(field[2], "latitude", NULL, Float);
	init_field(field[3], "transmission_time", NULL, Float);
	selete_data("tablue_name", field);
	for (size_t i = 0; field[i]!=NULL; i++)
	{
		if (field[i]->type == String)
		{
			printf("%s\n", field[i]->value);
		}
		if (field[i]->type == Int)
		{
			printf("%d\n", field[i]->value);
		}
		if (field[i]->type == Float)
		{
			printf("%s\n", field[i]->value);
		}
	}
	free_field(field);

	//删除
	new_field(field, 1);
	init_field(field[0], "id", &id, IntId);
	delete_data("tablue_name", field[0]);
	free_field(field);

	system("pause");
	return 0;
}
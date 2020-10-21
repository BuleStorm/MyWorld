#include "OperationDB.h"

#include <iostream>
#include <string.h>
#include <string>

static CreateWorld* createWorld = CreateWorld::getInstance();


OperationDB* OperationDB::getInstance()
{
	static OperationDB operation_db;
	return &operation_db;
}

void OperationDB::init()
{
	int ret = 0;
	const char* path = "./ce.db";

	ret = sqlite3_open_v2(path, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, nullptr);

	if (ret == SQLITE_OK) 
	{
		std::cout << "打开数据库成功" << std::endl;
	}
	else 
	{
		std::cout << "创建数据库ing..." << std::endl;
		ret = sqlite3_open_v2(path, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, nullptr);

		if (ret == SQLITE_OK)
			std::cout << "创建数据库成功" << std::endl;
		else
			std::cout << "创建数据库失败" << std::endl;
	}

	createUserTable();
	createUserSaveTable();
}

void OperationDB::saveBlockMap(vec3 *data, const char* table_name, int block_x, int block_z, int block_y)
{
	string block_data_str = "";

	for (int i = block_y * BLOCK_WIDTH; i < (block_y + 1) * BLOCK_WIDTH; i++)
		for (int j = block_z * BLOCK_WIDTH; j < (block_z + 1) * BLOCK_WIDTH; j++)
			for (int k = block_x * BLOCK_WIDTH; k < (block_x + 1) * BLOCK_WIDTH; k++)
			{
				string cube_type = "";
				if (int((*data)[k][i][j]) >= 10)
				{
					cube_type = std::to_string(int((*data)[k][i][j]));
				}
				else
				{
					cube_type = "0";
					cube_type.append(std::to_string(int((*data)[k][i][j])));
				}

				block_data_str.append(cube_type).append(" ");
			}

	string sql_str = "";
	sql_str.resize(13000);

	sprintf(&sql_str[0], "insert into %s values(%d, %d, %d, '%s')", table_name, block_x, block_z, block_y, block_data_str.data());

	sqlite3_stmt* stmt = nullptr;

	int ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (ret == SQLITE_OK)
	{
		sqlite3_step(stmt);
	}
	else
	{
		std::cout << "insert" << table_name << "失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}

	sqlite3_finalize(stmt);
}

void OperationDB::updateBlockMap(vec3* data, const char* table_name, int block_x, int block_z, int block_y)
{
	string block_data_str = "";

	for (int i = block_y * BLOCK_WIDTH; i < (block_y + 1) * BLOCK_WIDTH; i++)
		for (int j = block_z * BLOCK_WIDTH; j < (block_z + 1) * BLOCK_WIDTH; j++)
			for (int k = block_x * BLOCK_WIDTH; k < (block_x + 1) * BLOCK_WIDTH; k++)
			{
				string cube_type = "";
				if (int((*data)[k][i][j]) >= 10)
				{
					cube_type = std::to_string(int((*data)[k][i][j]));
				}
				else
				{
					cube_type = "0";
					cube_type.append(std::to_string(int((*data)[k][i][j])));
				}

				block_data_str.append(cube_type).append(" ");
			}

	string sql_str = "";
	sql_str.resize(13000);

	sprintf(&sql_str[0], "update %s set block_data = '%s' where block_x = %d and block_z = %d and block_y = %d",
			table_name, block_data_str.data(), block_x, block_z, block_y);

	sqlite3_stmt* stmt = nullptr;

	int ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (ret == SQLITE_OK)
	{
		sqlite3_step(stmt);
	}
	else
	{
		std::cout << "update" << table_name << "失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}

	sqlite3_finalize(stmt);
}

void OperationDB::saveWorldMap(std::string map_name)
{
	if (isMapExist(map_name))
		deleteRecordMap(map_name);
	else
		createRecordMap(map_name);

	table_name_map.append(map_name).append("_map");
	table_name_plant.append(map_name).append("_plant");
	table_name_leaf.append(map_name).append("_leaf");

	char* err_str;
	int ret = sqlite3_exec(sqlite, "begin", 0, 0, &err_str);

	if (ret != SQLITE_OK)
		cout << "开始事务失败: " << err_str << endl;

	sqlite3_free(err_str);

	for (int block_y = 0; block_y < WORLD_HEIGHT / 16; block_y++)
		for (int block_z = 0; block_z < WORLD_WIDTH / 16; block_z++)
			for (int block_x = 0; block_x < WORLD_WIDTH / 16; block_x++)
			{
				saveBlockMap(&createWorld->mapData, table_name_map.data(), block_x, block_z, block_y);
				saveBlockMap(&createWorld->plantData, table_name_plant.data(), block_x, block_z, block_y);
				saveBlockMap(&createWorld->leafData, table_name_leaf.data(), block_x, block_z, block_y);
			}


	ret = sqlite3_exec(sqlite, "commit", 0, 0, &err_str);
	if (ret != SQLITE_OK)
		cout << "提交事务失败: " << err_str << endl;

	sqlite3_free(err_str);
}

void OperationDB::loadWorldMap(std::string map_name)
{
	table_name_map.append(map_name).append("_map");
	table_name_plant.append(map_name).append("_plant");
	table_name_leaf.append(map_name).append("_leaf");

	int ret = 0;
	string sql_str = "";
	sqlite3_stmt* stmt = nullptr;

	sql_str = "select * from ";
	sql_str.append(map_name).append("_map");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (ret == SQLITE_OK) 
	{
		while (sqlite3_step(stmt) == SQLITE_ROW) 
		{
			int block_x = sqlite3_column_int(stmt, 0);
			int block_z = sqlite3_column_int(stmt, 1);
			int block_y = sqlite3_column_int(stmt, 2);

			const unsigned char* block_data = sqlite3_column_text(stmt, 3);

			int read_index = 0;
			char cube_type[2] = { 0 };

			for (int i = block_y * BLOCK_WIDTH; i < (block_y + 1) * BLOCK_WIDTH; i++)
				for (int j = block_z * BLOCK_WIDTH; j < (block_z + 1) * BLOCK_WIDTH; j++)
					for (int k = block_x * BLOCK_WIDTH; k < (block_x + 1) * BLOCK_WIDTH; k++)
					{
						memcpy(cube_type, block_data + read_index, sizeof(cube_type));
						read_index += 3;

						createWorld->mapData[k][i][j] = CubeType(atoi(cube_type));
					}
		}
	}
	else 
	{
		std::cout << "select block_data失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);

	sql_str = "select * from ";
	sql_str.append(map_name).append("_plant");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			int block_x = sqlite3_column_int(stmt, 0);
			int block_z = sqlite3_column_int(stmt, 1);
			int block_y = sqlite3_column_int(stmt, 2);

			const unsigned char* block_data = sqlite3_column_text(stmt, 3);

			int read_index = 0;
			char cube_type[2] = { 0 };

			for (int i = block_y * BLOCK_WIDTH; i < (block_y + 1) * BLOCK_WIDTH; i++)
				for (int j = block_z * BLOCK_WIDTH; j < (block_z + 1) * BLOCK_WIDTH; j++)
					for (int k = block_x * BLOCK_WIDTH; k < (block_x + 1) * BLOCK_WIDTH; k++)
					{
						memcpy(cube_type, block_data + read_index, sizeof(cube_type));
						read_index += 3;

						createWorld->plantData[k][i][j] = CubeType(atoi(cube_type));
					}
		}
	}
	else
	{
		std::cout << "select block_data失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);

	sql_str = "select * from ";
	sql_str.append(map_name).append("_leaf");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			int block_x = sqlite3_column_int(stmt, 0);
			int block_z = sqlite3_column_int(stmt, 1);
			int block_y = sqlite3_column_int(stmt, 2);

			const unsigned char* block_data = sqlite3_column_text(stmt, 3);

			int read_index = 0;
			char cube_type[2] = { 0 };

			for (int i = block_y * BLOCK_WIDTH; i < (block_y + 1) * BLOCK_WIDTH; i++)
				for (int j = block_z * BLOCK_WIDTH; j < (block_z + 1) * BLOCK_WIDTH; j++)
					for (int k = block_x * BLOCK_WIDTH; k < (block_x + 1) * BLOCK_WIDTH; k++)
					{
						memcpy(cube_type, block_data + read_index, sizeof(cube_type));
						read_index += 3;

						createWorld->leafData[k][i][j] = CubeType(atoi(cube_type));
					}
		}
	}
	else
	{
		std::cout << "select block_data失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);
}

bool OperationDB::isMapExist(std::string map_name)
{
	bool ret = false;
	sqlite3_stmt* stmt = nullptr;
	string sql_str = "select count(*)  from sqlite_master where type = 'table' and name = '";
	sql_str.append(map_name).append("_map'");

	sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0)
		ret = true;

	sqlite3_finalize(stmt);

	return ret;
}

void OperationDB::deleteRecordMap(std::string map_name)
{
	int ret = 0;
	sqlite3_stmt* stmt = nullptr;
	string sql_str = "";


	sql_str = "delete from ";
	sql_str.append(map_name).append("_map");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK)
		sqlite3_step(stmt);
	else
		std::cout << "delete from失败: " << sqlite3_errmsg(sqlite) << std::endl;
	sqlite3_finalize(stmt);


	sql_str = "delete from ";
	sql_str.append(map_name).append("_plant");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK)
		sqlite3_step(stmt);
	else
		std::cout << "delete from失败: " << sqlite3_errmsg(sqlite) << std::endl;
	sqlite3_finalize(stmt);


	sql_str = "delete from ";
	sql_str.append(map_name).append("_leaf");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK)
		sqlite3_step(stmt);
	else
		std::cout << "delete from失败: " << sqlite3_errmsg(sqlite) << std::endl;
	sqlite3_finalize(stmt);
}

OperationDB::OperationDB()
{

}

OperationDB::~OperationDB()
{
	if (sqlite) 
	{
		sqlite3_close_v2(sqlite);
		sqlite = nullptr;
	}
}

void OperationDB::createRecordMap(std::string map_name)
{
	int result = 0;
	char sql_str[100] = { 0 };
	sqlite3_stmt* stmt = nullptr;

	sprintf(sql_str, "create table %s_%s(block_x int, block_z int, block_y int, block_data text)", map_name.data(), "map");
	result = sqlite3_prepare_v2(sqlite, sql_str, -1, &stmt, nullptr);
	if (result == SQLITE_OK)
	{
		sqlite3_step(stmt);
		std::cout << "创建表map成功" << std::endl;
	}
	else
	{
		std::cout << "创建表map失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);


	sprintf(sql_str, "create table %s_%s(block_x int, block_z int, block_y int, block_data text)", map_name.data(), "plant");
	result = sqlite3_prepare_v2(sqlite, sql_str, -1, &stmt, nullptr);
	if (result == SQLITE_OK)
	{
		sqlite3_step(stmt);
		std::cout << "创建表plant成功" << std::endl;
	}
	else
	{
		std::cout << "创建表plant失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);


	sprintf(sql_str, "create table %s_%s(block_x int, block_z int, block_y int, block_data text)", map_name.data(), "leaf");
	result = sqlite3_prepare_v2(sqlite, sql_str, -1, &stmt, nullptr);
	if (result == SQLITE_OK)
	{
		sqlite3_step(stmt);
		std::cout << "创建表leaf成功" << std::endl;
	}
	else
	{
		std::cout << "创建表leaf失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);
}

bool OperationDB::comparedUserPass(std::string name, std::string password)
{
	string sql_str = "";
	sql_str.resize(13000);

	sprintf(&sql_str[0], "select user_id from user where user_name = '%s' and password = '%s'",
		name.c_str(), password.c_str());

	sqlite3_stmt* stmt = nullptr;

	int ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (ret == SQLITE_OK)
	{
		if ((ret = sqlite3_step(stmt)) == SQLITE_ROW)
		{
			return true;
		}
	}
	else
	{
		std::cout << "添加user失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}

	sqlite3_finalize(stmt);
	return false;
}

void OperationDB::insertUser(std::string name, std::string password)
{
	string sql_str = "";
	sql_str.resize(13000);

	sprintf(&sql_str[0], "insert into user(user_name, password) values('%s', '%s')",
		name.c_str(), password.c_str());

	sqlite3_stmt* stmt = nullptr;

	int ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (ret == SQLITE_OK)
	{
		sqlite3_step(stmt);
		std::cout << "添加user成功" << std::endl;
	}
	else
	{
		std::cout << "添加user失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}

	sqlite3_finalize(stmt);
}

void OperationDB::insertSaveMap(int user_id, std::string mapName)
{
	string sql_str = "";
	sql_str.resize(13000);

	sprintf(&sql_str[0], "insert into user_save(map_name, user_id) values('%s', %d)", mapName.data(), user_id);

	sqlite3_stmt* stmt = nullptr;

	int ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (ret == SQLITE_OK)
	{
		sqlite3_step(stmt);
		std::cout << "添加user_save成功" << std::endl;
	}
	else
	{
		std::cout << "添加user_save失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}

	sqlite3_finalize(stmt);
}

std::list<std::string> OperationDB::selectUserSaveMap(std::string name)
{
	std::list<std::string> saveList;

	string sql_str = "";
	sql_str.resize(13000);

	sprintf(&sql_str[0], "select user_id from user where user_name = '%s'",
		name.c_str());

	sqlite3_stmt* stmt = nullptr;

	int ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (ret == SQLITE_OK)
	{
		if ((ret = sqlite3_step(stmt)) == SQLITE_ROW)
		{
			user_id = sqlite3_column_int(stmt, 0);
		}
		else
		{
			std::cout << "selectUserSaveMap 未找到user_id" << sqlite3_errmsg(sqlite) << std::endl;
		}
	}
	else
	{
		std::cout << "selectUserSaveMap 未找到user_id" << sqlite3_errmsg(sqlite) << std::endl;
	}


	string sqlStr = "";
	sqlStr.resize(1000);
	sprintf(&sqlStr[0], "select map_id, map_name from user_save where user_id = %d", user_id);

	sqlite3_stmt* stmt1 = nullptr;

	ret = sqlite3_prepare_v2(sqlite, sqlStr.data(), -1, &stmt1, nullptr);

	if (ret == SQLITE_OK)
	{
		while ((ret = sqlite3_step(stmt1)) == SQLITE_ROW)
		{
			const char* map_name = (const char *)sqlite3_column_text(stmt1, 1);
			saveList.push_back(map_name);
		}
	}
	else
	{
		std::cout << "selectUserSaveMap 错误" << sqlite3_errmsg(sqlite) << std::endl;
	}
	return saveList;
}

void OperationDB::createUserTable()
{
	string sql_str = "";
	sql_str.resize(13000);

	sprintf(&sql_str[0], "create table if not exists user("
					"user_id integer primary key,"
                    "user_name char(20),"
                    "password char(20)"
					")");

	sqlite3_stmt* stmt = nullptr;

	int ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (ret == SQLITE_OK)
	{
		sqlite3_step(stmt);
		std::cout << "创建createUserTable成功" << std::endl;
	}
	else
	{
		std::cout << "创建createUserTable失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}

	sqlite3_finalize(stmt);
}

void OperationDB::createUserSaveTable()
{
	string sql_str = "";
	sql_str.resize(13000);

	sprintf(&sql_str[0], "create table if not exists user_save("
					"map_id integer primary key,"
					"map_name char(20),"
					"user_id int"
					")");

	sqlite3_stmt* stmt = nullptr;

	int ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (ret == SQLITE_OK)
	{
		sqlite3_step(stmt);
		std::cout << "创建createUserSaveTable成功" << std::endl;
	}
	else
	{
		std::cout << "创建createUserSaveTable失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}

	sqlite3_finalize(stmt);
}

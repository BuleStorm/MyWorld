#ifndef OPERATIONDB_H
#define OPERATIONDB_H

#include <sqlite3.h>
#include <list>
#include "CreateWorld.h"

// 区块边长
#define BLOCK_WIDTH 16

class OperationDB
{
public:
	static OperationDB* getInstance();
public:
	void init();
	void saveBlockMap(vec3 *data, const char* table_name, int block_x, int block_z, int block_y);
	void updateBlockMap(vec3* data, const char* table_name, int block_x, int block_z, int block_y);
	void saveWorldMap(std::string map_name);
	void loadWorldMap(std::string map_name);
	bool comparedUserPass(std::string name, std::string password);
	void insertUser(std::string name, std::string password);
	void insertSaveMap(int user_id, std::string mapName);
	std::list<std::string> selectUserSaveMap(std::string name);
	int user_id = -1;
	string table_name_map = "";
	string table_name_plant = "";
	string table_name_leaf = "";
private:
	OperationDB();
	~OperationDB();
	OperationDB(const OperationDB&) = delete;
	OperationDB& operator = (const OperationDB&) = delete;

private:
	sqlite3* sqlite = nullptr;

	bool isMapExist(std::string map_name);
	void deleteRecordMap(std::string map_name);
	void createRecordMap(std::string map_name);

	void createUserTable();
	void createUserSaveTable();
};

#endif // !OPERATIONDB_H
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "CharacterData.h"
#include "Character.h"

class Player;
class Enemy;

class CharacterFactory {

	//// プレイヤーのデータテーブル
	//static const CharacterData PlayerTable[];
	//// プレイヤーのスキルデータテーブル
	//static const Skill PlayerSkillTable[];
	//// エネミーのデータテーブル
	//static const CharacterData EnemyTable[];
	//// エネミーのスキルデータテーブル
	//static const Skill EnemySkillTable[];

	// プレイヤーのデータテーブル
	std::vector<CharacterData> PlayerTable;
	// エネミーのデータテーブル
	std::vector<CharacterData> EnemyTable;
	// スキルデータテーブル
	std::vector<Skill> SkillTable;

	bool LoadCSV(const std::string& path, std::vector<std::vector<std::string>>& outData);


public:

	//// 指定したIDのキャラクターの生成関数
	//static std::shared_ptr<Character> CreatePlayer(int ID, int sklID);
	//static std::shared_ptr<Character> CreateEnemy(int ID, int sklID);
	//// プレイヤーのデータテーブルサイズ
	//static const int PlayerTableSize;
	//// プレイヤーのスキルデータテーブルサイズ
	//static const int PlayerSkillTableSize;
	//// エネミーのデータテーブルサイズ
	//static const int EnemyTableSize;
	//// エネミーのスキルデータテーブルサイズ
	//static const int EnemySkillTableSize;


	bool LoadPlayerData(const std::string& path);
	bool LoadEnemyData(const std::string& path);
	bool LoadSkillData(const std::string& path);

	std::shared_ptr<Character> CreatePlayer(int id);
	std::shared_ptr<Character> CreateEnemy(int id);

	const CharacterData* GetCharacterData(int id) const;
	const Skill* GetSkillData(int id) const;

	const std::vector<CharacterData>& getPlayerTable() const { return PlayerTable; }


};

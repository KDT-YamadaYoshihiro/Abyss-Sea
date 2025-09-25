#pragma once
#include "characterData.h"
#include "Character.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

class Player;
class Enemy;

//class CharacterFactory {
//
//	 プレイヤーのデータテーブル
//	static const CharacterData PlayerTable[];
//	 プレイヤーのスキルデータテーブル
//	static const Skill PlayerSkillTable[];
//
//	 エネミーのデータテーブル
//	static const CharacterData EnemyTable[];
//	 エネミーのスキルデータテーブル
//	static const Skill EnemySkillTable[];
//
//
//public:
//
//	 指定したIDのキャラクターの生成関数
//	static std::shared_ptr<Character> CreatePlayer(int ID, int sklID);
//	static std::shared_ptr<Character> CreateEnemy(int ID, int sklID);
//
//	 プレイヤーのデータテーブルサイズ
//	static const int PlayerTableSize;
//	 プレイヤーのスキルデータテーブルサイズ
//	static const int PlayerSkillTableSize;
//	 エネミーのデータテーブルサイズ
//	static const int EnemyTableSize;
//	 エネミーのスキルデータテーブルサイズ
//	static const int EnemySkillTableSize;
//
//};

class CharacterFactory {

	CharacterFactory() = default;
	virtual ~CharacterFactory() {};

public:

	CharacterFactory(const CharacterFactory&) = delete;
	CharacterFactory& operator = (const CharacterFactory) = delete;
	static CharacterFactory& Instance() {
		static CharacterFactory instance;
		return instance;
	}


	// CSVのロード
	bool LoadCharacterTable(const std::string& fileName);
	bool LoadSkillTable(const std::string& fileName);

	// 生成
	CharacterData getCharacter(int id);
	SkillData getSkill(int id);

	std::shared_ptr<Character> CreateCharacter(int id);

private:

	std::vector<CharacterData> CharacterTable;
	std::vector<SkillData> SkillTable;


};
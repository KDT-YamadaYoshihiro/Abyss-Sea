#pragma once
#include <vector>
#include <memory>
#include <string>
#include "CharacterData.h"
#include "Character.h"

class Player;
class Enemy;

class CharacterFactory {

	//// �v���C���[�̃f�[�^�e�[�u��
	//static const CharacterData PlayerTable[];
	//// �v���C���[�̃X�L���f�[�^�e�[�u��
	//static const Skill PlayerSkillTable[];
	//// �G�l�~�[�̃f�[�^�e�[�u��
	//static const CharacterData EnemyTable[];
	//// �G�l�~�[�̃X�L���f�[�^�e�[�u��
	//static const Skill EnemySkillTable[];

	// �v���C���[�̃f�[�^�e�[�u��
	std::vector<CharacterData> PlayerTable;
	// �G�l�~�[�̃f�[�^�e�[�u��
	std::vector<CharacterData> EnemyTable;
	// �X�L���f�[�^�e�[�u��
	std::vector<Skill> SkillTable;

	bool LoadCSV(const std::string& path, std::vector<std::vector<std::string>>& outData);


public:

	//// �w�肵��ID�̃L�����N�^�[�̐����֐�
	//static std::shared_ptr<Character> CreatePlayer(int ID, int sklID);
	//static std::shared_ptr<Character> CreateEnemy(int ID, int sklID);
	//// �v���C���[�̃f�[�^�e�[�u���T�C�Y
	//static const int PlayerTableSize;
	//// �v���C���[�̃X�L���f�[�^�e�[�u���T�C�Y
	//static const int PlayerSkillTableSize;
	//// �G�l�~�[�̃f�[�^�e�[�u���T�C�Y
	//static const int EnemyTableSize;
	//// �G�l�~�[�̃X�L���f�[�^�e�[�u���T�C�Y
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

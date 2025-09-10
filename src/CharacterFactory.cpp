#include "CharacterFactory.h"
#include "CPlayer.h"
#include "CEnemy.h"

#include <fstream>
#include <sstream>
#include <iostream>

//
//// �����X�e�[�^�X���
//const CharacterData CharacterFactory::PlayerTable[] = {
//	//  ID, Name,		HP,		ATK,	 DEF,	AGR,	Lv, Exp, Alive,	
//	{AKIRA,	"�A�L��",	100,	35,		10,		10,		1,	0,	true},
//	{EL,	"�G��",		 90,	28,		15,		20,		1,	0,	true},
//	{ALIA,	"�A���A",	140,	15,		10,		18,		1,	0,	true},
//	{PINOT,	"�s�m",		 90,	22,		18,		25,		1,	0,	true},
//	{KAGE,	"�J�Q",		100,	25,		10,		32,		1,	0,	true}
//};
//
////�@�v���C���[�X�L�����
//const Skill CharacterFactory::PlayerSkillTable[] = {
//	// ID, skillName,					skilltype,			targetType				skillPower		skilltTrun		EffectType,				Details
//	{AKIRA, "����a",					SkillType::ATTACK,		SkillTargetType::SINGLE_ENEMY,	 1.5f,		0,	EffectType::EF_SKILL1,	"�G�P�̂ɋ��͂ȋZ�����"},
//	{EL,	"�Z���t�B�b�N�u���[�h",		SkillType::ATTACK,		SkillTargetType::SINGLE_ENEMY,	 1.5f,		0,	EffectType::EF_SKILL2,	"�G�P�̂ɘA�������"},
//	{ALIA,	"�q�[��",					SkillType::HEAL,		SkillTargetType::SINGLE_ALLY,	 0.6f,		0,	EffectType::EF_HEAL,	"�����P�̂�HP���񕜂�����"},
//	{PINOT,	"���̉���",					SkillType::ATK_BUFF,	SkillTargetType::SINGLE_ALLY,	 1.3f,		2,	EffectType::EF_BUFF,	"�����P�̂̍U���͂�\n�@�@�@�㏸������i2�^�[���p���j"},
//	{KAGE,	"�u���̉���",				SkillType::AGR_BUFF,	SkillTargetType::SINGLE_ALLY,	 1.3f,		2,	EffectType::EF_BUFF,	"�����P�̂̍s�����x��\n�@�@�@�㏸������i2�^�[���p���j"}
//
//};
//
//// �X�e�[�^�X���
//const CharacterData CharacterFactory::EnemyTable[] = {
//	//  ID,		Name,				HP,			ATK,	DEF,	AGR,	Lv,		Exp,	Alive,	
//	{MERFOLK,	"Zoth=Ommog",	    610,		   28,	   22,	 22,	 5,		 8000,	true},
//	{DEEPONES,	"Ythogtha",		   6500,		 1060,	  460,	 89,	17,		15000,	true},
//	{KTANID,	"Cthulhu",		  18680,		 2256,	 1860,	960,	28,		30000,	true}
//};
//
//const Skill CharacterFactory::EnemySkillTable[] = {
//	// ID,		skillName,	skilltype,			targetType,				skillPower,	skillTrun,	EffectType			Details
//	{MERFOLK,	"NoName",	SkillType::NONE,	SkillTargetType::SELF,	0.0f,		0,			EffectType::EF_NONE,	""},
//	{DEEPONES,	"NoName",	SkillType::NONE,	SkillTargetType::SELF,	0.0f,		0,			EffectType::EF_NONE,	""},
//	{KTANID,	"NoName",	SkillType::NONE,	SkillTargetType::SELF,	0.0f,		0,			EffectType::EF_NONE,	""}
//};
//
//
//// �v���C���[�f�[�^�e�[�u���T�C�Y
//const int CharacterFactory::PlayerTableSize = sizeof(PlayerTable) / sizeof(CharacterData);
//const int CharacterFactory::PlayerSkillTableSize = sizeof(PlayerSkillTable) / sizeof(Skill);
//
//// �G�l�~�[�f�[�^�e�[�u���T�C�Y
//const int CharacterFactory::EnemyTableSize = sizeof(EnemyTable) / sizeof(CharacterData);
//const int CharacterFactory::EnemySkillTableSize = sizeof(EnemySkillTable) / sizeof(Skill);
//
//// �v���C���[�̐����֐�
//std::shared_ptr<Character> CharacterFactory::CreatePlayer(int ID, int sklID)
//{
//	// ��������
//	std::shared_ptr<Character> player = std::make_shared<Player> (PlayerTable[ID], PlayerSkillTable[sklID]);
//	int face = CLoad::Instance().getPfaceGrh(ID);
//	player->setFaceHandle(face);
//
//	return player;
//
//}
//
//// �G�l�~�[�̐����֐�
//std::shared_ptr<Character> CharacterFactory::CreateEnemy(int ID, int sklID)
//{
//	// ��������
//	std::shared_ptr<Character> enemy = std::make_shared<Enemy>(EnemyTable[ID], EnemySkillTable[sklID]);
//	int face = CLoad::Instance().getEfaceGrh(ID);
//	enemy->setFaceHandle(face);
//
//	return enemy;
//
//}

bool CharacterFactory::LoadCSV(const std::string& path, std::vector<std::vector<std::string>>& outData) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "CSV�t�@�C�����J���܂���: " << path << std::endl;
        return false;
    }

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (firstLine) { firstLine = false; continue; } // �w�b�_�[�s���X�L�b�v

        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        outData.push_back(row);
    }
    file.close();
    return true;
}

bool CharacterFactory::LoadPlayerData(const std::string& path) {
    std::vector<std::vector<std::string>> csv;
    if (!LoadCSV(path, csv)) return false;

    PlayerTable.clear();
    for (auto& row : csv) {
        CharacterData data;
        data.ID = std::stoi(row[0]);
        data.Name = row[1];
        data.HP = std::stoi(row[2]);
        data.ATK = std::stoi(row[3]);
        data.DEF = std::stoi(row[4]);
        data.AGR = std::stoi(row[5]);
        data.Lv = std::stoi(row[6]);
        data.Exp = std::stoi(row[7]);
        data.skillID = std::stoi(row[8]);
        data.Alive = (row[9] == "true");
        PlayerTable.push_back(data);
    }
    return true;
}

bool CharacterFactory::LoadEnemyData(const std::string& path)
{
    std::vector<std::vector<std::string>> csv;
    if (!LoadCSV(path, csv)) return false;

    EnemyTable.clear();
    for (auto& row : csv) {
        CharacterData data;
        data.ID = std::stoi(row[0]);
        data.Name = row[1];
        data.HP = std::stoi(row[2]);
        data.ATK = std::stoi(row[3]);
        data.DEF = std::stoi(row[4]);
        data.AGR = std::stoi(row[5]);
        data.Lv = std::stoi(row[6]);
        data.Exp = std::stoi(row[7]);
        data.skillID = std::stoi(row[8]);
        data.Alive = (row[9] == "true");
        EnemyTable.push_back(data);
    }
    return true;
}

bool CharacterFactory::LoadSkillData(const std::string& path) {
    std::vector<std::vector<std::string>> csv;
    if (!LoadCSV(path, csv)) return false;

    SkillTable.clear();
    for (auto& row : csv) {
        Skill s;
        s.ID = std::stoi(row[0]);
        s.Name = row[1];
        s.type = static_cast<SkillType>(std::stoi(row[2]));
        s.targetType = static_cast<SkillTargetType>(std::stoi(row[3]));
        s.power = std::stof(row[4]);
        s.trun = std::stoi(row[5]);
        s.effectType = static_cast<EffectType>(std::stoi(row[6]));
        s.Details = row[7];
        SkillTable.push_back(s);
    }
    return true;
}

std::shared_ptr<Character> CharacterFactory::CreatePlayer(int id) {
    if (id < 0 || id >= (int)PlayerTable.size()) return nullptr;
    const CharacterData& cData = PlayerTable[id];
    const Skill& sData = SkillTable[cData.skillID];

    auto player = std::make_shared<Player>(cData, sData);
    int face = CLoad::Instance().getPfaceGrh(id);
    player->setFaceHandle(face);
    return player;
}

std::shared_ptr<Character> CharacterFactory::CreateEnemy(int id) {
    if (id < 0 || id >= (int)EnemyTable.size()) return nullptr;
    const CharacterData& cData = EnemyTable[id];
    const Skill& sData = SkillTable[cData.skillID];

    auto enemy = std::make_shared<Enemy>(cData, sData);
    int face = CLoad::Instance().getEfaceGrh(id);
    enemy->setFaceHandle(face);
    return enemy;
}

const CharacterData* CharacterFactory::GetCharacterData(int id) const
{
    // �v���C���[��������
    for (const auto& c : PlayerTable) {
        if (c.ID == id) return &c;
    }

    // �G�l�~�[��������
    for (const auto& c : EnemyTable) {
        if (c.ID == id) return &c;
    }

    return nullptr; // ������Ȃ���� nullptr
}

const Skill* CharacterFactory::GetSkillData(int id) const
{
    for (const auto& s : SkillTable) {
        if (s.ID == id) return &s;
    }
    return nullptr; // ������Ȃ������ꍇ
}

// �v���C���[�f�[�^�e�[�u���T�C�Y
const int CharacterFactory::PlayerTableSize = sizeof(PlayerTable) / sizeof(CharacterData);

// �G�l�~�[�f�[�^�e�[�u���T�C�Y
const int CharacterFactory::EnemyTableSize = sizeof(EnemyTable) / sizeof(CharacterData);

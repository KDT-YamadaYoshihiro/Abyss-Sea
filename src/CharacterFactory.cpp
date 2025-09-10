#include "CharacterFactory.h"
#include "CPlayer.h"
#include "CEnemy.h"

#include <fstream>
#include <sstream>
#include <iostream>

//
//// 初期ステータス情報
//const CharacterData CharacterFactory::PlayerTable[] = {
//	//  ID, Name,		HP,		ATK,	 DEF,	AGR,	Lv, Exp, Alive,	
//	{AKIRA,	"アキラ",	100,	35,		10,		10,		1,	0,	true},
//	{EL,	"エル",		 90,	28,		15,		20,		1,	0,	true},
//	{ALIA,	"アリア",	140,	15,		10,		18,		1,	0,	true},
//	{PINOT,	"ピノ",		 90,	22,		18,		25,		1,	0,	true},
//	{KAGE,	"カゲ",		100,	25,		10,		32,		1,	0,	true}
//};
//
////　プレイヤースキル情報
//const Skill CharacterFactory::PlayerSkillTable[] = {
//	// ID, skillName,					skilltype,			targetType				skillPower		skilltTrun		EffectType,				Details
//	{AKIRA, "豪烈斬",					SkillType::ATTACK,		SkillTargetType::SINGLE_ENEMY,	 1.5f,		0,	EffectType::EF_SKILL1,	"敵単体に強力な技を放つ"},
//	{EL,	"セラフィックブレード",		SkillType::ATTACK,		SkillTargetType::SINGLE_ENEMY,	 1.5f,		0,	EffectType::EF_SKILL2,	"敵単体に連撃を放つ"},
//	{ALIA,	"ヒール",					SkillType::HEAL,		SkillTargetType::SINGLE_ALLY,	 0.6f,		0,	EffectType::EF_HEAL,	"味方単体のHPを回復させる"},
//	{PINOT,	"剣の加護",					SkillType::ATK_BUFF,	SkillTargetType::SINGLE_ALLY,	 1.3f,		2,	EffectType::EF_BUFF,	"味方単体の攻撃力を\n　　　上昇させる（2ターン継続）"},
//	{KAGE,	"瞬足の加護",				SkillType::AGR_BUFF,	SkillTargetType::SINGLE_ALLY,	 1.3f,		2,	EffectType::EF_BUFF,	"味方単体の行動速度を\n　　　上昇させる（2ターン継続）"}
//
//};
//
//// ステータス情報
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
//// プレイヤーデータテーブルサイズ
//const int CharacterFactory::PlayerTableSize = sizeof(PlayerTable) / sizeof(CharacterData);
//const int CharacterFactory::PlayerSkillTableSize = sizeof(PlayerSkillTable) / sizeof(Skill);
//
//// エネミーデータテーブルサイズ
//const int CharacterFactory::EnemyTableSize = sizeof(EnemyTable) / sizeof(CharacterData);
//const int CharacterFactory::EnemySkillTableSize = sizeof(EnemySkillTable) / sizeof(Skill);
//
//// プレイヤーの生成関数
//std::shared_ptr<Character> CharacterFactory::CreatePlayer(int ID, int sklID)
//{
//	// 生成処理
//	std::shared_ptr<Character> player = std::make_shared<Player> (PlayerTable[ID], PlayerSkillTable[sklID]);
//	int face = CLoad::Instance().getPfaceGrh(ID);
//	player->setFaceHandle(face);
//
//	return player;
//
//}
//
//// エネミーの生成関数
//std::shared_ptr<Character> CharacterFactory::CreateEnemy(int ID, int sklID)
//{
//	// 生成処理
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
        std::cerr << "CSVファイルを開けません: " << path << std::endl;
        return false;
    }

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (firstLine) { firstLine = false; continue; } // ヘッダー行をスキップ

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
    // プレイヤー側を検索
    for (const auto& c : PlayerTable) {
        if (c.ID == id) return &c;
    }

    // エネミー側を検索
    for (const auto& c : EnemyTable) {
        if (c.ID == id) return &c;
    }

    return nullptr; // 見つからなければ nullptr
}

const Skill* CharacterFactory::GetSkillData(int id) const
{
    for (const auto& s : SkillTable) {
        if (s.ID == id) return &s;
    }
    return nullptr; // 見つからなかった場合
}

// プレイヤーデータテーブルサイズ
const int CharacterFactory::PlayerTableSize = sizeof(PlayerTable) / sizeof(CharacterData);

// エネミーデータテーブルサイズ
const int CharacterFactory::EnemyTableSize = sizeof(EnemyTable) / sizeof(CharacterData);

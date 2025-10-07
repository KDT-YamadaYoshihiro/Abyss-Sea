#include "CharacterFactory.h"
#include "CPlayer.h"
#include "CEnemy.h"

//　CSVの1行を分割する
std::vector<std::string> split(const std::string& line, char delimiter = ',') {
	std::vector<std::string> result;
	std::stringstream ss(line);
	std::string item;

	while (std::getline(ss,item,delimiter))
	{
		result.push_back(item);
	}
	return result;

}

bool CharacterFactory::LoadCharacterTable(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "キャラクターテーブルを開けません" << fileName << std::endl;
		
		return false;
	}

	std::string line;
	bool firstLine = true;
	while (std::getline(file,line))
	{
		if (firstLine) { firstLine = false; continue; }

		auto tokens = split(line, ',');
		if (tokens.size() < 10)continue;

		CharacterData data;
		data.ID = std::stoi(tokens[0]);
		data.Name = tokens[1];
		data.HP = std::stoi(tokens[2]);
		data.ATK = std::stoi(tokens[3]);
		data.DEF = std::stoi(tokens[4]);
		data.AGR = std::stoi(tokens[5]);
		data.Lv = std::stoi(tokens[6]);
		data.Exp = std::stoi(tokens[7]);
		data.skID = std::stoi(tokens[8]);
		data.Alive = (tokens[9] == "TRUE");

		CharacterTable.push_back(data);
	}
	return true;

}

bool CharacterFactory::LoadSkillTable(const std::string& filename) {

	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "スキルテーブルを開けません: " << filename << std::endl;
		return false;
	}

	std::string line;
	bool firstLine = true;
	while (std::getline(file, line)) {
		if (firstLine) { firstLine = false; continue; }

		auto tokens = split(line, ',');
		if (tokens.size() < 8) continue;

		SkillData data;
		data.ID = std::stoi(tokens[0]);
		data.Name = tokens[1];
		data.type = static_cast<SkillType>(std::stoi(tokens[2]));
		data.targetType = static_cast<SkillTargetType>(std::stoi(tokens[3]));
		data.power = std::stof(tokens[4]);
		data.trun = std::stoi(tokens[5]);
		data.effectType = static_cast<EffectType>(std::stoi(tokens[6]));
		data.Details = tokens[7];

		SkillTable.push_back(data);
	}
	return true;
}

CharacterData CharacterFactory::getCharacter(int id)
{
	for (auto& c : CharacterTable) {
		if (c.ID == id) return c;
	}
	// 見つからなかった場合空を返す
	return {};
}

SkillData CharacterFactory::getSkill(int id)
{
	for (auto& s : SkillTable) {
		if (s.ID == id) return s;
	}
	// 見つからなかった場合空を返す
	return {};
}

std::shared_ptr<Character> CharacterFactory::CreateCharacter(int id)
{
	CharacterData data = getCharacter(id);
	SkillData skill = getSkill(data.skID);

	if (id < 100) { // 頭桁=1 → プレイヤー
		return std::make_shared<Player>(
			data.ID, data.Name, data.HP, data.ATK,
			data.DEF, data.AGR, data.Lv, data.Exp, skill
		);
	}
	else if (id >= 100) { // 頭桁=2 → エネミー
		return std::make_shared<Enemy>(
			data.ID, data.Name, data.HP, data.ATK,
			data.DEF, data.AGR, data.Lv, data.Exp, skill
		);
	}

	return nullptr; // 不明なID
}


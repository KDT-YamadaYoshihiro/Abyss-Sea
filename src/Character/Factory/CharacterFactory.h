#pragma once
#include "Character/characterData.h"
#include "Character/Character.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

class Player;
class Enemy;


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


	// CSVÇÃÉçÅ[Éh
	bool LoadCharacterTable(const std::string& fileName);
	bool LoadSkillTable(const std::string& fileName);

	// ê∂ê¨
	CharacterData getCharacter(int id);
	SkillData getSkill(int id);

	template<typename T>
	std::shared_ptr<T> CreateCharacter(int id);

private:

	std::vector<CharacterData> CharacterTable;
	std::vector<SkillData> SkillTable;


};

template<typename T>
std::shared_ptr<T> CharacterFactory::CreateCharacter(int id)
{
	CharacterData data = getCharacter(id);
	SkillData skill = getSkill(data.skID);

	auto character = std::make_shared<T>(
		data.ID, data.Name, data.HP, data.ATK,
		data.DEF, data.AGR, data.Lv, data.Exp, skill);

	if (character != nullptr) {
		return character;
	}
	else
	{
		return nullptr; // ïsñæÇ»ID
	}
}

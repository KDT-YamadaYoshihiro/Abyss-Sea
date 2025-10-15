#pragma once
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

#include "enum.h"


// キャラクターデータ
struct CharacterData
{
	int ID;
	std::string Name;
	int HP;
	int ATK;
	int DEF;
	int AGR;
	int Lv;
	int Exp;
	int skID;
	bool Alive;
};

// ステータス効果（バフ/デバフ）
struct BuffEffect {
	float atkMultiplier = 1.0f;
	int atkturn = 0; // 残りターン
	float agrMultiplier = 1.0f;
	int agrturn = 0; // 残りターン
	float defMultiplier = 1.0f;
	int defturn = 0; // 残りターン
};

// スキルデータ
struct SkillData {
	int ID;
	std::string Name;
	SKILL_TYPE type;
	SKILL_TARGET_TYPE targetType;
	float power;					// 効果量
	int turn;						// 効果ターン
	EFFECT_TYPE effectType;			// エフェクトの種類
	std::string Details;			// スキル説明
};

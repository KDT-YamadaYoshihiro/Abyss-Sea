#pragma once
#include <string>
#include <memory>
#include <vector>

// エフェクト種別	   なし、通常攻撃、アキラ、エル、　アリア、ピノ
enum class EffectType {
	EF_NONE ,
	EF_ATTACK,
	EF_SKILL1,
	EF_SKILL2,
	EF_HEAL,
	EF_BUFF
};


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
	int skillID;
	bool Alive;

};

// スキルクラス			攻撃,  回復, 攻撃力バフ,　速度バフ, 防御力バフ, 無し
enum class SkillType { ATTACK, HEAL, ATK_BUFF, AGR_BUFF,  DEF_BUFF,	NONE };
// スキルのターゲット方向	単エネミー, 単味方,自分,全エネミー,全味方
enum class SkillTargetType { SINGLE_ENEMY, SINGLE_ALLY, SELF, ALL_ENEMY, ALL_ALLY };
// ステータス効果（バフ/デバフ）
struct BuffEffect {
	float atkMultiplier = 1.0f;
	int atkturn = 0; // 残りターン
	float agrMultiplier = 1.0f;
	int agrturn = 0; // 残りターン
	float defMultiplier = 1.0f;
	int defturn = 0; // 残りターン

};

struct Skill {
	int ID;
	std::string Name;
	SkillType type;
	SkillTargetType targetType;
	float power;					// 効果量
	int trun;						// 効果ターン
	EffectType effectType;			// エフェクトの種類
	std::string Details;			// スキル説明
};

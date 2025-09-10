#pragma once
#include <string>
#include <memory>
#include <vector>

// �G�t�F�N�g���	   �Ȃ��A�ʏ�U���A�A�L���A�G���A�@�A���A�A�s�m
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

// �X�L���N���X			�U��,  ��, �U���̓o�t,�@���x�o�t, �h��̓o�t, ����
enum class SkillType { ATTACK, HEAL, ATK_BUFF, AGR_BUFF,  DEF_BUFF,	NONE };
// �X�L���̃^�[�Q�b�g����	�P�G�l�~�[, �P����,����,�S�G�l�~�[,�S����
enum class SkillTargetType { SINGLE_ENEMY, SINGLE_ALLY, SELF, ALL_ENEMY, ALL_ALLY };
// �X�e�[�^�X���ʁi�o�t/�f�o�t�j
struct BuffEffect {
	float atkMultiplier = 1.0f;
	int atkturn = 0; // �c��^�[��
	float agrMultiplier = 1.0f;
	int agrturn = 0; // �c��^�[��
	float defMultiplier = 1.0f;
	int defturn = 0; // �c��^�[��

};

struct Skill {
	int ID;
	std::string Name;
	SkillType type;
	SkillTargetType targetType;
	float power;					// ���ʗ�
	int trun;						// ���ʃ^�[��
	EffectType effectType;			// �G�t�F�N�g�̎��
	std::string Details;			// �X�L������
};

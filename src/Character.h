#pragma once

#include "Mouse.h"
#include "UI.h"
#include "CEffect.h"
#include "CLoad.h"

#include "CBattle_SP.h"
#include "CharacterData.h"

// �L�����N�^�[���N���X
class Character {

protected:

	// �X�e�[�^�X���
	int ID = -1;
	std::string Name = "";
	int HP = -1;
	int maxHp = -1;
	int ATK = -1;
	int DEF = -1;
	int AGR = -1;
	int Lv = 1;
	int Exp = -1;
	bool Alive = false;

	int skillID = -1;
	std::string skillName = "";
	SkillType type;
	SkillTargetType targetType;
	int skilltrun = -1;
	float skillPower = -1.0f;
	EffectType ef;
	std::string Details = "";

	BuffEffect Buff;

	// �G�t�F�N�g
	std::shared_ptr<Effect> attackEffect;
	std::shared_ptr<Effect> skillEffect;

	// �A�C�R���摜�n���h���ϐ�
	int faceGrh = -1;
	// �s�����(����v���C���[�̂ݎg�p)
	int actionChoice = -1;

	// �N���b�N�I���Ŏg�p������W�ϐ�
	int posX = -1;
	int posY = -1;

	// �G�t�F�N�g�A�j���[�V�����p
	// �^�[�Q�b�g���W
	int targetPosX = -1;
	int targetPosY = -1;
	// �^�[�Q�b�g�̉摜�n�[�t�T�C�Y
	int targetSizeW = -1;
	int targetSizeH = -1;

	// �_���[�W�\���p
	bool drawPower = false;
	int Power = -1;
	int framePower = -1;

	// �J���[
	int r = -1;
	int g = -1;
	int b = -1;

	// �t�H���g�T�C�Y
	int fontSize = -1;

	// �G�l�~�[�p
	bool moveCheck = false;

public:

	// ������
	Character(const CharacterData& data, const Skill& skilldata)
		:ID(data.ID),
		Name(data.Name),
		HP(data.HP),
		maxHp(data.HP),
		ATK(data.ATK),
		DEF(data.DEF),
		AGR(data.AGR),
		Lv(data.Lv),
		Exp(data.Exp),
		Alive(data.Alive),
		// �X�L��
		skillID(skilldata.ID),
		skillName(skilldata.Name),
		type(skilldata.type),
		targetType(skilldata.targetType),
		skilltrun(skilldata.trun),
		skillPower(skilldata.power),
		ef(skilldata.effectType),
		Details(skilldata.Details)
	{
		attackEffect = std::make_shared<Effect>(CLoad::Instance().getEffectGrh(EffectType::EF_ATTACK));
		skillEffect = std::make_shared<Effect>(CLoad::Instance().getEffectGrh(ef));
		actionChoice = -1;
		posX = -1;
		posY = -1;
		targetPosX = -1;
		targetPosY = -1;
		targetSizeW = -1;
		targetSizeH = -1;
		drawPower = false;
		Power = 0;
		framePower = 0;
		fontSize = 0;
		moveCheck = false;
	}

	virtual ~Character() {};

	// �e��Q�b�^�[
	int getId() const { return ID; }
	std::string getName() const { return Name; }
	int getHp() const { return HP; }
	int getMaxHp() const { return maxHp; }
	int getAttack() const { return ATK; }
	int getDefense() const { return DEF; }
	int getSpeed() const { return AGR; }
	int getLv() const { return Lv; }
	int getExp() const { return Exp; }
	void setAlive(bool arg_alive) { Alive = arg_alive; }		// ��������̕ύX
	bool getAlive() const { return Alive; }
	int getSkillID() const { return skillID; }
	std::string getSkillName() const { return skillName; }
	SkillType getSkillType() const { return type; }
	SkillTargetType getTargetType() const { return targetType; }
	float getSkillPower() const { return skillPower; }
	EffectType getEfType() const { return ef; }
	std::string getDetails() const { return Details; }

	BuffEffect getAtkBuffEffect() const { return Buff; }
	float getAtkbuff() const { return Buff.atkMultiplier; }
	float getAgrbuff() const { return Buff.agrMultiplier; }
	float getDefbuff() const { return Buff.defMultiplier; }
	bool getDrawPower() const { return drawPower; }
	int getPower() const { return Power; }


	// �A�C�R���摜�̃Z�b�g,�Q�b�g�֐�
	void setFaceHandle(int handle) { faceGrh = handle; }
	int getFaceHandle() const { return faceGrh; }

	// �s����ނ̃Z�b�g,�Q�b�g�֐�
	void setActionChoice(int arg_choice) { actionChoice = arg_choice; }
	int getActionChoice() const { return actionChoice; }

	// ���W�̃Z�b�g,�Q�b�g�֐�
	// x
	void setPosX(int arg_posX) { posX = arg_posX; }
	int getPosX() const { return posX; }
	// y
	void setPosY(int arg_posY) { posY = arg_posY; }
	int getPosY()const { return posY; }

	// �G�l�~�[�p
	bool getMoveCheck() const { return moveCheck; }
	void setMoveCheck(bool arg_moveCheck) { moveCheck = arg_moveCheck; }

	// �G�t�F�N�g
	void setAttackEffect(std::shared_ptr<Effect> ef) { attackEffect = ef; }
	void setSkillEffect(std::shared_ptr<Effect> ef) { skillEffect = ef; }

	// �G�t�F�N�g�̃|�C���^�[�擾
	std::shared_ptr<Effect> getAttackEffect() const { return  attackEffect; }
	std::shared_ptr<Effect> getSkillEffect() const { return skillEffect; }

	// �G�t�F�N�g�̃Q�b�g�Z�b�g�֐�
	bool hasAttackEffect() const { return  attackEffect->getEfDraw(); }
	bool hasSkillEffect() const { return skillEffect->getEfDraw(); }

	// hp�̏���l�܂Ŗ߂�
	void hpReset() { this->HP = this->maxHp;  this->Alive = true; }

	// �G�t�F�N�g�A�j���[�V�������Z�b�g
	void effectrReset() {
		attackEffect->effectInit();
		skillEffect->effectInit();
	}

	// �`��֐�
	virtual void Render(int arg_posX, int arg_posY, int arg_grh) = 0;

	// ��{�I�ȍs���i�U���j�����z�֐��Ƃ��Ē�`
	virtual void takeAction(std::vector<std::shared_ptr<Character>>& targets) = 0;

	// �G�t�F�N�g�̍Đ�
	// �A�^�b�N
	void PlayAttackEffect(int x, int y, int sizeW, int sizeH) {
		if (!attackEffect->getEfDraw()) {
			targetPosX = x;					// ���W�ݒ�
			targetPosY = y;
			targetSizeW = sizeW;
			targetSizeH = sizeH;
			attackEffect->effectStart();	// �G�t�F�N�g�X�^�[�g�֐�
		}
	}
	// �X�L��
	void PlaySkillEffect(int x, int y, int sizeW, int sizeH) {
		if (!skillEffect->getEfDraw()) {
			targetPosX = x;					// ���W�ݒ�
			targetPosY = y;
			targetSizeW = sizeW;
			targetSizeH = sizeH;
			skillEffect->effectStart();		// �G�t�F�N�g�X�^�[�g�֐�
		}
	}

	// �_���[�W�\��
	void damageRenderStart(int arg_x, int arg_y, int arg_fontSize, int damage) {

		Power = damage;
		framePower = 60;
		drawPower = true;

		// �J���[�w��
		r = 255; g = 255; b = 0;
		// �t�H���g�T�C�Y
		fontSize = arg_fontSize;

		targetPosX = arg_x;
		targetPosY = arg_y;

	}
	// �񕜕\��
	void healRenderStart(int arg_x, int arg_y, int arg_fontSize, int heal) {

		Power = heal;
		framePower = 60;
		drawPower = true;

		// �J���[�w��
		r = 143; g = 195; b = 31;
		// �t�H���g�T�C�Y
		fontSize = arg_fontSize;
		targetPosX = arg_x;
		targetPosY = arg_y;
	}

	// �X�L���p���[�\��
	void powerRender() {
		if (drawPower) { framePower--; }

		if (framePower <= 0) { drawPower = false; }

		if (framePower > 0) {
			SetFontSize(fontSize);
			DrawFormatString(targetPosX, targetPosY, GetColor(r, g, b), "%d", Power);
		}
	}


	// �_���[�W��^����
	void takeDamage(int damage) {
		// �_���[�W��^����B
		this->HP -= damage;
		// ���S�m�F
		if (this->HP <= 0) {
			this->HP = 0;			// 0�ȉ��ɂȂ�Ȃ��悤�ɂ���
			Alive = false;			// ���������false�ɂ���B
		}
	}

	// �񕜂�����
	void takeHeal(int heal) {
		// ��������
		if (!Alive) { return; }

		// ��
		this->HP += heal;
		// HP�̉񕜏��
		if (this->HP >= this->maxHp) { 
			this->HP = this->maxHp;
		}

	}

	// �o�t��������
	// �U���̓o�t
	void takeAtkBuff(float buffPower, int turn) {
		Buff.atkMultiplier = buffPower;
		Buff.atkturn = turn;
	}

	// ���x�o�t
	void takeAgrBuff(float buffPower, int turn) {
		Buff.agrMultiplier = buffPower;
		Buff.agrturn = turn;

	}

	// �h��̓o�t
	void takeDefBuff(float buffPower, int turn) {
		Buff.defMultiplier = buffPower;
		Buff.defturn = turn;

	}


	// �o�t�̍X�V����
	void updateBuff() {
		if (Buff.atkturn > 0) {
			Buff.atkturn--;
			if (Buff.atkturn == 0) {
				Buff.atkMultiplier = 1.0f;
			}
		}

		if (Buff.agrturn > 0) {
			Buff.agrturn--;
			if (Buff.agrturn == 0) {
				Buff.agrMultiplier = 1.0f;
			}
		}

		if (Buff.defturn > 0) {
			Buff.defturn--;
			if (Buff.defturn == 0) {
				Buff.defturn = 1.0f;
			}
		}

	}

	// �o�t�̃��Z�b�g
	void BuffReset() {
		Buff.atkturn = 0;
		Buff.atkMultiplier = 1.0f;
		Buff.agrturn = 0;
		Buff.agrMultiplier = 1.0f;
		Buff.defturn = 0;
		Buff.defturn = 1.0f;
	}


};
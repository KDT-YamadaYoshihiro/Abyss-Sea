#pragma once
#include "Character.h"
#include "iostream"


class Player : public Character{

	// �A�N�V�����̎��
	enum ActionType {
		P_ATTACK,
		P_SKILL,
	};

#define LV_MAX	30

	// UI
	std::shared_ptr<UI> ui;

	// �v���C���[�̍��W
	int posX;
	int posY;

	// �v���C���[�̌o���l���
	int maxExp = -1;
	// �Œ�_���[�W
	int minDamage = -1;

	// �A�j���[�V����
	GRTYPE AnimType;

public:


	// �R���X�g���N�^
	Player(const CharacterData& data, const Skill& skilldata,
		float arg_x = 0, float arg_y = 0) : Character(data, skilldata) {

		posX = arg_x;
		posY = arg_y;

		maxExp = 100;
		minDamage = 10;

		ui =std::make_shared<UI>();
		AnimType = WAIT;
	}

	// �f�X�g���N�^
	virtual ~Player() = default;

	// �A�j���[�V�����^�C�v�̃Q�b�g�A�Z�b�g
	void setAnimType(GRTYPE arg_type) { AnimType = arg_type; }
	GRTYPE getAnimType() const { return AnimType; }

	// UI�փA�j���[�V�����̕ύX��UI��̃^�C�v���擾
	void setAnimChange(bool arg_change) { ui->setAnimWait(arg_change); }
	bool getAnimChange() const { return ui->getAnimWait(); }

	// �L�����N�^�[���W���擾�֐�
	float getPosX() const { return (float)posX; }
	float getPosY() const { return (float)posY; }
	int getMaxExp() const { return maxExp; }


	std::shared_ptr<UI> getUi() const { return ui; }

	// �A�j���[�V�����X�V����
	void AnimUpdata() {
		ui->PlayerAnim(AnimType);

		switch (ef)
		{
		case EffectType::EF_NONE:
			break;
		case EffectType::EF_ATTACK:
			break;
		case EffectType::EF_SKILL1:
			if (hasSkillEffect()) {skillEffect->lengthPlayerAnim();}
			break;
		case EffectType::EF_SKILL2:
			if (hasSkillEffect()) { skillEffect->PlayAnim(); }
			break;
		case EffectType::EF_HEAL:
			if (hasSkillEffect()) { skillEffect->sidePlayAnim(); }
			break;
		case EffectType::EF_BUFF:
			if (hasSkillEffect()) { skillEffect->sidePlayAnim(); }
			break;
		default:
			break;
		}
		if (hasAttackEffect()) { attackEffect->sidePlayAnim(); }

	}

	void animDraw() {
		if (hasAttackEffect()) {attackEffect->effectDraw(targetPosX, targetPosY,targetSizeW,targetSizeH);}
		if (hasSkillEffect()) {skillEffect->effectDraw(targetPosX, targetPosY, targetSizeW, targetSizeH);}
	}

	// �o���l�擾
	void addExp(int value) {
		this->Exp += value;
	}

	// ���x���A�b�v�֐�
	void levelUp() {

		// ���x�����グ��B
		this->Lv++;
		// �o���l���o���l������߂��B
		this->Exp -= maxExp;
		// �o���l�����10%�㏸������
		this->maxExp *= 1.3;
		// �Œ�_���[�W�̏㏸
		this->minDamage += 1;

		// �e�X�e�[�^�X��3%�㏸������
		this->maxHp = static_cast<int>(this->maxHp * 1.2f);
		this->ATK = static_cast<int>(this->ATK * 1.2f);
		this->DEF = static_cast<int>(this->DEF * 1.2f);
		this->AGR = static_cast<int>(this->AGR * 1.2f);
		//this->skillPower = this->skillPower * 1.1f;

	}

	// �s���I���֐�
	void takeAction(std::vector<std::shared_ptr<Character>>& targets) override {
		
		// �`�F�b�N�̔��肪����΁A�ΏۑI��
		if (!Alive)	return;

		int damage = -1;

		// �X�}�[�g�|�C���^�G�l�~�[��nullptr�Ő���
		switch (actionChoice)
		{
		case ATTACK:

			// ��������
			if (!Alive) { return; }

			//�^�[�Q�b�g���U��
			for (auto& tar : targets) {
				damage = (this->ATK * getAtkbuff()) - (tar->getDefense() * tar->getDefbuff());
				//�_���[�W�̍Œ�ۏ�
				if (damage < minDamage * tar->getDefbuff()) damage = minDamage * tar->getDefbuff();
				// �^�[�Q�b�g���U��
				tar->takeDamage(damage);
				tar->damageRenderStart(tar->getPosX() - 150, tar->getPosY() - 150, 40, damage);
				// �A�j���[�V�����^�C�v�ύX
				setAnimType(ATTACK);
			}

			// �I��

			break;

		case SKILL:

			// ��������
			if (!Alive) { return; }

			// �X�L���N���X�̃X�L���ݒ�Ɛ��l����
			// �^�[�Q�b�g�Ɋ֐��Ăяo��
			for (auto& tar : targets) {
				useSkill(tar, this->type, this->skillPower, this->skilltrun);
			}

			// �I��

			break;
		}

	}

	// �X�L��
	void useSkill(std::shared_ptr<Character>& arg_targets, SkillType arg_type, float arg_power, int arg_trun/*,int arg_combo*/) {

		int damage = -1;
		int heal = -1;
		int turn = -1;
		float arg_buff = -1.0f;

		switch (arg_type)
		{
		case SkillType::ATTACK:

			// �Ώێ҂ɍU����
			// �_���[�W�ʂ��v�Z
			// combo���ǉ��������\��
			damage = static_cast<int>((this->ATK * arg_power) * getAtkbuff()) - arg_targets->getDefense();
			// �O�����ɂ��Ȃ�
			if (damage < 0) damage = 0;
			//�@�_���[�W�֐�
			arg_targets->takeDamage(damage);
			arg_targets->damageRenderStart(arg_targets->getPosX() - 150, arg_targets->getPosY() - 150, 40, damage);
			// �A�j���[�V�����^�C�v
			setAnimType(SKILL);

			break;

		case SkillType::HEAL:

			// �Ώێ҂�HP���㏸
			// HP�񕜗ʂ��v�Z
			heal = static_cast<int>(this->HP * arg_power);
			// �q�[���֐�
			arg_targets->takeHeal(heal);
			arg_targets->healRenderStart(arg_targets->getPosX() - 20, arg_targets->getPosY() - 5, 20, heal);
			// �A�j���[�V�����^�C�v
			setAnimType(MAGIC);

			break;

		case SkillType::ATK_BUFF:

			// �Ώێ҂̍U���͏㏸
			// �o�t�ʂ��v�Z
			arg_buff = static_cast<float>(arg_power);
			turn = static_cast<int>(arg_trun);
			// �^�[�Q�b�g��x�^�[���̍U�������o�t�̕t�^
			arg_targets->takeAtkBuff(arg_buff, turn);
			//stateChange = true;
			// �A�j���[�V�����^�C�v
			setAnimType(MAGIC);

			break;

		case SkillType::AGR_BUFF:

			// �Ώۂ̑��x�㏸
			arg_buff = static_cast<float>(arg_power);
			turn = static_cast<int>(arg_trun);
			// �o�t�t�^
			arg_targets->takeAgrBuff(arg_buff, turn);
			// �A�j���[�V�����^�C�v
			setAnimType(MAGIC);

			break;

		case SkillType::DEF_BUFF:

			// �Ώۂ̑��x�㏸
			arg_buff = static_cast<float>(arg_power);
			turn = static_cast<int>(arg_trun);
			// �o�t�t�^
			arg_targets->takeDefBuff(arg_buff, turn);
			// �A�j���[�V�����^�C�v
			setAnimType(MAGIC);

			break;

		case SkillType::NONE:

			// �������Ȃ�

			break;

		default:
			// �������Ȃ�
			break;
		}

	}


	// �`��
	void Render(int arg_posX, int arg_posY, int arg_grh) override{

		// �{�̕`��
		posX = arg_posX;
		posY = arg_posY;
		ui->PlayerRender(posX, posY, arg_grh, AnimType);

		//�@HP�̕`��
		ui->HpRender(posX + 5, posY - 10, 40, 10, this->maxHp, this->HP, GetColor(0, 255, 0));
		// ���O�ƃ��x����`��
		SetFontSize(15);
		DrawFormatString(posX + 5, posY - 40, GetColor(255, 255, 255), "%s",this->Name.c_str());
		DrawFormatString(posX + 5, posY - 25, GetColor(255, 255, 255), "Lv:%d", this->Lv);

	}

	// �X�e�[�^�X�����Z�b�g
	void statusReset() {
		this->Lv = 1;
	}


};
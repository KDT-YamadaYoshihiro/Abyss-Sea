#pragma once
#include "DxLib.h"
#include "Manager.h"
#include "CBase.h"
#include "CPlayer.h"
#include "CEnemy.h"

#include "CTitle.h"
#include "CStage.h"
#include "CBattle.h"

// �O�p�`�̍��W��̍���
#define TRIANGEL_X	50
#define TRIANGLE_Y	20

// �e�L�X�g�T�C�Y
#define TEXT_SIZE	30

class CResult : public ScreenBase {

	enum NEXTSCREEN {
		STAGE,
		BATTLE,
		TITLE,

		MAX
	};

	// �e�L�����N�^�[�̃��x���A�b�v��
	std::vector<int> levelUpDiff;

	// ��
	std::shared_ptr<Sound>bgm;
	std::shared_ptr<Sound> se;

	// �t�F�[�h
	std::shared_ptr<Fade> fade;

	// �o���l�̍ĕ��z�h�~
	bool isDistributed = false;

	// �I�𒆂̔ԍ�
	int choiceNum = -1;

	// �I�𒆂̎O�p�`�̍��W�ϐ�
	int choicePosX = -1;
	int choicePosY = -1;

	// �e�L�X�g�̍��W
	int choiceText_X[MAX];
	int choiceText_Y[MAX];

	// �摜�\���p�̕ϐ�
	// ���W
	int baseX;
	int baseY;
	// �؂���ʒu
	int scrX = -1;
	int scrY = -1;
	// �؂���T�C�Y
	int grhSizeX = -1;
	int grhSizeY = -1;
	// �v���C���[�摜�̕`��Ԋu
	int spacingX = -1;

public:
	
	// ������	
	CResult(){

		// ����
		bgm = Manager::Instance().getbgm();
		se = Manager::Instance().getSe();

		// �t�F�[�h
		fade = Manager::Instance().getFade();

		// �o���l�̍ĕ��z�h�~
		isDistributed = false;

		// �I�𒆂̔ԍ�
		choiceNum = STAGE;

		// �I�𒆂̎O�p�`�̍��W�ϐ�
		choicePosX = -1;
		choicePosY = -1;

		// �e�L�X�g�̍��W
		for (int i = 0; i < MAX; i++) {
			choiceText_X[i] = 200;
			choiceText_Y[i] = 650 + i * 50;
		}

		// �摜�\���p�̕ϐ�
		baseX = 100;
		baseY = 120;
		scrX = 60;
		scrY = 0;
		grhSizeX = 250;
		grhSizeY = 418;
		spacingX = grhSizeX + 40;
		//�@������
		this->Init();

	}

private:


	void Init() {

		//// �t�F�[�h
		//if (!fade->checkOpen()) {
		//	fade->fadeStart(fade->FADE_OPEN);
		//}

		// bgm
		if (Manager::Instance().getbattleResult() == BattleResult::VICTORY) {
			bgm->PlayBgm(CLoad::Instance().getBgmHandle(BGM_CLEAR));
		}
		else {
			bgm->PlayBgm(CLoad::Instance().getBgmHandle(BGM_OVER));
		}
	}

	void Update() override;
	void Render() override;

	void ChoiceNectScreen();
};

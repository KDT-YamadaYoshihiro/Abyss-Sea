#pragma once
#include "Manager.h"

// �X�e�[�W�I���X�N���[���N���X
class CStage : public ScreenBase {

	// �~�̍ő吔
#define CIRCLEMAX	   3
	// �O�p�`�̍��W�萔
#define TRX			 90.0f
#define TRY_1		 525.0f
#define TRY_2		 575.0f;
#define TRY_3		 625.0f


	enum STAGENUM {
		STAGE1,
		STAGE2,
		STAGE3,

		MAX
	};

	// UI
	std::shared_ptr<UI> ui;
	
	// ��
	std::shared_ptr<Sound>bgm;
	std::shared_ptr<Sound> se;

	// �t�F�[�h
	std::shared_ptr<Fade> fade;


	// �I������X�e�[�W�A�C�R���i�����j
	float posX[CIRCLEMAX];				// X���W
	float posY[CIRCLEMAX];				// Y���W

	// ����{�^���̔�
	float boxX = -1;
	float boxY = -1;
	float boxSizeX = -1;
	float boxSizeY = -1;

	// �I�𒆂̃X�e�[�W�ԍ�
	int stageNum = -1;

	// �O�p�`�̍��W
	float TrX1 = -1;
	float TrY1 = -1;
	float TrX2 = -1;
	float TrY2 = -1;
	float TrX3 = -1;
	float TrY3 = -1;

	// �������x���\��
	int Suggest;

	// �p�[�e�B�[�����o�[�̊m�F
	bool selectMode = false;


public:

	void Update() override;

	void Render() override;


	// ������
	CStage()
		:stageNum(STAGE1),
		Suggest(1),
		TrX1(150.0f), TrY1(WINDOW_W / 2 - 350.0f),
		TrX2(TrX1 - 50), TrY2(TrX1 - 50),
		TrX3(TrX1 + 50), TrY3(TrX1 - 50),
		selectMode(false)
	{
		
		ui = std::make_shared<UI>();

		bgm = Manager::Instance().getbgm();
		se = Manager::Instance().getSe();
		fade = Manager::Instance().getFade();

		bgm->PlayBgm(CLoad::Instance().getBgmHandle(BGM_START));
		// �������W
		for (int i = 0; i < CIRCLEMAX; i++) {
			posX[i] = 100;
			posY[i] = 200 + i * 100;
		}

		// ����{�^�����W
		boxX = 900.0f;
		boxY = 600.0f;
		boxSizeX = 200.0f;
		boxSizeY = 100.0f;

	};

	// �f�X�g���N�^
	virtual ~CStage() {};

	int getStageNum() const { return stageNum; }

private:

	void Triangle();

};
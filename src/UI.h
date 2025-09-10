#pragma once
#include <string>
#include "enum.h"
#include "DxLib.h"

#define RADIUS_X	100
#define RADIUS_Y	 50

// �v���C���[�摜�T�C�Y
#define SIZE_W		64
#define SIZE_H		64

// SP�p�̉~�̃T�C�Y
#define SP_RADIUS	20



// UI�N���X
class UI {

	struct AnimInfo
	{
		int startCol;	// �J�nX����
		int row;		// �s�ԍ�
		int frames;		// �R�}��

	};

	static const AnimInfo animTable[ANIM_MAX];

	bool AnimWait = false;

	// �A�j���[�V�����ύX�p�J�E���g�t���[��
	int frame = -1;

	int scrX = -1;
	int scrY = -1;

	int startScrX = -1;
	int startScrY = -1;

	GRTYPE prev_type;

	int totalFrames = -1;


public:

	UI()
	{

		frame = 0;

		scrX = -1;
		scrY = -1;

		startScrX = -1;
		startScrY = -1;

		prev_type = WAIT;

		totalFrames = 3;

		AnimWait = false;

		
	};
	virtual ~UI() = default;

	int getScrX() const { return scrX; }
	int getStartScrX() const { return startScrX; }

	void setAnimWait(bool arg_animWait) { AnimWait = arg_animWait; }
	bool getAnimWait() const { return AnimWait; }

	// �ȉ~�`�̃{�^���֐�
	void Button(float arg_x, float arg_y,								// ���W
		unsigned int arg_color, int fillFlag,							// �ȉ~�̃J���[
		const TCHAR* text, unsigned int textcolor)						// �����̕`��ƕ����̃J���[
	{
		// �ȉ~�`�̕`��
		DrawOval(arg_x, arg_y, RADIUS_X, RADIUS_Y, arg_color, fillFlag);
		// ������̕`��(�U��)
		SetFontSize(50);
		DrawFormatString((int)arg_x - (RADIUS_X / 2 + 20), (int)arg_y - (RADIUS_Y / 2), textcolor, "%s", text);

	};

	// �{�^���\��
	void Button(int arg_x, int arg_y, int arg_scrX, int arg_scrY, int arg_handle) {
		DrawExtendGraph(arg_x, arg_y, arg_scrX, arg_scrY, arg_handle, true);
	}


	// �X�L���|�C���g�̕`��
	void SkillPoint(float arg_x, float arg_y, float arg_radius, int arg_skillPT, int arg_skillMaxPT) {

		SetFontSize(30);
		DrawString(arg_x - 70, arg_y - 50, "SP", GetColor(255, 255, 255));
		DrawFormatString(arg_x - 70, arg_y - 15, GetColor(255, 255, 255), "%d / ",arg_skillPT);

		// �ő�X�L���|�C���g�i�g�̕`��j
		for (int maxpt = 0; maxpt < arg_skillMaxPT; maxpt++) {
			DrawCircle(arg_x + maxpt * (arg_radius * 2),arg_y,arg_radius,GetColor(255,255,0),false);
		}
		// �c��X�L���|�C���g�i���g�̕`��j
		for (int pt = 0; pt < arg_skillPT; pt++) {
			DrawCircle(arg_x + pt * ( arg_radius * 2 ), arg_y, arg_radius, GetColor(255, 255, 0),true);
		}
		
	}

	void PlayerAnimStart(GRTYPE& type);
	void PlayerAnim(GRTYPE& type);

	// �w�i�`��
	void BgRender(int &arg_x, int &arg_y, int arg_scrX, int arg_scrY, int arg_grh) {
		DrawExtendGraph(arg_x, arg_y,arg_x + arg_scrX, arg_y + arg_scrY, arg_grh, true);
	}

	// �w�i�X�N���[��
	void BgScroll(int &arg_x, int &arg_y, int resetPosX, int resetPosY) {
		arg_x--;
		if (arg_x <= -resetPosX) { arg_x = resetPosX; }
	}

	// �v���C���[�L�����N�^�[�̕`��
	void PlayerRender(int arg_posX, int arg_posY, int grh, int GRtype) {

		DrawRectGraph(arg_posX,arg_posY, this->scrX, this->scrY,SIZE_W, SIZE_H, grh, true, true);
	}

	// �G�l�~�[�L�����N�^�[�̕`��
	void EnemyRender(int arg_posX, int arg_posY, int grh, float ext) {

		DrawRotaGraph(arg_posX, arg_posY, ext, 0, grh, true,false);
	}

	// HP�̊����\��
	float Ratio(int arg_Hp, int arg_HpMax) {

		// �����v�Z
		float ratio = (float)arg_Hp / (float)arg_HpMax;

		// �v�Z���ʂ�return
		return ratio;
	};

	// HP�̕`��
	void HpRender(float arg_x, float arg_y, float arg_xsize, float arg_ysize,
		int arg_HpMax, int arg_Hp, unsigned int color)
	{
		// ���W�v�Z
		float box_x = arg_x + arg_xsize;
		float box_y = arg_y + arg_ysize;

		float box_hp = arg_xsize * Ratio(arg_Hp, arg_HpMax);
		float box_x2 = arg_x + box_hp;

		// �{�b�N�X�̕`��(���g)
		DrawFillBox(arg_x, arg_y, box_x2, box_y, color);
		// �{�b�N�X��`��(�g)
		DrawLineBox(arg_x, arg_y, box_x, box_y, GetColor(255, 255, 255));

	};

	// �{�^��
	void DecButton(int arg_x, int arg_y, int arg_sizeW, int arg_sizeH, const TCHAR* text) {
		DrawLineBox(arg_x, arg_y, arg_x + arg_sizeW, arg_y + arg_sizeH, GetColor(255, 255, 255), true);
		SetFontSize(30);
		DrawFormatString(arg_x + 60, arg_y + 30, GetColor(255, 255, 255), "%s",text);
	}

};


// �t�F�[�h�N���X
class Fade {

	int radius = -1;		// �`�悷��~�̃T�C�Y
	int fadeMode = -1;			// �t�F�[�h�̏��
	bool fadeCheck = -1;	// �t�F�[�h���N������
	bool changeScreen = -1;

public:

	enum {
		FADE_NONE,
		FADE_CLAUSE,
		FADE_OPEN,
	};

	Fade() {
		radius = 0;
		fadeMode = FADE_NONE;
		fadeCheck = false;
		changeScreen = false;
	};

	virtual ~Fade() {};

	// �t�F�[�h�p
	void fadeCircleDraw(int x, int y) {
		DrawCircle(x, y, radius, GetColor(0, 0, 0), true);
	}
	// �X�^�[�g�֐�
	void fadeStart(int arg_open) {
		fadeMode = arg_open;
		fadeCheck = true;
	}
	// �A�b�v�f�[�g
	void fadeUpdate(int x) {

		switch (fadeMode)
		{
		case FADE_NONE:

			fadeCheck = false;

			break;

		case FADE_CLAUSE:		// �N���[�Y

			if (fadeCheck) {

				radius += 30;

				if (radius >= x) {
					radius = x;
					fadeMode = FADE_OPEN;
				}
			}

			break;

		case FADE_OPEN:			// �I�[�v��

			if (fadeCheck) {

				radius -= 30;

				if (radius <= 0) {
					radius = 0;
					fadeMode = FADE_NONE;
				}
			}

			break;

		}

	}

	bool Chackfade() const { return fadeMode; }
	int getRadius() const { return radius; }
	
	bool checkClause(int x) {

		bool check = false;

		if (radius >= x) {
			check = true;
			return check;
		}

		return check;
	}

	bool checkOpen() {

		bool check = false;

		if (radius <= 0) {
			check = true;
			return check;
		}

		return check;

	}
};
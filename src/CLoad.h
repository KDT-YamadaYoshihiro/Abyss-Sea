#pragma once
#include "enum.h"
#include"CharacterData.h"


class CLoad {

	// �摜�n���h��
	// �^�C�g���w�i
	int bg;
	// �^�C�g��
	int title;
	// �X�e�[�W�w�i
	int stageBg;
	// �t���X�N���[���p�J�[�\��
	int cursor;
	// �U���{�^���A�X�L���{�^��
	int atkButton;
	int sklButton;

	//�@�v���C���[�֘A
	int PlayerGrh[PLAYER_MAX];
	int PfaceGrh[PLAYER_MAX];
	int PbodyGrh[PLAYER_MAX];
	// �G�l�~�[�֘A
	int EnemyGrh[ENEMY_MAX];
	int EfaceGrh[ENEMY_MAX];

#define EF_MAX	6
	int effGrh[EF_MAX];

	// ���n���h��
	// SE
	int seh[SE_MAX];
	// bgm
	int bgmh[BGM_MAX];

	CLoad() {

		// �w�i
		bg = LoadGraph("data/bg/DD_Ruins1.jpg");
		// �^�C�g��
		title = LoadGraph("data/bg/title.png");
		// �X�e�[�W
		stageBg = LoadGraph("data/bg/stageBG.jpg");
		// �J�[�\��
		cursor = LoadGraph("data/bg/mouse_cursor.png");
		// �{�^��
		atkButton = LoadGraph("data/ui/atkB.jpg");
		sklButton = LoadGraph("data/ui/sklB.jpg");


		// �v���C���[
		for (int i = 0; i < PLAYER_MAX; i++) {
			char filepath[64];
			char Ffilepath[64];
			char Bfilepath[64];
			sprintf_s(filepath,"data/character/battle/chara_%d.png",i);
			sprintf_s(Ffilepath, "data/character/face/chara_%d.png", i);
			sprintf_s(Bfilepath, "data/character/body/chara_%d.png", i); // 380,418
			PlayerGrh[i] = LoadGraph(filepath);		// �{��
			PfaceGrh[i] = LoadGraph(Ffilepath);		// �t�F�C�X
			PbodyGrh[i] = LoadGraph(Bfilepath);		// �{�f�B
		}

		// �G�l�~�[
		for (int i = 0; i < ENEMY_MAX; i++) {
			char Bfilepath[64];
			char Ffilepah[64];
			sprintf_s(Bfilepath, "data/enemy/battle/enemy_%d.png", i);
			sprintf_s(Ffilepah, "data/enemy/face/enemy_%d.png", i);
			EnemyGrh[i] = LoadGraph(Bfilepath);
			EfaceGrh[i] = LoadGraph(Ffilepah);
		}
		
		// �G�t�F�N�g
		for (int i = 0; i < EF_MAX; i++) {
			char filepah[64];
			sprintf_s(filepah, "data/effect/effect_%d.png", i);
			effGrh[i] = LoadGraph(filepah);
		}

		// bgm
		for (int i = 0; i < BGM_MAX; i++) {
			char filepath[64];
			sprintf_s(filepath, "data/bgm/bgm_%d.mp3", i);
			bgmh[i] = LoadSoundMem(filepath);
		}

		// SE
		for (int i = 0; i < SE_MAX; i++) {
			char filepath[64];
			sprintf_s(filepath, "data/se/se_%d.mp3", i);
			seh[i] = LoadSoundMem(filepath);
		}

		// ���̑�

	};

	// delete
	virtual ~CLoad() {

		DeleteGraph(bg);
		DeleteGraph(title);
		DeleteGraph(stageBg);
		DeleteGraph(cursor);
		for (int i = 0; i < PLAYER_MAX; i++) {
			DeleteGraph(PlayerGrh[i]);
			DeleteGraph(PfaceGrh[i]);
			DeleteGraph(PbodyGrh[i]);
		}
		for (int i = 0; i < ENEMY_MAX; i++) {

			DeleteGraph(EnemyGrh[i]);
			DeleteGraph(EfaceGrh[i]);
		}
		for (int i = 0; i < EF_MAX; i++) {

			DeleteGraph(effGrh[i]);
		}
		for (int i = 0; i < BGM_MAX; i++) {
			DeleteSoundMem(bgmh[i]);
		}
		for (int i = 0; i < SE_MAX; i++) {
			DeleteSoundMem(seh[i]);
		}
	};

public:

	// �ǂݍ��݃N���X�Ȃ��߁A�V���O���g��
	CLoad(const CLoad&) = default;
	CLoad& operator = (const CLoad&) = default;
	static CLoad& Instance() {
		static CLoad instance;
		return instance;
	}

	// �w�肵���O���t�n���h����return����
	// �w�i
	int getBgGrh() const { return bg; }
	// �^�C�g��
	int getTitleGrh() const { return title; }
	// �X�e�[�W
	int getStageBgGrh() const { return stageBg; }
	// �J�[�\��
	int getCursorGrh() const { return cursor; }
	// �{�^��
	int getAtkButtonGrh() const { return atkButton; }
	int getSklButtonGrh() const { return sklButton; }
 	// �v���C���[
	int getPlayerGrh(int arg_index) const { return PlayerGrh[arg_index]; }
	int getPfaceGrh(int arg_index) const { return PfaceGrh[arg_index]; }
	int getPbodyGrh(int arg_index) const { return PbodyGrh[arg_index]; }
	// �G�l�~�[
	int getEnemyGrh(int arg_index) const { return EnemyGrh[arg_index]; }
	int getEfaceGrh(int arg_index) const { return EfaceGrh[arg_index]; }
	// �G�t�F�N�g
	int getEffectGrh(EffectType arg_index) const { return effGrh[static_cast<int>(arg_index)]; }
	// bgm
	int getBgmHandle(int arg_index) const { return bgmh[arg_index]; }
	// se
	int getSeHandle(int arg_index) const { return seh[arg_index]; }
};
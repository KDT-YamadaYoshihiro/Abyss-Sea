#pragma once
#include "enum.h"
#include"CharacterData.h"


class CLoad {

	// 画像ハンドル
	// タイトル背景
	int bg;
	// タイトル
	int title;
	// ステージ背景
	int stageBg;
	// フルスクリーン用カーソル
	int cursor;
	// 攻撃ボタン、スキルボタン
	int atkButton;
	int sklButton;

	//　プレイヤー関連
	int PlayerGrh[PLAYER_MAX];
	int PfaceGrh[PLAYER_MAX];
	int PbodyGrh[PLAYER_MAX];
	// エネミー関連
	int EnemyGrh[ENEMY_MAX];
	int EfaceGrh[ENEMY_MAX];

#define EF_MAX	6
	int effGrh[EF_MAX];

	// 音ハンドル
	// SE
	int seh[SE_MAX];
	// bgm
	int bgmh[BGM_MAX];

	CLoad() {

		// 背景
		bg = LoadGraph("data/bg/DD_Ruins1.jpg");
		// タイトル
		title = LoadGraph("data/bg/title.png");
		// ステージ
		stageBg = LoadGraph("data/bg/stageBG.jpg");
		// カーソル
		cursor = LoadGraph("data/bg/mouse_cursor.png");
		// ボタン
		atkButton = LoadGraph("data/ui/atkB.jpg");
		sklButton = LoadGraph("data/ui/sklB.jpg");


		// プレイヤー
		for (int i = 0; i < PLAYER_MAX; i++) {
			char filepath[64];
			char Ffilepath[64];
			char Bfilepath[64];
			sprintf_s(filepath,"data/character/battle/chara_%d.png",i);
			sprintf_s(Ffilepath, "data/character/face/chara_%d.png", i);
			sprintf_s(Bfilepath, "data/character/body/chara_%d.png", i); // 380,418
			PlayerGrh[i] = LoadGraph(filepath);		// 本体
			PfaceGrh[i] = LoadGraph(Ffilepath);		// フェイス
			PbodyGrh[i] = LoadGraph(Bfilepath);		// ボディ
		}

		// エネミー
		for (int i = 0; i < ENEMY_MAX; i++) {
			char Bfilepath[64];
			char Ffilepah[64];
			sprintf_s(Bfilepath, "data/enemy/battle/enemy_%d.png", i);
			sprintf_s(Ffilepah, "data/enemy/face/enemy_%d.png", i);
			EnemyGrh[i] = LoadGraph(Bfilepath);
			EfaceGrh[i] = LoadGraph(Ffilepah);
		}
		
		// エフェクト
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

		// その他

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

	// 読み込みクラスなため、シングルトン
	CLoad(const CLoad&) = default;
	CLoad& operator = (const CLoad&) = default;
	static CLoad& Instance() {
		static CLoad instance;
		return instance;
	}

	// 指定したグラフハンドルをreturnする
	// 背景
	int getBgGrh() const { return bg; }
	// タイトル
	int getTitleGrh() const { return title; }
	// ステージ
	int getStageBgGrh() const { return stageBg; }
	// カーソル
	int getCursorGrh() const { return cursor; }
	// ボタン
	int getAtkButtonGrh() const { return atkButton; }
	int getSklButtonGrh() const { return sklButton; }
 	// プレイヤー
	int getPlayerGrh(int arg_index) const { return PlayerGrh[arg_index]; }
	int getPfaceGrh(int arg_index) const { return PfaceGrh[arg_index]; }
	int getPbodyGrh(int arg_index) const { return PbodyGrh[arg_index]; }
	// エネミー
	int getEnemyGrh(int arg_index) const { return EnemyGrh[arg_index]; }
	int getEfaceGrh(int arg_index) const { return EfaceGrh[arg_index]; }
	// エフェクト
	int getEffectGrh(EffectType arg_index) const { return effGrh[static_cast<int>(arg_index)]; }
	// bgm
	int getBgmHandle(int arg_index) const { return bgmh[arg_index]; }
	// se
	int getSeHandle(int arg_index) const { return seh[arg_index]; }
};
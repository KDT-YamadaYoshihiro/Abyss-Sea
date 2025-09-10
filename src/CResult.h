#pragma once
#include "DxLib.h"
#include "Manager.h"
#include "CBase.h"
#include "CPlayer.h"
#include "CEnemy.h"

#include "CTitle.h"
#include "CStage.h"
#include "CBattle.h"

// 三角形の座標一の差異
#define TRIANGEL_X	50
#define TRIANGLE_Y	20

// テキストサイズ
#define TEXT_SIZE	30

class CResult : public ScreenBase {

	enum NEXTSCREEN {
		STAGE,
		BATTLE,
		TITLE,

		MAX
	};

	// 各キャラクターのレベルアップ数
	std::vector<int> levelUpDiff;

	// 音
	std::shared_ptr<Sound>bgm;
	std::shared_ptr<Sound> se;

	// フェード
	std::shared_ptr<Fade> fade;

	// 経験値の再分配防止
	bool isDistributed = false;

	// 選択中の番号
	int choiceNum = -1;

	// 選択中の三角形の座標変数
	int choicePosX = -1;
	int choicePosY = -1;

	// テキストの座標
	int choiceText_X[MAX];
	int choiceText_Y[MAX];

	// 画像表示用の変数
	// 座標
	int baseX;
	int baseY;
	// 切り取り位置
	int scrX = -1;
	int scrY = -1;
	// 切り取りサイズ
	int grhSizeX = -1;
	int grhSizeY = -1;
	// プレイヤー画像の描画間隔
	int spacingX = -1;

public:
	
	// 初期化	
	CResult(){

		// 音源
		bgm = Manager::Instance().getbgm();
		se = Manager::Instance().getSe();

		// フェード
		fade = Manager::Instance().getFade();

		// 経験値の再分配防止
		isDistributed = false;

		// 選択中の番号
		choiceNum = STAGE;

		// 選択中の三角形の座標変数
		choicePosX = -1;
		choicePosY = -1;

		// テキストの座標
		for (int i = 0; i < MAX; i++) {
			choiceText_X[i] = 200;
			choiceText_Y[i] = 650 + i * 50;
		}

		// 画像表示用の変数
		baseX = 100;
		baseY = 120;
		scrX = 60;
		scrY = 0;
		grhSizeX = 250;
		grhSizeY = 418;
		spacingX = grhSizeX + 40;
		//　初期化
		this->Init();

	}

private:


	void Init() {

		//// フェード
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

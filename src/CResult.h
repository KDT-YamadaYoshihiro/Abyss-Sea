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

	// 次のスクリーンの選択肢
	enum NEXTSCREEN {
		STAGE,
		BATTLE,
		TITLE,

		MAX
	};

	// 各キャラクターのレベルアップ数
	std::vector<int> level_up_diff;

	//　UI
	std::shared_ptr<UI> ui;

	// 音
	std::shared_ptr<Sound> bgm;
	std::shared_ptr<Sound> se;

	// フェード
	std::shared_ptr<Fade> fade;

	// 経験値の再分配防止
	bool is_distributed = false;

	// 選択中の番号
	int choice_num = -1;

	// 選択中の三角形の座標変数
	int choice_pos_x = -1;
	int choice_pos_y = -1;

	// テキストの座標
	int choice_text_x[MAX];
	int choice_text_y[MAX];

	// 画像表示用の変数
	// 座標
	int base_x;
	int base_y;
	// 切り取り位置
	int scr_x = -1;
	int scr_y = -1;
	// 切り取りサイズ
	int grh_size_x = -1;
	int grh_size_y = -1;
	// プレイヤー画像の描画間隔
	int spacing_x = -1;

	// 画面切り替え防止用フレームカウント
	int result_frame = -1;

	// 決定ボタン座標
	int box_x = -1;
	int box_y = -1;
	int size_w = -1;
	int size_h = -1;

public:
	
	// 初期化	
	CResult(){

		// 音源
		bgm = ScreenManager::Instance().getbgm();
		se = ScreenManager::Instance().getSe();

		// UI
		ui = std::make_shared<UI>();

		// フェード
		fade = ScreenManager::Instance().getFade();

		// 経験値の再分配防止
		is_distributed = false;

		// 選択中の番号
		choice_num = STAGE;

		// 選択中の三角形の座標変数
		choice_pos_x = -1;
		choice_pos_y = -1;

		// テキストの座標
		for (int i = 0; i < MAX; i++) {
			choice_text_x[i] = 600;
			choice_text_y[i] = 650 + i * 50;
		}

		// 画像表示用の変数
		base_x = 100;
		base_y = 120;
		scr_x = 60;
		scr_y = 0;
		grh_size_x = 250;
		grh_size_y = 418;
		spacing_x = grh_size_x + 40;

		result_frame = 60;

		//　初期化
		this->Init();

		box_x = 900;
		box_y = 700;
		size_w = 200;
		size_h = 80;
	}

private:


	void Init() {
		// bgm
		if (ScreenManager::Instance().getbattleResult() == BATTLE_RESULT::VICTORY) {
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

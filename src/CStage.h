#pragma once
#include "Manager.h"

// ステージ選択スクリーンクラス
class CStage : public ScreenBase {

	// 円の最大数
#define CIRCLEMAX	   3
	// 三角形の座標定数
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
	
	// 音
	std::shared_ptr<Sound>bgm;
	std::shared_ptr<Sound> se;

	// フェード
	std::shared_ptr<Fade> fade;


	// 選択するステージアイコン（文字）
	float posX[CIRCLEMAX];				// X座標
	float posY[CIRCLEMAX];				// Y座標

	// 決定ボタンの箱
	float boxX = -1;
	float boxY = -1;
	float boxSizeX = -1;
	float boxSizeY = -1;

	// 選択中のステージ番号
	int stageNum = -1;

	// 三角形の座標
	float TrX1 = -1;
	float TrY1 = -1;
	float TrX2 = -1;
	float TrY2 = -1;
	float TrX3 = -1;
	float TrY3 = -1;

	// 推奨レベル表示
	int Suggest;

	// パーティーメンバーの確認
	bool selectMode = false;


public:

	void Update() override;

	void Render() override;


	// 初期化
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
		// 文字座標
		for (int i = 0; i < CIRCLEMAX; i++) {
			posX[i] = 100;
			posY[i] = 200 + i * 100;
		}

		// 決定ボタン座標
		boxX = 900.0f;
		boxY = 600.0f;
		boxSizeX = 200.0f;
		boxSizeY = 100.0f;

	};

	// デストラクタ
	virtual ~CStage() {};

	int getStageNum() const { return stageNum; }

private:

	void Triangle();

};
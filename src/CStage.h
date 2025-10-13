#pragma once
#include "Manager.h"
#include "QuestBoard.h"

// ステージ選択スクリーンクラス
class CStage : public ScreenBase {

	// 円の最大数
#define CIRCLEMAX	   3
	// 三角形の座標定数
#define TRX			 90.0f
#define TRY_1		 525.0f
#define TRY_2		 575.0f;
#define TRY_3		 625.0f


	// UI
	std::shared_ptr<UI> ui;
	
	// 音
	std::shared_ptr<Sound> bgm;
	std::shared_ptr<Sound> se;

	// フェード
	std::shared_ptr<Fade> fade;

	// クエストボード
	QuestBoard board;

	// 選択するステージアイコン（文字）
	float posX[STAGE_MAX];				// X座標
	float posY[STAGE_MAX];				// Y座標

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
	int suggest = -1;

	// パーティーメンバーの確認
	bool selectMode = false;

	float speed = -1.0f;
	float vec = -1.0f;

	// ステージ詳細表
	int doc_posX[STAGE_MAX];
	int doc_posY[STAGE_MAX];
	std::string doc_name[STAGE_MAX];


public:

	void Update() override;

	void Render() override;


	// 初期化
	CStage()
		:ui(std::make_shared<UI>()),
		bgm(Manager::Instance().getbgm()),
		se(Manager::Instance().getSe()),
		fade(Manager::Instance().getFade()),
		// 決定ボタン座標
		boxX(1000.0f),		
		boxY(700.0f),
		boxSizeX(200.0f),
		boxSizeY(80.0f),
		// ステージ番号
		stageNum(-1),
		// 三角形の座標
		TrX1(150.0f),
		TrY1(WINDOW_W / 2 - 350),
		TrX2(TrX1 - 50),
		TrY2(TrY1 - 50),
		TrX3(TrX1 + 50),
		TrY3(TrY1 - 50),
		suggest(0),
		speed(0.0f),
		vec(0.1f)
	{
		
		bgm->PlayBgm(CLoad::Instance().getBgmHandle(BGM_START));
		// 文字座標
		for (int i = 0; i < STAGE_MAX; i++) {
			posX[i] = 150;
			posY[i] = 200 + i * 150;
		}

		doc_name[STAGE1] = "【漂う魚影】";
		doc_name[STAGE2] = "【深海の住人】";
		doc_name[STAGE3] = "【奈落の底のモノ】";
		doc_name[STAGE4] = "【深淵の恐怖】";

		board.addRequest({ 1,"漂う魚影", "海底に潜む影を討伐せよ" , 1, CLoad::Instance().getEfaceGrh(GHOUL)});
		board.addRequest({ 2,"深海の住人", "深海の魚人を討伐せよ" , 5, CLoad::Instance().getEfaceGrh(ZPTHOMMOG) });
		board.addRequest({ 3,"奈落の底のモノ", "深海に巣くう脅威を取り払え", 15, CLoad::Instance().getEfaceGrh(YTHOGTHA) });
		board.addRequest({ 4,"深淵の恐怖", "深淵に住まう王を討伐せよ", 22, CLoad::Instance().getEfaceGrh(CTHULHU) });


	};

	// デストラクタ
	virtual ~CStage() {};

	int getStageNum() const { return stageNum; }

private:

	void Triangle();

};
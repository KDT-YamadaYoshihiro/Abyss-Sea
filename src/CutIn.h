#pragma once
#include "DxLib.h"
#include <string>
#include "enum.h"

class Cutuin{

	// 名前
	std::string name;

	// 表示フラグ
	bool drawFlag;

	// 座標
	int x, y;

	// サイズ
	int sizeW, sizeH; 

	// 画像ハンドル
	int handle;

	// 速度
	// フェードイン速度
	int enterSpeed;
	// 画面中央で遅速
	int pauseSpeed;
	// フェードアウト速度
	int exitSpeed; 
	// 中央滞在時間
	int frameCounter;

	// 現在の速度
	CutinPhase phase;

public:

	// 初期化
	Cutuin()
		:drawFlag(false), x(0), y(0), sizeW(0), sizeH(0), handle(-1),
		enterSpeed(0), pauseSpeed(0), exitSpeed(0), frameCounter(0), phase(CutinPhase::Entering)
	{

	};

	// デストラクタ
	virtual ~Cutuin() {};

	// スタート関数
	void Start(std::string arga_name, int arg_x, int arg_y, int arg_sizeW, int arg_sizeH, int arg_handle,
		int arg_enterSpeed, int arg_pauseSpeed, int arg_exitSpeed);

	// リセット関数（再利用を考慮）
	void Reset();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 描画フラグのゲット
	bool getDrawFlag() const { return drawFlag; }

};
#pragma once
#include <string>
#include <memory>
#include "DxLib.h"
#include "enum.h"
#include "CLoad.h"
#include "CEffect.h"

class Cutin{

	// 名前
	std::string skill_name;

	// 表示フラグ
	bool draw_flag;
	//　カットイン終了フラグ
	bool end_flag;

	// 座標
	int pos_x;
	int pos_y;

	// サイズ
	int size_w;
	int size_h;

	// 画像ハンドル
	int handle;

	// 速度
	// フェードイン速度
	int enter_speed;
	// 画面中央で遅速
	int pause_speed;
	// フェードアウト速度
	int exit_speed; 
	// 中央滞在時間
	int frame_counter;

	// 現在の速度
	CUTIN_PHASE phase;

	// カットイン時のエフェクト
	std::shared_ptr<Effect> cutin_effect;


public:

	// 初期化
	Cutin()
		:skill_name(""), draw_flag(false), end_flag(false), pos_x(0), pos_y(0), size_w(300), size_h(600), handle(-1),
		enter_speed(10), pause_speed(2), exit_speed(10), frame_counter(0), 
		phase(CUTIN_PHASE::ENTERING), cutin_effect(nullptr)
	{
		cutin_effect = std::make_shared<Effect>(CLoad::Instance().getCutinEf(),3,3);
	};

	// デストラクタ
	virtual ~Cutin() {};

	// スタート関数
	void Start(int arg_x, int arg_y, int arg_handle, std::string arg_skillName, int arg_enterSpeed, int arg_pauseSpeed, int arg_exitSpeed);

	// リセット関数（再利用を考慮）
	void Reset();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 描画フラグのゲット
	bool GetDrawFlag() const { return draw_flag; }
	// 終了フラグのゲット
	bool GetEndFlag() const { return end_flag; }

};
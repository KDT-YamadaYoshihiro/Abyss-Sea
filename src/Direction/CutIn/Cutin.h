#pragma once
#include <string>
#include <memory>
#include "DxLib.h"
#include "Constants/enum.h"
#include "Load/Load.h"
#include "Direction/Effect/CEffect.h"

class Sound;

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
	// SEハンドル
	int se_handle;

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

	// カットインSE
	std::shared_ptr<Sound> se;


public:

	// 初期化
	Cutin();

	// デストラクタ
	virtual ~Cutin() {};

	// スタート関数
	void Start(int arg_x, int arg_y, int arg_handle, int arg_seHandle, std::string arg_skillName, int arg_enterSpeed, int arg_pauseSpeed, int arg_exitSpeed);

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
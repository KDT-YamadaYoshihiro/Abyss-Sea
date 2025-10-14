#pragma once
#include <string>
#include <memory>
#include "DxLib.h"
#include "enum.h"
#include "CLoad.h"
#include "CEffect.h"

class Cutin{

	// 名前
	std::string m_skill_name;

	// 表示フラグ
	bool m_drawFlag;
	//　カットイン終了フラグ
	bool m_endFlag;

	// 座標
	int m_pos_x;
	int m_pos_y;

	// サイズ
	int m_size_w;
	int m_size_h;

	// 画像ハンドル
	int m_handle;

	// 速度
	// フェードイン速度
	int m_enterSpeed;
	// 画面中央で遅速
	int m_pauseSpeed;
	// フェードアウト速度
	int m_exitSpeed; 
	// 中央滞在時間
	int m_frameCounter;

	// 現在の速度
	CutinPhase m_phase;

	// カットイン時のエフェクト
	std::shared_ptr<Effect> cutinEffect;


public:

	// 初期化
	Cutin()
		:m_skill_name(""), m_drawFlag(false), m_endFlag(false), m_pos_x(0), m_pos_y(0), m_size_w(300), m_size_h(600), m_handle(-1),
		m_enterSpeed(10), m_pauseSpeed(2), m_exitSpeed(10), m_frameCounter(0), 
		m_phase(CutinPhase::Entering), cutinEffect(nullptr)
	{
		cutinEffect = std::make_shared<Effect>(CLoad::Instance().getCutinEf(),3,3);
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
	bool GetDrawFlag() const { return m_drawFlag; }
	// 終了フラグのゲット
	bool GetEndFlag() const { return m_endFlag; }

};
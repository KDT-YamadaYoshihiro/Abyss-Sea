#pragma once
#include <memory>
#include "Constants/enum.h"

class BattleMenu
{
public:
	BattleMenu();
	~BattleMenu() = default;

	// 初期化
	void Init();
	// 更新処理
	BattleMenuState Update();
	// 描画
	void Draw();

private:
	MenuState m_menuState;

	// アニメーションや背景ブレンド用の変数
	int blendNum;
	int blendSpeed;

	// ボタンの配置座標
	int buttonPosX[BUTTAN_MAX];
	int buttonPosY[BUTTON_MAX];

	// 当たり判定
	bool CheckBoxClick(int arg_x, int arg_y, int arg_width, int arg_height);
};


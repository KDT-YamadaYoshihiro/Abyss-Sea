#pragma once
#include <memory>
#include "Constants/enum.h"

class Sound;
class UI;

class BattleMenu
{
private:
	// メニューの状態
	MenuState m_menuState;

	// UIクラスの生成
	std::shared_ptr<UI> ui;

	// 音
	std::shared_ptr<Sound> bgm;
	std::shared_ptr<Sound> se;

	// アニメーションや背景ブレンド用の変数
	int blendNum;
	int blendSpeed;

	// UIボタンの配置座標
	int ui_buttonX = -1;
	int ui_buttonY = -1;

	// SE,BGM音量変更ボタンの配置座標
	int buttonPosX[SOUND_BUTTON_MAX];
	int buttonPosY[SOUND_BUTTON_MAX];

	// クリックフレームカウンター（連続クリック防止用）
	int click_frame;


public:
	BattleMenu();
	~BattleMenu() = default;

	// 初期化
	void Init();
	// 更新処理
	BattleMenuResult Update();
	// 描画
	void Draw();

private:
	// 当たり判定
	bool CheckBoxClick(int arg_x, int arg_y, int arg_width, int arg_height);
};


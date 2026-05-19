#include "BattleMenu.h"
#include "DxLib.h"
#include "Load/Load.h"
#include "Scene/Manager/Manager.h"

#define FONT_BIGSIZE	 50
#define FONT_SIZE		 30
#define FONT_MINSIZE	 15

#define MAX_VOL			255

BattleMenu::BattleMenu()
	:m_menuState(MenuState::MAIN),
	blendNum(0),
	blendSpeed(2)
{
	// ボタンの座標初期化
	buttonPosX[BGM_UP] = WINDOW_W / 2 - 280;
	buttonPosX[BGM_UP] = WINDOW_H / 2 - 120;
	buttonPosX[BGM_DOWN] = WINDOW_W / 2 - 280;
	buttonPosX[BGM_DOWN] = WINDOW_H / 2 + 70;
	buttonPosX[SE_UP] = WINDOW_W / 2 + 260;
	buttonPosX[SE_UP] = WINDOW_H / 2 - 120;
	buttonPosX[SE_DOWN] = WINDOW_W / 2 + 260;
	buttonPosX[SE_DOWN] = WINDOW_H / 2 + 70;
}

void BattleMenu::Init()
{
	// メニューを開いたときは必ず最初から
	m_menuState = MenuState::MAIN;
	blendNum = 0;
}

BattleMenuResult BattleMenu::Update()
{
	// 背景のブレンドアニメーション更新（深海風演出）
	blendNum += blendSpeed;
	if (blendNum <= 0 || blendNum >= 50) { blendSpeed *= -1; }

	switch (m_menuState)
	{
	case MenuState::MAIN:

		// 選択画面の表示
		// バトル再開
		if (CheckBoxClick(ui_buttonX, ui_buttonY, size_w, size_h)) {
			return BattleMenuResult::CLOSE;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
		}
		// 再挑戦
		if (CheckBoxClick(ui_buttonX, ui_buttonY + 100, size_w, size_h)) {
			// モード変更
			m_menuState = MenuState::RETRY;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
		}
		// リタイア
		if (CheckBoxClick(ui_buttonX, ui_buttonY + 200, size_w, size_h)) {
			// モード変更
			m_menuState = MenuState::RETIRE;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
		}
		// （設定）
		if (CheckBoxClick(ui_buttonX, ui_buttonY + 300, size_w, size_h)) {
			m_menuState = MenuState::SETTING;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

		}


		break;
	case MenuState::RETRY:

		// 再確認
		// はい
		if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 6), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), size_w, size_h)) {
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
			return BattleMenuResult::RETRY;

		}
		// いいえ
		if (CheckBoxClick(WINDOW_W / 2 + (FONT_BIGSIZE * 2.5), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), size_w, size_h)) {
			// MENUの選択画面に戻る
			m_menuState = MenuState::MAIN;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
		}


		break;
	case MenuState::RETIRE:

		// 再確認
		// はい
		if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 6), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), size_w, size_h)) {
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
			// bgmの停止
			bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + ScreenManager::Instance().getStageScreen()));
			return BattleMenuResult::RETIRE;
		}
		// いいえ
		if (CheckBoxClick(WINDOW_W / 2 + (FONT_BIGSIZE * 2.5), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), size_w, size_h)) {
			// MENUの選択画面に戻る
			m_menuState = MenuState::MAIN;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));

		}

		break;

	case MenuState::SETTING:

		if (click_frame > 0) {
			click_frame--;
		}

		// 音量変更
		for (int i = 0; i < BUTTAN_MAX; i++) {

			int size;
			if (i % 2 == 0) { size = MAX_VOL / 10; }
			else { size = -MAX_VOL / 10; }

			if (click_frame == 0 && CheckCircleClick(tr_pos_x[i], tr_pos_y[i] + size, 50)) {

				int bgmVol = ScreenManager::Instance().getBgmVolume();
				int seVol = ScreenManager::Instance().getSeVolume();

				click_frame = 10;
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

				switch (i)
				{
				case BGM_UP:

					ScreenManager::Instance().setBgmVolume(bgmVol + 1);

					break;

				case BGM_DOWN:
					ScreenManager::Instance().setBgmVolume(bgmVol - 1);

					break;

				case SE_UP:
					ScreenManager::Instance().setSeVolume(seVol + 1);

					break;

				case SE_DOWN:
					ScreenManager::Instance().setSeVolume(seVol - 1);

					break;

				default:
					break;
				}

			}

		}

		// 決定ボタンを押した際
		if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H - 100, size_w, size_h)) {
			// MENUの選択画面に戻る
			m_menuState = MenuState::MAIN;
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
		}

		break;
	default:
		break;
	}

	return BattleMenuResult::KEEP_OPEN;
}
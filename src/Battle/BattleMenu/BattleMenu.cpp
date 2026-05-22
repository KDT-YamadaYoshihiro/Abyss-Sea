#include "BattleMenu.h"
#include "DxLib.h"
#include "Load/Load.h"
#include "Scene/Manager/Manager.h"
#include "Constants/define.h"
#include "Direction/Sound/Sound.h"

BattleMenu::BattleMenu()
	:m_menuState(MenuState::MAIN),
	blendNum(0),
	blendSpeed(2),
	click_frame(0)
{

	ui_buttonX = WINDOW_W / 2 - FONT_BIGSIZE * 1.5;
	ui_buttonY = 250;

	// ボタンの座標初期化
	buttonPosX[BGM_UP] = WINDOW_W / 2 - 280;
	buttonPosY[BGM_UP] = WINDOW_H / 2 - 120;
	buttonPosX[BGM_DOWN] = WINDOW_W / 2 - 280;
	buttonPosY[BGM_DOWN] = WINDOW_H / 2 + 70;
	buttonPosX[SE_UP] = WINDOW_W / 2 + 260;
	buttonPosY[SE_UP] = WINDOW_H / 2 - 120;
	buttonPosX[SE_DOWN] = WINDOW_W / 2 + 260;
	buttonPosY[SE_DOWN] = WINDOW_H / 2 + 70;
}

void BattleMenu::Init()
{
	ui = std::make_shared<UI>();

	se = ScreenManager::Instance().getSe();
	bgm = ScreenManager::Instance().getbgm();

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
		if (CheckBoxClick(ui_buttonX, ui_buttonY, BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
			return BattleMenuResult::CLOSE;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
		}
		// 再挑戦
		if (CheckBoxClick(ui_buttonX, ui_buttonY + 100, BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
			// モード変更
			m_menuState = MenuState::RETRY;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
		}
		// リタイア
		if (CheckBoxClick(ui_buttonX, ui_buttonY + 200, BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
			// モード変更
			m_menuState = MenuState::RETIRE;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
		}
		// （設定）
		if (CheckBoxClick(ui_buttonX, ui_buttonY + 300, BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
			m_menuState = MenuState::SETTING;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

		}


		break;
	case MenuState::RETRY:

		// 再確認
		// はい
		if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 6), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
			return BattleMenuResult::RETRY;

		}
		// いいえ
		if (CheckBoxClick(WINDOW_W / 2 + (FONT_BIGSIZE * 2.5), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
			// MENUの選択画面に戻る
			m_menuState = MenuState::MAIN;
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
		}


		break;
	case MenuState::RETIRE:

		// 再確認
		// はい
		if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 6), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
			// se再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
			// bgmの停止
			bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + ScreenManager::Instance().getStageScreen()));
			return BattleMenuResult::RETIRE;
		}
		// いいえ
		if (CheckBoxClick(WINDOW_W / 2 + (FONT_BIGSIZE * 2.5), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
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
		for (int i = 0; i < SOUND_BUTTON_MAX; i++) {

			int size;
			if (i % 2 == 0) { size = MAX_VOL / 10; }
			else { size = -MAX_VOL / 10; }

			if (click_frame == 0 && CheckCircleClick(buttonPosX[i], buttonPosY[i] + size, 50)) {

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
		if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H - 100, BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
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

void BattleMenu::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, WINDOW_W, WINDOW_H, GetColor(50, 50, 50), true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	switch (m_menuState)
	{
	case MenuState::MAIN:

		SetFontSize(FONT_BIGSIZE);

		ui->Button(ui_buttonX, ui_buttonY, ui_buttonX + BUTTAN_WIDTH, ui_buttonY + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(CONTINUE));
		ui->Button(ui_buttonX, ui_buttonY + 100, ui_buttonX + BUTTAN_WIDTH, ui_buttonY + 100 + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(RETRY));
		ui->Button(ui_buttonX, ui_buttonY + 200, ui_buttonX + BUTTAN_WIDTH, ui_buttonY + 200 + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(GIVEUP));
		ui->Button(ui_buttonX, ui_buttonY + 300, ui_buttonX + BUTTAN_WIDTH, ui_buttonY + 300 + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(SETTING));



		break;
	case MenuState::RETRY:

		// 文字
		SetFontSize(FONT_BIGSIZE);
		DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 4), WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "再挑戦しますか？");

		{
			int x1 = WINDOW_W / 2 - (FONT_BIGSIZE * 6);
			int x2 = WINDOW_W / 2 + (FONT_BIGSIZE * 2.5);
			int y = WINDOW_H / 2 + (FONT_BIGSIZE * 1.5);
			ui->Button(x1, y, x1 + BUTTAN_WIDTH, y + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(YES));
			ui->Button(x2, y, x2 + BUTTAN_WIDTH, y + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(NO));
		}

		break;

	case MenuState::RETIRE:

		// 文字
		SetFontSize(FONT_BIGSIZE);
		DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 4.5), WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "リタイアしますか？");
		{
			int x1 = WINDOW_W / 2 - (FONT_BIGSIZE * 6);
			int x2 = WINDOW_W / 2 + (FONT_BIGSIZE * 2.5);
			int y = WINDOW_H / 2 + (FONT_BIGSIZE * 1.5);
			ui->Button(x1, y, x1 + BUTTAN_WIDTH, y + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(YES));
			ui->Button(x2, y, x2 + BUTTAN_WIDTH, y + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(NO));
		}

		break;

	case MenuState::SETTING:

		SetFontSize(FONT_BIGSIZE);
		DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H / 2 - (FONT_BIGSIZE * 4.5), GetColor(255, 255, 255), "音量設定");
		DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 7), WINDOW_H / 2 - (FONT_BIGSIZE), GetColor(255, 255, 255), "BGM:%d", ScreenManager::Instance().getBgmVolume());
		DrawFormatString(WINDOW_W / 2 + (FONT_BIGSIZE * 4), WINDOW_H / 2 - (FONT_BIGSIZE), GetColor(255, 255, 255), "SE:%d", ScreenManager::Instance().getSeVolume());

		// 音量の調整ボタン
		for (int i = 0; i < SOUND_BUTTON_MAX; i++) {

			int x1;
			int x2;
			int y1;
			int y2;

			switch (i)
			{
			case BGM_UP:

				x1 = buttonPosX[BGM_UP] - 30;
				x2 = buttonPosX[BGM_UP] + 30;
				y1 = y2 = buttonPosY[BGM_UP] + 50;

				break;
			case BGM_DOWN:


				x1 = buttonPosX[BGM_DOWN] - 30;
				x2 = buttonPosX[BGM_DOWN] + 30;
				y1 = y2 = buttonPosY[BGM_DOWN] - 50;


				break;
			case SE_UP:

				x1 = buttonPosX[SE_UP] - 30;
				x2 = buttonPosX[SE_UP] + 30;
				y1 = y2 = buttonPosY[SE_UP] + 50;


				break;
			case SE_DOWN:

				x1 = buttonPosX[SE_DOWN] - 30;
				x2 = buttonPosX[SE_DOWN] + 30;
				y1 = y2 = buttonPosY[SE_DOWN] - 50;

				break;

			default:
				break;
			}

			DrawTriangle(buttonPosX[i], buttonPosY[i], x1, y1, x2, y2, GetColor(255, 255, 255), true);
		}

		ui->Button(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H - 100, WINDOW_W / 2 - (FONT_BIGSIZE * 2) + BUTTAN_WIDTH, WINDOW_H - 100 + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(DECISION));


		break;


	default:
		break;
	}
}

bool BattleMenu::CheckBoxClick(int arg_x, int arg_y, int arg_width, int arg_height)
{
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		if (mouseX >= arg_x && mouseX <= arg_x + arg_width && mouseY >= arg_y && mouseY <= arg_y + arg_height) {
			return true;
		}
	}
	return false;
}

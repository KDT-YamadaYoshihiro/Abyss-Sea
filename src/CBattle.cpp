#include "CBattle.h"
#include "CResult.h"
#include "DxLib.h"

void CBattle::Update()
{

	// フェードの更新処理
	fade->fadeUpdate(WINDOW_W);

	for (auto& p : Manager::Instance().getParty()) {
		p->AnimUpdata();
	}


	blendNum += blendSpeed;
	if (blendNum <= 0) { blendSpeed *= -1; }
	if (blendNum >= 50) { blendSpeed *= -1; }


	switch (state)
	{
	case CBattle::State::BATTLE:		// バトル

		if (fade->checkOpen()) {
			BattleMain();
		}

		break;

	case CBattle::State::MENU:			// MENU展開時

		BattleMenu();

		break;
	case CBattle::State::DEAD_EFFECT:

		PlayEndEffect();

		break;

	case CBattle::State::END:			// バトル終了時

		BattleEnd();

		break;

	default:
		break;
	}


	// fadeが起動後CResultに切り替え
	if (fade->checkClause(WINDOW_W) && BattleOver()) {
		Manager::Instance().ChangeScreen<CResult>();
	}

}

void CBattle::Render()
{
	// 背景描画
	int x = 0;
	int y = 0;
	ui->BgRender(x, y, WINDOW_W, WINDOW_H,CLoad::Instance().getBgGrh());

	// 指定したIDのエネミーを描画
	for (auto& e : enemies) {
		// クリック時に使用する変数セット
		e->setPosX(ePosX);
		e->setPosY(ePosY);
		//	本体
		e->Render(ePosX, ePosY, CLoad::Instance().getEnemyGrh((e_id / 10000) - 21));
		e->powerRender();

		if (!e->getAlive()) { e->DeadAnimDraw(); }

	}
	// プレイヤーの描画
	for (size_t i = 0; i < Manager::Instance().getParty().size(); i++) {
		auto& players = Manager::Instance().getParty();
		// クリック時に使用する変数セット
		int x = pPosX[i] + (i % 2) * 50;
		int y = pPosY[i] + i * 80;

		players[i]->setPosX(x);
		players[i]->setPosY(y);
		//	本体
		players[i]->Render(x, y, CLoad::Instance().getPlayerGrh(i));
		
	}
	
	// プレイヤーのエフェクトアニメーション
	for (auto& p : Manager::Instance().getParty()) {
		p->animDraw();
		p->powerRender();
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendNum);
	DrawBox(0, 0, WINDOW_W, WINDOW_H, (0, 0, 255), true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	// 何ターン目か表示
	SetFontSize(FONT_SIZE);
	DrawFormatString(FONT_SIZE, 10, GetColor(255, 255, 255), "%2dターン目", this->turn);
	// プレイヤーとエネミーの1ターン分の行動順を表示
	DrawTurnOrderIcons(30, 50);


	// 選択できるキャラクターを円を描画
	if (targetInput == TargetInput::TARGETCHOICE) {

		// 行動キャンセル用のボタン
		SetFontSize(FONT_BIGSIZE);
		DrawFormatString(WINDOW_W - 160, 70, GetColor(255, 255, 255), "<戻る>");

		for (auto& t : TargetList) {
			Position pos = GetCharacterCenter(t);
			// 赤色の枠
			DrawCircle(pos.x, pos.y, 40, GetColor(255, 0, 0), FALSE); 
		}
	}

	// 
	if (targetInput == TargetInput::ACTIONCHOICE) {
		// ボタンの表示
		ui->Button(ButtonX, atButtonY, ButtonX + ButtonSizeX, atButtonY + ButtonSizeY, CLoad::Instance().getAtkButtonGrh());
		ui->Button(ButtonX, skButtonY, ButtonX + ButtonSizeX, skButtonY + ButtonSizeY, CLoad::Instance().getSklButtonGrh());

		// スキルポイントの表示
		ui->SkillPoint(100, 300, SP_RADIUS, sp->getSP(), sp->getMaxSP());
	}
	// メニューバーの表示
	SetFontSize(FONT_BIGSIZE);
	if (state == State::BATTLE) {
		int posX = WINDOW_W - 250;
		int posY = 10;
		int size_w = 250;
		int size_h = 150;
		DrawExtendGraph(posX, posY, posX + size_w, posY + size_h, CLoad::Instance().getMenuButton(),true);
	}

	//　MENUの中身
	if (state == State::MENU) {

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, WINDOW_W, WINDOW_H, GetColor(50,50,50), true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		switch (menuState)
		{
		case CBattle::MenuState::NONE:

			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5) , 250, GetColor(255, 255, 255), "続ける");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5), 350, GetColor(255, 255, 255), "再挑戦");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 2), 450, GetColor(255, 255, 255), "リタイア");
			DrawFormatString(WINDOW_W / 2 - FONT_BIGSIZE, 550, GetColor(255, 255, 255), "設定");

			break;
		case CBattle::MenuState::RETRY:

			// 文字
			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE *4),		WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "再挑戦しますか？");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 3),	WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "はい");
			DrawFormatString(WINDOW_W / 2 + FONT_BIGSIZE,			WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "いいえ");

			break;
		case CBattle::MenuState::RETIRE:

			// 文字
			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 4.5), WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "リタイアしますか？");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 3), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "はい");
			DrawFormatString(WINDOW_W / 2 + FONT_BIGSIZE, WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "いいえ");

			break;

		case CBattle::MenuState::SETTING:

			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H / 2 - (FONT_BIGSIZE * 4.5), GetColor(255, 255, 255), "音量設定");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 7), WINDOW_H / 2 - (FONT_BIGSIZE), GetColor(255, 255, 255), "BGM:%d",Manager::Instance().getBgmVolume());
			DrawFormatString(WINDOW_W / 2 + (FONT_BIGSIZE * 4), WINDOW_H / 2 - (FONT_BIGSIZE), GetColor(255, 255, 255), "SE:%d",Manager::Instance().getSeVolume());

			// 音量の調整ボタン
			for (int i = 0; i < BUTTAN_MAX; i++) {

				int x1;
				int x2;
				int y1;
				int y2;

				switch (i)
				{
				case BGM_UP:

					x1 = trPosX[BGM_UP] - 30;
					x2 = trPosX[BGM_UP] + 30;
					y1 = y2 = trPosY[BGM_UP] + 50;

					break;
				case BGM_DOWN:


					x1 = trPosX[BGM_DOWN] - 30;
					x2 = trPosX[BGM_DOWN] + 30;
					y1 = y2 = trPosY[BGM_DOWN] - 50;


					break;
				case SE_UP:

					x1 = trPosX[SE_UP] - 30;
					x2 = trPosX[SE_UP] + 30;
					y1 = y2 = trPosY[SE_UP] + 50;


					break;
				case SE_DOWN:

					x1 = trPosX[SE_DOWN] - 30;
					x2 = trPosX[SE_DOWN] + 30;
					y1 = y2 = trPosY[SE_DOWN] - 50;

					break;

				default:
					break;
				}

				DrawTriangle(trPosX[i], trPosY[i], x1,y1,x2,y2, GetColor(255, 255, 255), true);
			}

			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE*2), WINDOW_H - 200, GetColor(255, 255, 255), "<決定>");

			break;


		default:
			break;
		}
	}

}

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
		int id = e->getId() - 100;
		e->Render(ePosX, ePosY, CLoad::Instance().getEnemyGrh(id));
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
		players[i]->Render(x, y, CLoad::Instance().getPlayerGrh(players[i]->getId()));
		
	}

	for (size_t p = 0; p < Manager::Instance().getParty().size(); p++) {

		for (size_t i = 0; i < TargetList.size(); i++) {
			auto& players = Manager::Instance().getParty();

			players[p]->EffectDraw(TargetList[i]->getPosX(), TargetList[i]->getPosY());
		}
	}
	
	// プレイヤーのエフェクトアニメーション
	for (auto& p : Manager::Instance().getParty()) {
		p->powerRender();
	}

	// 深海っぽくするために、青のボックス描画
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
		ui->Button(boxX, boxY, boxX + sizeW, boxY + sizeH, CLoad::Instance().getButtonGrh(BACK));

		for (auto& t : TargetList) {
			Position pos = GetCharacterCenter(t);
			// 赤色の枠
			DrawCircle(pos.x, pos.y, 40, GetColor(255, 0, 0), FALSE); 
		}

	}
	
	// スキル選択時説明の表示
	if (skdescDraw) {
		// 説明の表示
		SetFontSize(FONT_MINSIZE);
		DrawFormatString(100, 200, GetColor(255, 255, 255), turnOrder[currentTurnIndex]->getSkillName().c_str());
		DrawFormatString(100, 200 + FONT_MINSIZE, GetColor(255, 255, 255), turnOrder[currentTurnIndex]->getDetails().c_str());
	}


	// UI
	if (targetInput == TargetInput::ACTIONCHOICE) {
		// ボタンの表示
		ui->Button(ButtonX, atButtonY, ButtonX + ButtonSizeX, atButtonY + ButtonSizeY, CLoad::Instance().getAtkButtonGrh());
		ui->Button(ButtonX, skButtonY, ButtonX + ButtonSizeX, skButtonY + ButtonSizeY, CLoad::Instance().getSklButtonGrh());

		// スキルポイントの表示
		ui->SkillPoint(100, 300, SP_RADIUS, sp->getSP(), sp->getMaxSP());
	}

	// 行動中のキャラクターのステータス
	if (currentTurnIndex < turnOrder.size()) {
		ui->SelectStatus(20, WINDOW_H - 120, 330, 100, turnOrder[currentTurnIndex]->getFaceHandle(), turnOrder[currentTurnIndex]);
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

			ui->Button(buttonX, buttonY, buttonX + sizeW, buttonY + sizeH, CLoad::Instance().getButtonGrh(CONTINUE));
			ui->Button(buttonX, buttonY + 100, buttonX + sizeW, buttonY + 100 + sizeH, CLoad::Instance().getButtonGrh(RETRY));
			ui->Button(buttonX, buttonY + 200, buttonX + sizeW, buttonY + 200 + sizeH, CLoad::Instance().getButtonGrh(GIVEUP));
			ui->Button(buttonX, buttonY + 300, buttonX + sizeW, buttonY + 300 + sizeH, CLoad::Instance().getButtonGrh(SETTING));
			


			break;
		case CBattle::MenuState::RETRY:

			// 文字
			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 4),		WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "再挑戦しますか？");

			{
				int x1 = WINDOW_W / 2 - (FONT_BIGSIZE * 6);
				int x2 = WINDOW_W / 2 + (FONT_BIGSIZE * 2.5);
				int y = WINDOW_H / 2 + (FONT_BIGSIZE * 1.5);
				ui->Button(x1, y, x1 + sizeW, y + sizeH, CLoad::Instance().getButtonGrh(YES));
				ui->Button(x2, y, x2 + sizeW, y + sizeH, CLoad::Instance().getButtonGrh(NO));
			}

			break;

		case CBattle::MenuState::RETIRE:

			// 文字
			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 4.5), WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "リタイアしますか？");
			{
				int x1 = WINDOW_W / 2 - (FONT_BIGSIZE * 6);
				int x2 = WINDOW_W / 2 + (FONT_BIGSIZE * 2.5);
				int y = WINDOW_H / 2 + (FONT_BIGSIZE * 1.5);
				ui->Button(x1, y, x1 + sizeW, y + sizeH, CLoad::Instance().getButtonGrh(YES));
				ui->Button(x2, y, x2 + sizeW, y + sizeH, CLoad::Instance().getButtonGrh(NO));
			}

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

			ui->Button(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H - 100, WINDOW_W / 2 - (FONT_BIGSIZE * 2) + sizeW, WINDOW_H - 100 + sizeH, CLoad::Instance().getButtonGrh(DECISION));


			break;


		default:
			break;
		}
	}

}

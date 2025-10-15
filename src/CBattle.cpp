#include "CBattle.h"
#include "CResult.h"
#include "DxLib.h"

void CBattle::Update()
{

	// フェードの更新処理
	fade->fadeUpdate(WINDOW_W);

	// エネミーのカットイン更新
	for (auto& e : enemies) {
		e->CutinUpdate();
	}

	for (auto& p : ScreenManager::Instance().getParty()) {
		// プレイヤーのアニメーション更新
		p->AnimUpdata();
		// プレイヤーのカットイン更新
		p->CutinUpdate();
	}

	// 深海っぽくするために、青のボックス描画
	blend_num += blend_speed;
	// 範囲制限
	if (blend_num <= 0) { blend_speed *= -1; }
	if (blend_num >= 50) { blend_speed *= -1; }

	// ミッション内容の取得
	MissionManager::Instance().GetByStage(ScreenManager::Instance().getStageScreen());
	
	// ミッションのクリア判定
	MissionManager::Instance().SetCleared(ScreenManager::Instance().getStageScreen(), MissionType::TURN_LIMIT, (turn <= 15));

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

	// ミッションのクリア判定
	MissionManager::Instance().TurnLimit(stage, turn);

	// fadeが起動後CResultに切り替え
	if (fade->checkClause(WINDOW_W) && BattleOver()) {

		// リザルト画面へ
		ScreenManager::Instance().ChangeScreen<CResult>();
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
		e->setPosX(e_pos_x);
		e->setPosY(e_pos_y);
		//	本体
		int id = e->getId() - 100;
		e->Render(e_pos_x, e_pos_y, CLoad::Instance().getEnemyGrh(id));
		e->powerRender();

		if (!e->getAlive()) { e->DeadAnimDraw(); }

	}

	// プレイヤーの描画
	for (size_t i = 0; i < ScreenManager::Instance().getParty().size(); i++) {
		auto& players = ScreenManager::Instance().getParty();
		// クリック時に使用する変数セット
		int x = p_pos_x[i] + (i % 2) * 50;
		int y = p_pos_y[i] + i * 50;

		players[i]->setPosX(x);
		players[i]->setPosY(y);
		//	本体
		players[i]->Render(x, y, CLoad::Instance().getPlayerGrh(players[i]->getId()));
	}

	// エフェクトの再生
	for (size_t p = 0; p < ScreenManager::Instance().getParty().size(); p++) {
		
		auto& players = ScreenManager::Instance().getParty();

		// 複数再生
		if (turn_order[current_turn_index]->getTargetType() == SKILL_TARGET_TYPE::ALL_ALLY || 
			turn_order[current_turn_index]->getTargetType() == SKILL_TARGET_TYPE::ALL_ENEMY) {

			for (size_t i = 0; i < target_list.size(); i++) {
				players[p]->EffectDraw(target_list[i]->getPosX(), target_list[i]->getPosY());
			}
		}
		else { // 単体時&&selectTargetがnullptrでないとき
			if (select_target != nullptr) {
				players[p]->EffectDraw(select_target->getPosX(), select_target->getPosY());
			}
		}
	}
	
	// プレイヤーのエフェクトアニメーション
	for (auto& p : ScreenManager::Instance().getParty()) {
		p->powerRender();
	}

	// 深海っぽくするために、青のボックス描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blend_num);
	DrawBox(0, 0, WINDOW_W, WINDOW_H, (0, 0, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	// 何ターン目か表示
	SetFontSize(FONT_SIZE);
	DrawFormatString(FONT_SIZE, 10, GetColor(255, 255, 255), "%2dターン目", this->turn);
	// プレイヤーとエネミーの1ターン分の行動順を表示
	DrawTurnOrderIcons(30, 50);

	{
		int x = 30;
		int y = 150;
		// ベースを表示
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(x, y , x + 300, y + 100, GetColor(0,0,0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// ミッション表示
		SetFontSize(FONT_MINSIZE);
		ScreenManager::Instance().DrawStageUI(ScreenManager::Instance().getStageScreen(), x + 30, y + 25);
	}

	// 選択できるキャラクターを円を描画
	if (targetInput == TargetInput::TARGETCHOICE) {

		// 行動キャンセル用のボタン
		SetFontSize(FONT_BIGSIZE);
		ui->Button(box_x, box_y, box_x + size_w, box_y + size_h, CLoad::Instance().getButtonGrh(BACK));

		for (auto& t : target_list) {
			Position pos = GetCharacterCenter(t);
			// 赤色の枠
			DrawCircle(pos.x, pos.y, 40, GetColor(255, 0, 0), FALSE); 
		}
	}
	

	// UI
	if (targetInput == TargetInput::ACTIONCHOICE) {
		// ボタンの表示
		ui->Button(at_button_x, button_y, at_button_x + button_size_x, button_y + button_size_y, CLoad::Instance().getAtkButtonGrh());
		ui->Button(sk_button_x, button_y, sk_button_x + button_size_x, button_y + button_size_y, CLoad::Instance().getSklButtonGrh());
	}
	// スキルポイントの表示
	ui->SkillPoint(530, WINDOW_H - 85, SP_RADIUS, sp->getSP(), sp->getMaxSP());

	// 行動中のキャラクターのステータス
	for (size_t i = 0; i < ScreenManager::Instance().getParty().size(); i++) {
		auto paty = ScreenManager::Instance().getParty();
		int x = 20 + (i * 105);
		ui->SelectStatus(x, WINDOW_H - 125, 100, 120, paty[i]->getFaceHandle(), paty[i]);
		// バフ・デバフアイコン表示
		// 描画時
		paty[i]->DrawIcons(x, WINDOW_H - 160);

	}
	
	// スキル選択時説明の表示
	if (sk_desc_draw) {

		int x = 20;
		int y = WINDOW_H - 210;
		int size_w = 400;
		int size_h = 50;

		// 
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(x, y, x + size_w, y + size_h, GetColor(100, 100, 100), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// 説明の表示
		SetFontSize(FONT_MINSIZE);
		DrawFormatString(x + 10, y + 10, GetColor(255, 255, 255), turn_order[current_turn_index]->getSkillName().c_str());
		DrawFormatString(x + 10, y + 10 + FONT_MINSIZE, GetColor(255, 255, 255), turn_order[current_turn_index]->getDetails().c_str());
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

			ui->Button(ui_buttonX, ui_buttonY, ui_buttonX + size_w, ui_buttonY + size_h, CLoad::Instance().getButtonGrh(CONTINUE));
			ui->Button(ui_buttonX, ui_buttonY + 100, ui_buttonX + size_w, ui_buttonY + 100 + size_h, CLoad::Instance().getButtonGrh(RETRY));
			ui->Button(ui_buttonX, ui_buttonY + 200, ui_buttonX + size_w, ui_buttonY + 200 + size_h, CLoad::Instance().getButtonGrh(GIVEUP));
			ui->Button(ui_buttonX, ui_buttonY + 300, ui_buttonX + size_w, ui_buttonY + 300 + size_h, CLoad::Instance().getButtonGrh(SETTING));
			


			break;
		case CBattle::MenuState::RETRY:

			// 文字
			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 4),		WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "再挑戦しますか？");

			{
				int x1 = WINDOW_W / 2 - (FONT_BIGSIZE * 6);
				int x2 = WINDOW_W / 2 + (FONT_BIGSIZE * 2.5);
				int y = WINDOW_H / 2 + (FONT_BIGSIZE * 1.5);
				ui->Button(x1, y, x1 + size_w, y + size_h, CLoad::Instance().getButtonGrh(YES));
				ui->Button(x2, y, x2 + size_w, y + size_h, CLoad::Instance().getButtonGrh(NO));
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
				ui->Button(x1, y, x1 + size_w, y + size_h, CLoad::Instance().getButtonGrh(YES));
				ui->Button(x2, y, x2 + size_w, y + size_h, CLoad::Instance().getButtonGrh(NO));
			}

			break;

		case CBattle::MenuState::SETTING:

			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H / 2 - (FONT_BIGSIZE * 4.5), GetColor(255, 255, 255), "音量設定");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 7), WINDOW_H / 2 - (FONT_BIGSIZE), GetColor(255, 255, 255), "BGM:%d",ScreenManager::Instance().getBgmVolume());
			DrawFormatString(WINDOW_W / 2 + (FONT_BIGSIZE * 4), WINDOW_H / 2 - (FONT_BIGSIZE), GetColor(255, 255, 255), "SE:%d",ScreenManager::Instance().getSeVolume());

			// 音量の調整ボタン
			for (int i = 0; i < BUTTAN_MAX; i++) {

				int x1;
				int x2;
				int y1;
				int y2;

				switch (i)
				{
				case BGM_UP:

					x1 = tr_pos_x[BGM_UP] - 30;
					x2 = tr_pos_x[BGM_UP] + 30;
					y1 = y2 = tr_pos_y[BGM_UP] + 50;

					break;
				case BGM_DOWN:


					x1 = tr_pos_x[BGM_DOWN] - 30;
					x2 = tr_pos_x[BGM_DOWN] + 30;
					y1 = y2 = tr_pos_y[BGM_DOWN] - 50;


					break;
				case SE_UP:

					x1 = tr_pos_x[SE_UP] - 30;
					x2 = tr_pos_x[SE_UP] + 30;
					y1 = y2 = tr_pos_y[SE_UP] + 50;


					break;
				case SE_DOWN:

					x1 = tr_pos_x[SE_DOWN] - 30;
					x2 = tr_pos_x[SE_DOWN] + 30;
					y1 = y2 = tr_pos_y[SE_DOWN] - 50;

					break;

				default:
					break;
				}

				DrawTriangle(tr_pos_x[i], tr_pos_y[i], x1,y1,x2,y2, GetColor(255, 255, 255), true);
			}

			ui->Button(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H - 100, WINDOW_W / 2 - (FONT_BIGSIZE * 2) + size_w, WINDOW_H - 100 + size_h, CLoad::Instance().getButtonGrh(DECISION));


			break;


		default:
			break;
		}
	}


	// カットインの描画
	for (auto& e : enemies) {
		e->CutinDraw();
	}
	for (auto& p : ScreenManager::Instance().getParty()) {
		p->CutinDraw();
	}

}

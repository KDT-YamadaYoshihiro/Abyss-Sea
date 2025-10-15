 #include "CResult.h"

void CResult::Update()
{
	// フェードの更新処理
	fade->fadeUpdate(WINDOW_W);

	result_frame--;

	// 選択中の三角形の座標
	ChoiceNectScreen();

	// クリックしたテキストに選択変更
	for (int i = 0; i < MAX; i++) {
		if (CheckBoxClick(choice_text_x[i], choice_text_y[i], TEXT_SIZE * 7, TEXT_SIZE)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			choice_num = i;
		}
	}

	// レベルアップ
	if (ScreenManager::Instance().getbattleResult() == BATTLE_RESULT::VICTORY && !is_distributed) {

		auto& party = ScreenManager::Instance().getParty();
		auto& players = ScreenManager::Instance().getPlayers();

		int totalExp = 0;

		// エネミーの経験値を取得
		totalExp = ScreenManager::Instance().getExp();


		// 取得したエネミーの経験値をパーティメンバー分の割る。
				// 割った経験値を各キャラクターにキャスト
		int preMamber = (party.empty() ? 0 : totalExp / static_cast<int>(party.size()));

		// 初期化
		level_up_diff.assign(party.size(), 0); 

		// 経験値MaxExp以上で1LvUp
		for (size_t i = 0; i < party.size(); i++) {
			auto& p = party[i];
			p->addExp(preMamber);

			// レベルが上がる前のレベルを記録
			int beforeLevel = p->getLv();
			// レベル30以上ならレベルアップは終了する
			while (p->getLv() < LV_MAX && players[i]->getExp() >= players[i]->getMaxExp()) {
				if (players[i]->getId() == p->getId()) {
					players[i]->levelUp();
				}
			}
			
			
			// レベルアップ後のレベルを記録
			int afterLevel = p->getLv();

			// 上がったレベルを記録
			level_up_diff[i] = afterLevel - beforeLevel;
		}

		// 経験値の再分配防止
		is_distributed = true;
	}


	// 所定地をクリックすると画面が切り替わる。
		// (CResult ->	CStage
		//				CBattle
		//				CTitle )
	if (CheckBoxClick(box_x, box_y, size_w, size_h) && result_frame <= 0) {

		auto& players = ScreenManager::Instance().getParty();

		se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));

		//hpのリセット
		for (auto& p : players) {
			p->hpReset();
		}
		// bgmの停止
		if (ScreenManager::Instance().getbattleResult() == BATTLE_RESULT::VICTORY) {
			bgm->stopBgm(CLoad::Instance().getBgmHandle(BGM_CLEAR));
		}
		else {
			bgm->stopBgm(CLoad::Instance().getBgmHandle(BGM_OVER));
		}

		// フェード起動
		fade->fadeStart(fade->FADE_CLAUSE);

	}

	// 画面切り替え
	if (fade->checkClause(WINDOW_W) && result_frame <= 0) {
		switch (choice_num)
		{
		case STAGE:

			ScreenManager::Instance().ChangeScreen<CStage>();

			break;

		case BATTLE:

			ScreenManager::Instance().ChangeScreen<CBattle>();

			break;

		case TITLE:

			ScreenManager::Instance().ChangeScreen<CTitle>();

			break;

		default:
			break;
		}


	}

}

void CResult::Render()
{

	// 勝敗判定表示
	if (ScreenManager::Instance().getbattleResult() == BATTLE_RESULT::VICTORY) {
		SetFontSize(60);
		DrawString(550, 10, "VICTORY!", GetColor(255, 255, 255));
	}
	else {
		SetFontSize(60);
		DrawString(600, 10, "DEFEAT", GetColor(255, 255, 255));
	}

	auto& players = ScreenManager::Instance().getParty();

	for (size_t i = 0; i < players.size(); i++) {
		auto& p = players[i];

		int posX = base_x + spacing_x * static_cast<int>(i);
		int posY = base_y;

		SetFontSize(30);
		// キャラクターの画像表示
		DrawRectGraph(posX, posY, scr_x, scr_y, grh_size_x, grh_size_y,
			CLoad::Instance().getPbodyGrh(p->getId()),false);
		// 枠の表示
		DrawExtendGraph(posX, posY, posX + grh_size_x, posY + grh_size_y, CLoad::Instance().getAmountGrh(BODY), true);
		// 名前表示
		int textPosY = posY + grh_size_y + 10;
		DrawString(posX, textPosY, p->getName().c_str(),GetColor(255,255,255));
		// レベル表示
		char levelStr[64];
		sprintf_s(levelStr, "Lv:%2d", p->getLv());
		DrawString(posX, textPosY + 40, levelStr, GetColor(255, 255, 255));
		// アップしたレベル表示
		if (is_distributed) {
			if (level_up_diff.size() > i) {
				if (level_up_diff[i] > 0) {
					char upStr[16];
					sprintf_s(upStr, "+%2d", level_up_diff[i]);
					DrawString(posX + 90, textPosY + 40, upStr, GetColor(0, 255, 0));
				}
			}
		}
	}


	// 選択中を示す三角形
	int trPosY = choice_pos_y + 60;
	DrawTriangle(choice_pos_x, trPosY,
		choice_pos_x - TRIANGEL_X, trPosY - TRIANGLE_Y, choice_pos_x - TRIANGEL_X, trPosY + TRIANGLE_Y,
		GetColor(255,0,0),true);

	// 次のスクリーン選択
	SetFontSize(TEXT_SIZE);
	DrawFormatString(choice_text_x[STAGE], choice_text_y[STAGE], GetColor(255, 255, 255), "ステージを選択");
	DrawFormatString(choice_text_x[BATTLE], choice_text_y[BATTLE], GetColor(255, 255, 255), "再挑戦");
	DrawFormatString(choice_text_x[TITLE], choice_text_y[TITLE], GetColor(255, 255, 255), "タイトルへ");

	{
		int x = 100;
		int y = 650;
		// ベースを表示
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(x, y, x + 300, y + 100, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// ミッション表示
		SetFontSize(FONT_MINSIZE);
		ScreenManager::Instance().DrawStageUI(ScreenManager::Instance().getStageScreen(), x + 30, y + 25);
	}

	// 決定ボタン
	ui->Button(box_x, box_y, box_x + size_w, box_y + size_h, CLoad::Instance().getButtonGrh(DECISION));

}

// 選択中の三角形座標
void CResult::ChoiceNectScreen()
{

	switch (choice_num)
	{

	case STAGE:

		choice_pos_x = 550;
		choice_pos_y = 600;

		break;

	case BATTLE:

		choice_pos_x = 550;
		choice_pos_y = 645;

		break;

	case TITLE:

		choice_pos_x = 550;
		choice_pos_y = 700;

		break;

	default:
		break;
	}
}

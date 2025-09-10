 #include "CResult.h"

void CResult::Update()
{
	// フェードの更新処理
	fade->fadeUpdate(WINDOW_W);

	// 選択中の三角形の座標
	ChoiceNectScreen();

	// クリックしたテキストに選択変更
	for (int i = 0; i < MAX; i++) {
		if (CheckBoxClick(choiceText_X[i], choiceText_Y[i], TEXT_SIZE * 7, TEXT_SIZE)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			choiceNum = i;
		}
	}

	// レベルアップ
	if (Manager::Instance().getbattleResult() == BattleResult::VICTORY && !isDistributed) {

		auto& party = Manager::Instance().getParty();
		auto& players = Manager::Instance().getPlayers();

		int totalExp = 0;

		// エネミーの経験値を取得
		totalExp = Manager::Instance().getExp();


		// 取得したエネミーの経験値をパーティメンバー分の割る。
				// 割った経験値を各キャラクターにキャスト
		int preMamber = (party.empty() ? 0 : totalExp / static_cast<int>(party.size()));

		// 初期化
		levelUpDiff.assign(party.size(), 0); 

		// 経験値100以上で1LvUp
		for (size_t i = 0; i < party.size(); i++) {
			auto& p = party[i];
			p->addExp(preMamber);

			// レベルが上がる前のレベルを記録
			int beforeLevel = p->getLv();
			// レベル30以上ならレベルアップは終了する
				while (p->getLv() < LV_MAX && p->getExp() >= p->getMaxExp()) {
					p->levelUp();
					if (players[i]->getId() == p->getId()) {
						players[i]->levelUp();
					}
				}
			
			// レベルアップ後のレベルを記録
			int afterLevel = p->getLv();

			// 上がったレベルを記録
			levelUpDiff[i] = afterLevel - beforeLevel;
		}

		// 経験値の再分配防止
		isDistributed = true;
	}


	// 所定地をクリックすると画面が切り替わる。
		// (CResult ->	CStage
		//				CBattle
		//				CTitle )
	if (CheckBoxClick(700, 700, TEXT_SIZE * 2, TEXT_SIZE)) {

		auto& players = Manager::Instance().getParty();

		se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));

		//hpのリセット
		for (auto& p : players) {
			p->hpReset();
		}
		// bgmの停止
		if (Manager::Instance().getbattleResult() == BattleResult::VICTORY) {
			bgm->stopBgm(CLoad::Instance().getBgmHandle(BGM_CLEAR));
		}
		else {
			bgm->stopBgm(CLoad::Instance().getBgmHandle(BGM_OVER));
		}
		// fadeを軌道
		fade->fadeStart(fade->FADE_CLAUSE);

	}

	if (fade->checkClause(WINDOW_W)) {

		switch (choiceNum)
		{
		case STAGE:

			Manager::Instance().ChangeScreen<CStage>();

			break;

		case BATTLE:

			Manager::Instance().ChangeScreen<CBattle>();

			break;

		case TITLE:

			Manager::Instance().ChangeScreen<CTitle>();

			break;

		default:
			break;
		}
	}

}

void CResult::Render()
{

	// 勝敗判定表示
	if (Manager::Instance().getbattleResult() == BattleResult::VICTORY) {
		SetFontSize(60);
		DrawString(550, 10, "VICTORY!", GetColor(255, 255, 255));
	}
	else {
		SetFontSize(60);
		DrawString(600, 10, "DEFEAT", GetColor(255, 255, 255));
	}

	auto& players = Manager::Instance().getParty();

	for (size_t i = 0; i < players.size(); i++) {
		auto& p = players[i];

		int posX = baseX + spacingX * static_cast<int>(i);
		int posY = baseY;

		SetFontSize(30);
		// キャラクターの画像表示
		DrawRectGraph(posX, posY, scrX, scrY, grhSizeX, grhSizeY,
			CLoad::Instance().getPbodyGrh(p->getId()),false);
		// 名前表示
		int textPosY = posY + grhSizeY + 10;
		DrawString(posX, textPosY, p->getName().c_str(),GetColor(255,255,255));
		// レベル表示
		char levelStr[64];
		sprintf_s(levelStr, "Lv:%2d", p->getLv());
		DrawString(posX, textPosY + 40, levelStr, GetColor(255, 255, 255));
		// アップしたレベル表示
		if (isDistributed) {
			if (levelUpDiff.size() > i) {
				if (levelUpDiff[i] > 0) {
					char upStr[16];
					sprintf_s(upStr, "+%2d", levelUpDiff[i]);
					DrawString(posX + 90, textPosY + 40, upStr, GetColor(0, 255, 0));
				}
			}
		}
	}


	// 選択中を示す三角形
	int trPosY = choicePosY + 60;
	DrawTriangle(choicePosX, trPosY,
		choicePosX - TRIANGEL_X, trPosY - TRIANGLE_Y, choicePosX - TRIANGEL_X, trPosY + TRIANGLE_Y,
		GetColor(255,0,0),true);

	// 次のスクリーン選択
	SetFontSize(TEXT_SIZE);
	DrawFormatString(choiceText_X[STAGE], choiceText_Y[STAGE], GetColor(255, 255, 255), "ステージを選択");
	DrawFormatString(choiceText_X[BATTLE], choiceText_Y[BATTLE], GetColor(255, 255, 255), "再挑戦");
	DrawFormatString(choiceText_X[TITLE], choiceText_Y[TITLE], GetColor(255, 255, 255), "タイトルへ");

	// 決定ボタン
	DrawFormatString(700, 700, GetColor(255, 255, 255), "決定");

}

// 選択中の三角形座標
void CResult::ChoiceNectScreen()
{

	switch (choiceNum)
	{

	case STAGE:

		choicePosX = 150;
		choicePosY = 600;

		break;

	case BATTLE:

		choicePosX = 150;
		choicePosY = 650;

		break;

	case TITLE:

		choicePosX = 150;
		choicePosY = 700;

		break;

	default:
		break;
	}
}

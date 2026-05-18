#include "CPartySelect.h"

// 更新処理
void CPartySelect::Update()
{

	// フェードアップデート
	fade->fadeUpdate();

	if (uiState == UI_PARTYSTATE::NORMAL && fade->checkOpen()) {
		
		// フレームの増加
		if (frame < 30) { frame++; }
		// 戦闘ボタンでスクリーンをバトル画面に切り替え。
		if (frame == 30 && CheckBoxClick(boxX, boxY, sizeW, sizeH)) {
			// スタートSE
			se->PlaySe(CLoad::Instance().getSeHandle(SE_START));
			// フェードアウトスタート関数
			fade->fadeStart(fade->FADE_CLAUSE);
			screen_change = true;
		}

		// 戻るボタンでスクリーンをステージ選択画面に切り替える
		if (frame == 30 && CheckBoxClick(20, 20, sizeW, sizeH)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			// フェードアウトスタート関数
			fade->fadeStart(fade->FADE_CLAUSE);
			screen_change = false;
		}

		// パーティー枠をクリックで変更モードに（全キャラクター一覧を展開）
		for (int i = 0; i < 4; i++) {
			if (CheckBoxClick(baseX + spacingX * i, baseY, grhSizeX, grhSizeY)) {
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
				selectedSlot = i;
				uiState = UI_PARTYSTATE::SELECTING;
				selectedIndex = -1;
			}
		}


	}
	else if (uiState == UI_PARTYSTATE::SELECTING) {

		// 全キャラクター一覧展開モード
		for (int i = 0; i < (int)allChara.size(); i++) {
			int posX = 100 + (i % 5) * 120;
			int posY = 200 + (i / 5) * 120;
			// 一覧の画像をクリックでパーティメンバーの差し替え候補に
			if (CheckBoxClick(posX, posY, 100, 100)) {
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
				selectedIndex = i;
			}
		}

		// 決定ボタンがクリックされたとき、差し替えを行う。
		if (CheckBoxClick(boxX, boxY, sizeW, sizeH) && selectedIndex != -1) {
			auto newChar = allChara[selectedIndex];

			// 差し替え候補がすでにパーティーにいるか確認。
			int foundIndex = -1;
			for (int i = 0; i < (int)party.size(); i++) {
				if (party[i]->getId() == newChar->getId()) {
					foundIndex = i;
					break;
				}
			}

			if (foundIndex != -1) {
				// すでにパーティにいる → 順番を入れ替える
				auto tmp = party[selectedSlot];
				party[selectedSlot] = party[foundIndex];
				party[foundIndex] = tmp;
			}
			else {
				// パーティにいない → 選択した枠に新しいキャラをセット
				party[selectedSlot] = newChar;
			}

			se->PlaySe(CLoad::Instance().getSeHandle(SE_CHARACHANGE));
			// 一覧を閉じる
			uiState = UI_PARTYSTATE::NORMAL;
			selectedIndex = -1;
			// フレームをリセット
			frame = 0;
		}

		// 一覧を閉じてパーティー確認画面に戻る。
		if (CheckBoxClick(20, 20, 200, 100)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			uiState = UI_PARTYSTATE::NORMAL;
			frame = 0;
		}

	}

	if (fade->checkClause()) {
		ScreenManager::Instance().setParty(party);
		bgm->stopBgm(CLoad::Instance().getBgmHandle(BGM_START));
		if (screen_change) {
			ScreenManager::Instance().ChangeScreen<CBattle>();
		}
		else {
			ScreenManager::Instance().ChangeScreen<CStage>();
		}
	}

}

// 描画処理
void CPartySelect::Render()
{
	DrawString(100, 110, "パーティー編成", GetColor(255, 255, 255), true);

	if (uiState == UI_PARTYSTATE::NORMAL) {
		// パーティー表示中
		for (int i = 0; i < (int)party.size(); i++) {

			int posX = baseX + spacingX * i;
			int posY = baseY;

			SetFontSize(FONT_SIZE);
			std::string name = (party[i] ? party[i]->getName() : "未設定");

			// 現在のパーティーメンバーのbody画像を表示。
			if (party[i]) {
				DrawRectGraph(posX, posY, scrX, scrY, grhSizeX, grhSizeY, CLoad::Instance().getPbodyGrh(party[i]->getId()), TRUE);
				// 枠の表示
				DrawExtendGraph(posX, posY, posX + grhSizeX, posY + grhSizeY, CLoad::Instance().getAmountGrh(BODY), true);
			}
			else {
				DrawBox(posX, posY, posX + grhSizeX, posY + grhSizeY, GetColor(100, 100, 100), TRUE);
				DrawString(posX + 20, posY + 50, "空き枠", GetColor(255, 255, 255));
				// 枠の表示
				DrawExtendGraph(posX, posY, posX + grhSizeX, posY * grhSizeY, CLoad::Instance().getAmountGrh(BODY), true);
			}
			// 名前表示
			int textPosY = posY + grhSizeY + 10;
			DrawString(posX, textPosY, name.c_str(), GetColor(255, 255, 255));
			// レベル表示
			char levelStr[64];
			sprintf_s(levelStr, "Lv:%2d", party[i]->getLv());
			DrawString(posX, textPosY + 40, levelStr, GetColor(255, 255, 255));
		}

		// バトル開始ボタン
		SetFontSize(FONT_BIGSIZE);
		ui->Button(boxX, boxY, boxX + sizeW, boxY + sizeH, CLoad::Instance().getButtonGrh(START));

	}
	else if (uiState == UI_PARTYSTATE::SELECTING) {

		// 全キャラ一覧表示展開モード
		for (int i = 0; i < (int)allChara.size(); i++) {
			// 全キャラ一覧のface画像を表示。（左半分に表示　横は5体まで）
			int posX = 100 + (i % 5) * 120;
			int posY = 200 + (i / 5) * 120;
			DrawExtendGraph(posX, posY,posX + 100, posY + 100, CLoad::Instance().getPfaceGrh(i), TRUE);
			DrawExtendGraph(posX - 10, posY - 10, posX + 110, posY + 110, CLoad::Instance().getAmountGrh(FACE), true);

			// 選択したキャラクターに枠を表示
			if (i == selectedIndex) {
				DrawBox(posX - 2, posY - 2, posX + 102, posY + 102, GetColor(255, 0, 0), FALSE);
			}

		}

		// 選択しているキャラクターのステータスを表示（右半分に表示）
			//表示するステータス（名前、レベル、攻撃力、防御力、素早さ、スキル名、スキル詳細（具体的に何をするスキルなのかその説明） 
		if (selectedIndex != -1) {
			SetFontSize(FONT_SIZE);
			auto& c = allChara[selectedIndex];
			int infoX = 800;
			int infoY = 200;
			DrawString(infoX, infoY, c->getName().c_str(), GetColor(255, 255, 255));

			char buf[128];
			sprintf_s(buf, "Lv:%d ATK:%d DEF:%d SPD:%d", c->getLv(), c->getAttack(), c->getDefense(), c->getSpeed());
			DrawString(infoX, infoY + 40, buf, GetColor(255, 255, 255));

			DrawString(infoX, infoY + 80, ("スキル: " + c->getSkillName()).c_str(), GetColor(255, 255, 255));
			std::string detail = ui->LineBreakText(c->getDetails(), 13);
			DrawString(infoX, infoY + 120, ("説明: " + detail).c_str(), GetColor(255, 255, 255));
		}

		// 変更決定ボタン
		SetFontSize(FONT_BIGSIZE);
		ui->Button(boxX, boxY, boxX + sizeW, boxY + sizeH, CLoad::Instance().getButtonGrh(DECISION));

	}

	// ベースを表示
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(textPosX - 30, textPosY- 20, textPosX + 280, textPosY + 100, GetColor(255, 255, 255), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// ミッション表示
	SetFontSize(FONT_MINSIZE);
	ScreenManager::Instance().DrawStageUI(ScreenManager::Instance().getStageScreen(), textPosX, textPosY);
	// 推奨LEVEL表示
	DrawFormatString(textPosX, textPosY+FONT_MINSIZE*5, GetColor(255, 255, 255), "推奨レベル:%2d", Suggest);

	// ステージ選択画面に戻るボタン
	SetFontSize(FONT_BIGSIZE);
	ui->Button(20, 20, 20 + sizeW, 20 + sizeH, CLoad::Instance().getButtonGrh(BACK));


	fade->fadeDraw();

}

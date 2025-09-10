#include "CPartySelect.h"

// 更新処理
void CPartySelect::Update()
{
	// フェードアップデート
	fade->fadeUpdate(WINDOW_W);

	if (uiState == PartyUIState::NORMAL) {
		
		// フレームの増加
		if (frame < 30) { frame++; }
		// 戦闘ボタンでスクリーンをバトル画面に切り替え。
		if (frame == 30 && CheckBoxClick(boxX, boxY, sizeW, sizeH)) {
			// フェードアウトスタート関数
			fade->fadeStart(fade->FADE_CLAUSE);
		}

		if (fade->checkClause(WINDOW_W)) {
			Manager::Instance().setParty(party);
			Manager::Instance().ChangeScreen<CBattle>();
			bgm->stopBgm(CLoad::Instance().getBgmHandle(BGM_START));
		}

		// 戻るボタンでスクリーンをステージ選択画面に切り替える
		if (frame == 30 && CheckBoxClick(20, 20, 200, 100)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			Manager::Instance().ChangeScreen<CStage>();
		}

		// パーティー枠をクリックで変更モードに（全キャラクター一覧を展開）
		for (int i = 0; i < 4; i++) {
			if (CheckBoxClick(baseX + spacingX * i, baseY, grhSizeX, grhSizeY)) {
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
				selectedSlot = i;
				uiState = PartyUIState::SELECTING;
				selectedIndex = -1;
			}
		}
	}
	else if (uiState == PartyUIState::SELECTING) {

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
			uiState = PartyUIState::NORMAL;
			selectedIndex = -1;
			// フレームをリセット
			frame = 0;
		}

		// 一覧を閉じてパーティー確認画面に戻る。
		if (CheckBoxClick(20, 20, 200, 100)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			uiState = PartyUIState::NORMAL;
			frame = 0;
		}

	}
}

// 描画処理
void CPartySelect::Render()
{
	if (uiState == PartyUIState::NORMAL) {
		// パーティー表示中
		for (int i = 0; i < 4; i++) {
			int posX = baseX + spacingX * i;
			int posY = baseY;

			SetFontSize(30);
			std::string name = (party[i] ? party[i]->getName() : "未設定");

			// 現在のパーティーメンバーのbody画像を表示。
			if (party[i]) {
				DrawRectGraph(posX, posY, scrX, scrY, grhSizeX, grhSizeY, CLoad::Instance().getPbodyGrh(party[i]->getId()), TRUE);
			}
			else {
				DrawBox(posX, posY, posX + grhSizeX, posY + grhSizeY, GetColor(100, 100, 100), TRUE);
				DrawString(posX + 20, posY + 50, "空き枠", GetColor(255, 255, 255));
			}
			// 名前表示
			int textPosY = posY + grhSizeY + 10;
			DrawString(posX, textPosY, name.c_str(), GetColor(255, 255, 255));
			// レベル表示
			char levelStr[64];
			sprintf_s(levelStr, "Lv:%2d", party[i]->getLv());
			DrawString(posX, textPosY + 40, levelStr, GetColor(255, 255, 255));
		}

		// 推奨LEVEL表示
		SetFontSize(30);
		DrawFormatString(textPosX, textPosY, GetColor(255, 255, 255), "推奨レベル:%2d", Suggest);

		// バトル開始ボタン
		SetFontSize(50);
		DrawFormatString(boxX, boxY, GetColor(255, 255, 255), "<バトル開始>");

	}
	else if (uiState == PartyUIState::SELECTING) {

		// 全キャラ一覧表示展開モード
		for (int i = 0; i < (int)allChara.size(); i++) {
			// 全キャラ一覧のface画像を表示。（左半分に表示　横は5体まで）
			int posX = 100 + (i % 5) * 120;
			int posY = 200 + (i / 5) * 120;
			DrawExtendGraph(posX, posY,posX + 100, posY + 100, CLoad::Instance().getPfaceGrh(i), TRUE);

			// 選択したキャラクターに枠を表示
			if (i == selectedIndex) {
				DrawBox(posX - 2, posY - 2, posX + 102, posY + 102, GetColor(255, 255, 0), FALSE);
			}

		}

		// 選択しているキャラクターのステータスを表示（右半分に表示）
			//表示するステータス（名前、レベル、攻撃力、防御力、素早さ、スキル名、スキル詳細（具体的に何をするスキルなのかその説明） 
		if (selectedIndex != -1) {
			SetFontSize(30);
			auto& c = allChara[selectedIndex];
			int infoX = 800;
			int infoY = 200;
			DrawString(infoX, infoY, c->getName().c_str(), GetColor(255, 255, 255));

			char buf[128];
			sprintf_s(buf, "Lv:%d\nATK:%d\nDEF:%d\nSPD:%d\nスキル:%s\n説明:%s", c->getLv(), c->getAttack(), c->getDefense(), c->getSpeed(),c->getSkillName().c_str(),c->getDetails().c_str());
			DrawString(infoX, infoY + 40, buf, GetColor(255, 255, 255));

			//DrawString(infoX, infoY + 80, ("スキル: " + c->getSkillName()).c_str(), GetColor(255, 255, 255));
			//DrawString(infoX, infoY + 120, ("説明: " + c->getDetails()).c_str(), GetColor(255, 255, 255));
		}

		// 推奨LEVEL表示
		SetFontSize(30);
		DrawFormatString(textPosX, textPosY, GetColor(255, 255, 255), "推奨レベル:%2d", Suggest);

		// 変更決定ボタン
		SetFontSize(50);
		DrawFormatString(boxX, boxY, GetColor(255, 255, 255), "<決定>");

	}

	// ステージ選択画面に戻るボタン
	SetFontSize(50);
	DrawFormatString(20, 10, GetColor(255, 255, 255), "<戻る>");

	fade->fadeCircleDraw(WINDOW_W / 2, WINDOW_H / 2);

}

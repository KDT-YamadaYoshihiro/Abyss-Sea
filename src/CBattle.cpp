#include "CBattle.h"
#include "CResult.h"
#include "DxLib.h"

void CBattle::Update()
{

	// フェードの更新処理
	fade->fadeUpdate(WINDOW_W);
	//retryFade->fadeUpdate(WINDOW_W);

	if (pauseState == PauseState::NONE && fade->checkOpen()) {

		for (auto& p : Manager::Instance().getParty()) {
			p->AnimUpdata();
		}

		// インデックスが0の時、ソートを行う。
		if (turnOrder.empty()) TurnOrder();

		if (targetInput == TargetInput::P_NONE) targetInput = TargetInput::ACTIONCHOICE;
		// 行動選択と実行
		if (currentTurnIndex < turnOrder.size()) {

			auto character = turnOrder[currentTurnIndex];

			// 移動する
			playerMove();
			EnemyMove();

			if (character->getAlive() && !BattleOver()) { // 行動中に戦闘が終了していないかチェック


				// プレイヤーとエネミーの型判定を安全にしたいためdynamic
				if (std::dynamic_pointer_cast<Player>(character)) {	// プレイヤー					

					// 入力待ち状態
					if (targetInput == TargetInput::ACTIONCHOICE) {
						// 実行前にリストを空にする。
						ListClea();
						ActionChoice(character);
					}
					// ターゲットリスト制作
					if (targetInput == TargetInput::LISTCREATE)TargetListCreate(character);
					//　ターゲット選択
					if (targetInput == TargetInput::TARGETCHOICE) TargetChoice(character);
					// 終了
					for (auto& p : Manager::Instance().getParty()) {
						if (p->getAnimChange()) {
							if (targetInput == TargetInput::END) PEnd(character);
						}
					}

				}
				else {	// エネミー
					// 敵が行動する場合のターゲットはプレイヤー
					// 自動でターゲットを決める
					if (actionMode == ActionMode::E_NONE) {
						ListClea();
						actionMode = ActionMode::TARGETRANDOM;
					}

					if (actionMode == ActionMode::TARGETRANDOM)EnemyAction(character);
					if (actionMode == ActionMode::END)EEnd(character);
						
					
				}
			}
			else {
				// 死亡していたら即ターン終了
				PEnd(character);
			}

		}
		else {
			// ターン終了関数
			TurnEnd();
		}

		// バトル進行関数
		// バトルの終了をチェック。
		if (BattleOver()) {
			// 音源の停止
			bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + stage));
			for (int i = 0; i < SE_MAX; i++) {
				se->stopSe(CLoad::Instance().getSeHandle(i));
			}
			// falseならfadeを軌道。
			fade->fadeStart(fade->FADE_CLAUSE);
		}

		// メニューバーが押されたとき、モードの切り替え
		if (CheckBoxClick(WINDOW_W - (FONT_BIGSIZE * 3 + 10), 20, FONT_BIGSIZE * 2, FONT_BIGSIZE)) {
			pauseState = PauseState::MENU;
		}
	}

	// MENU選択画面
	if (pauseState != PauseState::NONE) {

		switch (pauseState)
		{
		case CBattle::PauseState::MENU:

			// 選択画面の表示
			// バトル再開
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5), 250, FONT_BIGSIZE * 3, FONT_BIGSIZE)) {
				pauseState = PauseState::NONE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// 再挑戦
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5), 350, FONT_BIGSIZE * 3, FONT_BIGSIZE)){
				// モード変更
				pauseState = PauseState::RETRY;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// リタイア
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 2), 450, FONT_BIGSIZE * 4, FONT_BIGSIZE)) {
				// モード変更
				pauseState = PauseState::RETIRE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// （設定）
			if (CheckBoxClick(WINDOW_W / 2 - FONT_BIGSIZE, 550, FONT_BIGSIZE * 2, FONT_BIGSIZE)) {
				pauseState = PauseState::SETTING;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

			}


			break;
		case CBattle::PauseState::RETRY:

			// 再確認
			// はい
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 3), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), FONT_BIGSIZE * 2, FONT_BIGSIZE)) {
				// fadeを軌道
				fade->fadeStart(fade->FADE_CLAUSE);
				// 再挑戦
				BattleInit();
				// バトル画面に切り替える
				pauseState = PauseState::NONE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));

			}
			// いいえ
			if (CheckBoxClick(WINDOW_W / 2 + FONT_BIGSIZE, WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), FONT_BIGSIZE * 3, FONT_BIGSIZE)) {
				// MENUの選択画面に戻る
				pauseState = PauseState::MENU;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			}


			break;
		case CBattle::PauseState::RETIRE:

			// 再確認
			// はい
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 3), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), FONT_BIGSIZE * 2, FONT_BIGSIZE)) {
				
				// Party全員を死亡判定に
				for (auto& p : Manager::Instance().getParty()) {
					p->setAlive(false);
				}
				// fadeを軌道
				fade->fadeStart(fade->FADE_CLAUSE);
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
				// bgmの停止
				bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + Manager::Instance().getStageScreen()));
			}
			// いいえ
			if (CheckBoxClick(WINDOW_W / 2 + FONT_BIGSIZE, WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), FONT_BIGSIZE * 3, FONT_BIGSIZE)) {
				// MENUの選択画面に戻る
				pauseState = PauseState::MENU;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));

			}

			break;

		case CBattle::PauseState::SETTING:

			if (clickFrame > 0) {
				clickFrame--;
			}

			// 音量変更
			for (int i = 0; i < BUTTAN_MAX; i++) {

				int size;
				if (i % 2 == 0) {size = MAX_VOL / 10;}
				else {size = -MAX_VOL / 10;}

				if (clickFrame == 0 && CheckCircleClick(trPosX[i], trPosY[i] + size, 50)) {

					int bgmVol = Manager::Instance().getBgmVolume();
					int seVol = Manager::Instance().getSeVolume();

					clickFrame = 10;
					se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

					switch (i)
					{
					case BGM_UP:

						Manager::Instance().setBgmVolume(bgmVol + 1);

						break;

					case BGM_DOWN:
						Manager::Instance().setBgmVolume(bgmVol - 1);

						break;

					case SE_UP:
						Manager::Instance().setSeVolume(seVol + 1);

						break;

					case SE_DOWN:
						Manager::Instance().setSeVolume(seVol - 1);

						break;

					default:
						break;
					}

				}

			}

			// 決定ボタンを押した際
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H - 200, FONTSIZE * 3, FONTSIZE)) {
				// MENUの選択画面に戻る
				pauseState = PauseState::MENU;
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}

			break;
		default:
			break;
		}
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
	// 何ターン目か表示
	SetFontSize(FONT_SIZE);
	DrawFormatString(FONT_SIZE, 10, GetColor(255, 255, 255), "%2dターン目", this->turn);
	// プレイヤーとエネミーの1ターン分の行動順を表示
	DrawTurnOrderIcons(30, 50);

	// 指定したIDのエネミーを描画
	for (auto& e : enemies) {
		// クリック時に使用する変数セット
		e->setPosX(ePosX);
		e->setPosY(ePosY);
		//	本体
		e->Render(ePosX, ePosY, CLoad::Instance().getEnemyGrh(stage));
		e->powerRender();
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
	
	// プレイヤーのエフェクトアニメーション
	for (auto& p : Manager::Instance().getParty()) {
		p->animDraw();
		p->powerRender();
	}

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


	if (targetInput == TargetInput::ACTIONCHOICE) {
		// ボタンの表示
		ui->Button(ButtonX, atButtonY, ButtonX + ButtonSizeX, atButtonY + ButtonSizeY, CLoad::Instance().getAtkButtonGrh());
		ui->Button(ButtonX, skButtonY, ButtonX + ButtonSizeX, skButtonY + ButtonSizeY, CLoad::Instance().getSklButtonGrh());

		// スキルポイントの表示
		ui->SkillPoint(100, 300, SP_RADIUS, sp->getSP(), sp->getMaxSP());
	}
	// メニューバーの表示
	SetFontSize(FONT_BIGSIZE);
	if (pauseState == PauseState::NONE) {
		DrawFormatString(WINDOW_W - (FONT_BIGSIZE * 3 + 10), 20, GetColor(255, 255, 255), "<MENU>");
	}

	//　MENUの中身
	if (pauseState != PauseState::NONE) {

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, WINDOW_W, WINDOW_H, GetColor(50,50,50), true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		switch (pauseState)
		{
		case CBattle::PauseState::MENU:

			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5) , 250, GetColor(255, 255, 255), "続ける");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5), 350, GetColor(255, 255, 255), "再挑戦");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 2), 450, GetColor(255, 255, 255), "リタイア");
			DrawFormatString(WINDOW_W / 2 - FONT_BIGSIZE, 550, GetColor(255, 255, 255), "設定");

			break;
		case CBattle::PauseState::RETRY:

			// 文字
			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE *4),		WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "再挑戦しますか？");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 3),	WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "はい");
			DrawFormatString(WINDOW_W / 2 + FONT_BIGSIZE,			WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "いいえ");

			break;
		case CBattle::PauseState::RETIRE:

			// 文字
			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 4.5), WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "リタイアしますか？");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 3), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "はい");
			DrawFormatString(WINDOW_W / 2 + FONT_BIGSIZE, WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "いいえ");

			break;

		case CBattle::PauseState::SETTING:

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

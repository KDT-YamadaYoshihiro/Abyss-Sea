#include "CBattle.h"
#include "Scene/Result/CResult.h"
#include "DxLib.h"

void CBattle::Update()
{
	std::vector<std::shared_ptr<Character>> select_party;
	for (auto& p : ScreenManager::Instance().getParty()) {
		select_party.push_back(p);
	}
	// パーティーミッションのクリア状態を確認
	MissionManager::Instance().IncludeCharacter(
		ScreenManager::Instance().getStageScreen(), select_party);

	// フェードの更新処理
	fade->fadeUpdate();

	// エネミーのカットイン更新
	for (auto& e : enemies) {
		e->CutinUpdate();
	}

	for (auto& p : ScreenManager::Instance().getParty()) {
		// プレイヤーのアニメーション更新
		p->AnimUpdata();
		// プレイヤーのカットイン更新
		p->CutinUpdate();

		if (!p->getAlive()) {
			p->setAnimType(GRTYPE::DEAD);
		}

	}

	// 深海っぽくするために、青のボックス描画
	blend_num += blend_speed;
	// 範囲制限
	if (blend_num <= 0) { blend_speed *= -1; }
	if (blend_num >= 50) { blend_speed *= -1; }

	// MENU展開時の状況結果の受け取り変数
	BattleMenuResult result; 
	switch (state)
	{
	case CBattle::State::BATTLE:		// バトル

		if (!fade->checkOpen()) {
			return;
		}

		BattleMain();

		// メニューバーが押されたとき、モードの切り替え
		if (CheckBoxClick(WINDOW_W - 250, 10, 250, 150)) {
			state = State::MENU;
		}

		break;

	case CBattle::State::MENU:			// MENU展開時

		result = battleMenu->Update();

		if (result == BattleMenuResult::CLOSE) {
			state = State::BATTLE; // バトル再開
		}
		else if (result == BattleMenuResult::RETRY) {
			// 敵を削除
			Delete();
			// 再初期化
			BattleInit();
			// バトル画面に切り替える
			state = State::BATTLE;

		}
		else if (result == BattleMenuResult::RETIRE) {
			// Party全員を死亡判定に
			for (auto& p : ScreenManager::Instance().getParty()) {
				p->setAlive(false);
			}
			// fadeを起動
			fade->fadeStart(fade->FADE_CLAUSE);
		}

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
	if (fade->checkClause() && BattleOver()) {
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
		ui->Button(box_x, box_y, box_x + BUTTAN_WIDTH, box_y + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(BACK));

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

	// カットインの描画
	for (auto& e : enemies) {
		e->CutinDraw();
	}
	for (auto& p : ScreenManager::Instance().getParty()) {
		p->CutinDraw();
	}

	// メニュー展開時の描画
	if (state == State::MENU) {
		battleMenu->Draw(); 
	}

}

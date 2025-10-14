#pragma once
#include "Manager.h"
#include "CBase.h"
#include "CBattle.h"
#include "CharacterFactory.h"

class CPartySelect : public ScreenBase {

	//　ui
	std::shared_ptr<UI> ui;
	// パーティーメンバー（４人固定）
	std::vector<std::shared_ptr<Player>> party;

	// プレイヤー全キャラ一覧
	std::vector<std::shared_ptr<Player>> allChara;

	// 現在の表示モード
	PartyUIState uiState = PartyUIState::NORMAL;

	// 音
	std::shared_ptr<Sound>bgm;
	std::shared_ptr<Sound> se;

	// フェード
	std::shared_ptr<Fade> fade;

	// パーティ枠（４枠）
	int selectedSlot = -1;

	// パーティメンバー入れ替え候補
	int selectedIndex = -1;

	// 決定ボタンの座標
	int boxX = -1;
	int boxY = -1;	

	// 決定ボタンのボックス枠サイズ
	int sizeW = -1;
	int sizeH = -1;

	int frame = -1;

	//　画像表示用変数
	int baseX = -1;
	int baseY = -1;
	int scrX = -1;
	int scrY = -1;
	int grhSizeX = -1;
	int grhSizeY = -1;
	int spacingX = -1;

	// 推奨LEVEL
	int Suggest = -1;

	// 推奨LEVEL表示用
	int textPosX = -1;
	int textPosY = -1;

	// 画面切り替え
	bool screen_change;

public:

	// 初期化
	CPartySelect():
		// UI生成
		ui(std::make_shared<UI>()),
		// 音源
		bgm(Manager::Instance().getbgm()), 
		se(Manager::Instance().getSe()),
		// フェード
		fade(Manager::Instance().getFade()),
		// パーティメンバー
		party(Manager::Instance().getParty()),
		// 全キャラクター
		allChara(Manager::Instance().getPlayers()),
		// バトルスタート
		uiState(PartyUIState::NORMAL),
		selectedSlot(-1),
		selectedIndex(-1),
		// 座標とサイズ
		boxX(1000),
		boxY(700),
		sizeW(200),
		sizeH(80),
		frame(0)
	{


		// パーティーが四人いるか確認
		if (party.size() < 4) {
			party.resize(4, nullptr);
		}

		// 画像表示用の変数
		baseX = 100;
		baseY = 180;
		scrX = 60;
		scrY = 0;
		grhSizeX = 250;
		grhSizeY = 418;
		spacingX = grhSizeX + 40;

		Suggest = Manager::Instance().getSuggest();
		textPosX = WINDOW_W - 300;
		textPosY = 50;

		screen_change = true;
	}

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

};
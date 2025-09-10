#include "Manager.h"
#include "CTitle.h"

void Manager::Init()
{
	//// プレイヤーの生成
	//for (int member = 0; member < PLAYER_MAX; member++) {
	//	players.push_back(std::dynamic_pointer_cast<Player>(CharacterFactory::CreatePlayer(member, member)));
	//}
	//for (int member = 0; member < 4; member++) {
	//	party.push_back(std::dynamic_pointer_cast<Player>(CharacterFactory::CreatePlayer(member, member)));
	//}

	// キャラクターデータ読み込み
	if (!characterFactory.LoadPlayerData("data/CharacterData.csv")) {
		std::cerr << "プレイヤーデータ読み込み失敗" << std::endl;
	}

	if (!characterFactory.LoadSkillData("Data/SkillData.csv")) {
		std::cerr << "スキルデータ読み込み失敗" << std::endl;
	}

	// 既存のプレイヤーをクリア
	players.clear();

	// CSVからプレイヤーを生成してplayersに登録
	for (int i = 0; i < (int)characterFactory.getPlayerTable().size(); i++) {
		auto player = characterFactory.CreatePlayer(i);
		players.push_back(std::dynamic_pointer_cast<Player>(player));
	}

	// 初期パーティを設定
	party.clear();
	if (!players.empty()) {
		for (int i = 0; i < (int)players.size() && i < 4; i++) {
			party.push_back(players[i]);
		}
	}

	// フェードやBGM等の初期化
	fade = std::make_shared<Fade>();

	bgm = std::make_shared<Sound>();
	se = std::make_shared<Sound>();

	ChangeScreen<CTitle>();

	mouseX = -1;
	mouseY = -1;

	bgmVolume = 7;
	seVolume = 7;

}

void Manager::Update(){
	
	// スクリーンポインター更新処理
	scrPtr->Update();

	// 音量設定
	bgmVolume = max(0, min(bgmVolume, 10));
	seVolume = max(0, min(seVolume, 10));


	for (int i = 0; i < BGM_MAX; i++) {
		ChangeVolumeSoundMem(bgmVolume * 22.5, CLoad::Instance().getBgmHandle(i));
	}
	for (int i = 0; i < SE_MAX; i++) {
		ChangeVolumeSoundMem(seVolume * 22.5, CLoad::Instance().getSeHandle(i));
	}
}

void Manager::Render()
{
	// スクリーンポインター描画処理
	scrPtr->Render();

	// mouse座標の取得
	GetMousePoint(&mouseX, &mouseY);
	// カーソル描画
	if (!window_mode) {
		DrawExtendGraph(mouseX, mouseY, mouseX + 30, mouseY + 30, CLoad::Instance().getCursorGrh(), true);
	}

	// フェード
	fade->fadeCircleDraw(WINDOW_W / 2, WINDOW_H / 2);
}

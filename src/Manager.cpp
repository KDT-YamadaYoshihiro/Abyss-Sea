#include "Manager.h"
#include "CTitle.h"

void Manager::Init()
{
	
	// テーブル読み込み
	// キャラクターのデータ、スキルデータの読み込み
	CharacterFactory::Instance().LoadCharacterTable("data/Table/CharaTable.csv");
	CharacterFactory::Instance().LoadSkillTable("data/Table/SkillTable.csv");
	// ミッションデータ読み込み
	missionManager.Init("data/Table/MissionTable.csv");

	 // 全プレイヤー生成
	for (int i = 0; i < PLAYER_MAX; i++) {
		auto player =  std::dynamic_pointer_cast<Player>(
			CharacterFactory::Instance().CreateCharacter(i));
		players.push_back(player);
	}

	// 初期パーティ（先頭4人）
	for (int i = 0; i < 4; i++) {
		party.push_back(players[i]);
	}

	bgm = std::make_shared<Sound>();
	se = std::make_shared<Sound>();

	fade = std::make_shared<Fade>();

	ChangeScreen<CTitle>();

	bgmVolume = 7;
	seVolume = 7;

}

void Manager::Update(){
	// スクリーンポインター更新処理
	scrPtr->Update();

	// ボリューム設定
	bgmVolume = max(0, min(bgmVolume, 10));
	seVolume = max(0, min(seVolume, 10));

	// ボリュームの変更更新
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

	// フェード
	fade->fadeCircleDraw(WINDOW_W / 2, WINDOW_H / 2);
}

void Manager::DrawStageUI(int arg_stageID, int arg_x, int arg_y) {
	missionManager.DrawMissions(arg_stageID, arg_x, arg_y);
}
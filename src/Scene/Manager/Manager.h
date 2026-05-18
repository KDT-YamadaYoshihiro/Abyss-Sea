#pragma once
#include "DxLib.h"
#include "Scene/CBase.h"
#include "Character/Player/CPlayer.h"
#include "Character/Character.h"
#include "Character/Factory/CharacterFactory.h"
#include "Load/Load.h"
#include "Mission/Manager/MissionManager.h"

class ScreenManager {

	// 
	std::vector<std::shared_ptr<Player>> players;
	std::vector<std::shared_ptr<Player>> party;

	// 音源
	std::shared_ptr<Sound> bgm;
	std::shared_ptr<Sound> se;

	// フェード
	std::shared_ptr<FadeAlpha> fade;

	// 
	int stage_screen = -1;

	BATTLE_RESULT result;
	// 経験値
	int exp = -1;

	// ウィンドウモード
	bool window_mode = true;

	// 音量設定
	int bgmVolume = -1;
	int seVolume = -1;

	// 推奨LEVEL
	int suggest = -1;


    // シングルトン
    // 初期化
	ScreenManager() = default;

    virtual ~ScreenManager() {};

public:

	ScreenManager(const ScreenManager&) = delete;
	ScreenManager& operator = (const ScreenManager&) = delete;
	static ScreenManager& Instance() {
		static ScreenManager instance;
		return instance;
	}

	std::shared_ptr<ScreenBase> scrPtr = nullptr;

	std::shared_ptr<CLoad> load = nullptr;


	// 初期化
	void Init();

	// 更新処理
	void Update();

	// 描画処理
	void Render();

	// 画面切り替えメソッド
	template<typename T>
	void ChangeScreen();

	// ミッションの描画
	void DrawStageUI(int arg_stageID, int arg_x, int arg_y);

	// ステージに応じたエネミーIDを返す
	int getEnemyID(int  arg_stage) {

		int num;

		switch (arg_stage)
		{
		case STAGE1:

			num = 103;

			break;

		case STAGE2:

			num = 100;

			break;

		case STAGE3:

			num = 101;

			break;

		case STAGE4:

			num = 102;

			break;

		default:
			break;
		}

		return num;
	}

	// ステージをセット
	void setStageScreen(int arg_stage) {
		stage_screen = arg_stage;
	}
	// ステージを番号を取得
	int getStageScreen() const { return stage_screen; }

	// バトル勝敗のセット、ゲット関数
	void setBattleResult(BATTLE_RESULT  arg_result) {
		result = arg_result;
	}
	BATTLE_RESULT getbattleResult() const { return result; }

	//　全プレイヤーのゲット関数
	std::vector<std::shared_ptr<Player>>& getPlayers() { return players; }
	// パーティメンバーのセットゲット関数
	void setParty(std::vector<std::shared_ptr<Player>> arg_paty) {
		party = arg_paty;
	}
	std::vector<std::shared_ptr<Player>>& getParty() { return party; }

	// bgm,seのゲット関数
	std::shared_ptr<Sound> getbgm() const { return bgm; }
	std::shared_ptr<Sound> getSe() const { return se; }

	// UI
	std::shared_ptr<FadeAlpha> getFade() const { return fade; }

	// 音量設定
	void setBgmVolume(int arg_volume) { bgmVolume = arg_volume; }
	int getBgmVolume() const { return bgmVolume; }

	void setSeVolume(int arg_volume) { seVolume = arg_volume; }
	int getSeVolume() const { return seVolume; }


	void setExp(int arg_exp) {
		exp = arg_exp;
	}

	int getExp() const { return exp; }

	bool getWindowMode() const { return window_mode; }

	// 推奨LEVEL
	void setSuggest(int arg_suggest) { suggest = arg_suggest; }
	int getSuggest() const { return suggest; }
};

// 
template<typename T>
void ScreenManager::ChangeScreen()
{
			// 関数名・クラス<型>();
	scrPtr = std::make_shared<T>();
}

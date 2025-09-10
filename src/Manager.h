#pragma once
#include "DxLib.h"
#include "InPutKey.h"
#include "CBase.h"
#include "CPlayer.h"
#include "Character.h"
#include "CharacterFactory.h"
#include "CLoad.h"

enum class BattleResult {
	BATTLE,
	VICTORY,
	DEFEAT
};

class Manager {

	CharacterFactory characterFactory;

	// 
	std::vector<std::shared_ptr<Player>> players;
	std::vector<std::shared_ptr<Player>> party;


	// 音源
	std::shared_ptr<Sound> bgm;
	std::shared_ptr<Sound> se;

	// フェード
	std::shared_ptr<Fade> fade;

	// 
	int stageScreen = -1;

	BattleResult result;
	// 経験値
	int Exp = -1;

	// mouse座標
	bool window_mode = true;
	int mouseX = -1;
	int mouseY = -1;

	// 音量設定
	int bgmVolume;
	int seVolume;

	// 推奨LEVEL
	int Suggest;


    // シングルトン
    // 初期化
	Manager() = default;

    virtual ~Manager() {};

public:

	Manager(const Manager&) = default;
	Manager& operator = (const Manager&) = default;
	static Manager& Instance() {
		static Manager instance;
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

	// ステージをセット
	void setStageScreen(int arg_stage) {
		stageScreen = arg_stage;
	}
	// ステージを番号を取得
	int getStageScreen() const { return stageScreen; }

	// バトル勝敗のセット、ゲット関数
	void setBattleResult(BattleResult  arg_result) {
		result = arg_result;
	}
	BattleResult getbattleResult() const { return result; }

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
	std::shared_ptr<Fade> getFade() const { return fade; }

	// 音量設定
	void setBgmVolume(int arg_volume) { bgmVolume = arg_volume; }
	int getBgmVolume() const { return bgmVolume; }

	void setSeVolume(int arg_volume) { seVolume = arg_volume; }
	int getSeVolume() const { return seVolume; }


	void setExp(int arg_exp) {
		Exp = arg_exp;
	}

	int getExp() const { return Exp; }

	bool getWindowMode() const { return window_mode; }

	// 推奨LEVEL
	void setSuggest(int arg_suggest) { Suggest = arg_suggest; }
	int getSuggest() const { return Suggest; }
};

// 
template<typename T>
void Manager::ChangeScreen()
{
			// 関数名・クラス<型>();
	scrPtr = std::make_shared<T>();
}

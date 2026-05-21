#pragma once
#include "DisplaySystem/UI/UI.h"
#include "Scene/StageSelect/CStage.h"
#include "Character/Factory/CharacterFactory.h"
#include "Character/Character.h"
#include "Character/Enemy/Enemy.h"
#include "Math/Calculation.h"
#include "Constants/define.h"
#include "Battle/BattleMenu/BattleMenu.h"

// バトルスクリーンクラス
class CBattle : public ScreenBase {


	// バトルモード	 バトル、MENU、終了
	enum class State{BATTLE,MENU,DEAD_EFFECT,END};
	State state = State::BATTLE;

	// バトルメニュークラスの生成
	std::unique_ptr<BattleMenu> battleMenu;


	// フェード
	std::shared_ptr<FadeAlpha> fade;

	std::shared_ptr<Effect> effect;

	std::shared_ptr<Calculation> cal;



	//// 戻るボタンの座標、サイズ変数
	//int box_x = -1;
	//int box_y = -1;

	// スキル説明の表示有無
	bool sk_desc_draw = false;

public:

	// 初期化
	CBattle() :
		sk_desc_draw(false)
	{		
		// 初期化関数
		BattleInit();
	};

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:


	// バトル初期化（再挑戦時使用)
	void BattleInit() {

		// バトルメニュークラス生成
		battleMenu = std::make_unique<BattleMenu>();
	}
	

	//// バトル終了確認関数
	//bool BattleOver() {
	//	// プレイヤーが生きているか
	//	bool allPlayersDefeated = true;
	//	for (auto& p : ScreenManager::Instance().getParty()) {
	//		if (p->getAlive()) {
	//			allPlayersDefeated = false;
	//			break;
	//		}
	//		else {
	//			// 敗北
	//			ScreenManager::Instance().setBattleResult(BATTLE_RESULT::DEFEAT);
	//			// ミッションリセット
	//			MissionManager::Instance().ResetMissions(stage);
	//		}
	//	}

	//	// エネミーが生きているか
	//	bool EnemiesDefeated = true;
	//	for (const auto& e : enemies) {
	//		if (e->getAlive()) {
	//			EnemiesDefeated = false;
	//			break;
	//		}
	//		else {
	//			// 勝利
	//			ScreenManager::Instance().setBattleResult(BATTLE_RESULT::VICTORY);
	//			// 経験値セット
	//			ScreenManager::Instance().setExp(e->getExp());
	//			// ミッション確認
	//			MissionManager::Instance().ClearQuest(stage);

	//		}
	//	}

	//	// 双方falseならバトルは続行
	//	// どちらかがtrueならバトル終了
	//	return allPlayersDefeated || EnemiesDefeated;
	//}


	//// プレイヤーのバトル終了時エフェクト
	//void PlayEndEffect() {

	//	end_frame--;

	//	for (auto& e : enemies) {

	//		if (!e->getAlive()) {

	//			for (auto& p : ScreenManager::Instance().getParty()) {
	//				p->setAnimType(GRTYPE::WIN);
	//			}
	//		}
	//		else {

	//			for (auto& p : ScreenManager::Instance().getParty()) {
	//				p->setAnimType(GRTYPE::LOSE);
	//			}
	//		}
	//	}

	//	if (end_frame <= 0) {
	//		state = State::END;
	//	}
	//}

	//// 終了
	//void BattleEnd() {

	//	// falseならfadeを軌道。
	//	fade->fadeStart(fade->FADE_CLAUSE);
	//	// 音源の停止
	//	bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + stage));
	//	for (int i = 0; i < SE_MAX; i++) {
	//		se->stopSe(CLoad::Instance().getSeHandle(i));
	//	}

	//}

};
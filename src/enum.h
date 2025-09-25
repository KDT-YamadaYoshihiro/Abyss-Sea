#pragma once

// バトル結果
enum class BattleResult {
	BATTLE,
	VICTORY,
	DEFEAT
};


// ステージ
enum STAGENUM {
	STAGE1,
	STAGE2,
	STAGE3,

	MAX
};

// プレイヤー
enum PLAYER {
	// プレイヤー
	AKIRA,
	EL,
	ALIA,
	PINOT,
	KAGE,

	PLAYER_MAX			// プレイヤー最大数
};

//	エネミー
enum ENEMY {
	// エネミー
	MERFOLK,
	DEEPONES,
	KTANID,

	ENEMY_MAX			//　最大数(for文用)
};


// BGM・SE
enum BGM {
	BGM_START,
	BGM_STAGE1,
	BGM_STAGE2,
	BGM_STAGE3,
	BGM_CLEAR,
	BGM_OVER,

	BGM_MAX				// 最大数(for文用)
};

// SE
enum SE {
	SE_START,
	SE_DECISION,
	SE_CANCEL,
	SE_CHARACHANGE,
	SE_CLICK,
	SE_ATTACK,
	SE_SKILL1,
	SE_SKILL2,
	SE_BUFF,
	SE_HEAL,


	SE_MAX				// 最大数(for文用)
};

//	バトルの状態
enum GRTYPE {
	WAIT,		// 待機中
	ATTACK,		// 通常攻撃（突き）
	MAGIC,		// 魔法スキル(凡用スキル)
	SKILL,		// 攻撃スキル(振り)
	DAMAGE,		// 攻撃を受けたとき
	WIN,		// バトル勝利
	LOSE,		// 戦闘不能
	ANIM_MAX
};

// プレイヤーアイコン枠
enum AMOUNT {
	BODY,
	FACE,
	AMOUNT_MAX
};

// パーティ選択画面のUI状態
enum class PartyUIState {
	NORMAL,     // パーティ表示のみ
	SELECTING   // キャラ選択中（一覧展開）
};

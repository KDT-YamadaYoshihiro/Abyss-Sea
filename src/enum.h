#pragma once
enum PLAYER {
	// プレイヤー
	AKIRA,
	EL,
	ALIA,
	PINOT,
	KAGE,

	PLAYER_MAX			// プレイヤー最大数
};

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

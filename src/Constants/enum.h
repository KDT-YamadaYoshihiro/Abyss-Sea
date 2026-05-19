#pragma once

// ウィンドウサイズ
#define WINDOW_W		1298		
#define WINDOW_H		 808		

// フォントサイズ
#define FONTSIZE		 100

// CLICK TO START
// 文字列の上下揺れの範囲
#define TOP_RANGE		 615
#define DWON_RANGE		 585



// バトル結果
enum class BATTLE_RESULT {
	BATTLE,
	VICTORY,
	DEFEAT
};


// ステージ
enum STAGENUM {
	STAGE1,
	STAGE2,
	STAGE3,
	STAGE4,

	STAGE_MAX
};

// スキルタイプ			攻撃,  回復, 攻撃力バフ,　速度バフ, 防御力バフ, 無し
enum class SKILL_TYPE { ATTACK, HEAL, ATK_BUFF, AGR_BUFF, DEF_BUFF, NONE };
// スキルのターゲット方向	単エネミー, 単味方,自分,全エネミー,全味方
enum class SKILL_TARGET_TYPE { SINGLE_ENEMY, SINGLE_ALLY, SELF, ALL_ENEMY, ALL_ALLY };

// プレイヤー
enum PLAYER {
	// プレイヤー
	AKIRA,
	EL,
	ALIA,
	PINOT,
	KAGE,
	RAIZEL,
	ELINA,
	ALDEUS,
	ARDINA,

	PLAYER_MAX			// プレイヤー最大数
};

// エフェクト種別
enum class EFFECT_TYPE {
	EF_NONE,			//　無し
	EF_ATTACK,			// 通常攻撃
	EF_SKILL1,			// skill1(アキラ)
	EF_SKILL2,			// skill2(エル)
	EF_ALL_SKILL1,		// skill全体1(アルデウス)
	EF_ALL_SKILL2,		// skill全体2(アルディナ)
	EF_HEAL,			// 回復（アリア,エリナ)
	EF_BUFF,			// バフ(ピノット、カゲ、ライゼル,)
	EF_DEBUFF,			// デバフ()

	EF_MAX
};

// プレイヤーアクション
enum PLAYER_ACTION {
	NONE_ACTION,
	CHOICE,
	CUTIN,
	ACTHON,
};


//	エネミー
enum ENEMY {
	// エネミー
	ZPTHOMMOG,
	YTHOGTHA,
	CTHULHU,
	GHOUL,

	ENEMY_MAX			//　最大数(for文用)
};

// エネミーの行動
enum class ENEMY_ACTION {
	TURN_START,
	ATTACK,
	TURN_END
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
	SE_CUTIN1,
	SE_CUTIN2,

	SE_MAX				// 最大数(for文用)
};

// プレイヤーのアニメーションタイプ
enum GRTYPE {
	WAIT,		// 待機中
	ATTACK,		// 通常攻撃（突き）
	MAGIC,		// 魔法スキル(凡用スキル)
	SKILL,		// 攻撃スキル(振り)
	DAMAGE,		// 攻撃を受けたとき
	DEAD,		// 戦闘不能
	WIN,		// バトル勝利
	LOSE,		// バトル敗北
	ANIM_MAX
};

// プレイヤーアイコン枠
enum AMOUNT {
	BODY,
	FACE,
	AMOUNT_MAX
};

// パーティ選択画面のUI状態
enum class UI_PARTYSTATE {
	NORMAL,     // パーティ表示のみ
	SELECTING   // キャラ選択中（一覧展開）
};

// ボタンUI
enum UI_BUTTON {
	CHALLEMGE,
	BACK,
	YES,
	NO,
	CONTINUE,
	GIVEUP,
	SETTING,
	START,
	DECISION,
	RETRY,

	BUTTON_MAX
};


// アイコンUI
enum BUFF_TYPE {
	BUFF_ATK,
	BUFF_DEF,
	BUFF_AGR,

	BUFF_MAX
};

// カットインのフェーズ
enum class CUTIN_PHASE {
	ENTERING,   // 勢いよく入ってくる
	PAUSING,    // 中央でゆっくり進む
	EXITING     // 加速して退出
};

// バトル側に返す結果
enum class BattleMenuResult {
	KEEP_OPEN,	// メニュー継続
	CLOSE,		// メニューを閉じてバトルへ
	RETRY,		// 再挑戦の選択
	RETIRE,		// リタイアの選択
};

enum class MenuState
{
	MAIN,		// メニューの選択中
	RETRY,		// 再挑戦の確認画面
	RETIRE,		// リタイアの確認画面
	SETTING		// 音量などの設定
};

// サウンド系
enum SOUND_BUTTON {
	BGM_UP,
	BGM_DOWN,
	SE_UP,
	SE_DOWN,

	BUTTAN_MAX
};

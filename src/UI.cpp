#include "UI.h"

// テーブルの定義
const UI::AnimInfo UI::animTable[ANIM_MAX] = {
	{0,0,3},	// WAIT
	{3,0,3},	// ATTACK
	{3,3,3},	// MAGIC
	{3,1,3},	// SKILL
	{0,4,3},	// DAMAGE
	{6,1,3},	// WIN
	{6,5,3},	// LOSE
};


void UI::PlayerAnimStart(GRTYPE& arg_type)
{
	// フレームをリセット
	frame = 0;

	const AnimInfo& anim = animTable[arg_type];

	scrX = SIZE_W * anim.startCol;
	startScrX = SIZE_W * anim.startCol;

	scrY = SIZE_H * anim.row;
	startScrY = SIZE_H * anim.row;

	// モーションごとのコマ数を設定
	totalFrames = anim.frames; // 例：全モーション3コマ

}

void UI::PlayerAnim(GRTYPE& arg_type)
{
	// 状態変化検出
	if (arg_type != prev_type) {
		PlayerAnimStart(arg_type);
		prev_type = arg_type;
	}

	frame++;

	// 8フレームごとに次のコマへ
	if (frame % 20 == 0) {
		//
		scrX += SIZE_W;

		// コマが末尾まで行った
		if ((scrX - startScrX) / SIZE_W >= totalFrames) {
			// 待機じゃなかったら戻す
			if (prev_type == ATTACK || prev_type == MAGIC || prev_type == SKILL || prev_type == DAMAGE) {
				arg_type = WAIT;
				PlayerAnimStart(arg_type);
				prev_type = arg_type;
				AnimWait = true;

			}
			else {

				scrX = startScrX; // ループ
				AnimWait = false;
			}
		}
	}

}



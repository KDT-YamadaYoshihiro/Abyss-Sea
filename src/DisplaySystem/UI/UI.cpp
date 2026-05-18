#include "UI.h"
#include "Character/Character.h"


// テーブルの定義
const UI::AnimInfo UI::animTable[ANIM_MAX] = {
	{0,0,3},	// WAIT
	{3,0,3},	// ATTACK
	{3,3,3},	// MAGIC
	{3,1,3},	// SKILL
	{0,4,3},	// DAMAGE
	{6,2,3},	// DEAD
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

void UI::SelectStatus(int arg_x, int arg_y, int arg_sizeX, int arg_sizeY, int arg_handle, const std::shared_ptr<Character>& character)
{
	const int ICON_SIZE = 60;  // アイコンの表示サイズ

	//　背景（黒）
	int sizeX = arg_x + arg_sizeX;
	int sizeY = arg_y + arg_sizeY;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(arg_x, arg_y, sizeX, sizeY, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// 枠(白)
	DrawLineBox(arg_x, arg_y, sizeX, sizeY, GetColor(255, 255, 255), true);

	// アイコン画像
	int drawX = (arg_x + arg_sizeX / 2) - ICON_SIZE / 2;
	int drawY = arg_y + 5;
	DrawExtendGraph(drawX, drawY, drawX + ICON_SIZE, drawY + ICON_SIZE, arg_handle, TRUE);

	// status
	// NAME
	SetFontSize(15);
	int y = drawY + ICON_SIZE;
	DrawFormatString(drawX - 10, y, GetColor(255, 255, 255), character->getName().c_str(), true);
	// HP
	DrawFormatString(drawX - 10, y + 20, GetColor(255, 255, 255), "HP\n%4d/%4d", character->getHp(), character->getMaxHp(), true);

}

void UI::stateIcon(int arg_x, int arg_y, int arg_handle, int arg_turn)
{
	// アイコンの幅と高さ
	const int icon_size = 20;

	// アイコン描画
	// 幅を合わせた描画
	int size_x = arg_x + icon_size;
	int size_y = arg_y + icon_size;
	DrawExtendGraph(arg_x, arg_y, size_x, size_y, arg_handle, true);
	// 残りターン数
	DrawFormatString(size_x, size_y, GetColor(255, 255, 255), "%d", arg_turn);

}



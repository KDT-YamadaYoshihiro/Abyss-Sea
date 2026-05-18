#pragma once
#include <string>
#include <memory>
#include "Constants/enum.h"
#include "DxLib.h"

#define RADIUS_X	100
#define RADIUS_Y	 50

// プレイヤー画像サイズ
#define SIZE_W		64
#define SIZE_H		64

// SP用の円のサイズ
#define SP_RADIUS	20

// ステージ枠のサイズ調整数
#define FRAME_X		30
#define FRAME_Y		10

class Character;

// UIクラス
class UI {

	struct AnimInfo
	{
		int startCol;	// 開始X方向
		int row;		// 行番号
		int frames;		// コマ数

	};

	static const AnimInfo animTable[ANIM_MAX];

	bool AnimWait = false;

	// アニメーション変更用カウントフレーム
	int frame = -1;

	int scrX = -1;
	int scrY = -1;

	int startScrX = -1;
	int startScrY = -1;

	GRTYPE prev_type;

	int totalFrames = -1;


public:

	UI()
		:frame(0),
		scrX(-1),scrY(-1),
		startScrX(-1),startScrY(-1),
		prev_type(WAIT),
		totalFrames(3),
		AnimWait(false)
	{
	
	};
	virtual ~UI() = default;

	int getScrX() const { return scrX; }
	int getStartScrX() const { return startScrX; }

	void setAnimWait(bool arg_animWait) { AnimWait = arg_animWait; }
	bool getAnimWait() const { return AnimWait; }

	// 楕円形のボタン関数
	void Button(float arg_x, float arg_y,								// 座標
		unsigned int arg_color, int fillFlag,							// 楕円のカラー
		const TCHAR* text, unsigned int textcolor)						// 文字の描画と文字のカラー
	{
		// 楕円形の描画
		DrawOval(arg_x, arg_y, RADIUS_X, RADIUS_Y, arg_color, fillFlag);
		// 文字列の描画(攻撃)
		SetFontSize(50);
		DrawFormatString((int)arg_x - (RADIUS_X / 2 + 20), (int)arg_y - (RADIUS_Y / 2), textcolor, "%s", text);

	};

	// ボタン表示
	void Button(int arg_x, int arg_y, int arg_scrX, int arg_scrY, int arg_handle) {
		DrawExtendGraph(arg_x, arg_y, arg_scrX, arg_scrY, arg_handle, true);
	}

	// スキルポイントの描画
	void SkillPoint(float arg_x, float arg_y, float arg_radius, int arg_skillPT, int arg_skillMaxPT) {

		SetFontSize(30);
		DrawString(arg_x - 70, arg_y - 50, "SP", GetColor(255, 255, 255));
		DrawFormatString(arg_x - 70, arg_y - 15, GetColor(255, 255, 255), "%d / ",arg_skillPT);

		// 最大スキルポイント（枠の描画）
		for (int maxpt = 0; maxpt < arg_skillMaxPT; maxpt++) {
			DrawCircle(arg_x + maxpt * (arg_radius * 2),arg_y,arg_radius,GetColor(255,255,0),false);
		}
		// 残りスキルポイント（中身の描画）
		for (int pt = 0; pt < arg_skillPT; pt++) {
			DrawCircle(arg_x + pt * ( arg_radius * 2 ), arg_y, arg_radius, GetColor(255, 255, 0),true);
		}
		
	}

	void PlayerAnimStart(GRTYPE& type);
	void PlayerAnim(GRTYPE& type);

	// 背景描画
	void BgRender(int &arg_x, int &arg_y, int arg_scrX, int arg_scrY, int arg_grh) {
		DrawExtendGraph(arg_x, arg_y,arg_x + arg_scrX, arg_y + arg_scrY, arg_grh, true);
	}

	// 背景スクロール
	void BgScroll(int &arg_x, int &arg_y, int resetPosX, int resetPosY) {
		arg_x--;
		if (arg_x <= -resetPosX) { arg_x = resetPosX; }
	}

	// プレイヤーキャラクターの描画
	void PlayerRender(int arg_posX, int arg_posY, int grh, int GRtype) {

		DrawRectGraph(arg_posX,arg_posY, this->scrX, this->scrY,SIZE_W, SIZE_H, grh, true, true);
	}

	// エネミーキャラクターの描画
	void EnemyRender(int arg_posX, int arg_posY, int grh, float ext) {

		DrawRotaGraph(arg_posX, arg_posY, ext, 0, grh, true,false);
	}

	// HPの割合表示
	float Ratio(int arg_Hp, int arg_HpMax) {

		// 割合計算
		float ratio = (float)arg_Hp / (float)arg_HpMax;

		// 計算結果をreturn
		return ratio;
	};

	// HPの描画
	void HpRender(float arg_x, float arg_y, float arg_xsize, float arg_ysize,
		int arg_HpMax, int arg_Hp, unsigned int color)
	{
		// 座標計算
		float box_x = arg_x + arg_xsize;
		float box_y = arg_y + arg_ysize;

		float box_hp = arg_xsize * Ratio(arg_Hp, arg_HpMax);
		float box_x2 = arg_x + box_hp;

		// ボックスの描画(中身)
		DrawFillBox(arg_x, arg_y, box_x2, box_y, color);
		// ボックスを描画(枠)
		DrawLineBox(arg_x, arg_y, box_x, box_y, GetColor(255, 255, 255));

	};

	 //ステージ選択枠
	void DrawStage(int arg_x, int arg_y, int arg_sizeX, int arg_sizeY, int arg_number,std::string arg_stageName, int arg_handle) {
		
		// 枠の描画
		StageFrame(arg_x, arg_y, arg_sizeX, arg_sizeY, arg_handle);
		
		//　ステージナンバーの表示
		SetFontSize(30);
		DrawFormatString(arg_x + 10, arg_y + 5,GetColor(255,255,255),"stage%02d",arg_number);

		// ベースを描画
		int sizeX = arg_x + arg_sizeX;
		int sizeY = arg_y + arg_sizeY;
		DrawBox(arg_x, arg_y, sizeX, sizeY, GetColor(255, 255, 255), false);
	
		// ステージネームの表示
		SetFontSize(50);
		int width = GetDrawStringWidth(arg_stageName.c_str(),-1);
		int x2 = (arg_x + arg_sizeX / 2) - width / 2;
		DrawFormatString( x2, arg_y + 40, GetColor(255,255,255),arg_stageName.c_str());

	}

	// 枠の描画
	void StageFrame(int arg_x, int arg_y, int arg_sizeX, int arg_sizeY, int arg_handle) {
		// ベースを描画
		int sizeX = arg_x + arg_sizeX;
		int sizeY = arg_y + arg_sizeY;
		DrawBox(arg_x, arg_y, sizeX, sizeY, GetColor(0, 0, 0), true);

		// 枠の描画
		int x = arg_x - FRAME_X;
		int y = arg_y - FRAME_Y;
		int sizeX2 = sizeX + FRAME_X;
		int sizeY2 = sizeY + FRAME_Y;
		DrawExtendGraph(x, y, sizeX2, sizeY2, arg_handle, true);

	}

	// ステージ詳細表示（クリック時）
	void StageDoc(int arg_x, int arg_y, int arg_sizeX, int arg_sizeY, int arg_suggest, int arg_number, std::string arg_stageName, int arg_handle) {

		// ベース画像を表示
		DrawExtendGraph(arg_x,arg_y,arg_sizeX,arg_sizeY,arg_handle,true);

		//　ステージナンバーの表示
		SetFontSize(30);
		DrawFormatString(arg_x + 10, arg_y + 5, GetColor(255, 255, 255), "stage%02d", arg_number);

		// 推奨レベルの表示
		SetFontSize(30);
		DrawFormatString(arg_x + 20, arg_y + 50,GetColor(255,255,255),"推奨レベル:%2d", arg_suggest);

		// ステージ名
		SetFontSize(50);
		int width = GetDrawStringWidth(arg_stageName.c_str(), -1);
		int x2 = (arg_x + arg_sizeX / 2) - width / 2;
		DrawFormatString(x2, arg_y + 40, GetColor(255, 255, 255), arg_stageName.c_str());

	}

	// ボタン
	void DecButton(int arg_x, int arg_y, int arg_sizeW, int arg_sizeH, const TCHAR* text) {
		DrawLineBox(arg_x, arg_y, arg_x + arg_sizeW, arg_y + arg_sizeH, GetColor(255, 255, 255), true);
		SetFontSize(30);
		DrawFormatString(arg_x + 60, arg_y + 30, GetColor(255, 255, 255), "%s",text);
	}

	// バトル中のステータス表示
	void SelectStatus(int arg_x, int arg_y, int arg_sizeX, int arg_sizeY, int arg_handle,const std::shared_ptr<Character>& character);

	//　改行関数
	// テキストと改行する文字数
	std::string LineBreakText(const std::string& text, size_t lineFew)
	{

		std::string result;
		std::size_t count = 0;

		for (char ch : text) {
			result += ch;
			count++;

			// 指定文字数ごとに改行
			if (count >= lineFew * 2) {
				result += '\n';
				count = 0;
			}
		}

		return result;

	}

	// 改行や不要な制御文字を取り除く
	std::string sanitize(const std::string& str) {
		std::string result;
		for (char ch : str) {
			// '\r' を無視する
			if (ch != '\r') {
				result += ch;
			}
		}
		return result;
	}

	// バフ、デバフアイコン描画
	void stateIcon(int arg_x, int arg_y, int arg_handle, int arg_turn);
};




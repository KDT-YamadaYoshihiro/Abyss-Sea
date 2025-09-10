#pragma once

// キャラクターのHP管理クラス
class CHP {

	// 今のHPを示す変数
	int Hp;
	// HPの最大値を示す変数
	int HpMax;

public:

	virtual ~CHP() = default;

	// 初期化
	CHP(int hp, int hpMax):Hp(hp),HpMax(hpMax) {
	}

	// HPの割合表示
	float Ratio() {

		// 割合計算
		float ratio = Hp / HpMax;

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

		float box_hp = Ratio();
		float box_x2 = arg_x + box_hp;

		// ボックスを描画(枠)
		DrawLineBox(arg_x, arg_y, box_x, box_y, GetColor(255, 255, 255));
		// ボックスの描画(中身)
		DrawFillBox(arg_x, arg_y, box_x2,box_y,color);
	};


};
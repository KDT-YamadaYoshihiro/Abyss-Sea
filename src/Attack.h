#pragma once

// アタッククラス
class Attack{

	// 攻撃力の変数
	float ATK;
	// バフ量の変数
	float buff;
	// 防御力の変数
	float def;
	// コンボ数の変数
	float combo;

public:

	virtual ~Attack() = default;

	// 初期化
	Attack(float arg_atk, float arg_buff, float arg_def, int arg_combo)
		:ATK(arg_atk), buff(arg_buff), def(arg_def), combo(arg_combo) {
	};

	// ダメージ計算(攻撃力,バフ,防御力,コンボ数,)
	int  Calculation(int arg_atk, int arg_buff, int arg_def, int arg_combo) {
		
		ATK = arg_atk;
		buff = 1.0f + arg_buff;
		def = arg_buff;
		combo = 1.0f + arg_combo / 10.0f;

		float damage = (ATK * buff) * combo - def;

		return damage;
	}
	
	// 攻撃力バフ
	float Buff(float arg_buff) {
		
		buff += arg_buff;

		return buff;
	}


};


#pragma once

class Cutuin{

	// 表示フラグ
	bool drawFlag;

	// 座標
	int x, y;

	// サイズ
	int sizeW, sizeH; 

	// 画像ハンドル
	int handle;

	// アニメーションフレーム
	int frame;

public:

	// 初期化
	Cutuin(int arg_x, int arg_y, int arg_sizeW, int arg_sizeH, int arg_handle)
		:drawFlag(false), x(arg_x), y(arg_y), sizeW(arg_sizeW), sizeH(arg_sizeH), handle(arg_handle), frame(0)
	{

	};

	// デストラクタ
	virtual ~Cutuin() {};

	// スタート関数
	void Start();

	// 更新
	void Update();

	// 描画
	void Draw();


};
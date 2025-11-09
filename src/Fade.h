#pragma once
#include "DxLib.h"
#include "enum.h"

// フェード基底クラス
class Fade {

protected:

	int fadeMode = -1;			// フェードの状態
	bool fadeCheck = -1;	// フェードが起動中か
	bool changeScreen = -1;

public:

	// フェード状態
	enum {
		FADE_NONE,
		FADE_CLAUSE,
		FADE_OPEN,
	};

	// 初期化
	Fade() {
		fadeMode = FADE_NONE;
		fadeCheck = false;
		changeScreen = false;
	};

	// 
	virtual ~Fade() {};

	void Init() {
		fadeMode = FADE_NONE;
		fadeCheck = false;
		changeScreen = false;
	}

	// スタート関数
	void fadeStart(int arg_open) {
		fadeMode = arg_open;
		fadeCheck = true;
	}

	//　描画
	virtual void fadeDraw() = 0;

	//　更新
	virtual void fadeUpdate() = 0;

};

// 円形フェードクラス
class FadeCircle : public Fade {

	int radius = -1;		// 描画する円のサイズ

public:

	FadeCircle() {
		radius = 0;
		fadeMode = FADE_NONE;
		fadeCheck = false;
		changeScreen = false;
	};

	virtual ~FadeCircle() {};


	// フェード用
	void fadeDraw() override {}
	void fadeDraw(int arg_x, int arg_y) {
		// 円の描画
		DrawCircle(arg_x, arg_y, radius, GetColor(0, 0, 0), true);
	}


	void fadeUpdate() override {}
	// アップデート
	void fadeUpdate(int x) {

		static const int speed = 50;

		switch (fadeMode)
		{
		case FADE_NONE:

			Init();

			break;

		case FADE_CLAUSE:		// クローズ

			if (fadeCheck) {

				radius += speed;

				if (radius >= x) {
					radius = x;
					fadeMode = FADE_OPEN;
				}
			}

			break;

		case FADE_OPEN:			// オープン

			if (fadeCheck) {

				radius -= speed;

				if (radius <= 0) {
					radius = 0;
					fadeMode = FADE_NONE;
				}
			}

			break;

		}

	}

	bool Chackfade() const { return fadeMode; }
	int getRadius() const { return radius; }

	bool checkClause(int x) {

		bool check = false;

		if (radius >= x) {
			check = true;
			return check;
		}

		return check;
	}

	bool checkOpen() {

		bool check = false;

		if (radius <= 0) {
			check = true;
			return check;
		}

		return check;

	}


};

// 透明度フェードクラス
class FadeAlpha : public Fade {
	int alpha = -1;		// 透明度

public:

	FadeAlpha() {
		alpha = 0;
		fadeMode = FADE_NONE;
		fadeCheck = false;
		changeScreen = false;
	};

	virtual ~FadeAlpha() {};

	// フェード用
	void fadeDraw() override {
		// 透明度の設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		// 四角形の描画
		DrawBox(0, 0, WINDOW_W, WINDOW_H, GetColor(0, 0, 0), true);
		// ブレンドモードの解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// アップデート
	void fadeUpdate() override {
		static const int speed = 5;
		switch (fadeMode)
		{
		case FADE_NONE:
			Init();
			break;
		case FADE_CLAUSE:		// クローズ
			if (fadeCheck) {
				alpha += speed;
				if (alpha >= 255) {
					alpha = 255;
					fadeMode = FADE_OPEN;
				}
			}
			break;
		case FADE_OPEN:			// オープン
			if (fadeCheck) {
				alpha -= speed;
				if (alpha <= 0) {
					alpha = 0;
					fadeMode = FADE_NONE;
				}
			}
			break;
		}
	}


	bool Chackfade() const { return fadeMode; }
	int getAlpha() const { return alpha; }

	// クローズ完了チェック
	bool checkClause() {
		bool check = false;
		if (alpha >= 255) {
			check = true;
			return check;
		}
		return check;
	}

	// オープン完了チェック
	bool checkOpen() {
		bool check = false;
		if (alpha <= 0) {
			check = true;
			return check;
		}
		return check;
	}

};

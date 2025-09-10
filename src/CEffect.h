#pragma once
#include "DxLib.h"

class Effect {

	// 画像サイズ
	int imageSizeW = -1;
	int imageSizeH = -1;
	// 画像の切り取り位置
	int ef_scrX = -1;
	int ef_scrY = -1;
	// 画像の切り取りサイズ
	int scrSizeW = -1;
	int scrSizeH = -1;
	// 画像ハンドル
	int ef_grh;
	// アニメーション切り替えフレーム
	int  animFrame = -1;
	// 描画の表示非表示
	bool efDraw = false;


public:


	// 初期化
	Effect(int arg_grh)
		:ef_grh(arg_grh)
	{
		GetGraphSize(ef_grh, &imageSizeW, &imageSizeH);
		ef_scrX = 0;
		ef_scrY = 0;

		if (imageSizeW == 600 && imageSizeH == 120) {
			scrSizeW = 120;
			scrSizeH = 120;
		}
		else if (imageSizeW == 640 && imageSizeH == 4800) {
			scrSizeW = 640;
			scrSizeH = 480;
		}
		else if (imageSizeW == 2400 && imageSizeH == 1440) {
			scrSizeW = 480;
			scrSizeH = 480;
		}
		else {
			scrSizeW = 120;
			scrSizeH = 120;
		}
		animFrame = 0;
		efDraw = false;
	};

	// ゲット関数
	bool getEfDraw() const { return efDraw; }

	// 演出関数

	// 初期化関数
	void effectInit()
	{
		efDraw = false;
		ef_scrX = 0;
		ef_scrY = 0;
	}

	// スタート関数
	void effectStart() {
		if (efDraw == false) { efDraw = true; }
		ef_scrX = 0;
		ef_scrY = 0;
	}

	// 画像が横向きのアニメーション再生関数
	void sidePlayAnim() {

		// フレームをカウント
		if (efDraw) { animFrame++; }

		if (animFrame % 8 == 0) {
			// 非表示にする
			if (ef_scrX >= imageSizeW) {
				efDraw = false; }
			// scrXを移動
			else{ ef_scrX += scrSizeW;}
			
		}

	}

	// 画像が縦向きのアニメーション再生関数
	void lengthPlayerAnim() {

		// フレームをカウント
		if (efDraw) { animFrame++; }

		if (animFrame % 5 == 0) {
			// 非表示にする
			if (ef_scrY >= imageSizeH) { efDraw = false; }
			// scrYを移動
			else{ ef_scrY += scrSizeH;}
			
		}

	}

	// 縦横のアニメーション再生関数
	void PlayAnim() {
		if (efDraw) { animFrame++; }

		if (animFrame % 5 == 0) {
			// scrXを移動
			ef_scrX += scrSizeW;
			if (ef_scrX >= imageSizeW) {
				
				// 非表示にする
				if (ef_scrY >= imageSizeH) { efDraw = false; }
				// scrYを移動
				else{ef_scrY += scrSizeH;}

				ef_scrX = 0;
			}

		}
	}

	// 描画
	void effectDraw(int arg_x, int arg_y, int sizeW, int sizeH) {
		if (efDraw == true) {
			DrawRectGraph(arg_x - sizeW, arg_y - sizeH, ef_scrX, ef_scrY, scrSizeW, scrSizeH, ef_grh, true);
		}
	}
	

};



class Sound {

public:

	//　初期化
	Sound() {};

	// 音源再生
	// BGM(ループ再生)
	void PlayBgm(int bgmh) {
		if (CheckSoundMem(bgmh) == false) {
			PlaySoundMem(bgmh, DX_PLAYTYPE_LOOP);
		}
	}
	// SE(単再生)
	void PlaySe(int seh) {
		if (CheckSoundMem(seh) == false) { PlaySoundMem(seh,DX_PLAYTYPE_BACK); }
	}

	// 音源停止
	void stopBgm(int bgmh) {
		if (CheckSoundMem(bgmh) == true) { StopSoundMem(bgmh,false); }
	}
	void stopSe(int seh) {
		if (CheckSoundMem(seh) == true) { StopSoundMem(seh, false); }

	}

};
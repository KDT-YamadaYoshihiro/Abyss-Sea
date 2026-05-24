#pragma once
#include "DxLib.h"

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
		if (CheckSoundMem(seh) == false) { PlaySoundMem(seh, DX_PLAYTYPE_BACK); }
	}

	// 音源停止
	void stopBgm(int bgmh) {
		if (CheckSoundMem(bgmh) == true) { StopSoundMem(bgmh, false); }
	}
	void stopSe(int seh) {
		if (CheckSoundMem(seh) == true) { StopSoundMem(seh, false); }

	}

};
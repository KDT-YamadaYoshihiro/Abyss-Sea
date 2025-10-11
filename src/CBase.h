#pragma once
#include <vector>
#include <memory>
#include "enum.h"
#include "DxLib.h"
#include "Mouse.h"

//	カラーコード一覧
#define BLACK	GetColor(0, 0, 0)			//	黒
#define WHITE	GetColor(255, 255, 255)		//	白
#define RED		GetColor(255, 0, 0)			//	赤
#define GREEN	GetColor(60, 179, 113)		//	緑
#define BLUE	GetColor(0, 0, 255)			//	青
#define YELLOW	GetColor(255, 255, 0)		//	黄色
#define PINK	GetColor(255, 0, 255)		//	ピンク
#define SKYBLUE	GetColor(0, 255, 255)		//	水色


// フォントサイズ
#define FONTSIZE		 100

// CLICK TO START
// 文字列の上下揺れの範囲
#define TOP_RANGE		 615
#define DWON_RANGE		 585

enum BUTTAN {
	BGM_UP,
	BGM_DOWN,
	SE_UP,
	SE_DOWN,

	BUTTAN_MAX
};

// スクリーンベースクラス
class ScreenBase {

protected:

public:

	virtual void Update() = 0;

	virtual void Render() = 0;

};
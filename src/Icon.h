// CIcon.h
#pragma once
#include <DxLib.h>
#include "enum.h"

class CIcon {
private:
	int x, y;           // 座標
	int size;           // サイズ
	int handle;         // 画像ハンドル
	int remainingTurn;  // 残りターン数
	BUFF_TYPE type;      // バフの種類

public:
	CIcon(BUFF_TYPE t, int posX, int posY, int iconSize, int imgHandle, int turn)
		: type(t), x(posX), y(posY), size(iconSize), handle(imgHandle), remainingTurn(turn) {
	}

	void Draw() const {
		DrawExtendGraph(x, y, x + size, y + size, handle, true);
		DrawFormatString(x + size / 2 + 5, y + size - 10, GetColor(255, 255, 255), "%d", remainingTurn);
	}

	void DecreaseTurn() {
		if (remainingTurn > 0) remainingTurn--;
	}

	bool isExpired() const {
		return remainingTurn <= 0;
	}

	int getTurn() const { return remainingTurn; }
	BUFF_TYPE getType() const { return type; }

	void setPos(int posX, int posY) { x = posX; y = posY; }
};

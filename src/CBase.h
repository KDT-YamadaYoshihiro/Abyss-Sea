#pragma once
#include <vector>
#include <memory>
#include "enum.h"
#include "DxLib.h"
#include "Mouse.h"

// スクリーンベースクラス
class ScreenBase {

protected:

public:

	virtual void Update() = 0;

	virtual void Render() = 0;

};
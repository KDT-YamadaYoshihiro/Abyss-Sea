#pragma once
#include "DxLib.h"
#include <vector>
#include <memory>
#include "Constants/enum.h"
#include "System/InPut/Mouse/Mouse.h"
#include "DisplaySystem/Fade/Fade.h"

// スクリーンベースクラス
class ScreenBase {

protected:

public:

	virtual void Update() = 0;

	virtual void Render() = 0;

};
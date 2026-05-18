#pragma once
// ライブラリ
#include "Dxlib.h"

#include <ctime>
#include <math.h>
#include <cmath>
#include <vector>

// system
#include "System/InPut/Key/InPutKey.h"
#include "System/InPut/Pad/InPutPad.h"

#include "Scene/Manager/Manager.h"
#include "Character/Factory/CharacterFactory.h"
#include "Character/Character.h"

#define DegToRad 0.017453293f			//	弧度法へ
#define RadToDeg  57.29577951f


#define R2D(rad) rad * RadToDeg			// 
#define D2R(deg) deg * DegToRad			// ラジアン返還

// ゲームループクラス
class CGame {

	// コンストラクタ
	CGame() = default;
	// デストラクタ
	virtual ~CGame() = default;


public:

	CGame(const CGame&) = delete;
	CGame& operator = (const CGame&) = delete;
	static CGame& Instance() {
		static CGame instance;
		return instance;
	}

	// 初期化
	bool Init();
	// ループ処理
	void Run();
	// 更新
	void Update();
	// 描画
	void Render();
	// 終了
	void End();

};

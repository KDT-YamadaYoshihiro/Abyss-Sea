#pragma once
// ライブラリ
#include "Dxlib.h"

#include <ctime>
#include <math.h>
#include <cmath>
#include <vector>

// system
#include "Float2.h"
#include "InPutKey.h"
#include "InPutPad.h"

#include "Manager.h"
#include "CharacterFactory.h"
#include "Character.h"

#define DegToRad 0.017453293f			//	弧度法へ
#define RadToDeg  57.29577951f


#define R2D(rad) rad * RadToDeg			// 
#define D2R(deg) deg * DegToRad			// ラジアン返還

// ゲームループクラス
class CGame {

	// コンストラクタ
	CGame();
	// デストラクタ
	virtual ~CGame();


public:

	CGame(const CGame&) = default;
	CGame& operator = (const CGame&) = default;
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

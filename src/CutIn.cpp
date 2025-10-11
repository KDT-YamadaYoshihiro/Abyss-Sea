#include "CutIn.h"

// スタート関数（座標指定、速度指定）
void Cutuin::Start(std::string arga_name,int arg_x, int arg_y, int arg_sizeW, int arg_sizeH, int arg_handle,
	int arg_enterSpeed, int arg_pauseSpeed, int arg_exitSpeed)
{
	name = arga_name;
	x = arg_x;
	y = arg_y;
	sizeW = arg_sizeW;
	sizeH = arg_sizeH;
	handle = arg_handle;

	enterSpeed = arg_enterSpeed;
	pauseSpeed = arg_pauseSpeed;
	exitSpeed = arg_exitSpeed;

	phase = CutinPhase::Entering;
	frameCounter = 0;

	drawFlag = true;
}

// リセット関数（再利用を考慮）
void Cutuin::Reset()
{
	drawFlag = false;
	x = 0;
	y = 0;
}

// 更新
void Cutuin::Update()
{

    if (!drawFlag) return;

    switch (phase)
    {
    case CutinPhase::Entering:
        x += enterSpeed;

        // 画面中央に到達したら減速（位置 or フレームで判断）
        if (x + sizeW / 2 >= 640) {  // 中央の例：1280pxの中央
            phase = CutinPhase::Pausing;
            frameCounter = 0;
        }
        break;

    case CutinPhase::Pausing:
        x += pauseSpeed;
        frameCounter++;

        // しばらくしたら退場フェーズへ（30フレーム例）
        if (frameCounter > 30) {
            phase = CutinPhase::Exiting;
        }
        break;

    case CutinPhase::Exiting:
        x += exitSpeed;

        // 画面外に出たら終了
        if ((exitSpeed > 0 && x > 1280) || (exitSpeed < 0 && x + sizeW < 0)) {
            drawFlag = false;
        }
        break;
    }
    
}

void Cutuin::Draw()
{
	if (drawFlag) {

		// 背景ボックス
		DrawBox(x, y, x + sizeW, y + sizeH, GetColor(255, 255, 255), true);

		// 画像描画
		DrawExtendGraph(x, y, x + sizeW, y + sizeH, handle, true);

		// 名前表示
		SetFontSize(20);
		DrawFormatString(x + 20, y - 20, GetColor(255, 255, 255), name.c_str());

	}
}

#include "CTitle.h"
#include "CStage.h"

// 更新処理
void CTitle::Update()
{

	bgm->PlayBgm(CLoad::Instance().getBgmHandle(BGM_START));

	// CLICK　TO START
	// 上下に揺れる。
	posY += sway;
	if (posY >= TOP_RANGE) {
		sway *= -1;
	}
	if (posY <= DWON_RANGE) {
		sway *= -1;
	}

	// 背景移動
	for (int i = 0; i < TWO; i++) {
		ui[i]->BgScroll(bgPosX[i], bgPosY[i], WINDOW_W, WINDOW_H);
	}

	// タイトルスクリーン宙にクリックすると画面が切り替わる。
	// (CTitle -> CStage)
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		se->PlaySe(CLoad::Instance().getSeHandle(SE_START));
		Manager::Instance().ChangeScreen<CStage>();
	}

}

// 描画処理
void CTitle::Render()
{
	// 背景描画
	for (int i = 0; i < TWO; i++)
	{
		ui[i]->BgRender(bgPosX[i], bgPosY[i], WINDOW_W, WINDOW_H, CLoad::Instance().getBgGrh());
	}

	// タイトル文字
	SetFontSize(FONTSIZE);
	DrawGraph(100, 200, CLoad::Instance().getTitleGrh(), true);

	// クリックtoスタート
	SetFontSize(FONTSIZE / 2);
	DrawFormatString(posX, posY, GetColor(255, 255, 255), "CLICK TO START", true);


}

// デストラクタ
CTitle::~CTitle()
{
}

#include "CTitle.h"
#include "Scene/StageSelect/CStage.h"

// 更新処理
void CTitle::Update()
{

	// フェードの更新処理
	fade->fadeUpdate();

	// BGMスタート
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

	bubble->Update();

	// タイトルスクリーン宙にクリックすると画面が切り替わる。
	// (CTitle -> CStage)
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		// SE再生
		se->PlaySe(CLoad::Instance().getSeHandle(SE_START));
		// フェード起動
		fade->fadeStart(fade->FADE_CLAUSE);
	}

	// 画面切り替え
	if (fade->checkClause()) {
		ScreenManager::Instance().ChangeScreen<CStage>();
	}

}

// 描画処理
void CTitle::Render()
{
	// 背景描画
	ui->BgRender(bgPosX, bgPosY, WINDOW_W, WINDOW_H, CLoad::Instance().getBgGrh());

	bubble->Render();

	// タイトル文字
	DrawGraph(100, 200, CLoad::Instance().getTitleGrh(), true);

	// クリックtoスタート
	SetFontSize(FONTSIZE / 2);
	DrawFormatString(posX, posY, GetColor(255, 255, 255), "CLICK TO START", true);


}

// デストラクタ
CTitle::~CTitle()
{
}

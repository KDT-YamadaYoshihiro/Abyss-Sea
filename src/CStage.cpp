#include "CStage.h"
#include "CBattle.h"
#include "CPartySelect.h"

// 更新処理
void CStage::Update()
{

	// フェードの更新処理
	fade->fadeUpdate(WINDOW_W);

	// 三角形の座標更新
	Triangle();

	// ステージスクリーン中に決定ボタンが押されたら画面が切り替わる
	// (CStage -> CPatySelect)
	if (CheckBoxClick(boxX, boxY, boxSizeX, boxSizeY) == true) {
		se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
		Manager::Instance().setStageScreen(stageNum);
		Manager::Instance().setSuggest(Suggest);
		Manager::Instance().ChangeScreen<CPartySelect>();
	}

}

// 描画処理
void CStage::Render()
{
	// 背景
	DrawExtendGraph(0, 0, WINDOW_W, WINDOW_H, CLoad::Instance().getStageBgGrh(), true);

	// 
	DrawString(100, 100, "ステージ", GetColor(255, 255, 255), true);

	// 選択中の三角形
	DrawTriangle(TrX1, TrY1, TrX2, TrY2, TrX3, TrY3, GetColor(255, 0, 0), true);

	// 決定ボタン（四角）
	SetFontSize(50);
	DrawFormatString(boxX, boxY, GetColor(255, 255, 255), "<編成>");

	// ステージ名
	SetFontSize(50);
	DrawFormatString(posX[STAGE1], posY[STAGE1], GetColor(255, 255, 255), "【stage1:漂う魚影？】");
	DrawFormatString(posX[STAGE2], posY[STAGE2], GetColor(255, 255, 255), "【stage2:奈落の底のモノ】");
	DrawFormatString(posX[STAGE3], posY[STAGE3], GetColor(255, 255, 255), "【stage3:深淵の恐怖】");
	
}

void CStage::Triangle()
{


	// 円がClickされたら
	if (!selectMode) {
		for (int i = 0; i < CIRCLEMAX; i++) {
			if (CheckBoxClick(posX[i], posY[i], 700, 50)) {
				if (stageNum != i) {
					// 三角形の座標をClickした円の上部にする
					stageNum = i;
					se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
				}
				break;
			}
		}
	}

	// 選択中のステージの上部に三角形を描画させる。
	switch (stageNum)
	{
	case STAGE1:
		TrX1 = posX[STAGE1];
		TrY1 = posY[STAGE1]+ 25;
		TrX2 = TrX1 - 50;
		TrY2 = TrY1 + 20;
		TrX3 = TrX1 - 50;
		TrY3 = TrY1 - 20;

		Suggest = 1;

		break;

	case STAGE2:
		TrX1 = posX[STAGE2];
		TrY1 = posY[STAGE2] +25;
		TrX2 = TrX1 - 50;
		TrY2 = TrY1 + 20;
		TrX3 = TrX1 - 50;
		TrY3 = TrY1 - 20;

		Suggest = 10;

		break;

	case STAGE3:
		TrX1 = posX[STAGE3];
		TrY1 = posY[STAGE3] + 25;
		TrX2 = TrX1 - 50;
		TrY2 = TrY1 + 20;
		TrX3 = TrX1 - 50;
		TrY3 = TrY1 - 20;

		Suggest = 25;

		break;

	default:
		break;
	}


}

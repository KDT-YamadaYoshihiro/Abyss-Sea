#include "CStage.h"
#include "CBattle.h"
#include "CPartySelect.h"

// 更新処理
void CStage::Update()
{

	// フェードアップデート
	fade->fadeUpdate();

	if (fade->checkOpen()) {
		// 三角形の座標更新
		Triangle();

		speed += vec;
		TrX1 += speed;
		if (speed <= -1.0f) { vec *= -1; }
		if (speed >= 1.0f) { vec *= -1; }
			
		board.Update();

		// ステージスクリーン中に決定ボタンが押されたら画面が切り替わる
		// (CStage -> CPatySelect)
		if (CheckBoxClick(boxX, boxY, boxSizeX, boxSizeY) == true) {
			// 決定SE
			se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
			// フェードアウトスタート関数
			fade->fadeStart(fade->FADE_CLAUSE);
		}
	}

	// フェードを最後まで行ったとき画面切り替え
	if (fade->checkClause()) {
		ScreenManager::Instance().setStageScreen(stageNum);
		ScreenManager::Instance().setSuggest(suggest);
		ScreenManager::Instance().ChangeScreen<CPartySelect>();
	}

}

// 描画処理
void CStage::Render()
{
	// 背景
	DrawExtendGraph(0, 0, WINDOW_W, WINDOW_H, CLoad::Instance().getStageBgGrh(), true);

	SetFontSize(FONT_BIGSIZE);
	DrawString(100, 100, "ステージ", GetColor(255, 255, 255), true);

	// 選択中の三角形
	DrawTriangle(TrX1, TrY1, TrX1 - 50, TrY2, TrX1 - 50, TrY3, GetColor(255, 0, 0), true);

	// 決定ボタン（四角）
	if (stageNum != -1) {
		SetFontSize(FONT_BIGSIZE);
		ui->Button(boxX, boxY, boxX + boxSizeX, boxY + boxSizeY, CLoad::Instance().getButtonGrh(CHALLEMGE));
	}

	// ステージ名
	SetFontSize(FONT_BIGSIZE);
	for (int i = 0; i < STAGE_MAX; i++) {
		ui->DrawStage(posX[i], posY[i], 500, 100, i + 1, doc_name[i], CLoad::Instance().getStageFrameGrh());
	}

	// 依頼書
	if (stageNum != -1) {
		board.Draw(CLoad::Instance().getStageDoc(),400,500);
	}

	// フェード
	fade->fadeDraw();
}

// 三角形の座標
void CStage::Triangle()
{

	// 円がClickされたら
	for (int i = 0; i < STAGE_MAX; i++) {

		if (CheckBoxClick(posX[i], posY[i], 700, 100)) {
			// 三角形の座標をClickした円の上部にする
			stageNum = i;
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			// 依頼書の表示変更
			board.setStage(i);

			// 選択中のステージの上部に三角形を描画させる。
			switch (stageNum)
			{
			case STAGE1:
				TrX1 = posX[STAGE1] - 50;
				TrY1 = posY[STAGE1] + 50;
				TrX2 = TrX1 - 50;
				TrY2 = TrY1 + 20;
				TrX3 = TrX1 - 50;
				TrY3 = TrY1 - 20;

				suggest = 1;

				break;

			case STAGE2:
				TrX1 = posX[STAGE2] - 50;
				TrY1 = posY[STAGE2] + 50;
				TrX2 = TrX1 - 50;
				TrY2 = TrY1 + 20;
				TrX3 = TrX1 - 50;
				TrY3 = TrY1 - 20;

				suggest = 5;

				break;

			case STAGE3:
				TrX1 = posX[STAGE3] - 50;
				TrY1 = posY[STAGE3] + 50;
				TrX2 = TrX1 - 50;
				TrY2 = TrY1 + 20;
				TrX3 = TrX1 - 50;
				TrY3 = TrY1 - 20;

				suggest = 10;

				break;

			case STAGE4:
				TrX1 = posX[STAGE4] - 50;
				TrY1 = posY[STAGE4] + 50;
				TrX2 = TrX1 - 50;
				TrY2 = TrY1 + 20;
				TrX3 = TrX1 - 50;
				TrY3 = TrY1 - 20;
				suggest = 25;
				break;

			}

		}

		
	}

}


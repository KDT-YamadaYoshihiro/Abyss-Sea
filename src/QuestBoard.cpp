#include "QuestBoard.h"
#include "Manager.h"
#include "DxLib.h"


// 初期化（初期位置設定）
void QuestBoard::Init() {
	for (int i = 0; i < requests.size(); ++i) {
		requests[i].x = baseX + i * 20;
		requests[i].y = baseY + i * 20;
		requests[i].targetX = requests[i].x;
		requests[i].targetY = requests[i].y;
		requests[i].active = true;
	}
}

void QuestBoard::setStage(int stage)
{

	nextStage = stage;
	const float offset = 20.0f;
	isMoving = true;

	for (int i = 0; i < requests.size(); ++i) {
		auto& r = requests[i];

		if (i < nextStage) {
			// 選択より前 → 右側へスライドアウト
			r.targetX = baseX + 600;
			r.targetY = baseY;
		}
		else if (i == nextStage) {
			// 選択中 → メイン位置
			r.targetX = baseX;
			r.targetY = baseY;
		}
		else {
			// 選択より後 → 右下に重ねて表示
			r.targetX = baseX + (i - nextStage) * offset;
			r.targetY = baseY + (i - nextStage) * offset;
		}
	}
}

void QuestBoard::Update()
{
	if (!isMoving) return;

	bool allStopped = true;
	for (auto& r : requests) {
		float dx = r.targetX - r.x;
		float dy = r.targetY - r.y;
		float dist = std::sqrt(dx * dx + dy * dy);

		if (dist > 1.0f) {
			allStopped = false;
			float spd = r.speed; // 補間速度（柔らかく動かす）
			if (dist < spd) spd = dist;
			r.x += spd * dx / dist;
			r.y += spd * dy / dist;
		}
		else {
			r.x = r.targetX;
			r.y = r.targetY;
		}
	}

	if (allStopped) {
		isMoving = false;
		currentStage = nextStage;
	}
}

void QuestBoard::Draw() const
{
	SetFontSize(18);

	// 重なり順に描画（下の方から順に）
	int max = requests.size() - 1;
	for (int i = max; i >= 0 ; i--) {
		const auto& r = requests[i];

		// 背景（塗りつぶし）
		DrawBoxAA(r.x - 10, r.y - 10, r.x + 300, r.y + 150, GetColor(40, 40, 60), TRUE);
		// 枠線
		DrawBoxAA(r.x - 10, r.y - 10, r.x + 300, r.y + 150, GetColor(255, 255, 255), FALSE);
		// テキスト
		DrawFormatString((int)r.x, (int)r.y, GetColor(255, 255, 255), "【%s】", r.title.c_str());
		DrawFormatString((int)r.x, (int)r.y + 40, GetColor(255, 255, 255), "%s", r.description.c_str());
	}
}

void QuestBoard::Draw(int arg_handle)
{
	// 重なり順に描画（下の方から順に）
	int max = requests.size() - 1;
	for (int i = max; i >= 0; i--) {
		const auto& r = requests[i];

		// 画像描画
		DrawExtendGraph(r.x - 10, r.y - 10, r.x + 300, r.y + 400, arg_handle, true);

		// テキスト
		DrawFormatString((int)r.x, (int)r.y, GetColor(255, 255, 255), "【%s】", r.title.c_str());
		DrawFormatString((int)r.x, (int)r.y + 40, GetColor(255, 255, 255), "%s", r.description.c_str());
	}
}

void QuestBoard::Draw(int arg_handle, int arg_sizeX, int arg_sizeY)
{
	// 重なり順に描画（下の方から順に）
	int max = requests.size() - 1;
	for (int i = max; i >= 0; i--) {
		const auto& r = requests[i];

		// 画像描画
		DrawExtendGraph(r.x - 10, r.y - 10, r.x + arg_sizeX, r.y + arg_sizeY, arg_handle, true);

		// エネミーアイコン描画
		int x = (int)r.x + 160;
		int y = (int)r.y + 280;
		static int ICON_SIZE = 180;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawExtendGraph(x, y, x + ICON_SIZE, y + ICON_SIZE, r.enemy_iconHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// テキスト
		SetFontSize(50);
		DrawFormatString((int)r.x, (int)r.y, GetColor(255, 255, 255), "stage%d",i+1);
		DrawFormatString((int)r.x + 20, (int)r.y + 80, GetColor(255, 255, 255), "%s", r.title.c_str());
		SetFontSize(18);
		DrawFormatString((int)r.x + 50, (int)r.y + 150, GetColor(255, 255, 255), "%s", r.description.c_str());
		DrawFormatString((int)r.x + 50, (int)r.y + 180, GetColor(255, 255, 255), "推奨レベル:%d", r.needLv);

		// ミッション描画
		ScreenManager::Instance().DrawStageUI(i, r.x + 20,r.y + 250);

	}

}

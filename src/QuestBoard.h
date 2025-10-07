#pragma once
#include <vector>
#include <string>
#include <cmath>

struct RequestPaper {

	int stageID;				// ステージID
	int x;						// 現在表示座標
	int y;
	int targetX;				// 移動先座標
	int targetY;
	int speed;					// 移動速度
	bool active;				// 表示中か

	std::string title;			// ステージタイトル
	std::string description;	// 説明

	RequestPaper(int id, const std::string& t, const std::string& d)
		: stageID(id), x(1280), y(100), targetX(900), targetY(100), speed(20.0f), active(false),
		title(t), description(d) {
	}
};

class QuestBoard {

private:
	std::vector<RequestPaper> requests;	// クエストリスト
	int currentStage = 0;				// 現在のステージ
	int nextStage = 0;					// 現表示中の次のステージ番号
	bool isMoving = false;				// 移動中か
	int baseX = 750;
	int baseY = 100;
	float offsetX = 1280.0f;			// 画面外右位置

public:
	// 初期化（初期位置設定）
	void Init();
	// クエストの追加
	void addRequest(const RequestPaper& r) { requests.push_back(r); }
	// ステージ内容のセット
	void setStage(int stage);
	// 更新
	void Update();
	// 描画
	void Draw() const;
	// 描画、画像ハンドルあり
	void Draw(int arg_handle);
	// 描画、画像ハンドルあり、サイズ指定あり
	void Draw(int arg_handle, int arg_sizeX, int arg_sizeY);
};
#pragma once
#include <vector>
#include "Mission.h"

class MissionManager {
private:

	// ミッションリスト
    std::vector<Mission> missions;

	// コンストラクタ
	MissionManager() = default;

public:

	// デストラクタ
	virtual ~MissionManager() {};

	// シングルトン
	MissionManager(const MissionManager&) = delete;
	MissionManager& operator = (const MissionManager&) = delete;
	static MissionManager& Instance() {
		static MissionManager instance;
		return instance;
	}

	// CSVからミッションデータを読み込み
    void LoadMissionTable(const std::string& csvFile);
	// ステージIDに応じたミッションリストを取得
    void DrawMissions(int stageID, int x, int y) const;

	// 全ミッション取得（デバッグ用）
    const std::vector<Mission>& GetAll() const { return missions; }
	// ステージIDでミッションを取得
    std::vector<Mission> GetByStage(int stageID) const;
	// ミッション状態更新
    void SetCleared(int stageID, MissionType type, bool cleared);
};

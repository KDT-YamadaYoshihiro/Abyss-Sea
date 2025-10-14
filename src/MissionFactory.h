#pragma once
#include <vector>
#include <string>
#include "Mission.h"

class MissionFactory {
private:
    std::vector<Mission> missions;

public:

	// CSVからミッションデータを読み込み
    static std::vector<Mission> LoadFromCSV(const std::string& filename);
    // ステージIDに応じたミッションリストを取得
    const std::vector<Mission>& GetMissions(int stageID) const;
	// ミッションの描画
    void DrawMissions(int stageID, int x, int y) const;

    // ミッション状態更新
    void SetCleared(int missionID, bool cleared);
};

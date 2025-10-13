#pragma once
#include "Mission.h"
#include <vector>

// ミッションセットクラス
class MissionSet {

private:

    std::vector<Mission> missions;

public:

	// 初期化
    MissionSet(int includeCharacterId, int turnLimit) {
        // 1ステージにつき3つ固定
        missions.push_back(Mission(MissionType::CLEAR_QUEST));
        missions.push_back(Mission(MissionType::INCLUDE_CHARACTER, includeCharacterId));
        missions.push_back(Mission(MissionType::TURN_LIMIT, -1, turnLimit));
    }

	// ミッション達成条件のチェック関数
    void checkAll(bool questCleared, const std::vector<int>& partyIds, int usedTurn) {
        for (auto& m : missions) {
            m.checkClear(questCleared, partyIds, usedTurn);
        }
    }

    const std::vector<Mission>& getMissions() const { return missions; }
};

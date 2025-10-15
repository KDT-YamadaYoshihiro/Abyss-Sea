#include "MissionManager.h"
#include "MissionFactory.h"
#include "DxLib.h"

void MissionManager::LoadMissionTable(const std::string& csvFile) {
    missions = MissionFactory::LoadFromCSV(csvFile);
}

// ステージIDでミッションを取得
std::vector<Mission> MissionManager::GetByStage(int stageID) const {
    std::vector<Mission> result;
    for (const auto& m : missions) {
        if (m.stageID == stageID)
            result.push_back(m);
    }
    return result;
}

// ミッションのクリア状態を設定
void MissionManager::SetCleared(int stageID, MissionType type, bool cleared) {
    for (auto& m : missions) {
        if (m.stageID == stageID && m.type == type) {
            m.isCleared = cleared;
            break;
        }
    }
}

// ミッションの描画
void MissionManager::DrawMissions(int stageID, int x, int y) const {
    int offsetY = 0;
    for (const auto& m : missions) {
        if (m.stageID != stageID) continue;


        // 塗りつぶし円
        DrawCircle(x, y + offsetY, 8, GetColor(255,255,0), m.isCleared);

        // ミッション説明文
        DrawFormatString(x + 20, y + offsetY - 8, GetColor(255, 255, 255), "%s", m.description.c_str());

        offsetY += 25;
    }
}

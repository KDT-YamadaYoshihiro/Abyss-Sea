#include "MissionManager.h"
#include "MissionFactory.h"
#include "DxLib.h"

void MissionManager::Init(const std::string& csvFile) {
    missions = MissionFactory::LoadFromCSV(csvFile);
}

std::vector<Mission> MissionManager::GetByStage(int stageID) const {
    std::vector<Mission> result;
    for (const auto& m : missions) {
        if (m.stageID == stageID)
            result.push_back(m);
    }
    return result;
}

void MissionManager::SetCleared(int stageID, MissionType type, bool cleared) {
    for (auto& m : missions) {
        if (m.stageID == stageID && m.type == type) {
            m.isCleared = cleared;
            break;
        }
    }
}

void MissionManager::DrawMissions(int stageID, int x, int y) const {
    int offsetY = 0;
    for (const auto& m : missions) {
        if (m.stageID != stageID) continue;

        int color = m.isCleared ? GetColor(255, 215, 0) : GetColor(180, 180, 180);

        // ìhÇËÇ¬Ç‘Çµâ~
        DrawCircle(x, y + offsetY, 8, color, TRUE);

        // É~ÉbÉVÉáÉìê‡ñæï∂
        DrawFormatString(x + 20, y + offsetY - 8, GetColor(255, 255, 255), "%s", m.description.c_str());

        offsetY += 25;
    }
}

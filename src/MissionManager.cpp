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

void MissionManager::ClearQuest(int arg_stageID)
{
    SetCleared(arg_stageID, MissionType::CLEAR_QUEST, true);
}

void MissionManager::IncludeCharacter(int arg_stageID, int arg_characterID)
{
    for (auto& m : missions) {

        if (m.stageID == arg_stageID && m.type == MissionType::INCLUDE_CHARACTER) {

            // ステージに紐づく指定キャラID（仮: m.targetCharacterID）
            if (m.targetID == arg_characterID) {
                m.isCleared = true;
            }
            // 一致しなければ現状維持
            break;
        }
    }
}

void MissionManager::TurnLimit(int arg_stageID, int arg_turn)
{
	for (auto& m : missions) {
		if (m.stageID == arg_stageID && m.type == MissionType::TURN_LIMIT) {
			if (arg_turn <= m.turnLimit) {
				m.isCleared = true;
			}
			// 一致しなければ現状維持
			break;
		}
	}
}

void MissionManager::ResetMissions(int stageID)
{
    for (auto& m : missions) {
        if (m.stageID == stageID) {
            m.isCleared = false;
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

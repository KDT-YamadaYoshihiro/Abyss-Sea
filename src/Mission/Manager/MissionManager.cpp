#include "MissionManager.h"
#include "Mission/Factory/MissionFactory.h"
#include "Character/Character.h"
#include "DxLib.h"

void MissionManager::LoadMissionTable(const std::string& csvFile) {
    missions = MissionFactory::LoadFromCSV(csvFile);
}

// ステージIDでミッションを取得
std::vector<MissionData> MissionManager::GetByStage(int stageID) const {
    std::vector<MissionData> result;
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

void MissionManager::IncludeCharacter(int arg_stageID, std::vector<std::shared_ptr<Character>> arg_chara)
{
    for (auto& m : missions) {
        // ステージクリアミッションがクリア済みならリセットしない
        if (m.stageID == arg_stageID && m.type == MissionType::CLEAR_QUEST && m.isCleared == true) {
            if (m.stageID == arg_stageID && m.type == MissionType::INCLUDE_CHARACTER && m.isCleared == true)
            {
                return;
            }
        }

        if (m.stageID == arg_stageID && m.type == MissionType::INCLUDE_CHARACTER) {

			// キャラクターが編成にいるか確認
            bool found = false;

			// 編成キャラを一人ずつ確認
            for (auto& c : arg_chara) {
                if (c->getId() == m.targetID) {
                    found = true;
                    break;
                }
            }

            m.isCleared = found;  // 最後に結果をまとめて反映
            break;
        }
    }
}

void MissionManager::TurnLimit(int arg_stageID, int arg_turn)
{


	for (auto& m : missions) {

        // ステージクリアミッションがクリア済みならリセットしない
        if (m.stageID == arg_stageID && m.type == MissionType::CLEAR_QUEST && m.isCleared == true){
            if (m.stageID == arg_stageID && m.type == MissionType::TURN_LIMIT && m.isCleared == true) {

                return;
            }
        }

		if (m.stageID == arg_stageID && m.type == MissionType::TURN_LIMIT) {
			
            // 指定ターン以内にクリアしていればクリア
            if (arg_turn <= m.turnLimit) {
				m.isCleared = true;
            }
            else {
				// クリアしていなければ現状維持
                m.isCleared = false;
            }
			// 一致しなければ現状維持
			break;
		}
	}
}

void MissionManager::ResetMissions(int arg_stageID)
{
    for (auto& m : missions) {

		// ステージクリアミッションがクリア済みならリセットしない
        if (m.stageID == arg_stageID && m.type == MissionType::CLEAR_QUEST && m.isCleared == true) {
            return;
        }

        if (m.stageID == arg_stageID) {
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

#pragma once
#include <string>
#include <vector>

#include "enum.h"

// Missionクラス
class Mission {

    MissionType mission_type;
    int targetId;      // INCLUDE_CHARACTERで使用
    int limitTurn;     // TURN_LIMITで使用
    bool isCleared;    // 達成状態


public:

    // 初期化
    Mission(MissionType arg_type, int arg_targetId = -1, int arg_limitTurn = -1)
        : mission_type(arg_type), targetId(arg_targetId), limitTurn(arg_limitTurn), isCleared(false) {
    }

    // ミッション内容のゲット関数
    std::string getDescription() const {
        switch (mission_type) {
        case MissionType::CLEAR_QUEST:
            return "クエストをクリアする";
        case MissionType::INCLUDE_CHARACTER:
            return "特定キャラクター(ID:" + std::to_string(targetId) + ")を編成する";
        case MissionType::TURN_LIMIT:
            return std::to_string(limitTurn) + "ターン以内にクリアする";
        default:
            return "不明なミッション";
        }
    }

	// ミッション達成条件のチェック関数
    void checkClear(bool questCleared, const std::vector<int>& partyIds, int usedTurn) {
        switch (mission_type) {
        case MissionType::CLEAR_QUEST:
            isCleared = questCleared;
            break;

        case MissionType::INCLUDE_CHARACTER:
            for (auto id : partyIds) {
                if (id == targetId) {
                    isCleared = true;
                    break;
                }
            }
            break;

        case MissionType::TURN_LIMIT:
            if (questCleared && usedTurn <= limitTurn)
                isCleared = true;
            break;
        }
    }

    bool getIsCleared() const { return isCleared; }

};
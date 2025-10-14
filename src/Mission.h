#pragma once
#include <string>
#include <vector>

// ミッションタイプ
enum class MissionType {
    CLEAR_QUEST,       // クエストクリア
    INCLUDE_CHARACTER, // 特定キャラクターを編成
    TURN_LIMIT,         // 指定ターン以内にクリア
    UNKNOWN             // 不明
};

// ミッションデータ構造体
struct Mission {
    int stageID;
    MissionType type;
    int targetID;
    int turnLimit;
    std::string description;
    bool isCleared = false;
};


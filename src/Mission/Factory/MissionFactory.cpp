#include "MissionFactory.h"
#include <fstream>
#include <sstream>

MissionType ToMissionType(const std::string& str) {
    if (str == "CLEAR_STAGE") return MissionType::CLEAR_QUEST;
    if (str == "HAVE_CHARACTER") return MissionType::INCLUDE_CHARACTER;
    if (str == "TURN_LIMIT") return MissionType::TURN_LIMIT;
    return MissionType::UNKNOWN;
}

std::vector<MissionData> MissionFactory::LoadFromCSV(const std::string& filename) {
    std::vector<MissionData> missions;
    std::ifstream file(filename);
    if (!file.is_open()) return missions;

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
        if (isHeader) {  // 最初の行はヘッダーとしてスキップ
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string stageID, typeStr, targetID, turnLimit, desc;

        std::getline(ss, stageID, ',');
        std::getline(ss, typeStr, ',');
        std::getline(ss, targetID, ',');
        std::getline(ss, turnLimit, ',');
        std::getline(ss, desc, ',');

        MissionData m;
        m.stageID = std::stoi(stageID);
        m.type = ToMissionType(typeStr);
        m.targetID = std::stoi(targetID);
        m.turnLimit = std::stoi(turnLimit);
        m.description = desc;
        m.isCleared = false;

        missions.push_back(m);
    }

    return missions;
}

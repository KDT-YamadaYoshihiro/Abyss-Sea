#pragma once
#include <vector>
#include "Mission.h"

class MissionManager {
private:
    std::vector<Mission> missions;

public:
    void Init(const std::string& csvFile);
    void DrawMissions(int stageID, int x, int y) const;

    const std::vector<Mission>& GetAll() const { return missions; }
    std::vector<Mission> GetByStage(int stageID) const;
    void SetCleared(int stageID, MissionType type, bool cleared);
};

#pragma once
#include <vector>
#include <string>
#include "Mission.h"

class MissionFactory {
private:
	// ミッションリスト
    std::vector<Mission> missions;

	// コンストラクタ
	MissionFactory() = default;

public:

	// デストラクタ
	virtual ~MissionFactory() {};

	// シングルトン
	MissionFactory(const MissionFactory&) = delete;
	MissionFactory& operator = (const MissionFactory&) = delete;
	static MissionFactory& Instance() {
		static MissionFactory instance;
		return instance;
	}


	// CSVからミッションデータを読み込み
    static std::vector<Mission> LoadFromCSV(const std::string& filename);
    // ステージIDに応じたミッションリストを取得
    const std::vector<Mission>& GetMissions(int stageID) const;
	// ミッションの描画
    void DrawMissions(int stageID, int x, int y) const;

    // ミッション状態更新
    void SetCleared(int missionID, bool cleared);
};

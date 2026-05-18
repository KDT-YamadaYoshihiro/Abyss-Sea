#pragma once
#include <vector>
#include <string>
#include "Mission/MissionData.h"

class MissionFactory {
private:
	// ミッションリスト
    std::vector<MissionData> missions;

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
    static std::vector<MissionData> LoadFromCSV(const std::string& filename);
};

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
};

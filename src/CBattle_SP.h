#pragma once

// スタートスキルポイント
#define START_SP	3
// MAXスキルポイント（定数）
#define MAX_SP	5

class BattleSP {
private:

    // 現在のSP
    int SP = START_SP;
    // 最大SP
    const int maxSP = MAX_SP;

public:
    // スキル使用可能かチェック
    bool comfirmSP() const { return this->SP > 0; }
    // 現在のSPの取得
    int getSP() const { return SP; }
    // 最大SPの取得
    int getMaxSP() const { return maxSP; }


    // スキル使用時、SPの消費
    void consumeSP() {
        if (!comfirmSP()) { return; }
        SP--;
    }

    // 通常攻撃時、SPの上昇
    void recoverSP() {
        if (SP < maxSP) { SP++;}
    }

    // SPの初期化
    void ResetSP() { SP = START_SP; }

};
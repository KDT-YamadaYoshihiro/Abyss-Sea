#pragma once
#include <vector>
#include "DxLib.h"
#include "Constants/define.h" 

class BubbleEffect {
private:
    // 個々の泡のデータ構造
    struct Bubble {
        float x, y;          // 現在の座標
        float baseX;         // 左右に揺れる基準となるX座標
        float radius;        // 泡の半径
        float speed;         // 上昇速度
        float angle;         // 揺れ計算用の角度（ラジアン）
        float swaySpeed;     // 揺れる速度
        float swayWidth;     // 揺れる横幅
    };

    std::vector<Bubble> m_bubbles; // 画面内の泡リスト
    bool m_isActive = false;       // エフェクトが再生中か

    // 再生用のパラメータ保持
    float m_baseRadius = 0.0f;
    int   m_density = 0;
    float m_baseSpeed = 0.0f;
    int   m_coolTime = 0;
    int   m_timer = 0;             // クールタイム管理用カウンター

    // 泡を1個生成する内部メソッド
    void CreateBubble();

public:
    BubbleEffect() = default;
    ~BubbleEffect() = default;

    // 再生（泡の基準半径、1回あたりの発生密度、上昇速度、発生クールタイムフレーム）
    void Play(float arg_radius, int arg_density, float arg_speed, int arg_coolTime);

    // 更新処理
    void Update();

    // 停止（新規発生を止め、画面内の泡が消えたら完全終了）
    // ※引数に true を渡すと画面内の泡も即座に全消去します
    void Stop(bool immediate = false);

    // 描画処理
    void Render();

    // エフェクトが完全に終了したか（フェード終了判定などに使用可能）
    bool IsFinished() const { return !m_isActive && m_bubbles.empty(); }
};
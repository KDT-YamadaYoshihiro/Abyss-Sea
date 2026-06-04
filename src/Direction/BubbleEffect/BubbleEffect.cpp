#include "BubbleEffect.h"
#include <cmath>
#include <cstdlib>

// 泡を1個生成する
void BubbleEffect::CreateBubble() {
    Bubble b;
    // 画面の横幅全体からランダムにベース位置を決定
    b.baseX = static_cast<float>(rand() % WINDOW_W);
    // 画面の一番下（画面外）から発生させる
    b.y = static_cast<float>(WINDOW_H + 20);

    // パラメータに少しランダムなバラつきを持たせて自然に見せる
    b.radius = m_baseRadius * (0.5f + (rand() % 100) / 100.0f); // 半径は50%〜150%の幅
    b.speed = m_baseSpeed * (0.8f + (rand() % 50) / 100.0f);    // 速度は80%〜130%の幅

    b.angle = static_cast<float>(rand() % 360) * 3.141592f / 180.0f; // 初期角度をランダムに
    b.swaySpeed = 0.03f + (rand() % 50) / 1000.0f;              // 揺れの周期
    b.swayWidth = b.radius * 0.5f + (rand() % 15);              // 半径に応じた揺れ幅

    b.x = b.baseX + sinf(b.angle) * b.swayWidth;

    m_bubbles.push_back(b);
}

// 再生開始
void BubbleEffect::Play(float arg_radius, int arg_density, float arg_speed, int arg_coolTime) {
    m_isActive = true;
    m_baseRadius = arg_radius;
    m_density = arg_density;
    m_baseSpeed = arg_speed;
    m_coolTime = arg_coolTime;
    m_timer = 0;
}

// 更新
void BubbleEffect::Update() {
    // 再生中の場合、クールタイムを計算して泡を生成
    if (m_isActive) {
        if (m_timer <= 0) {
            for (int i = 0; i < m_density; ++i) {
                CreateBubble();
            }
            m_timer = m_coolTime;
        }
        m_timer--;
    }

    // 泡の移動・削除ループ
    for (auto it = m_bubbles.begin(); it != m_bubbles.end();) {
        // 上昇
        it->y -= it->speed;

        // 左右にゆらゆら揺らす
        it->angle += it->swaySpeed;
        it->x = it->baseX + sinf(it->angle) * it->swayWidth;

        // 画面上部（完全に画面外）に出たら削除
        if (it->y < -(it->radius * 2)) {
            it = m_bubbles.erase(it);
        }
        else {
            ++it;
        }
    }
}

// 停止
void BubbleEffect::Stop(bool immediate) {
    m_isActive = false;
    if (immediate) {
        m_bubbles.clear();
    }
}

// 描画
void BubbleEffect::Render() {
    if (m_bubbles.empty()) return;

    // 泡っぽさを出すためにアルファブレンドを使用
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);

    unsigned int color = GetColor(240, 255, 255); // ほんのり青みがかった白

    for (const auto& b : m_bubbles) {
        // 1. 泡の輪郭（外側の線）を描画
        DrawCircle(static_cast<int>(b.x), static_cast<int>(b.y), static_cast<int>(b.radius), color, FALSE);

        // 2. 泡の立体感を出すため、内側に少し小さな塗りつぶし（超薄い半透明）を重ねる
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
        DrawCircle(static_cast<int>(b.x), static_cast<int>(b.y), static_cast<int>(b.radius * 0.8f), color, TRUE);

        // 3. ハイライト（左上にちょっとだけ白い点を入れると急に球体・水滴っぽくなります）
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
        DrawCircle(static_cast<int>(b.x - b.radius * 0.3f), static_cast<int>(b.y - b.radius * 0.3f), static_cast<int>(b.radius * 0.15f + 1), GetColor(255, 255, 255), TRUE);

        // ブレンドモードを戻す
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
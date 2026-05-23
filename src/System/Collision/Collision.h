#pragma once
#include "DxLib.h" 

namespace Collision {

    // ボックス当たり判定
    static bool CheckBoxClick(int x, int y, int w, int h) {
        // マウス座標を直接取得
        int mx, my;
        GetMousePoint(&mx, &my); 
        return (mx >= x && mx <= x + w && my >= y && my <= y + h);
    }

    // 円当たり判定 
    static bool CheckCircleClick(int cx, int cy, float r) {
        // マウス座標を直接取得
        int mx, my;
        GetMousePoint(&mx, &my); 
        float dx = (float)(mx - cx);
        float dy = (float)(my - cy);
        return (dx * dx + dy * dy) <= (r * r);
    }

}
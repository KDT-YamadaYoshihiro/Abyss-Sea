#include "Mouse.h"

namespace Mouse {
    // =====================
    // 円がクリックされたか
    // =====================
    bool CheckCircleClick(float circleX, float circleY, float radius) {

        // マウスの座標を取得する
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);

        // クリック判定
        if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { // 左クリックが押された場合
            int distanceSquared = (mouseX - circleX) * (mouseX - circleX) + (mouseY - circleY) * (mouseY - circleY);
            if (distanceSquared <= radius * radius) {
                // クリックされた場所が円の範囲内であればtrueをreturnする
                return true;
            }

        }
        return false;

    }

    // ========================
    // 四角がクリックされたとき
    // ========================
    bool CheckBoxClick(float boxX, float boxY, float boxSizeX, float boxSizeY) {

        // マウスの座標を取得する
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);

        // クリック判定
        if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { // 左クリックが押された場合

            if (mouseX >= boxX && mouseX <= boxX + boxSizeX &&
                mouseY >= boxY && mouseY <= boxY + boxSizeY)
            {
                // クリックされた場所が四角の範囲内であればtrueをreturnする
                return true;
            }
        }
        return false;

    }

}
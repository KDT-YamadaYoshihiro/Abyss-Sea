#include "DxLib.h"

namespace Mouse {
    // =====================
    // 円がクリックされたか
    // =====================
    bool CheckCircleClick(float circleX, float circleY, float radius);
    // ========================
    // 四角がクリックされたとき
    // ========================
    bool CheckBoxClick(float boxX, float boxY, float boxSizeX, float boxSizeY);
}
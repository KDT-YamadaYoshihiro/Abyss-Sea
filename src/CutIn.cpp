#include "Cutin.h"

// スタート関数（座標指定、速度指定）
void Cutin::Start(int arg_x, int arg_y, int arg_handle, std::string arg_skillName, int arg_enterSpeed, int arg_pauseSpeed, int arg_exitSpeed)
{
    // エンドフラグリセット
    m_endFlag = false;

    // 位置
    m_pos_x = arg_x;
    m_pos_y = arg_y;

    // 画像ハンドル
    m_handle = arg_handle;

    // 名前
    m_skill_name = arg_skillName;

    // 速度
    m_enterSpeed = arg_enterSpeed;
    m_pauseSpeed = arg_pauseSpeed;
    m_exitSpeed = arg_exitSpeed;

	m_phase = CutinPhase::Entering;
	m_frameCounter = 0;

	// 画像フラグをオン
	m_drawFlag = true;
}

// リセット関数（再利用を考慮）
void Cutin::Reset()
{
	m_drawFlag = false;
	m_pos_x = 0;
	m_pos_y = 0;
}

// 更新
void Cutin::Update()
{

    if (!m_drawFlag) return;

    switch (m_phase)
    {
    case CutinPhase::Entering:
        m_pos_y += m_enterSpeed;

        // 画面中央に到達したら減速（位置 or フレームで判断）
        if ((m_exitSpeed > 0 && m_pos_y + m_size_h > WINDOW_H / 2) || (m_exitSpeed < 0 && m_pos_y < WINDOW_H / 2)) {  // 中央の例：1280pxの中央
            m_phase = CutinPhase::Pausing;
            m_frameCounter = 0;
        }
        break;

    case CutinPhase::Pausing:
        m_pos_y += m_pauseSpeed;
        m_frameCounter++;

        // しばらくしたら退場フェーズへ（30フレーム例）
        if (m_frameCounter > 30) {
            m_phase = CutinPhase::Exiting;
        }
        break;

    case CutinPhase::Exiting:
        m_pos_y += m_exitSpeed;

        // 画面外に出たら終了
        if ((m_exitSpeed > 0 && m_pos_y > WINDOW_H) || (m_exitSpeed < 0 && m_pos_y > WINDOW_H)) {
            m_drawFlag = false;
			m_endFlag = true;
        }
        break;
    }
    
}

void Cutin::Draw()
{
    if (m_drawFlag) {
        // 背景（三角形二つを使用して台形に）
        {
            int x = m_pos_x + m_size_w / 2;
            int y = m_pos_y + m_size_h;
            DrawTriangle(m_pos_x, m_pos_y, m_pos_x + m_size_w, m_pos_y, x, y, GetColor(255, 255, 255), true);
            int y2 = m_pos_y + m_size_h;
            DrawTriangle(m_pos_x, m_pos_y, m_pos_x - m_size_w / 2, y2, m_pos_x + m_size_w / 2, y2, GetColor(255, 255, 255), true);
        }
        // 画像描画
        int const ICON_SIZE = m_size_w;
        int y = m_pos_y + (m_size_h / 2 - ICON_SIZE / 2);

        DrawExtendGraph(m_pos_x, y, m_pos_x + ICON_SIZE, y + ICON_SIZE, m_handle, true);

        // スキル名表示
        SetFontSize(20);
        DrawFormatString(m_pos_x + 20, m_pos_y - 20, GetColor(255, 255, 255), m_skill_name.c_str());
    }

}

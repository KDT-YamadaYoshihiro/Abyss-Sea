#include "Cutin.h"

// スタート関数（座標指定、速度指定）
void Cutin::Start(int arg_x, int arg_y, int arg_handle, std::string arg_skillName, int arg_enterSpeed, int arg_pauseSpeed, int arg_exitSpeed)
{
    // エンドフラグリセット
    end_flag = false;

    // 位置
    pos_x = arg_x;
    pos_y = arg_y;

    // 画像ハンドル
    handle = arg_handle;

    // 名前
    skill_name = arg_skillName;

    // 速度
    enter_speed = arg_enterSpeed;
    pause_speed = arg_pauseSpeed;
    exit_speed = arg_exitSpeed;

	phase = CUTIN_PHASE::ENTERING;
	frame_counter = 0;

	// 画像フラグをオン
	draw_flag = true;

	// エフェクトのスタート関数
	cutin_effect->effectStart();

}

// リセット関数（再利用を考慮）
void Cutin::Reset()
{
	draw_flag = false;
	pos_x = 0;
	pos_y = 0;
}

// 更新
void Cutin::Update()
{
    if (!draw_flag)
    {
        return;
    }

	// フェーズごとの動作
    switch (phase)
    {
    case CUTIN_PHASE::ENTERING:
        pos_y += enter_speed;

        // 画面中央に到達したら減速（位置 or フレームで判断）
        if ((exit_speed > 0 && pos_y + size_h > WINDOW_H / 2) || (exit_speed < 0 && pos_y < WINDOW_H / 2)) {  // 中央の例：1280pxの中央
            phase = CUTIN_PHASE::PAUSING;
            frame_counter = 0;
        }
        break;

    case CUTIN_PHASE::PAUSING:
        pos_y += pause_speed;
        frame_counter++;

        // しばらくしたら退場フェーズへ（30フレーム例）
        if (frame_counter > 30) {
            phase = CUTIN_PHASE::EXITING;
        }
        break;

    case CUTIN_PHASE::EXITING:
        pos_y += exit_speed;

        // 画面外に出たら終了
        if ((exit_speed > 0 && pos_y > WINDOW_H) || (exit_speed < 0 && pos_y > WINDOW_H)) {
            draw_flag = false;
			end_flag = true;
        }
        break;
    }

	// エフェクト更新
    cutin_effect->PlayAnim();
    
}

void Cutin::Draw()
{
    if (draw_flag) {

		// エフェクト描画
        int x = pos_x - 170;
		cutin_effect->effectDraw(x, 0, 0, 0);

        // 背景（三角形二つを使用して台形に）
        {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); 
            int x = pos_x + size_w / 2;
            int y = pos_y + size_h;
            DrawTriangle(pos_x, pos_y, pos_x + size_w, pos_y, x, y, GetColor(255, 255, 255), true);
            int y2 = pos_y + size_h;
            DrawTriangle(pos_x, pos_y, pos_x - size_w / 2, y2, pos_x + size_w / 2, y2, GetColor(255, 255, 255), true);
			// ブレンドモードを元に戻す
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
        // 画像描画
        int const ICON_SIZE = size_w;
        int y = pos_y + (size_h / 2 - ICON_SIZE / 2);

        DrawExtendGraph(pos_x, y, pos_x + ICON_SIZE, y + ICON_SIZE, handle, true);

        //// スキル名表示
        //SetFontSize(20);
        //DrawFormatString(m_pos_x + 20, m_pos_y - 20, GetColor(255, 255, 255), m_skill_name.c_str());
    }

}

//---------------------------------------------------------------------------
//!	@file	InputPad.h
//! @brief	パッド入力管理
//---------------------------------------------------------------------------
#pragma once
#include <DxLib.h>
#include "typedef.h"

//---------------------------------------------------------------------------
//! パッド種別
//---------------------------------------------------------------------------
//@{

enum class PAD_NO : unsigned int
{
    PAD_NO1 = 0,
    PAD_NO2,
    PAD_NO3,
    PAD_NO4,

    PAD_NO_MAX
};

//@}
//---------------------------------------------------------------------------
//! ボタン種別
//---------------------------------------------------------------------------
//@{
enum class PAD_ID : unsigned int
{
    PAD_A = 0,    // Aキー
    PAD_B,        // Bキー
    PAD_X,        // Xキー
    PAD_Y,        // Yキー
    PAD_L,        // LBボタン（左人差し指側）
    PAD_R,        // RBボタン（右人差し指側）
    PAD_START,    // STARTボタン
    PAD_BACK,     // BACKボタン
    PAD_L_PUSH,   // 左アナログスティック押し込み
    PAD_R_PUSH,   // 右アナログスティック押し込み

    // 左アナログスティック
    PAD_UP,      // 上
    PAD_DOWN,    // 下
    PAD_LEFT,    // 左
    PAD_RIGHT,   // 右

    // 右アナログスティック
    PAD_R_UP,      // 上
    PAD_R_DOWN,    // 下
    PAD_R_LEFT,    // 左
    PAD_R_RIGHT,   // 右

    // 方向キー
    PAD_D_UP,      // 上
    PAD_D_RIGHT,   // 右
    PAD_D_DOWN,    // 下
    PAD_D_LEFT,    // 左
    PAD_D_ALL,     // 方向キー全て

    PAD_ID_MAX,

    // 場所不明
    PAD_C,   // 対応キーなし
    PAD_Z,   // 対応キーなし
    PAD_M,   // 対応キーなし

};

//@}
//===========================================================================
//!	@name	システム関数
//===========================================================================
//@{

//! 初期化
void InputPadInit();

//! 更新
void InputPadUpdate();

//! 終了
void InputPadExit();

//@}
//===========================================================================
//!	@name	入力関連関数
//===========================================================================
//@{

//! 指定パッドのボタンの1回だけ押下検証
//! @param	[in]	pad_id	パッドのボタン種別
//! @param	[in]	pad_no	パッドの種別(指定がなければPAD_NO1)
//! @retval true	引数に指定されたパッドのボタンが押された
//! @retval false	引数に指定されたパッドのボタンが押されていない
bool IsPadOn(PAD_ID pad_id, PAD_NO pad_no = PAD_NO::PAD_NO1);

//! 指定パッドのボタンの1回だけ押下検証
//! @param	[in]	pad_id	パッドのボタン種別
//! @param	[in]	pad_no	パッドの種別(指定がなければPAD_NO1)
//! @retval true	引数に指定されたパッドのボタンが離されている
//! @retval false	引数に指定されたパッドのボタンが押されている
bool IsPadRelease(PAD_ID pad_id, PAD_NO pad_no = PAD_NO::PAD_NO1);

//! 指定パッドのボタンの1回だけ押下検証
//! @param	[in]	pad_id	パッドのボタン種別
//! @param	[in]	pad_no	パッドの種別(指定がなければPAD_NO1)
//! @retval true	引数に指定されたパッドのボタンが長押し（2フレーム以上）されている
//! @retval false	引数に指定されたパッドのボタンが押されていない（押されているフレーム数が1以下）
bool IsPadRepeat(PAD_ID pad_id, PAD_NO pad_no = PAD_NO::PAD_NO1);

////////////////////////////////////////////
//		新規追加
////////////////////////////////////////////
//! 指定パッドの入力状態の詳細を取得
//! @param	[in]	pad_state	パッドの入力状態の詳細（参照）
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval true	成功
//! @retval false	失敗
bool GetPadInputState(DINPUT_JOYSTATE& pad_state, PAD_NO pad_no = PAD_NO::PAD_NO1);

//! 接続されているパッドの数を取得
//! @param	[in]	なし
//! @retval s32		-1: 接続されているパッドなし / それ以外: 接続されているパッドの数
s32 GetConnecetdPadNum();

//! 指定パッドの左アナログスティックのX方向の入力値を取得
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval f32	-1（左に100%傾いている）～0（傾きなし）～1（右に100％傾いている）
f32 GetPadInputAnalogLX(PAD_NO pad_no = PAD_NO::PAD_NO1);
//! 指定パッドの左アナログスティックのY方向の入力値を取得
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval f32	-1（上に100%傾いている）～0（傾きなし）～1（下に100％傾いている）
f32 GetPadInputAnalogLY(PAD_NO pad_no = PAD_NO::PAD_NO1);

//! 指定パッドの右アナログスティックのX方向の入力値を取得
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval f32	-1（左に100%傾いている）～0（傾きなし）～1（右に100％傾いている）
f32 GetPadInputAnalogRX(PAD_NO pad_no = PAD_NO::PAD_NO1);
//! 指定パッドの右アナログスティックのY方向の入力値を取得
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval f32	-1（上に100%傾いている）～0（傾きなし）～1（下に100％傾いている）
f32 GetPadInputAnalogRY(PAD_NO pad_no = PAD_NO::PAD_NO1);

//! 指定パッドの左アナログスティックの方向を取得
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval s32		-1: 入力なし / 0: 左 / 1: 右 / 2: 上 / 3: 下
s32 GetPadInputAnalogLDirection(PAD_NO pad_no = PAD_NO::PAD_NO1);

// Debug
//void InputPadRender();

//@}
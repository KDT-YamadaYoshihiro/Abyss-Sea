//---------------------------------------------------------------------------
//!	@file	InputPad.h
//! @brief	�p�b�h���͊Ǘ�
//---------------------------------------------------------------------------
#pragma once
#include <DxLib.h>
#include "typedef.h"

//---------------------------------------------------------------------------
//! �p�b�h���
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
//! �{�^�����
//---------------------------------------------------------------------------
//@{
enum class PAD_ID : unsigned int
{
    PAD_A = 0,    // A�L�[
    PAD_B,        // B�L�[
    PAD_X,        // X�L�[
    PAD_Y,        // Y�L�[
    PAD_L,        // LB�{�^���i���l�����w���j
    PAD_R,        // RB�{�^���i�E�l�����w���j
    PAD_START,    // START�{�^��
    PAD_BACK,     // BACK�{�^��
    PAD_L_PUSH,   // ���A�i���O�X�e�B�b�N��������
    PAD_R_PUSH,   // �E�A�i���O�X�e�B�b�N��������

    // ���A�i���O�X�e�B�b�N
    PAD_UP,      // ��
    PAD_DOWN,    // ��
    PAD_LEFT,    // ��
    PAD_RIGHT,   // �E

    // �E�A�i���O�X�e�B�b�N
    PAD_R_UP,      // ��
    PAD_R_DOWN,    // ��
    PAD_R_LEFT,    // ��
    PAD_R_RIGHT,   // �E

    // �����L�[
    PAD_D_UP,      // ��
    PAD_D_RIGHT,   // �E
    PAD_D_DOWN,    // ��
    PAD_D_LEFT,    // ��
    PAD_D_ALL,     // �����L�[�S��

    PAD_ID_MAX,

    // �ꏊ�s��
    PAD_C,   // �Ή��L�[�Ȃ�
    PAD_Z,   // �Ή��L�[�Ȃ�
    PAD_M,   // �Ή��L�[�Ȃ�

};

//@}
//===========================================================================
//!	@name	�V�X�e���֐�
//===========================================================================
//@{

//! ������
void InputPadInit();

//! �X�V
void InputPadUpdate();

//! �I��
void InputPadExit();

//@}
//===========================================================================
//!	@name	���͊֘A�֐�
//===========================================================================
//@{

//! �w��p�b�h�̃{�^����1�񂾂���������
//! @param	[in]	pad_id	�p�b�h�̃{�^�����
//! @param	[in]	pad_no	�p�b�h�̎��(�w�肪�Ȃ����PAD_NO1)
//! @retval true	�����Ɏw�肳�ꂽ�p�b�h�̃{�^���������ꂽ
//! @retval false	�����Ɏw�肳�ꂽ�p�b�h�̃{�^����������Ă��Ȃ�
bool IsPadOn(PAD_ID pad_id, PAD_NO pad_no = PAD_NO::PAD_NO1);

//! �w��p�b�h�̃{�^����1�񂾂���������
//! @param	[in]	pad_id	�p�b�h�̃{�^�����
//! @param	[in]	pad_no	�p�b�h�̎��(�w�肪�Ȃ����PAD_NO1)
//! @retval true	�����Ɏw�肳�ꂽ�p�b�h�̃{�^����������Ă���
//! @retval false	�����Ɏw�肳�ꂽ�p�b�h�̃{�^����������Ă���
bool IsPadRelease(PAD_ID pad_id, PAD_NO pad_no = PAD_NO::PAD_NO1);

//! �w��p�b�h�̃{�^����1�񂾂���������
//! @param	[in]	pad_id	�p�b�h�̃{�^�����
//! @param	[in]	pad_no	�p�b�h�̎��(�w�肪�Ȃ����PAD_NO1)
//! @retval true	�����Ɏw�肳�ꂽ�p�b�h�̃{�^�����������i2�t���[���ȏ�j����Ă���
//! @retval false	�����Ɏw�肳�ꂽ�p�b�h�̃{�^����������Ă��Ȃ��i������Ă���t���[������1�ȉ��j
bool IsPadRepeat(PAD_ID pad_id, PAD_NO pad_no = PAD_NO::PAD_NO1);

////////////////////////////////////////////
//		�V�K�ǉ�
////////////////////////////////////////////
//! �w��p�b�h�̓��͏�Ԃ̏ڍׂ��擾
//! @param	[in]	pad_state	�p�b�h�̓��͏�Ԃ̏ڍׁi�Q�Ɓj
//! @param	[in]	pad_no		�p�b�h�̎��(�w�肪�Ȃ����PAD_NO1)
//! @retval true	����
//! @retval false	���s
bool GetPadInputState(DINPUT_JOYSTATE& pad_state, PAD_NO pad_no = PAD_NO::PAD_NO1);

//! �ڑ�����Ă���p�b�h�̐����擾
//! @param	[in]	�Ȃ�
//! @retval s32		-1: �ڑ�����Ă���p�b�h�Ȃ� / ����ȊO: �ڑ�����Ă���p�b�h�̐�
s32 GetConnecetdPadNum();

//! �w��p�b�h�̍��A�i���O�X�e�B�b�N��X�����̓��͒l���擾
//! @param	[in]	pad_no		�p�b�h�̎��(�w�肪�Ȃ����PAD_NO1)
//! @retval f32	-1�i����100%�X���Ă���j�`0�i�X���Ȃ��j�`1�i�E��100���X���Ă���j
f32 GetPadInputAnalogLX(PAD_NO pad_no = PAD_NO::PAD_NO1);
//! �w��p�b�h�̍��A�i���O�X�e�B�b�N��Y�����̓��͒l���擾
//! @param	[in]	pad_no		�p�b�h�̎��(�w�肪�Ȃ����PAD_NO1)
//! @retval f32	-1�i���100%�X���Ă���j�`0�i�X���Ȃ��j�`1�i����100���X���Ă���j
f32 GetPadInputAnalogLY(PAD_NO pad_no = PAD_NO::PAD_NO1);

//! �w��p�b�h�̉E�A�i���O�X�e�B�b�N��X�����̓��͒l���擾
//! @param	[in]	pad_no		�p�b�h�̎��(�w�肪�Ȃ����PAD_NO1)
//! @retval f32	-1�i����100%�X���Ă���j�`0�i�X���Ȃ��j�`1�i�E��100���X���Ă���j
f32 GetPadInputAnalogRX(PAD_NO pad_no = PAD_NO::PAD_NO1);
//! �w��p�b�h�̉E�A�i���O�X�e�B�b�N��Y�����̓��͒l���擾
//! @param	[in]	pad_no		�p�b�h�̎��(�w�肪�Ȃ����PAD_NO1)
//! @retval f32	-1�i���100%�X���Ă���j�`0�i�X���Ȃ��j�`1�i����100���X���Ă���j
f32 GetPadInputAnalogRY(PAD_NO pad_no = PAD_NO::PAD_NO1);

//! �w��p�b�h�̍��A�i���O�X�e�B�b�N�̕������擾
//! @param	[in]	pad_no		�p�b�h�̎��(�w�肪�Ȃ����PAD_NO1)
//! @retval s32		-1: ���͂Ȃ� / 0: �� / 1: �E / 2: �� / 3: ��
s32 GetPadInputAnalogLDirection(PAD_NO pad_no = PAD_NO::PAD_NO1);

// Debug
//void InputPadRender();

//@}
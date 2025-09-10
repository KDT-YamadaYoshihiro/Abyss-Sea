//---------------------------------------------------------------------------
//!	@file	InputPad.cpp
//! @brief	�p�b�h���͊Ǘ�
//---------------------------------------------------------------------------
#include "typedef.h"
#include "InPutPad.h"
#include <vector>
#include <array>

namespace
{
    constexpr unsigned int MAX_PAD_NUM = static_cast<unsigned int>(PAD_NO::PAD_NO_MAX);   //<! PAD�ڑ��ő吔
    constexpr unsigned int MAX_PAD_TYPE = static_cast<unsigned int>(
        PAD_ID::PAD_ID_MAX);   //<! �㉺���E/A B C X Y Z L R START M�{�^��
    constexpr unsigned int MIN_PAD_TYPE = static_cast<unsigned int>(
        PAD_ID::PAD_A);                                        //<! �㉺���E/A B C X Y Z L R START M�{�^��
    constexpr unsigned int MAX_PAD_DIRECT_NUM = 4;         //<! �����L�[�̕����ő�l
    constexpr float        MAX_ANALOG_INPUT_VALUE = 1000.0f;   //<! �A�i���O�X�e�B�b�N�̓��͒l�̍ő�l
    constexpr unsigned int PAD_BUTTON_NUM = static_cast<unsigned int>(PAD_ID::PAD_R_PUSH) -
        static_cast<unsigned int>(PAD_ID::PAD_A) + 1;

    std::vector<DINPUT_JOYSTATE>  pad_input_states;   //<! �ڍׂȃp�b�h�̏�Ԃ̎擾
    std::array<bool, MAX_PAD_NUM> use_pads;           //<! �g�p���Ă���p�b�h�̏�Ԃ��Ǘ�
    std::array<std::array<unsigned int, PAD_BUTTON_NUM>, MAX_PAD_NUM>
        pad_buttons;   //<! �g�p���Ă���p�b�h�̃{�^���̉�����Ԃ��Ǘ�
    std::array<std::array<unsigned int, MAX_PAD_DIRECT_NUM>, MAX_PAD_NUM>
        pad_direct_keys;        //<! �g�p���Ă���p�b�h�̕����L�[�̉�����Ԃ��Ǘ�
    bool is_all_on_direct_key;   //<! �����L�[�̂����ꂩ��������Ă��邩�ǂ���

    // �p�b�h�ԍ��͈̔͊O�`�F�b�N�֐�
    bool isOverPadNum(s32 pad_num)
    {
        return (pad_num < 0 || pad_num >= MAX_PAD_NUM);
    }
    // ���̃p�b�h�ԍ����g�p�����ǂ���
    bool isUsePadNum(s32 pad_num)
    {
        if (isOverPadNum(pad_num))
            return false;
        return use_pads[pad_num];
    }
    // ���̃p�b�hID�͈͓̔��`�F�b�N
    bool isOverPadID(u32 pad_id)
    {
        return (pad_id < MIN_PAD_TYPE || pad_id >= MAX_PAD_TYPE);
    }
};   // namespace

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
void InputPadInit()
{
    int connect_pad_num = GetJoypadNum();
    pad_input_states.resize(connect_pad_num);
    std::fill(use_pads.begin(), use_pads.end(), false);
    // �g�p���̃p�b�h�����ׂēo�^
    std::fill(use_pads.begin(), use_pads.begin() + connect_pad_num, true);

    is_all_on_direct_key = false;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void InputPadUpdate()
{
    // �p�b�h�̓��͏�Ԃ��ڍׂɎ擾
    for (int i = 0; i < MAX_PAD_NUM; ++i) {
        if (isUsePadNum(i) == false)
            continue;
        GetJoypadDirectInputState(DX_INPUT_PAD1 + i, &pad_input_states[i]);
    }

    for (int i = 0; i < MAX_PAD_NUM; ++i) {
        if (isUsePadNum(i) == false)
            continue;

        // �e�{�^����������Ă���t���[�������v�Z
        DINPUT_JOYSTATE input_state = pad_input_states[i];
        for (int j = 0; j < PAD_BUTTON_NUM; ++j) {
            if (input_state.Buttons[j] == 128) {
                ++pad_buttons[i][j];
                continue;
            }
            pad_buttons[i][j] = 0;
        }
        // �����L�[�̊e������������Ă���t���[�������v�Z
        int pov_0_index = ((input_state.POV[0] + 4500) % 36000) / 9000;
        if (input_state.POV[0] != -1) {
            pad_direct_keys[i][pov_0_index]++;
            continue;
        }
        else {
            pov_0_index = -1;
        }
        // �����L�[�̊e�����̉�����Ă��Ȃ��t���[���������Z�b�g
        for (int j = 0; j < MAX_PAD_DIRECT_NUM; ++j) {
            if (j == pov_0_index)
                continue;
            pad_direct_keys[i][j] = 0;
        }
    }
}

//---------------------------------------------------------------------------
//! �I��
//---------------------------------------------------------------------------
void InputPadExit()
{
    ;
}

//---------------------------------------------------------------------------
// ����p�b�h�{�^����1�񂾂���������
//---------------------------------------------------------------------------
bool IsPadOn(PAD_ID pad_id, PAD_NO pad_no)
{
    unsigned int pad_index = static_cast<unsigned int>(pad_no);
    if (isOverPadNum(pad_index))
        return false;
    if (!isUsePadNum(pad_index))
        return false;

    unsigned int id_index = static_cast<unsigned int>(pad_id);
    if (isOverPadID(id_index))
        return false;

    DINPUT_JOYSTATE input_state = pad_input_states[pad_index];
    //int             pov_0_index = ((input_state.POV[0] + 4500) % 36000) / 9000;
    unsigned int direct_id = static_cast<unsigned int>(pad_id) - static_cast<unsigned int>(PAD_ID::PAD_D_UP);

    bool all_repeat_direct_key = false;
    bool all_on_direct_key = false;
    for (int i = 0; i < MAX_PAD_DIRECT_NUM; ++i) {
        if (pad_direct_keys[pad_index][i] > 1) {
            all_repeat_direct_key = true;
            break;
        }
    }
    // �����L�[�̂����ꂩ�����łɉ�����Ă��Ȃ������ꍇ�A
    // �����L�[�̂����ꂩ�����t���O��false�ɂ���
    if (all_repeat_direct_key == false) {
        is_all_on_direct_key = false;
    }
    for (int i = 0; i < MAX_PAD_DIRECT_NUM; ++i) {
        if (pad_direct_keys[pad_index][i] != 1)
            continue;
        if (is_all_on_direct_key == true)
            continue;

        is_all_on_direct_key = true;
        all_on_direct_key = true;
        break;
    }

    switch (pad_id) {
        // �e�{�^��
    case PAD_ID::PAD_A:
    case PAD_ID::PAD_B:
    case PAD_ID::PAD_X:
    case PAD_ID::PAD_Y:
    case PAD_ID::PAD_L:
    case PAD_ID::PAD_R:
    case PAD_ID::PAD_START:
    case PAD_ID::PAD_BACK:
    case PAD_ID::PAD_L_PUSH:
    case PAD_ID::PAD_R_PUSH:
        return pad_buttons[pad_index][id_index] == 1;

        // ���A�i���O�X�e�B�b�N
    case PAD_ID::PAD_UP:
        return input_state.Y == -1;
    case PAD_ID::PAD_DOWN:
        return input_state.Y == 1;
    case PAD_ID::PAD_LEFT:
        return input_state.X == -1;
    case PAD_ID::PAD_RIGHT:
        return input_state.X == 1;
        // �E�A�i���O�X�e�B�b�N
    case PAD_ID::PAD_R_UP:
        return input_state.Ry == -1;
    case PAD_ID::PAD_R_DOWN:
        return input_state.Ry == 1;
    case PAD_ID::PAD_R_LEFT:
        return input_state.Rx == -1;
    case PAD_ID::PAD_R_RIGHT:
        return input_state.Rx == 1;
        // �����L�[
    case PAD_ID::PAD_D_UP:
    case PAD_ID::PAD_D_DOWN:
    case PAD_ID::PAD_D_LEFT:
    case PAD_ID::PAD_D_RIGHT:
        return pad_direct_keys[pad_index][direct_id] == 1;
    case PAD_ID::PAD_D_ALL:
        return all_on_direct_key;

        // ------------------------------
        //	�ȉ����Ή��̃L�[
        // ------------------------------
    case PAD_ID::PAD_C:
    case PAD_ID::PAD_Z:
    case PAD_ID::PAD_M:
        return false;
    }

    // �����ɓ��邱�Ƃ͂Ȃ�
    return false;
}

//---------------------------------------------------------------------------
// ����p�b�h�{�^���̉������
//	������Ă��Ȃ����Ƃ̊m�F
//---------------------------------------------------------------------------
bool IsPadRelease(PAD_ID pad_id, PAD_NO pad_no)
{
    unsigned int pad_index = static_cast<unsigned int>(pad_no);
    if (isOverPadNum(pad_index))
        return false;
    if (!isUsePadNum(pad_index))
        return false;

    unsigned int id_index = static_cast<unsigned int>(pad_id);
    if (isOverPadID(id_index))
        return false;

    DINPUT_JOYSTATE input_state = pad_input_states[pad_index];
    //int             pov_0_index = ((input_state.POV[0] + 4500) % 36000) / 9000;
    unsigned int direct_id = static_cast<unsigned int>(pad_id) - static_cast<unsigned int>(PAD_ID::PAD_D_UP);
    bool         all_release_dirct_key = true;
    for (int i = 0; i < MAX_PAD_DIRECT_NUM; ++i) {
        if (pad_direct_keys[pad_index][i] != 0) {
            all_release_dirct_key = false;
            break;
        }
    }

    switch (pad_id) {
        // �e�{�^��
    case PAD_ID::PAD_A:
    case PAD_ID::PAD_B:
    case PAD_ID::PAD_X:
    case PAD_ID::PAD_Y:
    case PAD_ID::PAD_L:
    case PAD_ID::PAD_R:
    case PAD_ID::PAD_START:
    case PAD_ID::PAD_BACK:
    case PAD_ID::PAD_L_PUSH:
    case PAD_ID::PAD_R_PUSH:
        return pad_buttons[pad_index][id_index] == 0;

        // ���A�i���O�X�e�B�b�N
    case PAD_ID::PAD_UP:
        return input_state.Y == 0;
    case PAD_ID::PAD_DOWN:
        return input_state.Y == 0;
    case PAD_ID::PAD_LEFT:
        return input_state.X == 0;
    case PAD_ID::PAD_RIGHT:
        return input_state.X == 0;
        // �E�A�i���O�X�e�B�b�N
    case PAD_ID::PAD_R_UP:
        return input_state.Ry == 0;
    case PAD_ID::PAD_R_DOWN:
        return input_state.Ry == 0;
    case PAD_ID::PAD_R_LEFT:
        return input_state.Rx == 0;
    case PAD_ID::PAD_R_RIGHT:
        return input_state.Rx == 0;

        // �����L�[
    case PAD_ID::PAD_D_UP:
    case PAD_ID::PAD_D_DOWN:
    case PAD_ID::PAD_D_LEFT:
    case PAD_ID::PAD_D_RIGHT:
        return pad_direct_keys[pad_index][direct_id] == 1;
    case PAD_ID::PAD_D_ALL:
        return all_release_dirct_key;
        // ------------------------------
        //	�ȉ����Ή��̃L�[
        // ------------------------------
    case PAD_ID::PAD_C:
    case PAD_ID::PAD_Z:
    case PAD_ID::PAD_M:
        return false;
    }

    // �����ɓ��邱�Ƃ͂Ȃ�
    return false;
}

//---------------------------------------------------------------------------
// ����p�b�h�{�^���̒���������
//---------------------------------------------------------------------------
bool IsPadRepeat(PAD_ID pad_id, PAD_NO pad_no)
{
    unsigned int pad_index = static_cast<unsigned int>(pad_no);
    if (isOverPadNum(pad_index))
        return false;
    if (!isUsePadNum(pad_index))
        return false;

    unsigned int id_index = static_cast<unsigned int>(pad_id);
    if (isOverPadID(id_index))
        return false;

    DINPUT_JOYSTATE input_state = pad_input_states[pad_index];
    //int             pov_0_index = ((input_state.POV[0] + 4500) % 36000) / 9000;
    unsigned int direct_id = static_cast<unsigned int>(pad_id) - static_cast<unsigned int>(PAD_ID::PAD_D_UP);
    bool         all_repeat_dirct_key = false;
    for (int i = 0; i < MAX_PAD_DIRECT_NUM; ++i) {
        if (pad_direct_keys[pad_index][i] <= 1)
            continue;

        all_repeat_dirct_key = true;
        break;
    }
    switch (pad_id) {
        // �e�{�^��
    case PAD_ID::PAD_A:
    case PAD_ID::PAD_B:
    case PAD_ID::PAD_X:
    case PAD_ID::PAD_Y:
    case PAD_ID::PAD_L:
    case PAD_ID::PAD_R:
    case PAD_ID::PAD_START:
    case PAD_ID::PAD_BACK:
    case PAD_ID::PAD_L_PUSH:
    case PAD_ID::PAD_R_PUSH:
        return pad_buttons[pad_index][id_index] > 1;

        // ���A�i���O�X�e�B�b�N
    case PAD_ID::PAD_UP:
        return input_state.Y <= -1;
    case PAD_ID::PAD_DOWN:
        return input_state.Y >= 1;
    case PAD_ID::PAD_LEFT:
        return input_state.X <= -1;
    case PAD_ID::PAD_RIGHT:
        return input_state.X >= 1;
        // �E�A�i���O�X�e�B�b�N
    case PAD_ID::PAD_R_UP:
        return input_state.Ry <= -1;
    case PAD_ID::PAD_R_DOWN:
        return input_state.Ry >= 1;
    case PAD_ID::PAD_R_LEFT:
        return input_state.Rx <= -1;
    case PAD_ID::PAD_R_RIGHT:
        return input_state.Rx >= 1;

        // �����L�[
    case PAD_ID::PAD_D_UP:
    case PAD_ID::PAD_D_DOWN:
    case PAD_ID::PAD_D_LEFT:
    case PAD_ID::PAD_D_RIGHT:
        return pad_direct_keys[pad_index][direct_id] >= 1;
        //return pad_direct_keys[pad_index][pov_0_index] >= 1;
    case PAD_ID::PAD_D_ALL:
        return all_repeat_dirct_key;

        // ------------------------------
        //	�ȉ����Ή��̃L�[
        // ------------------------------
    case PAD_ID::PAD_C:
    case PAD_ID::PAD_Z:
    case PAD_ID::PAD_M:
        return false;
    }

    // �����ɓ��邱�Ƃ͂Ȃ�
    return false;
}

//---------------------------------------------------------------------------
// �w��p�b�h�̓��͏�Ԃ̏ڍׂ��擾
//---------------------------------------------------------------------------
bool GetPadInputState(DINPUT_JOYSTATE& pad_state, PAD_NO pad_no)
{
    unsigned int pad_index = static_cast<unsigned int>(pad_no);
    if (isOverPadNum(pad_index))
        return false;
    if (!isUsePadNum(pad_index))
        return false;

    // �����ɓn���ꂽDINPUT_JOYSTATE�ɓn��
    pad_state = pad_input_states[pad_index];
    return true;
}

//---------------------------------------------------------------------------
// �ڑ�����Ă���p�b�h�̐����擾
//  �� DX���C�u�����́uGetJoypadNum�֐��v�̃��b�p
//---------------------------------------------------------------------------
s32 GetConnecetdPadNum()
{
    int pad_num = GetJoypadNum();

    return (pad_num == 0) ? -1 : pad_num;
}

//---------------------------------------------------------------------------
// �w��p�b�h�̍��A�i���O�X�e�B�b�N��X�����̓��͒l���擾
//---------------------------------------------------------------------------
f32 GetPadInputAnalogLX(PAD_NO pad_no)
{
    unsigned int pad_num = static_cast<unsigned int>(pad_no);

    if (isOverPadNum(pad_num))
        return 0;
    if (!isUsePadNum(pad_num))
        return 0;

    return pad_input_states[pad_num].X / MAX_ANALOG_INPUT_VALUE;
}

//---------------------------------------------------------------------------
// �w��p�b�h�̍��A�i���O�X�e�B�b�N��Y�����̓��͒l���擾
//---------------------------------------------------------------------------
f32 GetPadInputAnalogLY(PAD_NO pad_no)
{
    unsigned int pad_num = static_cast<unsigned int>(pad_no);

    if (isOverPadNum(pad_num))
        return 0;
    if (!isUsePadNum(pad_num))
        return 0;

    return pad_input_states[pad_num].Y / MAX_ANALOG_INPUT_VALUE;
}

//---------------------------------------------------------------------------
// �w��p�b�h�̉E�A�i���O�X�e�B�b�N��X�����̓��͒l���擾
//---------------------------------------------------------------------------
f32 GetPadInputAnalogRX(PAD_NO pad_no)
{
    unsigned int pad_num = static_cast<unsigned int>(pad_no);

    if (isOverPadNum(pad_num))
        return 0;
    if (!isUsePadNum(pad_num))
        return 0;

    return pad_input_states[pad_num].Rx / MAX_ANALOG_INPUT_VALUE;
}

//---------------------------------------------------------------------------
// �w��p�b�h�̉E�A�i���O�X�e�B�b�N��Y�����̓��͒l���擾
//---------------------------------------------------------------------------
f32 GetPadInputAnalogRY(PAD_NO pad_no)
{
    unsigned int pad_num = static_cast<unsigned int>(pad_no);

    if (isOverPadNum(pad_num))
        return 0;
    if (!isUsePadNum(pad_num))
        return 0;

    return pad_input_states[pad_num].Ry / MAX_ANALOG_INPUT_VALUE;
}

bool IsPadInputAnalogLDirection(f32* dir_nums)
{
    for (int i = 0; i < 4; ++i) {
        if (dir_nums[i] > 0) {
            return true;
        }
    }
    return false;
}

s32 GetPadInputAnalogLDirection(PAD_NO pad_no)
{
    float analog_x = GetPadInputAnalogLX(pad_no);
    float analog_y = GetPadInputAnalogLY(pad_no);
    float dir[4] = { 0 };   // 0: �� / 1: �E / 2: �� / 3: ��
    if (analog_x < 0) {
        dir[0] = abs(analog_x);
    }
    else {
        dir[1] = analog_x;
    }
    if (analog_y < 0) {
        dir[2] = abs(analog_y);
    }
    else {
        dir[3] = analog_y;
    }

    if (IsPadInputAnalogLDirection(dir) == false) {
        // ���͂Ȃ�
        return -1;
    }

    int max_dir_index = 0;
    for (int i = 1; i < 4; ++i) {
        if (dir[max_dir_index] > dir[i])
            continue;

        max_dir_index = i;
    }

    return max_dir_index;
}

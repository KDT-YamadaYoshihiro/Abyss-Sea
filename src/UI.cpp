#include "UI.h"

// �e�[�u���̒�`
const UI::AnimInfo UI::animTable[ANIM_MAX] = {
	{0,0,3},	// WAIT
	{3,0,3},	// ATTACK
	{3,3,3},	// MAGIC
	{3,1,3},	// SKILL
	{0,4,3},	// DAMAGE
	{6,1,3},	// WIN
	{6,5,3},	// LOSE
};


void UI::PlayerAnimStart(GRTYPE& arg_type)
{
	// �t���[�������Z�b�g
	frame = 0;

	const AnimInfo& anim = animTable[arg_type];

	scrX = SIZE_W * anim.startCol;
	startScrX = SIZE_W * anim.startCol;

	scrY = SIZE_H * anim.row;
	startScrY = SIZE_H * anim.row;

	// ���[�V�������Ƃ̃R�}����ݒ�
	totalFrames = anim.frames; // ��F�S���[�V����3�R�}

}

void UI::PlayerAnim(GRTYPE& arg_type)
{
	// ��ԕω����o
	if (arg_type != prev_type) {
		PlayerAnimStart(arg_type);
		prev_type = arg_type;
	}

	frame++;

	// 8�t���[�����ƂɎ��̃R�}��
	if (frame % 20 == 0) {
		//
		scrX += SIZE_W;

		// �R�}�������܂ōs����
		if ((scrX - startScrX) / SIZE_W >= totalFrames) {
			// �ҋ@����Ȃ�������߂�
			if (prev_type == ATTACK || prev_type == MAGIC || prev_type == SKILL || prev_type == DAMAGE) {
				arg_type = WAIT;
				PlayerAnimStart(arg_type);
				prev_type = arg_type;
				AnimWait = true;

			}
			else {

				scrX = startScrX; // ���[�v
				AnimWait = false;
			}
		}
	}

}



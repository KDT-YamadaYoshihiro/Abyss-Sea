#pragma once
enum PLAYER {
	// �v���C���[
	AKIRA,
	EL,
	ALIA,
	PINOT,
	KAGE,

	PLAYER_MAX			// �v���C���[�ő吔
};

enum ENEMY {
	// �G�l�~�[
	MERFOLK,
	DEEPONES,
	KTANID,

	ENEMY_MAX			//�@�ő吔(for���p)
};


// BGM�ESE
enum BGM {
	BGM_START,
	BGM_STAGE1,
	BGM_STAGE2,
	BGM_STAGE3,
	BGM_CLEAR,
	BGM_OVER,

	BGM_MAX				// �ő吔(for���p)
};

enum SE {
	SE_START,
	SE_DECISION,
	SE_CANCEL,
	SE_CHARACHANGE,
	SE_CLICK,
	SE_ATTACK,
	SE_SKILL1,
	SE_SKILL2,
	SE_BUFF,
	SE_HEAL,


	SE_MAX				// �ő吔(for���p)
};

enum GRTYPE {
	WAIT,		// �ҋ@��
	ATTACK,		// �ʏ�U���i�˂��j
	MAGIC,		// ���@�X�L��(�}�p�X�L��)
	SKILL,		// �U���X�L��(�U��)
	DAMAGE,		// �U�����󂯂��Ƃ�
	WIN,		// �o�g������
	LOSE,		// �퓬�s�\
	ANIM_MAX
};

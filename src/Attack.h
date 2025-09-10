#pragma once

// �A�^�b�N�N���X
class Attack{

	// �U���͂̕ϐ�
	float ATK;
	// �o�t�ʂ̕ϐ�
	float buff;
	// �h��͂̕ϐ�
	float def;
	// �R���{���̕ϐ�
	float combo;

public:

	virtual ~Attack() = default;

	// ������
	Attack(float arg_atk, float arg_buff, float arg_def, int arg_combo)
		:ATK(arg_atk), buff(arg_buff), def(arg_def), combo(arg_combo) {
	};

	// �_���[�W�v�Z(�U����,�o�t,�h���,�R���{��,)
	int  Calculation(int arg_atk, int arg_buff, int arg_def, int arg_combo) {
		
		ATK = arg_atk;
		buff = 1.0f + arg_buff;
		def = arg_buff;
		combo = 1.0f + arg_combo / 10.0f;

		float damage = (ATK * buff) * combo - def;

		return damage;
	}
	
	// �U���̓o�t
	float Buff(float arg_buff) {
		
		buff += arg_buff;

		return buff;
	}


};


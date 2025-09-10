#pragma once

// �L�����N�^�[��HP�Ǘ��N���X
class CHP {

	// ����HP�������ϐ�
	int Hp;
	// HP�̍ő�l�������ϐ�
	int HpMax;

public:

	virtual ~CHP() = default;

	// ������
	CHP(int hp, int hpMax):Hp(hp),HpMax(hpMax) {
	}

	// HP�̊����\��
	float Ratio() {

		// �����v�Z
		float ratio = Hp / HpMax;

		// �v�Z���ʂ�return
		return ratio;
	};

	// HP�̕`��
	void HpRender(float arg_x, float arg_y, float arg_xsize, float arg_ysize, 
		int arg_HpMax, int arg_Hp, unsigned int color)
	{
		// ���W�v�Z
		float box_x = arg_x + arg_xsize;
		float box_y = arg_y + arg_ysize;

		float box_hp = Ratio();
		float box_x2 = arg_x + box_hp;

		// �{�b�N�X��`��(�g)
		DrawLineBox(arg_x, arg_y, box_x, box_y, GetColor(255, 255, 255));
		// �{�b�N�X�̕`��(���g)
		DrawFillBox(arg_x, arg_y, box_x2,box_y,color);
	};


};
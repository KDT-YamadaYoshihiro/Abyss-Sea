#pragma once
// ���C�u����
#include "Dxlib.h"

#include <ctime>
#include <math.h>
#include <cmath>
#include <vector>

// system
#include "Float2.h"
#include "InPutKey.h"
#include "InPutPad.h"

#include "Manager.h"
#include "CharacterFactory.h"
#include "Character.h"

#define DegToRad 0.017453293f			//	�ʓx�@��
#define RadToDeg  57.29577951f


#define R2D(rad) rad * RadToDeg			// 
#define D2R(deg) deg * DegToRad			// ���W�A���Ԋ�

// �Q�[�����[�v�N���X
class CGame {

	// �R���X�g���N�^
	CGame();
	// �f�X�g���N�^
	virtual ~CGame();


public:

	CGame(const CGame&) = default;
	CGame& operator = (const CGame&) = default;
	static CGame& Instance() {
		static CGame instance;
		return instance;
	}

	// ������
	bool Init();
	// ���[�v����
	void Run();
	// �X�V
	void Update();
	// �`��
	void Render();
	// �I��
	void End();

};

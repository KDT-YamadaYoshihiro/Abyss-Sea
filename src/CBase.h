#pragma once
#include <vector>
#include <memory>
#include "DxLib.h"

//	�J���[�R�[�h�ꗗ
#define BLACK	GetColor(0, 0, 0)			//	��
#define WHITE	GetColor(255, 255, 255)		//	��
#define RED		GetColor(255, 0, 0)			//	��
#define GREEN	GetColor(60, 179, 113)		//	��
#define BLUE	GetColor(0, 0, 255)			//	��
#define YELLOW	GetColor(255, 255, 0)		//	���F
#define PINK	GetColor(255, 0, 255)		//	�s���N
#define SKYBLUE	GetColor(0, 255, 255)		//	���F

// �E�B���h�E�T�C�Y
#define WINDOW_W		1298		
#define WINDOW_H		 808		

// �t�H���g�T�C�Y
#define FONTSIZE		 100

// CLICK TO START
// ������̏㉺�h��͈̔�
#define TOP_RANGE		 615
#define DWON_RANGE		 585

enum BUTTAN {
	BGM_UP,
	BGM_DOWN,
	SE_UP,
	SE_DOWN,

	BUTTAN_MAX
};

// �X�N���[���x�[�X�N���X
class ScreenBase {

protected:

public:

	virtual void Update() = 0;

	virtual void Render() = 0;

};
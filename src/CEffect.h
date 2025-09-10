#pragma once
#include "DxLib.h"

class Effect {

	// �摜�T�C�Y
	int imageSizeW = -1;
	int imageSizeH = -1;
	// �摜�̐؂���ʒu
	int ef_scrX = -1;
	int ef_scrY = -1;
	// �摜�̐؂���T�C�Y
	int scrSizeW = -1;
	int scrSizeH = -1;
	// �摜�n���h��
	int ef_grh;
	// �A�j���[�V�����؂�ւ��t���[��
	int  animFrame = -1;
	// �`��̕\����\��
	bool efDraw = false;


public:


	// ������
	Effect(int arg_grh)
		:ef_grh(arg_grh)
	{
		GetGraphSize(ef_grh, &imageSizeW, &imageSizeH);
		ef_scrX = 0;
		ef_scrY = 0;

		if (imageSizeW == 600 && imageSizeH == 120) {
			scrSizeW = 120;
			scrSizeH = 120;
		}
		else if (imageSizeW == 640 && imageSizeH == 4800) {
			scrSizeW = 640;
			scrSizeH = 480;
		}
		else if (imageSizeW == 2400 && imageSizeH == 1440) {
			scrSizeW = 480;
			scrSizeH = 480;
		}
		else {
			scrSizeW = 120;
			scrSizeH = 120;
		}
		animFrame = 0;
		efDraw = false;
	};

	// �Q�b�g�֐�
	bool getEfDraw() const { return efDraw; }

	// ���o�֐�

	// �������֐�
	void effectInit()
	{
		efDraw = false;
		ef_scrX = 0;
		ef_scrY = 0;
	}

	// �X�^�[�g�֐�
	void effectStart() {
		if (efDraw == false) { efDraw = true; }
		ef_scrX = 0;
		ef_scrY = 0;
	}

	// �摜���������̃A�j���[�V�����Đ��֐�
	void sidePlayAnim() {

		// �t���[�����J�E���g
		if (efDraw) { animFrame++; }

		if (animFrame % 8 == 0) {
			// ��\���ɂ���
			if (ef_scrX >= imageSizeW) {
				efDraw = false; }
			// scrX���ړ�
			else{ ef_scrX += scrSizeW;}
			
		}

	}

	// �摜���c�����̃A�j���[�V�����Đ��֐�
	void lengthPlayerAnim() {

		// �t���[�����J�E���g
		if (efDraw) { animFrame++; }

		if (animFrame % 5 == 0) {
			// ��\���ɂ���
			if (ef_scrY >= imageSizeH) { efDraw = false; }
			// scrY���ړ�
			else{ ef_scrY += scrSizeH;}
			
		}

	}

	// �c���̃A�j���[�V�����Đ��֐�
	void PlayAnim() {
		if (efDraw) { animFrame++; }

		if (animFrame % 5 == 0) {
			// scrX���ړ�
			ef_scrX += scrSizeW;
			if (ef_scrX >= imageSizeW) {
				
				// ��\���ɂ���
				if (ef_scrY >= imageSizeH) { efDraw = false; }
				// scrY���ړ�
				else{ef_scrY += scrSizeH;}

				ef_scrX = 0;
			}

		}
	}

	// �`��
	void effectDraw(int arg_x, int arg_y, int sizeW, int sizeH) {
		if (efDraw == true) {
			DrawRectGraph(arg_x - sizeW, arg_y - sizeH, ef_scrX, ef_scrY, scrSizeW, scrSizeH, ef_grh, true);
		}
	}
	

};



class Sound {

public:

	//�@������
	Sound() {};

	// �����Đ�
	// BGM(���[�v�Đ�)
	void PlayBgm(int bgmh) {
		if (CheckSoundMem(bgmh) == false) {
			PlaySoundMem(bgmh, DX_PLAYTYPE_LOOP);
		}
	}
	// SE(�P�Đ�)
	void PlaySe(int seh) {
		if (CheckSoundMem(seh) == false) { PlaySoundMem(seh,DX_PLAYTYPE_BACK); }
	}

	// ������~
	void stopBgm(int bgmh) {
		if (CheckSoundMem(bgmh) == true) { StopSoundMem(bgmh,false); }
	}
	void stopSe(int seh) {
		if (CheckSoundMem(seh) == true) { StopSoundMem(seh, false); }

	}

};
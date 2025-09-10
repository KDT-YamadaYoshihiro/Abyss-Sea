#pragma once
#include "Manager.h"
#include "CBase.h"
#include "CBattle.h"
#include "Cplayer.h"
#include "CharacterFactory.h"

enum class PartyUIState {
	NORMAL,     // �p�[�e�B�\���̂�
	SELECTING   // �L�����I�𒆁i�ꗗ�W�J�j
};


class CPartySelect : public ScreenBase {

	//�@ui
	std::shared_ptr<UI> ui;
	// �p�[�e�B�[�����o�[�i�S�l�Œ�j
	std::vector<std::shared_ptr<Player>> party;

	// �v���C���[�S�L�����ꗗ
	std::vector<std::shared_ptr<Player>> allChara;

	// ���݂̕\�����[�h
	PartyUIState uiState = PartyUIState::NORMAL;

	// ��
	std::shared_ptr<Sound>bgm;
	std::shared_ptr<Sound> se;

	// �t�F�[�h
	std::shared_ptr<Fade> fade;

	// �p�[�e�B�g�i�S�g�j
	int selectedSlot = -1;

	// �p�[�e�B�����o�[����ւ����
	int selectedIndex = -1;

	// ����{�^���̍��W
	int boxX = -1;
	int boxY = -1;	

	// ����{�^���̃{�b�N�X�g�T�C�Y
	int sizeW = -1;
	int sizeH = -1;

	int frame = -1;

	//�@�摜�\���p�ϐ�
	int baseX = -1;
	int baseY = -1;
	int scrX = -1;
	int scrY = -1;
	int grhSizeX = -1;
	int grhSizeY = -1;
	int spacingX = -1;

	// ����LEVEL
	int Suggest = -1;

	// ����LEVEL�\���p
	int textPosX = -1;
	int textPosY = -1;

public:

	// ������
	CPartySelect() {

		// UI����
		ui = std::make_shared<UI>();

		// ����
		bgm = Manager::Instance().getbgm();
		se = Manager::Instance().getSe();

		fade = Manager::Instance().getFade();

		// ���݂̃p�[�e�B�[�����o�[���}�l�[�W���[����擾
		party = Manager::Instance().getParty();
		// �p�[�e�B�[���l�l���邩�m�F
		if (party.size() < 4) {
			party.resize(4, nullptr);
		}

		// �S�L�����ꗗ�𐶐�
		int tableSize = CharacterFactory::PlayerTableSize;

		// 
		allChara = Manager::Instance().getPlayers();

		uiState = PartyUIState::NORMAL;

		selectedSlot = -1;
		selectedIndex = -1;

		// ���W�ƃT�C�Y
		boxX = 900;
		boxY = 650;
		sizeW = 300;
		sizeH = 100;

		frame = 0;

		// �摜�\���p�̕ϐ�
		baseX = 100;
		baseY = 120;
		scrX = 60;
		scrY = 0;
		grhSizeX = 250;
		grhSizeY = 418;
		spacingX = grhSizeX + 40;

		Suggest = Manager::Instance().getSuggest();
		textPosX = WINDOW_W - 250;
		textPosY = 50;
	}

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

};
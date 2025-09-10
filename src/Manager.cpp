#include "Manager.h"
#include "CTitle.h"

void Manager::Init()
{
	//// �v���C���[�̐���
	//for (int member = 0; member < PLAYER_MAX; member++) {
	//	players.push_back(std::dynamic_pointer_cast<Player>(CharacterFactory::CreatePlayer(member, member)));
	//}
	//for (int member = 0; member < 4; member++) {
	//	party.push_back(std::dynamic_pointer_cast<Player>(CharacterFactory::CreatePlayer(member, member)));
	//}

	// �L�����N�^�[�f�[�^�ǂݍ���
	if (!characterFactory.LoadPlayerData("data/CharacterData.csv")) {
		std::cerr << "�v���C���[�f�[�^�ǂݍ��ݎ��s" << std::endl;
	}

	if (!characterFactory.LoadSkillData("Data/SkillData.csv")) {
		std::cerr << "�X�L���f�[�^�ǂݍ��ݎ��s" << std::endl;
	}

	// �����̃v���C���[���N���A
	players.clear();

	// CSV����v���C���[�𐶐�����players�ɓo�^
	for (int i = 0; i < (int)characterFactory.getPlayerTable().size(); i++) {
		auto player = characterFactory.CreatePlayer(i);
		players.push_back(std::dynamic_pointer_cast<Player>(player));
	}

	// �����p�[�e�B��ݒ�
	party.clear();
	if (!players.empty()) {
		for (int i = 0; i < (int)players.size() && i < 4; i++) {
			party.push_back(players[i]);
		}
	}

	// �t�F�[�h��BGM���̏�����
	fade = std::make_shared<Fade>();

	bgm = std::make_shared<Sound>();
	se = std::make_shared<Sound>();

	ChangeScreen<CTitle>();

	mouseX = -1;
	mouseY = -1;

	bgmVolume = 7;
	seVolume = 7;

}

void Manager::Update(){
	
	// �X�N���[���|�C���^�[�X�V����
	scrPtr->Update();

	// ���ʐݒ�
	bgmVolume = max(0, min(bgmVolume, 10));
	seVolume = max(0, min(seVolume, 10));


	for (int i = 0; i < BGM_MAX; i++) {
		ChangeVolumeSoundMem(bgmVolume * 22.5, CLoad::Instance().getBgmHandle(i));
	}
	for (int i = 0; i < SE_MAX; i++) {
		ChangeVolumeSoundMem(seVolume * 22.5, CLoad::Instance().getSeHandle(i));
	}
}

void Manager::Render()
{
	// �X�N���[���|�C���^�[�`�揈��
	scrPtr->Render();

	// mouse���W�̎擾
	GetMousePoint(&mouseX, &mouseY);
	// �J�[�\���`��
	if (!window_mode) {
		DrawExtendGraph(mouseX, mouseY, mouseX + 30, mouseY + 30, CLoad::Instance().getCursorGrh(), true);
	}

	// �t�F�[�h
	fade->fadeCircleDraw(WINDOW_W / 2, WINDOW_H / 2);
}

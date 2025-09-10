#pragma once
#include "DxLib.h"
#include "InPutKey.h"
#include "CBase.h"
#include "CPlayer.h"
#include "Character.h"
#include "CharacterFactory.h"
#include "CLoad.h"

enum class BattleResult {
	BATTLE,
	VICTORY,
	DEFEAT
};

class Manager {

	CharacterFactory characterFactory;

	// 
	std::vector<std::shared_ptr<Player>> players;
	std::vector<std::shared_ptr<Player>> party;


	// ����
	std::shared_ptr<Sound> bgm;
	std::shared_ptr<Sound> se;

	// �t�F�[�h
	std::shared_ptr<Fade> fade;

	// 
	int stageScreen = -1;

	BattleResult result;
	// �o���l
	int Exp = -1;

	// mouse���W
	bool window_mode = true;
	int mouseX = -1;
	int mouseY = -1;

	// ���ʐݒ�
	int bgmVolume;
	int seVolume;

	// ����LEVEL
	int Suggest;


    // �V���O���g��
    // ������
	Manager() = default;

    virtual ~Manager() {};

public:

	Manager(const Manager&) = default;
	Manager& operator = (const Manager&) = default;
	static Manager& Instance() {
		static Manager instance;
		return instance;
	}

	std::shared_ptr<ScreenBase> scrPtr = nullptr;

	std::shared_ptr<CLoad> load = nullptr;


	// ������
	void Init();

	// �X�V����
	void Update();

	// �`�揈��
	void Render();

	// ��ʐ؂�ւ����\�b�h
	template<typename T>
	void ChangeScreen();

	// �X�e�[�W���Z�b�g
	void setStageScreen(int arg_stage) {
		stageScreen = arg_stage;
	}
	// �X�e�[�W��ԍ����擾
	int getStageScreen() const { return stageScreen; }

	// �o�g�����s�̃Z�b�g�A�Q�b�g�֐�
	void setBattleResult(BattleResult  arg_result) {
		result = arg_result;
	}
	BattleResult getbattleResult() const { return result; }

	//�@�S�v���C���[�̃Q�b�g�֐�
	std::vector<std::shared_ptr<Player>>& getPlayers() { return players; }
	// �p�[�e�B�����o�[�̃Z�b�g�Q�b�g�֐�
	void setParty(std::vector<std::shared_ptr<Player>> arg_paty) {
		party = arg_paty;
	}
	std::vector<std::shared_ptr<Player>>& getParty() { return party; }

	// bgm,se�̃Q�b�g�֐�
	std::shared_ptr<Sound> getbgm() const { return bgm; }
	std::shared_ptr<Sound> getSe() const { return se; }

	// UI
	std::shared_ptr<Fade> getFade() const { return fade; }

	// ���ʐݒ�
	void setBgmVolume(int arg_volume) { bgmVolume = arg_volume; }
	int getBgmVolume() const { return bgmVolume; }

	void setSeVolume(int arg_volume) { seVolume = arg_volume; }
	int getSeVolume() const { return seVolume; }


	void setExp(int arg_exp) {
		Exp = arg_exp;
	}

	int getExp() const { return Exp; }

	bool getWindowMode() const { return window_mode; }

	// ����LEVEL
	void setSuggest(int arg_suggest) { Suggest = arg_suggest; }
	int getSuggest() const { return Suggest; }
};

// 
template<typename T>
void Manager::ChangeScreen()
{
			// �֐����E�N���X<�^>();
	scrPtr = std::make_shared<T>();
}

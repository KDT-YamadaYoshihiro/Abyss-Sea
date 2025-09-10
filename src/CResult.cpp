 #include "CResult.h"

void CResult::Update()
{
	// �t�F�[�h�̍X�V����
	fade->fadeUpdate(WINDOW_W);

	// �I�𒆂̎O�p�`�̍��W
	ChoiceNectScreen();

	// �N���b�N�����e�L�X�g�ɑI��ύX
	for (int i = 0; i < MAX; i++) {
		if (CheckBoxClick(choiceText_X[i], choiceText_Y[i], TEXT_SIZE * 7, TEXT_SIZE)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			choiceNum = i;
		}
	}

	// ���x���A�b�v
	if (Manager::Instance().getbattleResult() == BattleResult::VICTORY && !isDistributed) {

		auto& party = Manager::Instance().getParty();
		auto& players = Manager::Instance().getPlayers();

		int totalExp = 0;

		// �G�l�~�[�̌o���l���擾
		totalExp = Manager::Instance().getExp();


		// �擾�����G�l�~�[�̌o���l���p�[�e�B�����o�[���̊���B
				// �������o���l���e�L�����N�^�[�ɃL���X�g
		int preMamber = (party.empty() ? 0 : totalExp / static_cast<int>(party.size()));

		// ������
		levelUpDiff.assign(party.size(), 0); 

		// �o���l100�ȏ��1LvUp
		for (size_t i = 0; i < party.size(); i++) {
			auto& p = party[i];
			p->addExp(preMamber);

			// ���x�����オ��O�̃��x�����L�^
			int beforeLevel = p->getLv();
			// ���x��30�ȏ�Ȃ烌�x���A�b�v�͏I������
				while (p->getLv() < LV_MAX && p->getExp() >= p->getMaxExp()) {
					p->levelUp();
					if (players[i]->getId() == p->getId()) {
						players[i]->levelUp();
					}
				}
			
			// ���x���A�b�v��̃��x�����L�^
			int afterLevel = p->getLv();

			// �オ�������x�����L�^
			levelUpDiff[i] = afterLevel - beforeLevel;
		}

		// �o���l�̍ĕ��z�h�~
		isDistributed = true;
	}


	// ����n���N���b�N����Ɖ�ʂ��؂�ւ��B
		// (CResult ->	CStage
		//				CBattle
		//				CTitle )
	if (CheckBoxClick(700, 700, TEXT_SIZE * 2, TEXT_SIZE)) {

		auto& players = Manager::Instance().getParty();

		se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));

		//hp�̃��Z�b�g
		for (auto& p : players) {
			p->hpReset();
		}
		// bgm�̒�~
		if (Manager::Instance().getbattleResult() == BattleResult::VICTORY) {
			bgm->stopBgm(CLoad::Instance().getBgmHandle(BGM_CLEAR));
		}
		else {
			bgm->stopBgm(CLoad::Instance().getBgmHandle(BGM_OVER));
		}
		// fade���O��
		fade->fadeStart(fade->FADE_CLAUSE);

	}

	if (fade->checkClause(WINDOW_W)) {

		switch (choiceNum)
		{
		case STAGE:

			Manager::Instance().ChangeScreen<CStage>();

			break;

		case BATTLE:

			Manager::Instance().ChangeScreen<CBattle>();

			break;

		case TITLE:

			Manager::Instance().ChangeScreen<CTitle>();

			break;

		default:
			break;
		}
	}

}

void CResult::Render()
{

	// ���s����\��
	if (Manager::Instance().getbattleResult() == BattleResult::VICTORY) {
		SetFontSize(60);
		DrawString(550, 10, "VICTORY!", GetColor(255, 255, 255));
	}
	else {
		SetFontSize(60);
		DrawString(600, 10, "DEFEAT", GetColor(255, 255, 255));
	}

	auto& players = Manager::Instance().getParty();

	for (size_t i = 0; i < players.size(); i++) {
		auto& p = players[i];

		int posX = baseX + spacingX * static_cast<int>(i);
		int posY = baseY;

		SetFontSize(30);
		// �L�����N�^�[�̉摜�\��
		DrawRectGraph(posX, posY, scrX, scrY, grhSizeX, grhSizeY,
			CLoad::Instance().getPbodyGrh(p->getId()),false);
		// ���O�\��
		int textPosY = posY + grhSizeY + 10;
		DrawString(posX, textPosY, p->getName().c_str(),GetColor(255,255,255));
		// ���x���\��
		char levelStr[64];
		sprintf_s(levelStr, "Lv:%2d", p->getLv());
		DrawString(posX, textPosY + 40, levelStr, GetColor(255, 255, 255));
		// �A�b�v�������x���\��
		if (isDistributed) {
			if (levelUpDiff.size() > i) {
				if (levelUpDiff[i] > 0) {
					char upStr[16];
					sprintf_s(upStr, "+%2d", levelUpDiff[i]);
					DrawString(posX + 90, textPosY + 40, upStr, GetColor(0, 255, 0));
				}
			}
		}
	}


	// �I�𒆂������O�p�`
	int trPosY = choicePosY + 60;
	DrawTriangle(choicePosX, trPosY,
		choicePosX - TRIANGEL_X, trPosY - TRIANGLE_Y, choicePosX - TRIANGEL_X, trPosY + TRIANGLE_Y,
		GetColor(255,0,0),true);

	// ���̃X�N���[���I��
	SetFontSize(TEXT_SIZE);
	DrawFormatString(choiceText_X[STAGE], choiceText_Y[STAGE], GetColor(255, 255, 255), "�X�e�[�W��I��");
	DrawFormatString(choiceText_X[BATTLE], choiceText_Y[BATTLE], GetColor(255, 255, 255), "�Ē���");
	DrawFormatString(choiceText_X[TITLE], choiceText_Y[TITLE], GetColor(255, 255, 255), "�^�C�g����");

	// ����{�^��
	DrawFormatString(700, 700, GetColor(255, 255, 255), "����");

}

// �I�𒆂̎O�p�`���W
void CResult::ChoiceNectScreen()
{

	switch (choiceNum)
	{

	case STAGE:

		choicePosX = 150;
		choicePosY = 600;

		break;

	case BATTLE:

		choicePosX = 150;
		choicePosY = 650;

		break;

	case TITLE:

		choicePosX = 150;
		choicePosY = 700;

		break;

	default:
		break;
	}
}

#include "CPartySelect.h"

// �X�V����
void CPartySelect::Update()
{
	// �t�F�[�h�A�b�v�f�[�g
	fade->fadeUpdate(WINDOW_W);

	if (uiState == PartyUIState::NORMAL) {
		
		// �t���[���̑���
		if (frame < 30) { frame++; }
		// �퓬�{�^���ŃX�N���[�����o�g����ʂɐ؂�ւ��B
		if (frame == 30 && CheckBoxClick(boxX, boxY, sizeW, sizeH)) {
			// �t�F�[�h�A�E�g�X�^�[�g�֐�
			fade->fadeStart(fade->FADE_CLAUSE);
		}

		if (fade->checkClause(WINDOW_W)) {
			Manager::Instance().setParty(party);
			Manager::Instance().ChangeScreen<CBattle>();
			bgm->stopBgm(CLoad::Instance().getBgmHandle(BGM_START));
		}

		// �߂�{�^���ŃX�N���[�����X�e�[�W�I����ʂɐ؂�ւ���
		if (frame == 30 && CheckBoxClick(20, 20, 200, 100)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			Manager::Instance().ChangeScreen<CStage>();
		}

		// �p�[�e�B�[�g���N���b�N�ŕύX���[�h�Ɂi�S�L�����N�^�[�ꗗ��W�J�j
		for (int i = 0; i < 4; i++) {
			if (CheckBoxClick(baseX + spacingX * i, baseY, grhSizeX, grhSizeY)) {
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
				selectedSlot = i;
				uiState = PartyUIState::SELECTING;
				selectedIndex = -1;
			}
		}
	}
	else if (uiState == PartyUIState::SELECTING) {

		// �S�L�����N�^�[�ꗗ�W�J���[�h
		for (int i = 0; i < (int)allChara.size(); i++) {
			int posX = 100 + (i % 5) * 120;
			int posY = 200 + (i / 5) * 120;
			// �ꗗ�̉摜���N���b�N�Ńp�[�e�B�����o�[�̍����ւ�����
			if (CheckBoxClick(posX, posY, 100, 100)) {
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
				selectedIndex = i;
			}
		}

		// ����{�^�����N���b�N���ꂽ�Ƃ��A�����ւ����s���B
		if (CheckBoxClick(boxX, boxY, sizeW, sizeH) && selectedIndex != -1) {
			auto newChar = allChara[selectedIndex];

			// �����ւ���₪���łɃp�[�e�B�[�ɂ��邩�m�F�B
			int foundIndex = -1;
			for (int i = 0; i < (int)party.size(); i++) {
				if (party[i]->getId() == newChar->getId()) {
					foundIndex = i;
					break;
				}
			}

			if (foundIndex != -1) {
				// ���łɃp�[�e�B�ɂ��� �� ���Ԃ����ւ���
				auto tmp = party[selectedSlot];
				party[selectedSlot] = party[foundIndex];
				party[foundIndex] = tmp;
			}
			else {
				// �p�[�e�B�ɂ��Ȃ� �� �I�������g�ɐV�����L�������Z�b�g
				party[selectedSlot] = newChar;
			}

			se->PlaySe(CLoad::Instance().getSeHandle(SE_CHARACHANGE));
			// �ꗗ�����
			uiState = PartyUIState::NORMAL;
			selectedIndex = -1;
			// �t���[�������Z�b�g
			frame = 0;
		}

		// �ꗗ����ăp�[�e�B�[�m�F��ʂɖ߂�B
		if (CheckBoxClick(20, 20, 200, 100)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			uiState = PartyUIState::NORMAL;
			frame = 0;
		}

	}
}

// �`�揈��
void CPartySelect::Render()
{
	if (uiState == PartyUIState::NORMAL) {
		// �p�[�e�B�[�\����
		for (int i = 0; i < 4; i++) {
			int posX = baseX + spacingX * i;
			int posY = baseY;

			SetFontSize(30);
			std::string name = (party[i] ? party[i]->getName() : "���ݒ�");

			// ���݂̃p�[�e�B�[�����o�[��body�摜��\���B
			if (party[i]) {
				DrawRectGraph(posX, posY, scrX, scrY, grhSizeX, grhSizeY, CLoad::Instance().getPbodyGrh(party[i]->getId()), TRUE);
			}
			else {
				DrawBox(posX, posY, posX + grhSizeX, posY + grhSizeY, GetColor(100, 100, 100), TRUE);
				DrawString(posX + 20, posY + 50, "�󂫘g", GetColor(255, 255, 255));
			}
			// ���O�\��
			int textPosY = posY + grhSizeY + 10;
			DrawString(posX, textPosY, name.c_str(), GetColor(255, 255, 255));
			// ���x���\��
			char levelStr[64];
			sprintf_s(levelStr, "Lv:%2d", party[i]->getLv());
			DrawString(posX, textPosY + 40, levelStr, GetColor(255, 255, 255));
		}

		// ����LEVEL�\��
		SetFontSize(30);
		DrawFormatString(textPosX, textPosY, GetColor(255, 255, 255), "�������x��:%2d", Suggest);

		// �o�g���J�n�{�^��
		SetFontSize(50);
		DrawFormatString(boxX, boxY, GetColor(255, 255, 255), "<�o�g���J�n>");

	}
	else if (uiState == PartyUIState::SELECTING) {

		// �S�L�����ꗗ�\���W�J���[�h
		for (int i = 0; i < (int)allChara.size(); i++) {
			// �S�L�����ꗗ��face�摜��\���B�i�������ɕ\���@����5�̂܂Łj
			int posX = 100 + (i % 5) * 120;
			int posY = 200 + (i / 5) * 120;
			DrawExtendGraph(posX, posY,posX + 100, posY + 100, CLoad::Instance().getPfaceGrh(i), TRUE);

			// �I�������L�����N�^�[�ɘg��\��
			if (i == selectedIndex) {
				DrawBox(posX - 2, posY - 2, posX + 102, posY + 102, GetColor(255, 255, 0), FALSE);
			}

		}

		// �I�����Ă���L�����N�^�[�̃X�e�[�^�X��\���i�E�����ɕ\���j
			//�\������X�e�[�^�X�i���O�A���x���A�U���́A�h��́A�f�����A�X�L�����A�X�L���ڍׁi��̓I�ɉ�������X�L���Ȃ̂����̐����j 
		if (selectedIndex != -1) {
			SetFontSize(30);
			auto& c = allChara[selectedIndex];
			int infoX = 800;
			int infoY = 200;
			DrawString(infoX, infoY, c->getName().c_str(), GetColor(255, 255, 255));

			char buf[128];
			sprintf_s(buf, "Lv:%d\nATK:%d\nDEF:%d\nSPD:%d\n�X�L��:%s\n����:%s", c->getLv(), c->getAttack(), c->getDefense(), c->getSpeed(),c->getSkillName().c_str(),c->getDetails().c_str());
			DrawString(infoX, infoY + 40, buf, GetColor(255, 255, 255));

			//DrawString(infoX, infoY + 80, ("�X�L��: " + c->getSkillName()).c_str(), GetColor(255, 255, 255));
			//DrawString(infoX, infoY + 120, ("����: " + c->getDetails()).c_str(), GetColor(255, 255, 255));
		}

		// ����LEVEL�\��
		SetFontSize(30);
		DrawFormatString(textPosX, textPosY, GetColor(255, 255, 255), "�������x��:%2d", Suggest);

		// �ύX����{�^��
		SetFontSize(50);
		DrawFormatString(boxX, boxY, GetColor(255, 255, 255), "<����>");

	}

	// �X�e�[�W�I����ʂɖ߂�{�^��
	SetFontSize(50);
	DrawFormatString(20, 10, GetColor(255, 255, 255), "<�߂�>");

	fade->fadeCircleDraw(WINDOW_W / 2, WINDOW_H / 2);

}

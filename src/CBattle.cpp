#include "CBattle.h"
#include "CResult.h"
#include "DxLib.h"

void CBattle::Update()
{

	// �t�F�[�h�̍X�V����
	fade->fadeUpdate(WINDOW_W);
	//retryFade->fadeUpdate(WINDOW_W);

	if (pauseState == PauseState::NONE && fade->checkOpen()) {

		for (auto& p : Manager::Instance().getParty()) {
			p->AnimUpdata();
		}

		// �C���f�b�N�X��0�̎��A�\�[�g���s���B
		if (turnOrder.empty()) TurnOrder();

		if (targetInput == TargetInput::P_NONE) targetInput = TargetInput::ACTIONCHOICE;
		// �s���I���Ǝ��s
		if (currentTurnIndex < turnOrder.size()) {

			auto character = turnOrder[currentTurnIndex];

			// �ړ�����
			playerMove();
			EnemyMove();

			if (character->getAlive() && !BattleOver()) { // �s�����ɐ퓬���I�����Ă��Ȃ����`�F�b�N


				// �v���C���[�ƃG�l�~�[�̌^��������S�ɂ���������dynamic
				if (std::dynamic_pointer_cast<Player>(character)) {	// �v���C���[					

					// ���͑҂����
					if (targetInput == TargetInput::ACTIONCHOICE) {
						// ���s�O�Ƀ��X�g����ɂ���B
						ListClea();
						ActionChoice(character);
					}
					// �^�[�Q�b�g���X�g����
					if (targetInput == TargetInput::LISTCREATE)TargetListCreate(character);
					//�@�^�[�Q�b�g�I��
					if (targetInput == TargetInput::TARGETCHOICE) TargetChoice(character);
					// �I��
					for (auto& p : Manager::Instance().getParty()) {
						if (p->getAnimChange()) {
							if (targetInput == TargetInput::END) PEnd(character);
						}
					}

				}
				else {	// �G�l�~�[
					// �G���s������ꍇ�̃^�[�Q�b�g�̓v���C���[
					// �����Ń^�[�Q�b�g�����߂�
					if (actionMode == ActionMode::E_NONE) {
						ListClea();
						actionMode = ActionMode::TARGETRANDOM;
					}

					if (actionMode == ActionMode::TARGETRANDOM)EnemyAction(character);
					if (actionMode == ActionMode::END)EEnd(character);
						
					
				}
			}
			else {
				// ���S���Ă����瑦�^�[���I��
				PEnd(character);
			}

		}
		else {
			// �^�[���I���֐�
			TurnEnd();
		}

		// �o�g���i�s�֐�
		// �o�g���̏I�����`�F�b�N�B
		if (BattleOver()) {
			// �����̒�~
			bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + stage));
			for (int i = 0; i < SE_MAX; i++) {
				se->stopSe(CLoad::Instance().getSeHandle(i));
			}
			// false�Ȃ�fade���O���B
			fade->fadeStart(fade->FADE_CLAUSE);
		}

		// ���j���[�o�[�������ꂽ�Ƃ��A���[�h�̐؂�ւ�
		if (CheckBoxClick(WINDOW_W - (FONT_BIGSIZE * 3 + 10), 20, FONT_BIGSIZE * 2, FONT_BIGSIZE)) {
			pauseState = PauseState::MENU;
		}
	}

	// MENU�I�����
	if (pauseState != PauseState::NONE) {

		switch (pauseState)
		{
		case CBattle::PauseState::MENU:

			// �I����ʂ̕\��
			// �o�g���ĊJ
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5), 250, FONT_BIGSIZE * 3, FONT_BIGSIZE)) {
				pauseState = PauseState::NONE;
				// se�Đ�
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// �Ē���
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5), 350, FONT_BIGSIZE * 3, FONT_BIGSIZE)){
				// ���[�h�ύX
				pauseState = PauseState::RETRY;
				// se�Đ�
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// ���^�C�A
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 2), 450, FONT_BIGSIZE * 4, FONT_BIGSIZE)) {
				// ���[�h�ύX
				pauseState = PauseState::RETIRE;
				// se�Đ�
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// �i�ݒ�j
			if (CheckBoxClick(WINDOW_W / 2 - FONT_BIGSIZE, 550, FONT_BIGSIZE * 2, FONT_BIGSIZE)) {
				pauseState = PauseState::SETTING;
				// se�Đ�
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

			}


			break;
		case CBattle::PauseState::RETRY:

			// �Ċm�F
			// �͂�
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 3), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), FONT_BIGSIZE * 2, FONT_BIGSIZE)) {
				// fade���O��
				fade->fadeStart(fade->FADE_CLAUSE);
				// �Ē���
				BattleInit();
				// �o�g����ʂɐ؂�ւ���
				pauseState = PauseState::NONE;
				// se�Đ�
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));

			}
			// ������
			if (CheckBoxClick(WINDOW_W / 2 + FONT_BIGSIZE, WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), FONT_BIGSIZE * 3, FONT_BIGSIZE)) {
				// MENU�̑I����ʂɖ߂�
				pauseState = PauseState::MENU;
				// se�Đ�
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			}


			break;
		case CBattle::PauseState::RETIRE:

			// �Ċm�F
			// �͂�
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 3), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), FONT_BIGSIZE * 2, FONT_BIGSIZE)) {
				
				// Party�S�������S�����
				for (auto& p : Manager::Instance().getParty()) {
					p->setAlive(false);
				}
				// fade���O��
				fade->fadeStart(fade->FADE_CLAUSE);
				// se�Đ�
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
				// bgm�̒�~
				bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + Manager::Instance().getStageScreen()));
			}
			// ������
			if (CheckBoxClick(WINDOW_W / 2 + FONT_BIGSIZE, WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), FONT_BIGSIZE * 3, FONT_BIGSIZE)) {
				// MENU�̑I����ʂɖ߂�
				pauseState = PauseState::MENU;
				// se�Đ�
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));

			}

			break;

		case CBattle::PauseState::SETTING:

			if (clickFrame > 0) {
				clickFrame--;
			}

			// ���ʕύX
			for (int i = 0; i < BUTTAN_MAX; i++) {

				int size;
				if (i % 2 == 0) {size = MAX_VOL / 10;}
				else {size = -MAX_VOL / 10;}

				if (clickFrame == 0 && CheckCircleClick(trPosX[i], trPosY[i] + size, 50)) {

					int bgmVol = Manager::Instance().getBgmVolume();
					int seVol = Manager::Instance().getSeVolume();

					clickFrame = 10;
					se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

					switch (i)
					{
					case BGM_UP:

						Manager::Instance().setBgmVolume(bgmVol + 1);

						break;

					case BGM_DOWN:
						Manager::Instance().setBgmVolume(bgmVol - 1);

						break;

					case SE_UP:
						Manager::Instance().setSeVolume(seVol + 1);

						break;

					case SE_DOWN:
						Manager::Instance().setSeVolume(seVol - 1);

						break;

					default:
						break;
					}

				}

			}

			// ����{�^������������
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H - 200, FONTSIZE * 3, FONTSIZE)) {
				// MENU�̑I����ʂɖ߂�
				pauseState = PauseState::MENU;
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}

			break;
		default:
			break;
		}
	}

	// fade���N����CResult�ɐ؂�ւ�
	if (fade->checkClause(WINDOW_W) && BattleOver()) {
		Manager::Instance().ChangeScreen<CResult>();
	}

}

void CBattle::Render()
{
	// �w�i�`��
	int x = 0;
	int y = 0;
	ui->BgRender(x, y, WINDOW_W, WINDOW_H,CLoad::Instance().getBgGrh());
	// ���^�[���ڂ��\��
	SetFontSize(FONT_SIZE);
	DrawFormatString(FONT_SIZE, 10, GetColor(255, 255, 255), "%2d�^�[����", this->turn);
	// �v���C���[�ƃG�l�~�[��1�^�[�����̍s������\��
	DrawTurnOrderIcons(30, 50);

	// �w�肵��ID�̃G�l�~�[��`��
	for (auto& e : enemies) {
		// �N���b�N���Ɏg�p����ϐ��Z�b�g
		e->setPosX(ePosX);
		e->setPosY(ePosY);
		//	�{��
		e->Render(ePosX, ePosY, CLoad::Instance().getEnemyGrh(stage));
		e->powerRender();
	}
	// �v���C���[�̕`��
	for (size_t i = 0; i < Manager::Instance().getParty().size(); i++) {
		auto& players = Manager::Instance().getParty();
		// �N���b�N���Ɏg�p����ϐ��Z�b�g
		int x = pPosX[i] + (i % 2) * 50;
		int y = pPosY[i] + i * 80;

		players[i]->setPosX(x);
		players[i]->setPosY(y);
		//	�{��
		players[i]->Render(x, y, CLoad::Instance().getPlayerGrh(players[i]->getId()));
		
	}
	
	// �v���C���[�̃G�t�F�N�g�A�j���[�V����
	for (auto& p : Manager::Instance().getParty()) {
		p->animDraw();
		p->powerRender();
	}

	// �I���ł���L�����N�^�[���~��`��
	if (targetInput == TargetInput::TARGETCHOICE) {

		// �s���L�����Z���p�̃{�^��
		SetFontSize(FONT_BIGSIZE);
		DrawFormatString(WINDOW_W - 160, 70, GetColor(255, 255, 255), "<�߂�>");

		for (auto& t : TargetList) {
			Position pos = GetCharacterCenter(t);
			// �ԐF�̘g
			DrawCircle(pos.x, pos.y, 40, GetColor(255, 0, 0), FALSE); 
		}
	}


	if (targetInput == TargetInput::ACTIONCHOICE) {
		// �{�^���̕\��
		ui->Button(ButtonX, atButtonY, ButtonX + ButtonSizeX, atButtonY + ButtonSizeY, CLoad::Instance().getAtkButtonGrh());
		ui->Button(ButtonX, skButtonY, ButtonX + ButtonSizeX, skButtonY + ButtonSizeY, CLoad::Instance().getSklButtonGrh());

		// �X�L���|�C���g�̕\��
		ui->SkillPoint(100, 300, SP_RADIUS, sp->getSP(), sp->getMaxSP());
	}
	// ���j���[�o�[�̕\��
	SetFontSize(FONT_BIGSIZE);
	if (pauseState == PauseState::NONE) {
		DrawFormatString(WINDOW_W - (FONT_BIGSIZE * 3 + 10), 20, GetColor(255, 255, 255), "<MENU>");
	}

	//�@MENU�̒��g
	if (pauseState != PauseState::NONE) {

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, WINDOW_W, WINDOW_H, GetColor(50,50,50), true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		switch (pauseState)
		{
		case CBattle::PauseState::MENU:

			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5) , 250, GetColor(255, 255, 255), "������");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5), 350, GetColor(255, 255, 255), "�Ē���");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 2), 450, GetColor(255, 255, 255), "���^�C�A");
			DrawFormatString(WINDOW_W / 2 - FONT_BIGSIZE, 550, GetColor(255, 255, 255), "�ݒ�");

			break;
		case CBattle::PauseState::RETRY:

			// ����
			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE *4),		WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "�Ē��킵�܂����H");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 3),	WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "�͂�");
			DrawFormatString(WINDOW_W / 2 + FONT_BIGSIZE,			WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "������");

			break;
		case CBattle::PauseState::RETIRE:

			// ����
			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 4.5), WINDOW_H / 2 - (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "���^�C�A���܂����H");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 3), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "�͂�");
			DrawFormatString(WINDOW_W / 2 + FONT_BIGSIZE, WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), GetColor(255, 255, 255), "������");

			break;

		case CBattle::PauseState::SETTING:

			SetFontSize(FONT_BIGSIZE);
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H / 2 - (FONT_BIGSIZE * 4.5), GetColor(255, 255, 255), "���ʐݒ�");
			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE * 7), WINDOW_H / 2 - (FONT_BIGSIZE), GetColor(255, 255, 255), "BGM:%d",Manager::Instance().getBgmVolume());
			DrawFormatString(WINDOW_W / 2 + (FONT_BIGSIZE * 4), WINDOW_H / 2 - (FONT_BIGSIZE), GetColor(255, 255, 255), "SE:%d",Manager::Instance().getSeVolume());

			// ���ʂ̒����{�^��
			for (int i = 0; i < BUTTAN_MAX; i++) {

				int x1;
				int x2;
				int y1;
				int y2;

				switch (i)
				{
				case BGM_UP:

					x1 = trPosX[BGM_UP] - 30;
					x2 = trPosX[BGM_UP] + 30;
					y1 = y2 = trPosY[BGM_UP] + 50;

					break;
				case BGM_DOWN:


					x1 = trPosX[BGM_DOWN] - 30;
					x2 = trPosX[BGM_DOWN] + 30;
					y1 = y2 = trPosY[BGM_DOWN] - 50;


					break;
				case SE_UP:

					x1 = trPosX[SE_UP] - 30;
					x2 = trPosX[SE_UP] + 30;
					y1 = y2 = trPosY[SE_UP] + 50;


					break;
				case SE_DOWN:

					x1 = trPosX[SE_DOWN] - 30;
					x2 = trPosX[SE_DOWN] + 30;
					y1 = y2 = trPosY[SE_DOWN] - 50;

					break;

				default:
					break;
				}

				DrawTriangle(trPosX[i], trPosY[i], x1,y1,x2,y2, GetColor(255, 255, 255), true);
			}

			DrawFormatString(WINDOW_W / 2 - (FONT_BIGSIZE*2), WINDOW_H - 200, GetColor(255, 255, 255), "<����>");

			break;


		default:
			break;
		}
	}

}

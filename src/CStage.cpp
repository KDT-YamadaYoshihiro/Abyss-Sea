#include "CStage.h"
#include "CBattle.h"
#include "CPartySelect.h"

// �X�V����
void CStage::Update()
{

	// �t�F�[�h�̍X�V����
	fade->fadeUpdate(WINDOW_W);

	// �O�p�`�̍��W�X�V
	Triangle();

	// �X�e�[�W�X�N���[�����Ɍ���{�^���������ꂽ���ʂ��؂�ւ��
	// (CStage -> CPatySelect)
	if (CheckBoxClick(boxX, boxY, boxSizeX, boxSizeY) == true) {
		se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
		Manager::Instance().setStageScreen(stageNum);
		Manager::Instance().setSuggest(Suggest);
		Manager::Instance().ChangeScreen<CPartySelect>();
	}

}

// �`�揈��
void CStage::Render()
{
	// �w�i
	DrawExtendGraph(0, 0, WINDOW_W, WINDOW_H, CLoad::Instance().getStageBgGrh(), true);

	// 
	DrawString(100, 100, "�X�e�[�W", GetColor(255, 255, 255), true);

	// �I�𒆂̎O�p�`
	DrawTriangle(TrX1, TrY1, TrX2, TrY2, TrX3, TrY3, GetColor(255, 0, 0), true);

	// ����{�^���i�l�p�j
	SetFontSize(50);
	DrawFormatString(boxX, boxY, GetColor(255, 255, 255), "<�Ґ�>");

	// �X�e�[�W��
	SetFontSize(50);
	DrawFormatString(posX[STAGE1], posY[STAGE1], GetColor(255, 255, 255), "�ystage1:�Y�����e�H�z");
	DrawFormatString(posX[STAGE2], posY[STAGE2], GetColor(255, 255, 255), "�ystage2:�ޗ��̒�̃��m�z");
	DrawFormatString(posX[STAGE3], posY[STAGE3], GetColor(255, 255, 255), "�ystage3:�[���̋��|�z");
	
}

void CStage::Triangle()
{


	// �~��Click���ꂽ��
	if (!selectMode) {
		for (int i = 0; i < CIRCLEMAX; i++) {
			if (CheckBoxClick(posX[i], posY[i], 700, 50)) {
				if (stageNum != i) {
					// �O�p�`�̍��W��Click�����~�̏㕔�ɂ���
					stageNum = i;
					se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
				}
				break;
			}
		}
	}

	// �I�𒆂̃X�e�[�W�̏㕔�ɎO�p�`��`�悳����B
	switch (stageNum)
	{
	case STAGE1:
		TrX1 = posX[STAGE1];
		TrY1 = posY[STAGE1]+ 25;
		TrX2 = TrX1 - 50;
		TrY2 = TrY1 + 20;
		TrX3 = TrX1 - 50;
		TrY3 = TrY1 - 20;

		Suggest = 1;

		break;

	case STAGE2:
		TrX1 = posX[STAGE2];
		TrY1 = posY[STAGE2] +25;
		TrX2 = TrX1 - 50;
		TrY2 = TrY1 + 20;
		TrX3 = TrX1 - 50;
		TrY3 = TrY1 - 20;

		Suggest = 10;

		break;

	case STAGE3:
		TrX1 = posX[STAGE3];
		TrY1 = posY[STAGE3] + 25;
		TrX2 = TrX1 - 50;
		TrY2 = TrY1 + 20;
		TrX3 = TrX1 - 50;
		TrY3 = TrY1 - 20;

		Suggest = 25;

		break;

	default:
		break;
	}


}

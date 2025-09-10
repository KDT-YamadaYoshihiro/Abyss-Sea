#include "CTitle.h"
#include "CStage.h"

// �X�V����
void CTitle::Update()
{

	bgm->PlayBgm(CLoad::Instance().getBgmHandle(BGM_START));

	// CLICK�@TO START
	// �㉺�ɗh���B
	posY += sway;
	if (posY >= TOP_RANGE) {
		sway *= -1;
	}
	if (posY <= DWON_RANGE) {
		sway *= -1;
	}

	// �w�i�ړ�
	for (int i = 0; i < TWO; i++) {
		ui[i]->BgScroll(bgPosX[i], bgPosY[i], WINDOW_W, WINDOW_H);
	}

	// �^�C�g���X�N���[�����ɃN���b�N����Ɖ�ʂ��؂�ւ��B
	// (CTitle -> CStage)
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		se->PlaySe(CLoad::Instance().getSeHandle(SE_START));
		Manager::Instance().ChangeScreen<CStage>();
	}

}

// �`�揈��
void CTitle::Render()
{
	// �w�i�`��
	for (int i = 0; i < TWO; i++)
	{
		ui[i]->BgRender(bgPosX[i], bgPosY[i], WINDOW_W, WINDOW_H, CLoad::Instance().getBgGrh());
	}

	// �^�C�g������
	SetFontSize(FONTSIZE);
	DrawGraph(100, 200, CLoad::Instance().getTitleGrh(), true);

	// �N���b�Nto�X�^�[�g
	SetFontSize(FONTSIZE / 2);
	DrawFormatString(posX, posY, GetColor(255, 255, 255), "CLICK TO START", true);


}

// �f�X�g���N�^
CTitle::~CTitle()
{
}

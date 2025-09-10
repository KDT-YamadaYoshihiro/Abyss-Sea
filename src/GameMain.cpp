#include "GameMain.h"

// ������
CGame::CGame() {

}

// �I��
CGame::~CGame() {

}

// ����������
bool CGame::Init()
{
	SRand((int)time(NULL));
	InputKeyInit();
	InputPadInit();
	

	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(Manager::Instance().getWindowMode());
	SetGraphMode(WINDOW_W, WINDOW_H, 32);

	if (Manager::Instance().getWindowMode() == TRUE)
	{
		SetWindowSize(WINDOW_W, WINDOW_H);
	}
	SetBackgroundColor(0, 0, 0);
	SetMainWindowText("Abyss Sea");
	if (DxLib_Init() == -1) { return -1; }

	SetDrawScreen(DX_SCREEN_BACK);

	Manager::Instance().Init();

	

}
// ���[�v����
void CGame::Run()
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//	���t���b�V�����[�g��ݒ肷�邽�߂̏���
		clock_t check_fps = clock() + CLOCKS_PER_SEC / 60;

		clsDx();
		ClearDrawScreen();
		InputKeyUpdate();
		InputPadUpdate();

		this->Update();
		this->Render();

		//	���t���b�V�����[�g�����ɂȂ�܂ő҂���
		while (clock() < check_fps) {}

		ScreenFlip();
	}

}
// �X�V����
void CGame::Update()
{
	Manager::Instance().Update();
}

// �`�揈��
void CGame::Render()
{
	Manager::Instance().Render();
}

// �I������
void CGame::End()
{
	InputKeyExit();
	InputPadExit();
	DxLib_End();
}
// 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

	CGame::Instance().Init();
	CGame::Instance().Run();
	CGame::Instance().End();

	return 0;
}
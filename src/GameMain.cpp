#include "GameMain.h"


// 初期化処理
bool CGame::Init()
{
	SRand((int)time(NULL));
	InputKeyInit();
	InputPadInit();
	ChangeFont("源界明朝"); 


	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(ScreenManager::Instance().getWindowMode());
	SetGraphMode(WINDOW_W, WINDOW_H, 32);

	if (ScreenManager::Instance().getWindowMode() == TRUE)
	{
		SetWindowSize(WINDOW_W, WINDOW_H);
	}
	SetBackgroundColor(0, 0, 0);
	SetMainWindowText("Abyss Sea");
	if (DxLib_Init() == -1) { return -1; }

	SetDrawScreen(DX_SCREEN_BACK);

	ScreenManager::Instance().Init();

}
// ループ処理
void CGame::Run()
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//	リフレッシュレートを設定するための処理
		clock_t check_fps = clock() + CLOCKS_PER_SEC / 60;
		SetMouseDispFlag(TRUE);

		clsDx();
		ClearDrawScreen();
		InputKeyUpdate();
		InputPadUpdate();

		this->Update();
		this->Render();

		//	リフレッシュレートが一定になるまで待つ処理
		while (clock() < check_fps) {}

		ScreenFlip();
	}

}

// 終了処理
void CGame::End()
{
	InputKeyExit();
	InputPadExit();
	DxLib_End();
}


// 更新処理
void CGame::Update()
{
	ScreenManager::Instance().Update();
}

// 描画処理
void CGame::Render()
{
	ScreenManager::Instance().Render();
}

// 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

	CGame::Instance().Init();
	CGame::Instance().Run();
	CGame::Instance().End();

	return 0;
}
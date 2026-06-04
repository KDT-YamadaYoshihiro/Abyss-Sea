#pragma once
#include "Scene/Manager/Manager.h"
#include "Character/Character.h"
#include "Character/Factory/CharacterFactory.h"
#include "Direction/BubbleEffect/BubbleEffect.h"

// タイトルスクリーンクラス
class CTitle : public ScreenBase {

	// UIの数
#define UI_MAX		2

	std::shared_ptr<Sound>bgm;
	std::shared_ptr<Sound> se;

	std::shared_ptr<UI> ui;

	std::shared_ptr<FadeAlpha> fade;

	std::shared_ptr<BubbleEffect> bubble;

	float posX = -1.0f;
	float posY = -1.0f;
	float sway = -1.0f;

	int bgPosX;
	int bgPosY;

public:

	void Update() override;

	void Render() override;

	CTitle():
		bgm(nullptr),
		se(nullptr),
		fade(nullptr),
		posX(WINDOW_W / 2.0f),
		posY(600.0f),
		sway(0.1f)
	{

		bgm = ScreenManager::Instance().getbgm();
		se = ScreenManager::Instance().getSe();
		fade = ScreenManager::Instance().getFade();
		bubble = std::make_shared<BubbleEffect>();

		ui = std::make_shared<UI>();
			
		bgPosX= 0;
		bgPosY= 0;
		

		bubble->Play(20.0f, 1, 1.0f, 600);
	};

	virtual ~CTitle();

};
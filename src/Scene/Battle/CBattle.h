#pragma once
#include "Scene/CBase.h"
#include "DisplaySystem/UI/UI.h"
#include "Scene/StageSelect/CStage.h"
#include "Character/Factory/CharacterFactory.h"
#include "Character/Character.h"
#include "Character/Enemy/Enemy.h"
#include "Math/Calculation.h"
#include "Constants/define.h"
#include "Battle/BattleMenu/BattleMenu.h"
#include "Battle/BattleSystem/BattleSystem.h"
#include "Battle/BattleRenderer/BattleRenderer.h"
#include "Battle/BattlePresenter/BattlePresenter.h"
#include <memory>

class FadeAlpha;
class Effect;

class CBattle : public ScreenBase {
    enum class State { BATTLE, MENU, DEAD_EFFECT, END };
    State state = State::BATTLE;

    std::shared_ptr<FadeAlpha> fade;
    std::shared_ptr<Effect> effect;
    std::shared_ptr<Calculation> cal;

    std::shared_ptr<BattleSystem> m_battleSystem;
    std::shared_ptr<BattleRenderer> m_battleRenderer;
    std::shared_ptr<BattlePresenter> m_battlePresenter;
    std::unique_ptr<BattleMenu> battleMenu;

    int end_frame = 60;

public:
    CBattle();
    virtual ~CBattle() = default;

    void Update() override;
    void Render() override;

private:
    void BattleInit();
    void PlayEndEffect();
    void BattleEnd();
};
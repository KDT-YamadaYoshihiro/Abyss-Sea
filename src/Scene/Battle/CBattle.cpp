#include "CBattle.h"
#include "Scene/Result/CResult.h"
#include "DxLib.h"
#include "Mission/Manager/MissionManager.h"
#include "Scene/Manager/Manager.h"
#include "Load/Load.h"


CBattle::CBattle() {
    BattleInit();
}

void CBattle::BattleInit() {
    battleMenu = std::make_unique<BattleMenu>();
	fade = ScreenManager::Instance().getFade();
    m_battleSystem = std::make_shared<BattleSystem>();
    m_battleRenderer = std::make_shared<BattleRenderer>();
    m_battlePresenter = std::make_shared<BattlePresenter>(m_battleSystem, m_battleRenderer);

    m_battleSystem->Init();
    m_battlePresenter->Init();
}

void CBattle::Update() {
    std::vector<std::shared_ptr<Character>> select_party;
    for (auto& p : ScreenManager::Instance().getParty()) {
        select_party.push_back(p);
    }
    MissionManager::Instance().IncludeCharacter(ScreenManager::Instance().getStageScreen(), select_party);

    if (fade) fade->fadeUpdate();

    for (auto& e : m_battleSystem->GetEnemies()) {
        e->CutinUpdate();
        endEffectPlayed = e->UpdateDeathFade();
    }

    for (auto& p : ScreenManager::Instance().getParty()) {
        p->AnimUpdata();
        p->CutinUpdate();
        if (!p->getAlive()) {
            p->setAnimType(GRTYPE::DEAD);
        }
    }

    BattleMenuResult result;
    switch (state) {
    case State::BATTLE:
        if (fade && !fade->checkOpen()) { return; }

        m_battleSystem->Update();

        if (Mouse::CheckBoxClick(WINDOW_W - 250, 10, 250, 150)) {
            state = State::MENU;
        }

        if (m_battleSystem->IsBattleOver()) {
            state = State::DEAD_EFFECT;
        }
        break;

    case State::MENU:
        result = battleMenu->Update();
        if (result == BattleMenuResult::CLOSE) {
            state = State::BATTLE;
        }
        else if (result == BattleMenuResult::RETRY) {
            m_battleSystem->Delete();
            BattleInit();
            state = State::BATTLE;
        }
        else if (result == BattleMenuResult::RETIRE) {
            for (auto& p : ScreenManager::Instance().getParty()) {
                p->setAlive(false);
            }
            state = State::END;
        }
        break;

    case State::DEAD_EFFECT:
        PlayEndEffect();
        break;

    case State::END:
        BattleEnd();
        break;
    }

    MissionManager::Instance().TurnLimit(ScreenManager::Instance().getStageScreen(), m_battleSystem->GetTurn());

    if (fade && fade->checkClause() && m_battleSystem->IsBattleOver()) {
        m_battleSystem->StopBgm();
        ScreenManager::Instance().ChangeScreen<CResult>();
    }
}

void CBattle::Render() {
     m_battlePresenter->Render();

     if (state == State::MENU) {
         battleMenu->Draw();
     }
}

void CBattle::PlayEndEffect() {
    for (auto& e : m_battleSystem->GetEnemies()) {
        if (!e->getAlive()) {
            for (auto& p : ScreenManager::Instance().getParty()) p->setAnimType(GRTYPE::WIN);
        }
        else {
            for (auto& p : ScreenManager::Instance().getParty()) p->setAnimType(GRTYPE::LOSE);
        }
    }
    if (endEffectPlayed) {
        state = State::END;
    }
}

void CBattle::BattleEnd() {
    if (fade) fade->fadeStart(fade->FADE_CLAUSE);
}
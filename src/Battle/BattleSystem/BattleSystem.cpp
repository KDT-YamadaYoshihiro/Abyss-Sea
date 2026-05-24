#include "BattleSystem.h"
#include "Scene/Manager/Manager.h"
#include "Character/Factory/CharacterFactory.h"
#include "Character/Enemy/Enemy.h"
#include "Character/Player/CPlayer.h"
#include "Mission/Manager/MissionManager.h"
#include "Scene/Battle/CBattle_SP.h"
#include "Load/Load.h"
#include "System/InPut/Mouse/Mouse.h"
#include "Direction/Sound/Sound.h"

BattleSystem::BattleSystem()
    : m_currentPhase(BATTLE_PHASE::INIT),
    m_turn(0),
    m_atButtonX(450),
    m_buttonY(WINDOW_H - 60),
    m_skButtonX(660),
    m_buttonSizeX(BUTTAN_WIDTH),
    m_buttonSizeY(50),
    m_nameMax(6),
    m_blendNum(50),
    m_blendSpeed(1),
    m_endFrame(120),
    m_boxX(860),
    m_boxY(WINDOW_H - 90),
    m_skDescDraw(false)
{
    m_playerPosX.resize(PLAYER_MAX);
    m_playerPosY.resize(PLAYER_MAX);
    for (int i = 0; i < PLAYER_MAX; i++) {
        m_playerPosX[i] = 300 + (i % 2) * 50;
        m_playerPosY[i] = WINDOW_H / 2 + 60;
    }
}

void BattleSystem::Init()
{
    m_ui = std::make_shared<UI>();
    m_sp = std::make_shared<BattleSP>();
    m_cal = std::make_shared<Calculation>();

    m_stage = ScreenManager::Instance().getStageScreen();
    m_enemyId = ScreenManager::Instance().getEnemyID(m_stage);

    auto enemy = CharacterFactory::Instance().CreateCharacter<Enemy>(m_enemyId);
    if (enemy) {
        m_enemies.push_back(enemy);
    }

    for (auto& p : ScreenManager::Instance().getParty()) {
        p->hpReset();
        p->BuffReset();
        p->setAnimType(WAIT);
        p->effectrReset();
    }
    for (auto& e : m_enemies) {
        e->hpReset();
        e->BuffReset();
        e->effectrReset();
    }

    m_currentTurnIndex = 0;
    m_turn = 1;
    m_sp->ResetSP();
    ScreenManager::Instance().setBattleResult(BATTLE_RESULT::BATTLE);

	m_bgm = ScreenManager::Instance().getbgm();
	m_se = ScreenManager::Instance().getSe();

    m_bgm->PlayBgm(CLoad::Instance().getBgmHandle(1 + m_stage));
    m_endFrame = 120;

    TurnOrder();
    m_currentPhase = BATTLE_PHASE::TURN_START;
}

void BattleSystem::Update()
{
    CheckAliveStatus();

    if (IsBattleOver()) {
        return;
    }

    m_blendNum += m_blendSpeed;
    if (m_blendNum <= 0) { m_blendSpeed *= -1; }
    if (m_blendNum >= 50) { m_blendSpeed *= -1; }

    switch (m_currentPhase)
    {
    case BATTLE_PHASE::TURN_START:
        if (m_turnOrder.empty()) {
            TurnOrder();
        }
        if (m_targetInput == TargetInput::P_NONE) {
            m_targetInput = TargetInput::ACTIONCHOICE;
        }
        m_currentPhase = BATTLE_PHASE::ACTION;
        break;

    case BATTLE_PHASE::ACTION:
        if (m_currentTurnIndex < m_turnOrder.size()) {
            auto character = m_turnOrder[m_currentTurnIndex];

            PlayerMove();
            EnemyMove();

            if (character->getAlive() && !IsBattleOver()) {
                if (std::dynamic_pointer_cast<Player>(character)) {
                    if (m_targetInput == TargetInput::ACTIONCHOICE) {
                        ListClear();
                        ActionChoice(character);
                    }
                    if (m_targetInput == TargetInput::LISTCREATE) {
                        TargetListCreate(character);
                    }
                    if (m_targetInput == TargetInput::TARGETCHOICE) {
                        TargetChoice(character);
                    }
                    for (auto& p : ScreenManager::Instance().getParty()) {
                        if (p->getAnimChange() && m_targetInput == TargetInput::END) {
                            PEnd(character);
                        }
                    }
                }
                else {
                    if (m_actionMode == ActionMode::E_NONE) {
                        ListClear();
                        EnemyActionInit(character);
                    }
                    if (m_actionMode == ActionMode::TARGETRANDOM) {
                        EnemyAction(character);
                    }
                    if (m_actionMode == ActionMode::END) {
                        EEnd(character);
                    }
                }
            }
            else {
                PEnd(character);
            }
        }
        else if (m_turnOrder.empty() || m_currentTurnIndex >= m_turnOrder.size()) {
            m_currentPhase = BATTLE_PHASE::TURN_END;
            return;
        }
        break;

    case BATTLE_PHASE::TURN_END:
        TurnEnd();
        ProcessTurn();
        break;

    default:
        break;
    }
}

void BattleSystem::Delete()
{
    m_enemies.clear();
}

void BattleSystem::CheckAliveStatus()
{
    bool playerAlive = false;
    for (auto& p : ScreenManager::Instance().getParty()) {
        if (p->getAlive()) {
            playerAlive = true;
            break;
        }
    }

    bool enemyAlive = false;
    for (auto& e : m_enemies) {
        if (e->getAlive()) {
            enemyAlive = true;
            break;
        }
    }

    if (!playerAlive) {
        m_currentPhase = BATTLE_PHASE::LOSE;
    }
    else if (!enemyAlive) {
        m_currentPhase = BATTLE_PHASE::WIN;
    }
}

void BattleSystem::ProcessTurn()
{
    m_currentPhase = BATTLE_PHASE::TURN_START;
}

void BattleSystem::ActionChoice(std::shared_ptr<Character> arg_character) {
    if (Mouse::CheckBoxClick(m_atButtonX, m_buttonY, m_buttonSizeX, m_buttonSizeY)) {
         m_se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
        arg_character->setActionChoice(ATTACK);
        m_targetInput = TargetInput::LISTCREATE;
    }
    else if (Mouse::CheckBoxClick(m_skButtonX, m_buttonY, m_buttonSizeX, m_buttonSizeY)) {
        if (m_sp->getSP() > 0) {
             m_se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
            arg_character->setActionChoice(SKILL);
            m_skDescDraw = true;
            m_targetInput = TargetInput::LISTCREATE;
        }
        else {
             m_se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
        }
    }
}

void BattleSystem::TargetListCreate(std::shared_ptr<Character> arg_character) {
    m_targetList.clear();

    if (arg_character->getActionChoice() == ATTACK ||
        arg_character->getTargetType() == SKILL_TARGET_TYPE::SINGLE_ENEMY ||
        arg_character->getTargetType() == SKILL_TARGET_TYPE::ALL_ENEMY) {
        for (auto& e : m_enemies) {
            if (e->getAlive()) {
                m_targetList.push_back(e);
            }
        }
    }
    else if (arg_character->getTargetType() == SKILL_TARGET_TYPE::SINGLE_ALLY ||
        arg_character->getTargetType() == SKILL_TARGET_TYPE::ALL_ALLY) {
        for (auto& p : ScreenManager::Instance().getParty()) {
            if (p->getAlive()) {
                m_targetList.push_back(p);
            }
        }
    }
    else if (arg_character->getTargetType() == SKILL_TARGET_TYPE::SELF) {
        m_targetList.push_back(arg_character);
    }
    m_targetInput = TargetInput::TARGETCHOICE;
}

void BattleSystem::TargetChoice(std::shared_ptr<Character> arg_character) {
    if (Mouse::CheckBoxClick(m_boxX, m_boxY, BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
         m_se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
        m_targetInput = TargetInput::ACTIONCHOICE;
        m_skDescDraw = false;
        return;
    }

    for (auto& tar : m_targetList) {
        Position pos = GetCharacterCenter(tar);
        std::vector<std::shared_ptr<Character>> actTargets;

        if (m_playerAction == NONE_ACTION && Mouse::CheckCircleClick(pos.x, pos.y, 40.0f)) {
            m_playerAction = CHOICE;
             m_se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
            m_selectTarget = tar;
            TypeConversion();

            if (arg_character->getActionChoice() == SKILL && !m_sp->comfirmSP()) return;
        }

        switch (m_playerAction) {
        case NONE_ACTION:
            break;
        case CHOICE:
            if (arg_character->getActionChoice() == SKILL) {
                arg_character->StartCutin();
                m_playerAction = CUTIN;
            }
            else {
                m_playerAction = ACTHON;
            }
            break;
        case CUTIN:
            if (arg_character->getCutinEnd()) {
                m_playerAction = ACTHON;
            }
            break;
        case SKILL:
            if (arg_character->getTargetType() == SKILL_TARGET_TYPE::ALL_ENEMY ||
                arg_character->getTargetType() == SKILL_TARGET_TYPE::ALL_ALLY) {
                actTargets = m_targetList;
            }
            else {
                actTargets.push_back(m_selectTarget);
            }
            arg_character->TakeAction(actTargets);
            PlayEffectByType(arg_character, actTargets);
            m_targetInput = TargetInput::END;
            m_playerAction = NONE_ACTION;
            break;
        default:
            break;
        }
    }
}

void BattleSystem::PlayEffectByType(std::shared_ptr<Character> actor, const std::vector<std::shared_ptr<Character>>& targets) {
    EFFECT_TYPE efType = actor->getEfType();

    if (actor->getActionChoice() == SKILL) {
        switch (efType) {
        case EFFECT_TYPE::EF_SKILL1:
            for (auto& t : targets) actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 300, 250);
            break;
        case EFFECT_TYPE::EF_SKILL2:
            for (auto& t : targets) actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 350, 250);
            break;
        case EFFECT_TYPE::EF_ALL_SKILL1:
            for (auto& t : targets) actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 100, 100);
            break;
        case EFFECT_TYPE::EF_ALL_SKILL2:
            for (auto& t : targets) actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 250, 250);
            break;
        case EFFECT_TYPE::EF_HEAL:
            for (auto& t : targets) actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 30, 40);
            break;
        case EFFECT_TYPE::EF_BUFF:
        case EFFECT_TYPE::EF_DEBUFF:
            for (auto& t : targets) actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 50, 40);
            break;
        default:
            break;
        }
    }
    else {
        for (auto& t : targets) actor->PlayAttackEffect(t->getPosX(), t->getPosY(), 50, 40);
    }
}

void BattleSystem::EnemyActionInit(std::shared_ptr<Character> arg_character) {
    arg_character->StartCutin();
    m_actionMode = ActionMode::TARGETRANDOM;
}

void BattleSystem::EnemyAction(std::shared_ptr<Character> arg_character) {
    if (arg_character->getCutinEnd()) {
        for (auto& p : ScreenManager::Instance().getParty()) {
            if (p->getAlive()) m_targetList.push_back(p);
        }

        int index = rand() % m_targetList.size();
        m_selectTarget = m_targetList[index];
        TypeConversion();

        if (arg_character->getCutinEnd()) {
            arg_character->TakeAction(m_targets);
        }

        for (auto& e : m_enemies)
        {
            e->setMoveCheck(true);
        }

        auto& p = ScreenManager::Instance().getParty();
        for (size_t i = 0; i < p.size(); i++) {
            if (m_targetList[index]->getId() == p[i]->getId())
                p[i]->setAnimType(DAMAGE);
        }

        m_actionMode = ActionMode::END;
    }
}

void BattleSystem::PEnd(std::shared_ptr<Character> arg_character) {
    for (auto& p : ScreenManager::Instance().getParty()) {
        p->setAnimType(WAIT);
        p->setAnimChange(false);
    }

    m_skDescDraw = false;
    m_targetInput = TargetInput::ACTIONCHOICE;
    UpdataSP(arg_character);

    m_currentTurnIndex++;
    if (m_currentTurnIndex >= m_turnOrder.size()) {
        TurnEnd();
        return;
    }
}

void BattleSystem::EEnd(std::shared_ptr<Character> arg_character) {
    for (auto& p : ScreenManager::Instance().getParty()) {
        p->setAnimType(WAIT);
        p->setAnimChange(false);
    }

    m_actionMode = ActionMode::E_NONE;
    m_currentTurnIndex++;
    if (m_currentTurnIndex >= m_turnOrder.size()) {
        TurnEnd();
        return;
    }
}

void BattleSystem::TurnEnd() {
    for (auto& p : ScreenManager::Instance().getParty()) {
        p->updateBuff();
    }
    for (auto& e : m_enemies) {
        e->updateBuff();
    }

    TurnOrder();
    m_currentTurnIndex = 0;
    m_turn++;
    m_currentPhase = BATTLE_PHASE::TURN_START; // reset to start next turn
}

void BattleSystem::TypeConversion() {
    m_targets = { m_selectTarget };
}

Position BattleSystem::GetCharacterCenter(std::shared_ptr<Character> arg_chara) {
    int x = arg_chara->getPosX();
    int y = arg_chara->getPosY();

    if (std::dynamic_pointer_cast<Player>(arg_chara)) {
        x += 100 / 2; // SIZE_W
        y += 100 / 2; // SIZE_H
    }
    return { x, y };
}

void BattleSystem::PlayerMove() {
    for (size_t i = 0; i < ScreenManager::Instance().getParty().size(); i++) {
        auto& p = ScreenManager::Instance().getParty();
        if (m_turnOrder[m_currentTurnIndex]->getId() == p[i]->getId()) {
            m_playerPosX[i] += 10;
            if (m_playerPosX[i] >= WINDOW_W / 2 - 150) {
                m_playerPosX[i] = WINDOW_W / 2 - 150;
            }
        }
        else {
            m_playerPosX[i] -= 10;
            if (m_playerPosX[i] <= 300 + (i % 2) * 50) {
                m_playerPosX[i] = 300 + (i % 2) * 50;
            }
        }
    }
}

void BattleSystem::EnemyMove() {
    for (auto& e : m_enemies)
    {
        e->Update();
    }
}

void BattleSystem::TurnOrder() {
    m_turnOrder.clear();
    for (const auto& p : ScreenManager::Instance().getParty()) {
        if (p->getAlive()) m_turnOrder.push_back(p);
    }
    for (const auto& e : m_enemies) {
        if (e->getAlive()) m_turnOrder.push_back(e);
    }

    if (!m_turnOrder.empty()) {
        std::sort(m_turnOrder.begin(), m_turnOrder.end(), [this](const std::shared_ptr<Character>& a, const std::shared_ptr<Character>& b) {
            int a_speed = m_cal->BuffCal(a->getSpeed(), a->getAgrbuff());
            int b_speed = m_cal->BuffCal(b->getSpeed(), b->getAgrbuff());
            return a_speed > b_speed;
            });
    }
}

void BattleSystem::ListClear() {
    m_targetList.clear();
    m_targets.clear();
}

void BattleSystem::UpdataSP(std::shared_ptr<Character> arg_character) {
    if (arg_character->getActionChoice() == ATTACK) {
        m_sp->recoverSP();
    }
    else {
        m_sp->consumeSP();
    }
}

bool BattleSystem::IsBattleOver() {
    bool allPlayersDefeated = true;
    for (auto& p : ScreenManager::Instance().getParty()) {
        if (p->getAlive()) {
            allPlayersDefeated = false;
            break;
        }
        else {
            ScreenManager::Instance().setBattleResult(BATTLE_RESULT::DEFEAT);
            MissionManager::Instance().ResetMissions(m_stage);
        }
    }

    bool enemiesDefeated = true;
    for (const auto& e : m_enemies) {
        if (e->getAlive()) {
            enemiesDefeated = false;
            break;
        }
        else {
            ScreenManager::Instance().setBattleResult(BATTLE_RESULT::VICTORY);
            ScreenManager::Instance().setExp(e->getExp());
            MissionManager::Instance().ClearQuest(m_stage);
        }
    }

    return allPlayersDefeated || enemiesDefeated;
}
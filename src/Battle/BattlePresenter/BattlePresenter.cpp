#include "BattlePresenter.h"
#include "Battle/BattleSystem/BattleSystem.h"
#include "Battle/BattleRenderer/BattleRenderer.h"

BattlePresenter::BattlePresenter(std::shared_ptr<BattleSystem> system, std::shared_ptr<BattleRenderer> renderer)
    : m_system(system), m_renderer(renderer) {
}

void BattlePresenter::Init() {
    if (m_renderer) {
        m_renderer->Init();
    }
}

void BattlePresenter::Render() {
    if (m_system && m_renderer) {
        m_renderer->RenderBattle(m_system);
    }
}
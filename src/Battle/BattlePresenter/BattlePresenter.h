#pragma once
#include <memory>

class BattleSystem;
class BattleRenderer;

class BattlePresenter {
private:
    std::shared_ptr<BattleSystem> m_system;
    std::shared_ptr<BattleRenderer> m_renderer;

public:
    BattlePresenter(std::shared_ptr<BattleSystem> system, std::shared_ptr<BattleRenderer> renderer);
    ~BattlePresenter() = default;

    void Init();
    void Render();
};
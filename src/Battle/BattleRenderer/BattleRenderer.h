#pragma once
#include <memory>
#include <vector>

class BattleSystem;
class UI;

class BattleRenderer {
private:
    std::shared_ptr<UI> m_ui;

public:
    BattleRenderer();
    ~BattleRenderer() = default;

    void Init();
    void RenderBattle(std::shared_ptr<BattleSystem> sys);

private:
    void DrawTurnOrderIcons(std::shared_ptr<BattleSystem> sys, int x, int y);
};
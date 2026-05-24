#include "BattleRenderer.h"
#include "Battle/BattleSystem/BattleSystem.h"
#include "DxLib.h"
#include "DisplaySystem/UI/UI.h"
#include "Scene/Manager/Manager.h"
#include "Character/Character.h"
#include "Character/Enemy/Enemy.h"
#include "Load/Load.h"
#include "Scene/Battle/CBattle_SP.h"

BattleRenderer::BattleRenderer() {
}

void BattleRenderer::Init() {
    m_ui = std::make_shared<UI>();
}

void BattleRenderer::RenderBattle(std::shared_ptr<BattleSystem> sys) {
    if (!sys) return;

	// BG
    int x = 0;
    int y = 0;
    m_ui->BgRender(x, y, WINDOW_W, WINDOW_H, CLoad::Instance().getBgGrh());

    // 青い半透明背景
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, sys->GetBlendNum());
    DrawBox(0, 0, WINDOW_W, WINDOW_H, GetColor(0, 0, 255), true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ターン数表示
    SetFontSize(FONT_SIZE);
    DrawFormatString(FONT_SIZE, 10, GetColor(255, 255, 255), "%2dターン目", sys->GetTurn());

	// 行動順アイコン
    DrawTurnOrderIcons(sys, 30, 50);

	// ステージ情報表示
    int mx = 30, my = 150;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawBox(mx, my, mx + 300, my + 100, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    SetFontSize(FONT_MINSIZE);
    ScreenManager::Instance().DrawStageUI(ScreenManager::Instance().getStageScreen(), mx + 30, my + 25);

    // SP
    auto sp = sys->GetSP();
    if (sp) m_ui->SkillPoint(530, WINDOW_H - 85, SP_RADIUS, sp->getSP(), sp->getMaxSP());

    // プレイヤー描画
    auto party = ScreenManager::Instance().getParty();
    for (size_t i = 0; i < party.size(); i++) {
        int px = sys->GetPlayerPosX()[i] + (i % 2) * 50;
        int py = sys->GetPlayerPosY()[i] + i * 50;
        party[i]->setPosX(px);
        party[i]->setPosY(py);
        party[i]->Render(px, py, CLoad::Instance().getPlayerGrh(party[i]->getId()));

        int uiX = 20 + (i * 105);
        m_ui->SelectStatus(uiX, WINDOW_H - 125, 100, 120, party[i]->getFaceHandle(), party[i]);
        party[i]->DrawIcons(uiX, WINDOW_H - 160);
    }

    // エネミー描画
    for (auto& e : sys->GetEnemies()) {
        int id = e->getId() - 100;
        e->Render(CLoad::Instance().getEnemyGrh(id));
        e->powerRender();
        if (!e->getAlive()) { e->DeadAnimDraw(); }
    }

    // ターゲット選択・行動選択のボタンとターゲットサークル
    if (sys->GetTargetInput() == TargetInput::ACTIONCHOICE) {
        m_ui->Button(sys->GetAtButtonX(), sys->GetButtonY(), sys->GetAtButtonX() + sys->GetButtonSizeX(), sys->GetButtonY() + sys->GetButtonSizeY(), CLoad::Instance().getAtkButtonGrh());
        m_ui->Button(sys->GetSkButtonX(), sys->GetButtonY(), sys->GetSkButtonX() + sys->GetButtonSizeX(), sys->GetButtonY() + sys->GetButtonSizeY(), CLoad::Instance().getSklButtonGrh());
    }
    else if (sys->GetTargetInput() == TargetInput::TARGETCHOICE) {
		// Backボタン
        int backX = sys->GetBoxX();
        int backY = sys->GetBoxY();
        m_ui->Button(backX, backY, backX + BUTTAN_WIDTH, backY + BUTTAN_HEIGHT, CLoad::Instance().getButtonGrh(BACK));

		// ターゲットサークル
        for (auto& t : sys->GetTargetList()) {
            Position pos = sys->GetCharacterCenter(t);
            DrawCircle(pos.x - 20, pos.y - 15, 40, GetColor(255, 0, 0), FALSE);
        }
    }


	// 攻撃エフェクト描画
    auto currentChara = sys->GetCurrentTurnCharacter();
    if (currentChara) {
        for (size_t p = 0; p < party.size(); p++) {
            if (currentChara->getTargetType() == SKILL_TARGET_TYPE::ALL_ALLY ||
                currentChara->getTargetType() == SKILL_TARGET_TYPE::ALL_ENEMY) {
                for (auto& t : sys->GetTargetList()) {
                    party[p]->EffectDraw(t->getPosX(), t->getPosY());
                }
            }
            else {
                auto sTarget = sys->GetSelectTarget();
                if (sTarget) party[p]->EffectDraw(sTarget->getPosX(), sTarget->getPosY());
            }
        }
    }

    for (auto& p : party) p->powerRender();

	// スキル説明描画
    if (sys->GetSkDescDraw() && currentChara) {
        int sx = 20, sy = WINDOW_H - 210, sw = 400, sh = 50;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawBox(sx, sy, sx + sw, sy + sh, GetColor(100, 100, 100), true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        SetFontSize(FONT_MINSIZE);
        DrawFormatString(sx + 10, sy + 10, GetColor(255, 255, 255), currentChara->getSkillName().c_str());
        DrawFormatString(sx + 10, sy + 10 + FONT_MINSIZE, GetColor(255, 255, 255), currentChara->getDetails().c_str());
    }

	// メニューボタン描画
    SetFontSize(FONT_BIGSIZE);
    int posX = WINDOW_W - 250, posY = 10;
    DrawExtendGraph(posX, posY, posX + 250, posY + 150, CLoad::Instance().getMenuButton(), true);

	// カットイン描画
    for (auto& e : sys->GetEnemies()) e->CutinDraw();
    for (auto& p : party) p->CutinDraw();
}

void BattleRenderer::DrawTurnOrderIcons(std::shared_ptr<BattleSystem> sys, int x, int y) {
    const int ICON_SIZE = 60;
    const int PADDING = 10;
    int drawX = x;
    auto turn_order = sys->GetTurnOrder();
    auto current_character = sys->GetCurrentTurnCharacter();

    for (size_t i = 0; i < turn_order.size(); ++i) {
        const auto& ch = turn_order[i];
        DrawExtendGraph(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, ch->getFaceHandle(), TRUE);

        if (ch == current_character) {
            DrawBox(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, GetColor(255, 255, 0), FALSE);
        }
        else {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            DrawBox(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, GetColor(0, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        SetFontSize(FONT_MINSIZE);
        std::string name = ch->getName();
        if ((int)name.size() > sys->GetNameMax()) name = name.substr(0, sys->GetNameMax());
        DrawFormatString(drawX, y + ICON_SIZE + 4, GetColor(255, 255, 255), "%s", name.c_str());
        drawX += ICON_SIZE + PADDING;
    }
}
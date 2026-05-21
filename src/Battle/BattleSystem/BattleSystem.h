#pragma once
#include <vector>
#include <memory>
#include "Constants/enum.h"
#include "Constants/define.h"

// 前方宣言
class Character;
class Enemy;
class UI;
class BattleSP;
class Sound;

enum class BATTLE_PHASE {
    INIT,           // 初期化、開始演出
    TURN_START,     // ターン開始
    ACTION,         // プレイヤーとエネミーの行動選択と実行
    TURN_END,       // ターン終了
    WIN,            // 勝利
    LOSE            // 敗北
};

class BattleSystem
{
private:
    // 1. 内部ステータス・制御変数
    BATTLE_PHASE m_currentPhase;
    int m_stage = -1;
    int m_enemyId = 0;

    enum class TargetInput { P_NONE, ACTIONCHOICE, LISTCREATE, TARGETCHOICE, END };
    TargetInput targetInput = TargetInput::P_NONE;

    enum class ActionMode { E_NONE, TARGETRANDOM, END };
    ActionMode actionMode = ActionMode::E_NONE;

    PLAYER_ACTION player_action = NONE_ACTION;

    // 2. キャラクター・システムインスタンス
    std::vector<std::shared_ptr<Enemy>> m_enemies;
    std::vector<std::shared_ptr<Character>> turn_order; // 行動順リスト
    size_t current_turn_index = 0;

    std::shared_ptr<UI> ui;
    std::shared_ptr<BattleSP> sp;
    std::shared_ptr<Sound> bgm;
    std::shared_ptr<Sound> se;

    // 3. 座標・演出用変数
    int at_button_x;
    int sk_button_x;
    int button_y;
    int button_size_x;
    int button_size_y;
    int name_max;
    int blend_num;
    int blend_speed;
    int end_frame = 60; // 終了演出用フレーム

public:
    // ----------------------------------------------------
    // 1. 構造
    // ----------------------------------------------------
    BattleSystem();
    ~BattleSystem() = default;

    // ----------------------------------------------------
    // 2. 初期化・システム制御
    // ----------------------------------------------------
    void Init();
    void Update();
    void Delete();

    // ----------------------------------------------------
    // 3. ゲッター / セッター
    // ----------------------------------------------------
    const std::vector<std::shared_ptr<Enemy>>& GetEnemies() const { return m_enemies; }
    BATTLE_PHASE GetCurrentPhase() const { return m_currentPhase; }
    void SetPhase(BATTLE_PHASE phase) { m_currentPhase = phase; }
    bool IsBattleOver() const { return m_currentPhase == BATTLE_PHASE::WIN || m_currentPhase == BATTLE_PHASE::LOSE; }

    // CBattle側でスキル説明等を描画するために必要なゲッター
    bool IsSkillDescriptionRequired() const { return player_action == PLAYER_ACTION::ACTHON; }

private:
    // ----------------------------------------------------
    // 4. バトル進行・ターン制御ロジック
    // ----------------------------------------------------
    void ProcessTurn();
    void CheckAliveStatus();
    void ActionChoice(std::shared_ptr<Character> arg_character);
    void TargetListCreate(std::shared_ptr<Character> arg_character);
    void TargetChoice(std::shared_ptr<Character> arg_character);

    void EnemyActionInit(std::shared_ptr<Character> arg_character);
    void EnemyAction(std::shared_ptr<Character> arg_character);

    void EndCharacterTurn(std::shared_ptr<Character> arg_character); // PEndとEEndを統合
    void TurnEnd();
    bool CheckBattleOverCondition(); // 旧BattleOver()の名称変更（重複回避）
    void BattleEnd();

    // ----------------------------------------------------
    // 5. ヘルパーメソッド
    // ----------------------------------------------------
    void PlayEffectByType(std::shared_ptr<Character> actor, const std::vector<std::shared_ptr<Character>>& targets);
    void TypeConversion();
};
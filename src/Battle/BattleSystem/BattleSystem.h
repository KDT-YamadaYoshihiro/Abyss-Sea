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
class Calculation;

enum class BATTLE_PHASE {
    INIT,           // 初期化、開始演出
    TURN_START,     // ターン開始
    ACTION,         // プレイヤーとエネミーの行動選択と実行
    TURN_END,       // ターン終了
    WIN,            // 勝利
    LOSE            // 敗北
};

enum class TargetInput { P_NONE, ACTIONCHOICE, LISTCREATE, TARGETCHOICE, END };
enum class ActionMode { E_NONE, TARGETRANDOM, END };
struct Position {
    int x;
    int y;
};

class BattleSystem
{
private:
    // 内部ステータス・制御変数
    BATTLE_PHASE m_currentPhase;
    int m_stage = -1;
    int m_enemyId = 0;
    int m_turn;

    TargetInput m_targetInput = TargetInput::P_NONE;
    ActionMode m_actionMode = ActionMode::E_NONE;
    PLAYER_ACTION m_playerAction = NONE_ACTION;

    // 2. キャラクター・システムインスタンス
    std::vector<std::shared_ptr<Enemy>> m_enemies;
    std::vector<std::shared_ptr<Character>> m_turnOrder; // 行動順リスト
    size_t m_currentTurnIndex = 0;

    std::shared_ptr<UI> m_ui;
    std::shared_ptr<BattleSP> m_sp;
    std::shared_ptr<Sound> m_bgm;
    std::shared_ptr<Sound> m_se;
    std::shared_ptr<Calculation> m_cal;

    // 3. 座標・演出用変数
    int m_atButtonX;
    int m_skButtonX;
    int m_buttonY;
    int m_buttonSizeX;
    int m_buttonSizeY;
    int m_nameMax;
    int m_blendNum;
    int m_blendSpeed;
    int m_endFrame;
    int m_boxX;
    int m_boxY;
    int m_speed;
    bool m_skDescDraw;

    std::vector<int> m_playerPosX;
    std::vector<int> m_playerPosY;
    int m_enemyPosX;
    int m_enemyPosY;

    std::vector<std::shared_ptr<Character>> m_targetList;
    std::vector<std::shared_ptr<Character>> m_targets;
    std::shared_ptr<Character> m_selectTarget;

public:
    BattleSystem();
    ~BattleSystem() = default;

    void Init();
    void Update();
    void Delete();

    // 
    const std::vector<std::shared_ptr<Enemy>>& GetEnemies() const { return m_enemies; }
    const std::vector<std::shared_ptr<Character>>& GetTurnOrder() const { return m_turnOrder; }
    const std::vector<std::shared_ptr<Character>>& GetTargetList() const { return m_targetList; }
    std::shared_ptr<Character> GetCurrentTurnCharacter() const { return (m_currentTurnIndex < m_turnOrder.size()) ? m_turnOrder[m_currentTurnIndex] : nullptr; }
    std::shared_ptr<Character> GetSelectTarget() const { return m_selectTarget; }
    std::shared_ptr<BattleSP> GetSP() const { return m_sp; }

    BATTLE_PHASE GetCurrentPhase() const { return m_currentPhase; }
    TargetInput GetTargetInput() const { return m_targetInput; }
    PLAYER_ACTION GetPlayerAction() const { return m_playerAction; }

    int GetTurn() const { return m_turn; }
    int GetBlendNum() const { return m_blendNum; }
    int GetEnemyPosX() const { return m_enemyPosX; }
    int GetEnemyPosY() const { return m_enemyPosY; }
    const std::vector<int>& GetPlayerPosX() const { return m_playerPosX; }
    const std::vector<int>& GetPlayerPosY() const { return m_playerPosY; }
    int GetAtButtonX() const { return m_atButtonX; }
    int GetSkButtonX() const { return m_skButtonX; }
    int GetButtonY() const { return m_buttonY; }
    int GetButtonSizeX() const { return m_buttonSizeX; }
    int GetButtonSizeY() const { return m_buttonSizeY; }
    int GetBoxX() const { return m_boxX; }
    int GetBoxY() const { return m_boxY; }
    int GetNameMax() const { return m_nameMax; }
    bool GetSkDescDraw() const { return m_skDescDraw; }

    bool IsBattleOver();
    Position GetCharacterCenter(std::shared_ptr<Character> arg_chara);

private:
    void CheckAliveStatus();
    void ProcessTurn();
    void ActionChoice(std::shared_ptr<Character> arg_character);
    void TargetListCreate(std::shared_ptr<Character> arg_character);
    void TargetChoice(std::shared_ptr<Character> arg_character);
    void EnemyActionInit(std::shared_ptr<Character> arg_character);
    void EnemyAction(std::shared_ptr<Character> arg_character);
    void PEnd(std::shared_ptr<Character> arg_character);
    void EEnd(std::shared_ptr<Character> arg_character);
    void TurnEnd();
    void TurnOrder();
    void PlayEffectByType(std::shared_ptr<Character> actor, const std::vector<std::shared_ptr<Character>>& targets);
    void TypeConversion();
    void ListClear();
    void UpdataSP(std::shared_ptr<Character> arg_character);
    void PlayerMove();
    void EnemyMove();

    // 外部からの関数と仮定（DxLib等）
    bool CheckBoxClick(int x, int y, int w, int h);
    bool CheckCircleClick(int x, int y, float r);
};
#include "BattleSystem.h"
#include "Scene/Manager/Manager.h"
#include "Character/Factory/CharacterFactory.h"
#include "Character/Enemy/Enemy.h"

BattleSystem::BattleSystem()
	: m_currentPhase(BATTLE_PHASE::INIT),
	// ボタン座標
	at_button_x(450),
	button_y(WINDOW_H - 60),
	sk_button_x(at_button_x + 210),
	button_size_x(200),
	button_size_y(50),
	name_max(6),
	blend_num(50),
	blend_speed(1)
{
	// 座標変数
	for (int i = 0; i < PLAYER_MAX; i++) { p_pos_x[i] = 300 + (i % 2) * 50; p_pos_y[i] = WINDOW_H / 2 + 60; }
	for (int i = 0; i < ENEMY_MAX; i++) { e_pos_x = WINDOW_W / 2 + 300; e_pos_y = WINDOW_H / 2 + 150; }
}

void BattleSystem::Init()
{
	// UIクラス生成
	m_ui = std::make_shared<UI>();
	// sp
	m_sp = std::make_shared<BattleSP>();

	// ステージ
	m_stage = ScreenManager::Instance().getStageScreen();

	// ID
	m_enemyId = ScreenManager::Instance().getEnemyID(m_stage);

	// エネミー生成
	auto enemy = CharacterFactory::Instance().CreateCharacter<Enemy>(m_enemyId);
	m_enemies.push_back(enemy);

	// キャラクターリセット
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

	// キャラクターをインデックス。
	current_turn_index = 0;

	// ターン
	m_turn = 1;

	// SPの初期化
	m_sp->ResetSP();

	// 勝敗判定のリセット
	ScreenManager::Instance().setBattleResult(BATTLE_RESULT::BATTLE);

	// bgmの再生
	m_bgm->PlayBgm(CLoad::Instance().getBgmHandle(1 + m_stage));

	end_frame = 120;


	// ソートを行う。
	TurnOrder();

	m_currentPhase = BATTLE_PHASE::TURN_START;
}

void BattleSystem::Update()
{
    // 勝敗・生存ステータスのチェック
    CheckAliveStatus();

    // バトルが既に終了している場合はフェーズ移行を行わない
    if (IsBattleOver()) {
        return;
    }

    // 各バトル進行フェーズのルール処理
    switch (m_currentPhase)
    {
    case BATTLE_PHASE::TURN_START:
        // ターン開始時のバフ減少処理やターン順の構築ロジックをここに記述

		// インデックスが0の時、ソートを行う。
		if (m_turnOrder.empty())
		{
			TurnOrder();
		}

		// プレイヤーの行動モードが何も設定されていない場合、行動選択モードへ
		if (targetInput == TargetInput::P_NONE)
		{
			targetInput = TargetInput::ACTIONCHOICE;
		}


        m_currentPhase = BATTLE_PHASE::ACTION;
        break;

    case BATTLE_PHASE::ACTION:

		// 行動選択と実行
		if (current_turn_index < m_turnOrder.size()) {

			auto character = m_turnOrder[current_turn_index];

			// 移動する
			playerMove();
			EnemyMove();

			if (character->getAlive() && !BattleOver()) { // 行動中に戦闘が終了していないかチェック

				// プレイヤーとエネミーの型判定を安全にしたいためdynamic
				if (std::dynamic_pointer_cast<Player>(character)) {	// プレイヤー					

					// 入力待ち状態
					if (targetInput == TargetInput::ACTIONCHOICE) {
						// 実行前にリストを空にする。
						ListClea();
						ActionChoice(character);
					}
					// ターゲットリスト制作
					if (targetInput == TargetInput::LISTCREATE)
					{
						TargetListCreate(character);
					}
					//　ターゲット選択
					if (targetInput == TargetInput::TARGETCHOICE)
					{
						TargetChoice(character);
					}
					// 終了
					for (auto& p : ScreenManager::Instance().getParty()) {
						if (p->getAnimChange() && targetInput == TargetInput::END) {
							PEnd(character);
						}
					}

				}
				else {	// エネミー
					// 敵が行動する場合のターゲットはプレイヤー
					// 自動でターゲットを決める
					if (actionMode == ActionMode::E_NONE) {
						ListClea();
						EnemyActionInit(character);
					}

					// ランダムでターゲットを決定し、アクションを実行
					if (actionMode == ActionMode::TARGETRANDOM)
					{
						EnemyAction(character);
					}
					if (actionMode == ActionMode::END)
					{
						// エネミーのターン終了
						EEnd(character);
					}

				}
			}
			else {
				// 死亡していたら即ターン終了
				PEnd(character);
			}

		}
		else if (m_turnOrder.empty() || current_turn_index >= m_turnOrder.size()) {

			m_currentPhase = BATTLE_PHASE::TURN_END;
			return;
		}

		break;

    case BATTLE_PHASE::TURN_END:
		// ターン終了
		TurnEnd(); 
		// バトルの終了をチェック。
        ProcessTurn();
        break;

    default:
        break;
    }
}

void BattleSystem::Render()
{
	// 指定したIDのエネミーを描画
	for (auto& e : m_enemies) {
		// クリック時に使用する変数セット
		e->setPosX(e_pos_x);
		e->setPosY(e_pos_y);
		//	本体
		int id = e->getId() - 100;
		e->Render(e_pos_x, e_pos_y, CLoad::Instance().getEnemyGrh(id));
		e->powerRender();

		if (!e->getAlive()) { e->DeadAnimDraw(); }

	}

	// プレイヤーの描画
	for (size_t i = 0; i < ScreenManager::Instance().getParty().size(); i++) {
		auto& players = ScreenManager::Instance().getParty();
		// クリック時に使用する変数セット
		int x = p_pos_x[i] + (i % 2) * 50;
		int y = p_pos_y[i] + i * 50;

		players[i]->setPosX(x);
		players[i]->setPosY(y);
		//	本体
		players[i]->Render(x, y, CLoad::Instance().getPlayerGrh(players[i]->getId()));
	}

	// エフェクトの再生
	for (size_t p = 0; p < ScreenManager::Instance().getParty().size(); p++) {

		auto& players = ScreenManager::Instance().getParty();

		// 複数再生
		if (m_turnOrder[current_turn_index]->getTargetType() == SKILL_TARGET_TYPE::ALL_ALLY ||
			m_turnOrder[current_turn_index]->getTargetType() == SKILL_TARGET_TYPE::ALL_ENEMY) {

			for (size_t i = 0; i < m_targetList.size(); i++) {
				players[p]->EffectDraw(m_targetList[i]->getPosX(), m_targetList[i]->getPosY());
			}
		}
		else { // 単体時&&selectTargetがnullptrでないとき
			if (m_selectTarget != nullptr) {
				players[p]->EffectDraw(m_selectTarget->getPosX(), m_selectTarget->getPosY());
			}
		}
	}

	// プレイヤーのエフェクトアニメーション
	for (auto& p : ScreenManager::Instance().getParty()) {
		p->powerRender();
	}

}


void BattleSystem::CheckAliveStatus()
{
    // プレイヤー側の生存チェック
    bool playerAlive = false;
    for (auto& p : ScreenManager::Instance().getParty()) {
        if (p->getAlive()) {
            playerAlive = true;
            break;
        }
    }

    // エネミー側の生存チェック
    bool enemyAlive = false;
    for (auto& e : m_enemies) {
        if (e->getAlive()) {
            enemyAlive = true;
            break;
        }
    }

    // 勝敗フェーズへの遷移判定
    if (!playerAlive) {
        m_currentPhase = BATTLE_PHASE::LOSE;
    }
    else if (!enemyAlive) {
        m_currentPhase = BATTLE_PHASE::WIN;
    }
}

void BattleSystem::ProcessTurn()
{
    // ターン終了時のカウント更新やフェーズの巻き戻し処理

    m_currentPhase = BATTLE_PHASE::TURN_START;
}


// プレイヤー
//　攻撃かスキルか選択(入力)
void ActionChoice(std::shared_ptr<Character> arg_character) {

	//　通常攻撃orスキル選択（クリック）
	if (CheckBoxClick(at_button_x, button_y, button_size_x, button_size_y)) {
		se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
		arg_character->setActionChoice(ATTACK);
		targetInput = TargetInput::LISTCREATE;
	}
	else if (CheckBoxClick(sk_button_x, button_y, button_size_x, button_size_y)) {

		// spが0より大きいなら（1以上なら）
		if (sp->getSP() > 0) {
			// クリック音の再生
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			// 
			arg_character->setActionChoice(SKILL);
			// スキル説明表示
			sk_desc_draw = true;
			// ターゲットリスト作成へ
			targetInput = TargetInput::LISTCREATE;
		}
		else {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
		}
	}


}

//　ターゲットリスト作成
void TargetListCreate(std::shared_ptr<Character> arg_character) {

	target_list.clear();

	// 攻撃 or スキルで敵/味方をリストアップ
	if (arg_character->getActionChoice() == ATTACK ||
		arg_character->getTargetType() == SKILL_TARGET_TYPE::SINGLE_ENEMY ||
		arg_character->getTargetType() == SKILL_TARGET_TYPE::ALL_ENEMY) {

		for (auto& e : enemies) {
			if (e->getAlive())
			{
				target_list.push_back(e);
			}
		}
	}
	else if (arg_character->getTargetType() == SKILL_TARGET_TYPE::SINGLE_ALLY ||
		arg_character->getTargetType() == SKILL_TARGET_TYPE::ALL_ALLY) {

		for (auto& p : ScreenManager::Instance().getParty()) {
			if (p->getAlive())
			{
				// 
				target_list.push_back(p);
			}
		}

	}
	else if (arg_character->getTargetType() == SKILL_TARGET_TYPE::SELF) {
		// 自分のみ
		target_list.push_back(arg_character);
	}

	// ターゲット選択モードへ
	targetInput = TargetInput::TARGETCHOICE;
}

// ターゲットの選択
void TargetChoice(std::shared_ptr<Character> arg_character) {

	// 行動キャンセル処理
	if (CheckBoxClick(box_x, box_y, BUTTAN_WIDTH, BUTTAN_HEIGHT)) {
		// キャンセル音
		se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
		// ターゲットリストを空に
		targetInput = TargetInput::ACTIONCHOICE;
		// 
		sk_desc_draw = false;
		return;
	}

	// 各ターゲットに対してクリック判定
	for (auto& tar : target_list) {
		Position pos = GetCharacterCenter(tar);

		// 実際の対象リスト
		std::vector<std::shared_ptr<Character>> actTargets;

		if (player_action == NONE_ACTION && CheckCircleClick(pos.x, pos.y, 40.0f)) {

			player_action = CHOICE;
			// 決定音
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

			// 対象者決定
			select_target = tar;
			TypeConversion();

			// SP確認（スキル時のみ）
			if (arg_character->getActionChoice() == SKILL && !sp->comfirmSP()) return;

		}

		switch (player_action)
		{
		case NONE_ACTION:

			break;
		case CHOICE:
			// 次に以降
			if (arg_character->getActionChoice() == SKILL)
			{
				// スキルはカットインあり
				arg_character->StartCutin();
				player_action = CUTIN;

			}
			else {

				// 通常攻撃はカットインなし
				player_action = ACTHON;
			}

			break;
		case CUTIN:

			if (arg_character->getCutinEnd()) {
				player_action = ACTHON;
			}

			break;

		case SKILL:

			if (arg_character->getTargetType() == SKILL_TARGET_TYPE::ALL_ENEMY ||
				arg_character->getTargetType() == SKILL_TARGET_TYPE::ALL_ALLY) {
				actTargets = target_list;
			}
			else {
				actTargets.push_back(select_target);
			}

			// アクション実行
			arg_character->TakeAction(actTargets);

			// エフェクト再生
			PlayEffectByType(arg_character, actTargets);

			// ターゲット選択モードをオフ
			targetInput = TargetInput::END;

			player_action = NONE_ACTION;

			break;
		default:
			break;
		}


	}
}

// 再生するエフェクト
void PlayEffectByType(std::shared_ptr<Character> actor,
	const std::vector<std::shared_ptr<Character>>& targets) {

	EFFECT_TYPE efType = actor->getEfType();

	if (actor->getActionChoice() == SKILL) {
		switch (efType) {

			// 単体・全体スキル1/2
		case EFFECT_TYPE::EF_SKILL1:
			for (auto& t : targets)
				actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 300, 250);
			se->PlaySe(CLoad::Instance().getSeHandle(SE_SKILL1));
			break;

		case EFFECT_TYPE::EF_SKILL2:
			for (auto& t : targets)
				actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 350, 250);
			se->PlaySe(CLoad::Instance().getSeHandle(SE_SKILL2));
			break;

			// 全体スキル
		case EFFECT_TYPE::EF_ALL_SKILL1:
			for (auto& t : targets)
				actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 100, 100);
			se->PlaySe(CLoad::Instance().getSeHandle(SE_SKILL1));
			break;

		case EFFECT_TYPE::EF_ALL_SKILL2:
			for (auto& t : targets)
				actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 250, 250);
			se->PlaySe(CLoad::Instance().getSeHandle(SE_SKILL2));
			break;

			// 回復系
		case EFFECT_TYPE::EF_HEAL:
			for (auto& t : targets)
				actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 30, 40);
			se->PlaySe(CLoad::Instance().getSeHandle(SE_HEAL));
			break;

			// バフ・デバフ系
		case EFFECT_TYPE::EF_BUFF:
		case EFFECT_TYPE::EF_DEBUFF:
			for (auto& t : targets)
				actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 50, 40);
			se->PlaySe(CLoad::Instance().getSeHandle(SE_BUFF));
			break;

		default:
			break;
		}
	}
	else {
		// 通常攻撃
		for (auto& t : targets)
			actor->PlayAttackEffect(t->getPosX(), t->getPosY(), 50, 40);
		se->PlaySe(CLoad::Instance().getSeHandle(SE_ATTACK));

	}
}


// エネミーカットイン
void EnemyActionInit(std::shared_ptr<Character> arg_character) {
	arg_character->StartCutin();

	actionMode = ActionMode::TARGETRANDOM;
}

// エネミー攻撃
void EnemyAction(std::shared_ptr<Character> arg_character) {

	if (arg_character->getCutinEnd()) {

		for (auto& p : ScreenManager::Instance().getParty()) {
			if (p->getAlive()) target_list.push_back(p);
		}

		int index = rand() % target_list.size();
		select_target = target_list[index];
		TypeConversion();
		//　アクションに移行
		if (arg_character->getCutinEnd()) {
			arg_character->TakeAction(targets);
		}
		// プレイヤー攻撃時にアニメーション切り替え
		auto& p = ScreenManager::Instance().getParty();
		for (size_t i = 0; i < ScreenManager::Instance().getParty().size(); i++) {
			if (target_list[index]->getId() == p[i]->getId())
				p[i]->setAnimType(DAMAGE);
		}
		// se再生
		se->PlaySe(CLoad::Instance().getSeHandle(SE_ATTACK));

		actionMode = ActionMode::END;
	}
}

// キャラクターのターン終了
void PEnd(std::shared_ptr<Character> arg_character) {

	for (auto& p : ScreenManager::Instance().getParty()) {
		// アニメーションをWAITにする
		p->setAnimType(WAIT);
		// チェンジモードをfalseに
		p->setAnimChange(false);
	}

	// 説明の非表示
	sk_desc_draw = false;

	// ターゲット選択モードをオフ
	targetInput = TargetInput::P_NONE;
	// spの更新
	updataSP(arg_character);
	// インデックスをインクリメントして次のキャラクターの行動へ		
	current_turn_index++;
	if (current_turn_index >= turn_order.size()) {
		TurnEnd();
		return;
	}
}

void EEnd(std::shared_ptr<Character> arg_characte) {
	for (auto& p : ScreenManager::Instance().getParty()) {
		// アニメーションをWAITにする
		p->setAnimType(WAIT);
		// チェンジモードをfalseに
		p->setAnimChange(false);
	}

	actionMode = ActionMode::E_NONE;
	// アクション終了後インデックスをインクリメント
	current_turn_index++;
	if (current_turn_index >= turn_order.size()) {
		TurnEnd();
		return;
	}
}

// ターン終了時関数
void TurnEnd() {

	// バフデバフの更新
	for (auto& p : ScreenManager::Instance().getParty()) {
		p->updateBuff();
	}
	// エネミー
	for (auto& e : enemies) {
		e->updateBuff();
	}

	TurnOrder();
	// インデックスをリセット
	current_turn_index = 0;

	// ターンの増加
	this->turn++;

}

// smartpointerをベクター型に変換
void TypeConversion() {
	targets = { select_target };
}

// プレイヤーの画像中央座標を取得する関数
// ターゲット候補者の円の描画に使用
Position GetCharacterCenter(std::shared_ptr<Character> arg_chara) {
	// 元座標を取得
	int x = arg_chara->getPosX();
	int y = arg_chara->getPosY();

	if (std::dynamic_pointer_cast<Player>(arg_chara)) {
		// プレイヤーの場合：左上基準 → 中心に補正
		x += SIZE_W / 2;
		y += SIZE_H / 2;
	}
	// エネミーは中心基準なので補正不要
	return { x, y };
}

// 移動用関数
void playerMove() {
	// プレイヤー、エネミーの座標移動
	for (size_t i = 0; i < ScreenManager::Instance().getParty().size(); i++) {

		auto& p = ScreenManager::Instance().getParty();

		if (turn_order[current_turn_index]->getId() == p[i]->getId()) {
			p_pos_x[i] += 10;
			if (p_pos_x[i] >= WINDOW_W / 2 - 150) {
				p_pos_x[i] = WINDOW_W / 2 - 150;
			}
		}
		else {
			p_pos_x[i] -= 10;
			if (p_pos_x[i] <= 300 + (i % 2) * 50) {
				p_pos_x[i] = 300 + (i % 2) * 50;
			}
		}
	}

}
// 移動用関数
void EnemyMove() {
	for (auto& enemy : enemies) {
		// 移動
		if (enemy->getMoveCheck()) {
			e_pos_x -= speed;
		}
		// 移動上限
		if (speed >= 0 && e_pos_x <= WINDOW_W / 2 + 280) {
			speed *= -1;
		}
		if (speed <= 0 && e_pos_x >= WINDOW_W / 2 + 300) {
			speed *= -1;
			enemy->setMoveCheck(false);
		}
	}

}

// 毎ターン行うソート
void TurnOrder() {

	// 過去のソート情報を削除
	turn_order.clear();
	// 生きているキャラをソート対象に
	for (const auto& p : ScreenManager::Instance().getParty()) {
		if (p->getAlive() == true) turn_order.push_back(p);
	}
	for (const auto& e : enemies) {
		if (e->getAlive() == true) turn_order.push_back(e);
	}

	// 素早さでソート（降順）
	if (!turn_order.empty()) {
		// std::sort を使ってソート
		std::sort(turn_order.begin(), turn_order.end(), [this](const std::shared_ptr<Character>& a, const std::shared_ptr<Character>& b) {
			int a_speed = cal->BuffCal(a->getSpeed(), a->getAgrbuff());
			int b_speed = cal->BuffCal(b->getSpeed(), b->getAgrbuff());
			return a_speed > b_speed; // 降順にソート
			});
	}
}

// ソートした物をアイコン上に
void DrawTurnOrderIcons(int x, int y) const
{
	const int ICON_SIZE = 60;	// アイコンの表示サイズ
	const int PADDING = 10;		// アイコンの間隔
	int drawX = x;				//　スタート地点

	for (size_t i = 0; i < turn_order.size(); ++i) {
		const auto& ch = turn_order[i];
		int handle = ch->getFaceHandle();

		// アイコンを描画
		DrawExtendGraph(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, handle, TRUE);

		// 現在のターンキャラに枠を表示
		if (i == static_cast<size_t>(current_turn_index)) {
			DrawBox(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, GetColor(255, 255, 0), FALSE);
		}
		else {
			// 非行動者
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			DrawBox(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, GetColor(0, 0, 0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// 名前または略称を下に表示
		SetFontSize(FONT_MINSIZE);
		std::string name = ch->getName();
		if (name.size() > name_max) {
			name = name.substr(0, name_max);
		}
		DrawFormatString(drawX, y + ICON_SIZE + 4, GetColor(255, 255, 255), "%s", name.c_str());
		// 次のキャラの描画位置を横にずらす
		drawX += ICON_SIZE + PADDING;
	}
}

// リストを空にする
void ListClea() {
	target_list.clear();
	targets.clear();
}

// spの増減
void updataSP(std::shared_ptr<Character> arg_character) {
	// spの増減
	if (arg_character->getActionChoice() == ATTACK) {
		// sp増加
		sp->recoverSP();
	}
	else {
		// SP消費
		sp->consumeSP();
	}
}
// プレイヤーのバトル終了時エフェクト
void PlayEndEffect() {

	end_frame--;

	for (auto& e : enemies) {

		if (!e->getAlive()) {

			for (auto& p : ScreenManager::Instance().getParty()) {
				p->setAnimType(GRTYPE::WIN);
			}
		}
		else {

			for (auto& p : ScreenManager::Instance().getParty()) {
				p->setAnimType(GRTYPE::LOSE);
			}
		}
	}

	if (end_frame <= 0) {
		state = State::END;
	}
}

// 終了
void BattleEnd() {

	// falseならfadeを軌道。
	fade->fadeStart(fade->FADE_CLAUSE);
	// 音源の停止
	bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + stage));
	for (int i = 0; i < SE_MAX; i++) {
		se->stopSe(CLoad::Instance().getSeHandle(i));
	}

}

void Delete() {

	m_enemies.clear();
}

// バトル終了確認関数
bool BattleOver() {
	// プレイヤーが生きているか
	bool allPlayersDefeated = true;
	for (auto& p : ScreenManager::Instance().getParty()) {
		if (p->getAlive()) {
			allPlayersDefeated = false;
			break;
		}
		else {
			// 敗北
			ScreenManager::Instance().setBattleResult(BATTLE_RESULT::DEFEAT);
			// ミッションリセット
			MissionManager::Instance().ResetMissions(m_stage);
		}
	}

	// エネミーが生きているか
	bool EnemiesDefeated = true;
	for (const auto& e : m_enemies) {
		if (e->getAlive()) {
			EnemiesDefeated = false;
			break;
		}
		else {
			// 勝利
			ScreenManager::Instance().setBattleResult(BATTLE_RESULT::VICTORY);
			// 経験値セット
			ScreenManager::Instance().setExp(e->getExp());
			// ミッション確認
			MissionManager::Instance().ClearQuest(m_stage);

		}
	}

	// 双方falseならバトルは続行
	// どちらかがtrueならバトル終了
	return allPlayersDefeated || EnemiesDefeated;
}

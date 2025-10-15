#pragma once
#include "UI.h"
#include "CStage.h"
#include "CharacterFactory.h"
#include "Character.h"
#include "Calculation.h"

// バトルスクリーンクラス
class CBattle : public ScreenBase {

#define FONT_BIGSIZE	 50
#define FONT_SIZE		 30
#define FONT_MINSIZE	 15

#define MAX_VOL			255

	// キャラクターセット、ゲット関数用
	struct Position {int x; int y;};

	// プレイヤーの行動モード(何もしない,行動選択, ターゲットリスト作成,　ターゲット選択) 
	enum class TargetInput{P_NONE, ACTIONCHOICE, LISTCREATE,TARGETCHOICE,END};
	// プレイヤーの行動モード変数
	TargetInput targetInput = TargetInput::P_NONE;

	// エネミーの行動モード
	enum class ActionMode{E_NONE,TARGETRANDOM,END};
	ActionMode actionMode = ActionMode::E_NONE;

	// バトルモード	 バトル、MENU、終了
	enum class State{BATTLE,MENU,DEAD_EFFECT,END};
	State state = State::BATTLE;

	// MENUモード		デフォ、再挑戦、リタイア、設定
	enum class MenuState{NONE,RETRY,RETIRE,SETTING};
	MenuState menuState = MenuState::NONE;

	// UIクラスの生成
	std::shared_ptr<UI> ui;

	// sp
	std::shared_ptr<BattleSP> sp;
	
	// エネミー
	std::vector<std::shared_ptr<Character>> enemies;
	// 行動順リスト
	std::vector<std::shared_ptr<Character>> turn_order;

	// ターゲットリスト
	std::vector<std::shared_ptr<Character>> target_list;
	// 対象者
	std::shared_ptr<Character> select_target;
	// 決定したターゲットリスト
	std::vector<std::shared_ptr<Character>> targets;

	// 音
	std::shared_ptr<Sound> bgm;
	std::shared_ptr<Sound> se;

	// フェード
	std::shared_ptr<Fade> fade;
	// 再挑戦用のフェード
	//std::shared_ptr<Fade> retryFade;

	std::shared_ptr<Effect> effect;

	std::shared_ptr<Calculation> cal;

	// 現在ターンのインデックス
	int current_turn_index = 0;
	// ターン数
	int turn = -1;
	// 生成するステージ
	int stage = -1;
	// 生成するエネミーのID
	int e_id;

	// プレイヤーGRH
	int player_grh = - 1;
	// エネミーGRH
	int enemy_grh = -1;

	// アクションスイッチ
	PLAYER_ACTION player_action = NONE_ACTION;

	// 座標変数
	int p_pos_x[PLAYER_MAX];
	int p_pos_y[PLAYER_MAX];

	int e_pos_x = -1;
	int e_pos_y = -1;

	// ボタン座標
	int at_button_x = -1;
	int button_y = -1;
	int sk_button_x = -1;
	int button_size_x = -1;
	int button_size_y = -1;

	int speed = -1;


	// 三角形の座標
	int tr_pos_x[BUTTAN_MAX];
	int tr_pos_y[BUTTAN_MAX];

	// 音量設定時に使用　クリックフレーム
	int click_frame = -1;

	// アイコン下名前表示文字数制限
	int name_max = -1;

	// ブレンドモードの変数
	int blend_num = -1;
	int blend_speed = -1;

	// 終了フレームカウント
	int end_frame;

	// 戻るボタンの座標、サイズ変数
	int box_x = -1;
	int box_y = -1;
	int size_w = -1;
	int size_h = -1;

	// 他ボタンの座標
	int ui_buttonX = -1;
	int ui_buttonY = -1;

	// スキル説明の表示有無
	bool sk_desc_draw = false;

public:

	// 初期化
	CBattle() :
		// ボタン座標
		at_button_x(450),
		button_y(WINDOW_H - 60),
		sk_button_x(at_button_x + 210),
		button_size_x(200),
		button_size_y(50),
		click_frame(0),
		name_max(6),
		blend_num(50),
		blend_speed(1),
		// UIボタン座標
		box_x(WINDOW_W / 2 + 220),
		box_y(WINDOW_H - 100),
		size_w(200),
		size_h(80),
		ui_buttonX(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5)),
		ui_buttonY(250),
		sk_desc_draw(false)
	{
		
		// 座標変数
		for (int i = 0; i < PLAYER_MAX; i++) { p_pos_x[i] = 300 + (i % 2) * 50; p_pos_y[i] = WINDOW_H / 2 + 60; }
		for (int i = 0; i < ENEMY_MAX; i++) { e_pos_x = WINDOW_W / 2 + 300; e_pos_y = WINDOW_H / 2 + 150; }
		

		// 初期化関数
		BattleInit();

		tr_pos_x[BGM_UP] = WINDOW_W / 2 - 280;
		tr_pos_y[BGM_UP] = WINDOW_H / 2 - 120;
		tr_pos_x[BGM_DOWN] = WINDOW_W / 2 - 280;
		tr_pos_y[BGM_DOWN] = WINDOW_H / 2 + 70;
		tr_pos_x[SE_UP] = WINDOW_W / 2 + 260;
		tr_pos_y[SE_UP] = WINDOW_H / 2 - 120;
		tr_pos_x[SE_DOWN] = WINDOW_W / 2 + 260;
		tr_pos_y[SE_DOWN] = WINDOW_H / 2 + 70;
	};

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:


	// バトル初期化（再挑戦時使用)
	void BattleInit() {

		// UIクラス生成
		ui = std::make_shared<UI>();
		// sp
		sp = std::make_shared<BattleSP>();

		// ステージ
		stage = ScreenManager::Instance().getStageScreen();

		// 音源
		bgm = ScreenManager::Instance().getbgm();
		se = ScreenManager::Instance().getSe();

		// フェード
		fade = ScreenManager::Instance().getFade();

		// ID
		e_id = ScreenManager::Instance().getEnemyID(stage);

		// エネミー生成
		auto enemy = CharacterFactory::Instance().CreateCharacter(e_id);
		enemies.push_back(enemy);

		cal = std::make_shared<Calculation>();

		// キャラクターリセット
		for (auto& p : ScreenManager::Instance().getParty()) {
			p->hpReset();
			p->BuffReset();
			p->setAnimType(WAIT);
			p->effectrReset();
		}
		for (auto& e : enemies) {
			e->hpReset();
			e->BuffReset();
			e->effectrReset();
		}

		// キャラクターをインデックス。
		current_turn_index = 0;

		// ソートを行う。
		TurnOrder();

		// ターン
		turn = 1;

		// SPの初期化
		sp->ResetSP();

		// 勝敗判定のリセット
		ScreenManager::Instance().setBattleResult(BATTLE_RESULT::BATTLE);

		// bgmの再生
		bgm->PlayBgm(CLoad::Instance().getBgmHandle(1 + stage));

		end_frame = 120;
	}
	
	void Delete() {

		enemies.clear();
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
				MissionManager::Instance().ResetMissions(stage);
			}
		}

		// エネミーが生きているか
		bool EnemiesDefeated = true;
		for (const auto& e : enemies) {
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
				MissionManager::Instance().ClearQuest(stage);

			}
		}

		// 双方falseならバトルは続行
		// どちらかがtrueならバトル終了
		return allPlayersDefeated || EnemiesDefeated;
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
			DrawFormatString(drawX, y + ICON_SIZE + 4, GetColor(255, 255, 255), "%s",name.c_str()); 
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

	// プレイヤー
	//　攻撃かスキルか選択(入力)
	void ActionChoice(std::shared_ptr<Character> arg_character) {

		//　通常攻撃orスキル選択（クリック）
		if (CheckBoxClick(at_button_x, button_y, button_size_x,button_size_y)) {
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
		if (CheckBoxClick(box_x, box_y, size_w, size_h)) {
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
				arg_character->takeAction(actTargets);

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
				arg_character->takeAction(targets);
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
	Position GetCharacterCenter(std::shared_ptr<Character> c) {
		// 元座標を取得
		int x = c->getPosX();
		int y = c->getPosY();

		if (std::dynamic_pointer_cast<Player>(c)) {
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
	// メインループ関数
	// バトル
	void BattleMain() {

		// インデックスが0の時、ソートを行う。
		if (turn_order.empty())
		{
			TurnOrder();
		}

		// プレイヤーの行動モードが何も設定されていない場合、行動選択モードへ
		if (targetInput == TargetInput::P_NONE)
		{
			targetInput = TargetInput::ACTIONCHOICE;
		}

		// 行動選択と実行
		if (current_turn_index < turn_order.size()) {

			auto character = turn_order[current_turn_index];

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

		}else if (turn_order.empty() || current_turn_index >= turn_order.size()) {

			TurnEnd(); // ターン終了へ

			return;
		}

		// バトル進行関数
		// バトルの終了をチェック。
		if (BattleOver()) {
			// 勝敗結果に応じてモードを変更
			state = State::DEAD_EFFECT;
		}

		// メニューバーが押されたとき、モードの切り替え
		if (CheckBoxClick(WINDOW_W - 250, 10, 250, 150)) {
			state = State::MENU;
		}

	}

	// メニュー
	void BattleMenu() {

		switch (menuState)
		{
		case CBattle::MenuState::NONE:

			// 選択画面の表示
			// バトル再開
			if (CheckBoxClick(ui_buttonX, ui_buttonY, size_w, size_h)) {
				state = State::BATTLE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// 再挑戦
			if (CheckBoxClick(ui_buttonX, ui_buttonY + 100, size_w, size_h)) {
				// モード変更
				menuState = MenuState::RETRY;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// リタイア
			if (CheckBoxClick(ui_buttonX, ui_buttonY + 200, size_w, size_h)) {
				// モード変更
				menuState = MenuState::RETIRE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// （設定）
			if (CheckBoxClick(ui_buttonX, ui_buttonY + 300, size_w, size_h)) {
				menuState = MenuState::SETTING;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

			}


			break;
		case CBattle::MenuState::RETRY:

			// 再確認
			// はい
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 6), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), size_w, size_h)) {
				// fadeを軌道
				fade->fadeStart(fade->FADE_CLAUSE);

				// 再挑戦
				// 敵を削除
				Delete();
				// 再初期化
				BattleInit();
				// MENUの選択画面に戻る
				menuState = MenuState::NONE;
				// バトル画面に切り替える
				state = State::BATTLE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));

			}
			// いいえ
			if (CheckBoxClick(WINDOW_W / 2 + (FONT_BIGSIZE * 2.5), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), size_w, size_h)) {
				// MENUの選択画面に戻る
				menuState = MenuState::NONE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			}


			break;
		case CBattle::MenuState::RETIRE:

			// 再確認
			// はい
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 6), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), size_w, size_h)) {

				// Party全員を死亡判定に
				for (auto& p : ScreenManager::Instance().getParty()) {
					p->setAlive(false);
				}
				// fadeを軌道
				fade->fadeStart(fade->FADE_CLAUSE);
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
				// bgmの停止
				bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + ScreenManager::Instance().getStageScreen()));
			}
			// いいえ
			if (CheckBoxClick(WINDOW_W / 2 + (FONT_BIGSIZE * 2.5), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), size_w, size_h)) {
				// MENUの選択画面に戻る
				menuState = MenuState::NONE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));

			}

			break;

		case CBattle::MenuState::SETTING:

			if (click_frame > 0) {
				click_frame--;
			}

			// 音量変更
			for (int i = 0; i < BUTTAN_MAX; i++) {

				int size;
				if (i % 2 == 0) { size = MAX_VOL / 10; }
				else { size = -MAX_VOL / 10; }

				if (click_frame == 0 && CheckCircleClick(tr_pos_x[i], tr_pos_y[i] + size, 50)) {

					int bgmVol = ScreenManager::Instance().getBgmVolume();
					int seVol = ScreenManager::Instance().getSeVolume();

					click_frame = 10;
					se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

					switch (i)
					{
					case BGM_UP:

						ScreenManager::Instance().setBgmVolume(bgmVol + 1);

						break;

					case BGM_DOWN:
						ScreenManager::Instance().setBgmVolume(bgmVol - 1);

						break;

					case SE_UP:
						ScreenManager::Instance().setSeVolume(seVol + 1);

						break;

					case SE_DOWN:
						ScreenManager::Instance().setSeVolume(seVol - 1);

						break;

					default:
						break;
					}

				}

			}

			// 決定ボタンを押した際
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H - 100, size_w, size_h)) {
				// MENUの選択画面に戻る
				menuState = MenuState::NONE;
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}

			break;
		default:
			break;
		}

	}

	// 死亡時エフェクト
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

};
#pragma once

// ŒvZŒnƒNƒ‰ƒX
class Calculation {

public:

	// Šî–{Œ`
	// UŒ‚—Í‚Æ–hŒä—Í‚ÌŒvZ
	int DamageCal(int arg_atk, int arg_def) {
		int damage = arg_atk - arg_def;
		return damage;
	}
	// ƒoƒtŒvZ
	int BuffCal(int arg_status, float arg_buff) {
		int num = arg_status * arg_buff;
		return num;
	}
	// ƒfƒoƒtŒvZ
	int DeBuffCal(int arg_status, float arg_deBuff) {
		int num = arg_status * arg_deBuff;
		return num;
	}

	// UŒ‚Œn
	// UŒ‚—ÍEUŒ‚—ÍƒoƒtE–hŒä—Í‚ÌŒvZ
	int DamageCal(int arg_atk, float arg_atk_buff, int arg_def) {
		int damage = DamageCal(BuffCal(arg_atk , arg_atk_buff),arg_def);
		return damage;
	}
	// UŒ‚—ÍEUŒ‚—ÍƒoƒtE–hŒä—ÍE–hŒä—Íƒoƒt‚ÌŒvZ
	int DamageCal(int arg_atk, float arg_atk_buff, int arg_def, float arg_def_buff) {
		int atk = BuffCal(arg_atk, arg_atk_buff);	// UŒ‚—Í
		int def = BuffCal(arg_def, arg_def_buff);	// –hŒä—Í
		int damage = DamageCal(atk, def);			// Œ‹‰Ê
		return damage;
	}
	// UŒ‚—ÍEUŒ‚—ÍƒoƒtEUŒ‚—ÍƒfƒoƒtE–hŒä—ÍE–hŒä—Íƒoƒt‚ÌŒvZ
	int DamageCal(int arg_atk, float arg_atk_buff, float arg_atk_debuff, int arg_def, float arg_def_buff,float arg_def_debuff) {
		int atk = BuffCal(arg_atk, arg_atk_buff) - DeBuffCal(arg_atk, arg_atk_debuff);		// UŒ‚—Í
		int def = BuffCal(arg_def, arg_def_buff) - DeBuffCal(arg_def, arg_def_debuff);		// –hŒä—Í
		int damage = DamageCal(atk,def);													// Œ‹‰Ê
		return damage;
	}

	// ‰ñ•œŒn
	int Healcal(int arg_status, float arg_power) {
		int heal = arg_status * arg_power;
		return heal;
	}
};
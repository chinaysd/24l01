#include "bsp_key.h"
#include "TimeOut.h"

extern void Port_Key_Init(void);
extern unsigned char Port_Get_KeyIndex(void);

typedef enum
{
	KEY_STATE_IDLE,
	KEY_STATE_JITTER,
	KEY_STATE_PRESS_DOWN,
	KEY_STATE_CP
} key_sta_t;

static key_sta_t key_sta;
static unsigned char last_key_index = MSG_INVALID;
static TIMEOUT_PARA wait_timer, scan_timer;

static unsigned char KeyEvent[][5] =
		{
				{MSG_KEY_NONE, MSG_KEY_NONE, MSG_KEY_NONE, MSG_KEY_NONE, MSG_KEY_NONE},
				{MSG_K1_PRESS, MSG_K1_SHORT_PRESS, MSG_K1_LONGPRESS, MSG_K1_LONGPRESS_HD, MSG_K1_LONGPRESS_RE},
				{MSG_K2_PRESS, MSG_K2_SHORT_PRESS, MSG_K2_LONGPRESS, MSG_K2_LONGPRESS_HD, MSG_K2_LONGPRESS_RE},
};

void Bsp_Key_Init(void)
{
	Port_Key_Init();
	key_sta = KEY_STATE_IDLE;
	// TimeOut_Record(&scan_timer,KEY_SCAN_TIME);
}

key_msg_t Bsp_Key_Scan(void)
{
	unsigned char key_index;

	if (!(TimeOutDet_Check(&scan_timer)))
	{
		return MSG_INVALID;
	}

	TimeOut_Record(&scan_timer, KEY_SCAN_TIME);

	key_index = Port_Get_KeyIndex();

	switch (key_sta)
	{
	case KEY_STATE_IDLE:
	{
		if (!key_index)
		{
			return MSG_KEY_NONE;
		}

		last_key_index = key_index;
		TimeOut_Record(&wait_timer, KEY_JITTER_TIME);
		key_sta = KEY_STATE_JITTER;
		break;
	}

	case KEY_STATE_JITTER:
	{
		if (last_key_index != key_index)
		{
			key_sta = KEY_STATE_IDLE;
		}
		else if (TimeOutDet_Check(&wait_timer))
		{
			TimeOut_Record(&wait_timer, KEY_CP_TIME);
			key_sta = KEY_STATE_PRESS_DOWN;
			return (key_msg_t)KeyEvent[last_key_index][0];
		}
		break;
	}

	case KEY_STATE_PRESS_DOWN:
	{
		if (last_key_index != key_index)
		{
			key_sta = KEY_STATE_IDLE;
			return (key_msg_t)KeyEvent[last_key_index][1];
		}
		else if (TimeOutDet_Check(&wait_timer))
		{
			TimeOut_Record(&wait_timer, KEY_CPH_TIME);
			key_sta = KEY_STATE_CP;
			return (key_msg_t)KeyEvent[last_key_index][2];
		}
		break;
	}

	case KEY_STATE_CP:
	{
		if (last_key_index != key_index)
		{
			key_sta = KEY_STATE_IDLE;
			return (key_msg_t)KeyEvent[last_key_index][4];
		}
		else if (TimeOutDet_Check(&wait_timer))
		{
			TimeOut_Record(&wait_timer, KEY_CPH_TIME);
			return (key_msg_t)KeyEvent[last_key_index][3];
		}
		break;
	}

	default:
	{
		key_sta = KEY_STATE_IDLE;
		break;
	}
	}
	return MSG_INVALID;
}

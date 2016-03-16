#pragma once
#include <windows.h>

#define 	VK_A 	0x41 
#define 	VK_B 	0x42 
#define 	VK_C 	0x43 
#define 	VK_D 	0x44 
#define 	VK_E 	0x45 
#define 	VK_F 	0x46 
#define 	VK_G 	0x47 
#define 	VK_H 	0x48 
#define 	VK_I 	0x49 
#define 	VK_J 	0x4A 
#define 	VK_K 	0x4B 
#define 	VK_L 	0x4C 
#define 	VK_M 	0x4D 
#define 	VK_N 	0x4E 
#define 	VK_O 	0x4F 
#define 	VK_P 	0x50 
#define 	VK_Q 	0x51 
#define 	VK_R 	0x52 
#define 	VK_S 	0x53 
#define 	VK_T 	0x54 
#define 	VK_U 	0x55 
#define 	VK_V 	0x56 
#define 	VK_W 	0x57
#define 	VK_X 	0x58 
#define 	VK_Y 	0x59 
#define 	VK_Z 	0x5A

#define FREEKEY	0x10	// 눌르지 않은 상태.
#define PULLKEY	0x20	// 눌렀다 띄었을때
#define PUSHKEY	0x40	// 누르자 마자
#define HOLDKEY	0x80	// 누르고 있는 중    2진수 10000000


class CMyInput
{
public:
	CMyInput(void) {};
	~CMyInput(void) {};

	void KeyboardProcess()
	{
		static	BYTE	byOldKey[256] = { 0 };

		if(GetKeyboardState(byKey))	
		{
			// DOWN 및 UP계산
			for(int i = 0; i < 256; i++)
			{
				if(byKey[i] & HOLDKEY)
				{
					if(!byOldKey[i])
					{
						byOldKey[i] = 1;
						byKey[i] |= PUSHKEY;
					}
				}
				else
				{
					if(byOldKey[i])
					{
						byOldKey[i] = 0;
						byKey[i] = PULLKEY;
					}
				}
			}
		}
	}

	inline bool GetKeyDown( int KeyCode )
	{
		return (byKey[KeyCode] & PUSHKEY);
	}

	inline bool GetKey( int KeyCode ) const
	{
		return (byKey[KeyCode] & HOLDKEY);
	}

	inline bool GetKeyUp( int KeyCode ) const
	{
		return (byKey[KeyCode] & PULLKEY);
	}


private:
	// Buffer
	BYTE	byKey[256];
};

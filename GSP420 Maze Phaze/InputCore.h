////////////////////////////////////////////////////////////////////////////////
//
//	InputCore.h
//	GSP-420 Team D
//
//	Guilbert Tellez
//	Stuart Webster
//
//	Description:
//		Classes of Input Core
//
////////////////////////////////////////////////////////////////////////////////
#ifndef INPUTCORE_H
#define INPUTCORE_H
#define WIN_32_LEAN_AND_MEAN

#include <windows.h>
#include <WindowsX.h>

struct Vector
{
	float	x, y, z, w;

	Vector():x(0),y(0),z(0),w(1){}
	Vector(float v1, float v2, float v3=0.f)
		:x(v1),y(v2),z(v3),w(1){}
	Vector(int v1, int v2, int v3=0)
		:x(float(v1)),y(float(v2)),z(float(v3)),w(1){}
	Vector(Vector &pt)
		:x(pt.x),y(pt.y),z(pt.z),w(pt.w){}

	// Assignment operator
	Vector &operator=(Vector &v){x=v.x; y=v.y; z=v.z; w=v.w; return *this;}
};

class RawInputHandler
{
private:
	bool			RawInput[256];
	bool			PrevRawInput[256];

	Vector			RawMousePos;
	Vector			PrevRawMousePos;
	
	bool SetRawInputState(char Key, bool State);

	bool SetRawKeyInputState(char Key, bool State);
	bool SetRawMouseInputState(UINT uMsg, bool State);

	bool SetMousePos(LPARAM lParam);
	bool SetMousePos(Vector pos);
	bool SetMousePos(int x, int y);
public:
	RawInputHandler(void);
	~RawInputHandler(void);

	bool ClearRawInput(void);
	bool StorePrevInput(void);

	bool GetRawInputState(char Key);
	bool GetRawInputDownEvent(char Key);
	bool GetRawInputUpEvent(char Key);
	
	Vector GetRawMousePos(void);
	Vector GetClientMousePos(HWND hWnd);

	bool KillInput();
	bool PollInput();

	bool CheckInput(UINT uMsg,WPARAM wParam,LPARAM lParam);
};
#endif
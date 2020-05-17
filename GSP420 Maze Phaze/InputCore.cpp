////////////////////////////////////////////////////////////////////////////////
//
//	InputCore.cpp
//	GSP-420 Team D
//
//	Guilbert Tellez
//	Stuart Webster
//
//	Description:
//		Functions of Input Core
//
////////////////////////////////////////////////////////////////////////////////
#include "InputCore.h"

////////////////////////////////////////////////////////////////////////////////
//
// START RawInputHandler Methods and Functions
//
////////////////////////////////////////////////////////////////////////////////

////
//	Default Constructor
////
RawInputHandler::RawInputHandler(void)
{
	for (int i = 0; i < 256; i++)
	{
		RawInput[i] = false;
		PrevRawInput[i] = false;
	}
}

////
//	Default Destructor
////
RawInputHandler::~RawInputHandler(void)
{
	//delete [] RawInput;
	//delete [] PrevRawInput;
}

////
//	Clear raw input
////
bool RawInputHandler::ClearRawInput(void)
{
	for (int i = 0; i < 256; i++)
	{
		RawInput[i] = false;
		PrevRawInput[i] = false;
	}
	RawMousePos = Vector(0,0,0);
	PrevRawMousePos = Vector(0,0,0);

	return true;
}

////
//	Save input for one frame. Important for event detection.
////
bool RawInputHandler::StorePrevInput(void)
{
	for (int i = 0; i < 256; i++)
	{
		PrevRawInput[i] = RawInput[i];
	}

	return true;
}

////
//	Return the state of a raw boolean input
////
bool RawInputHandler::GetRawInputState(char Key)
{
	return RawInput[Key];
}

////
//	Return the event of a raw boolean input
////
bool RawInputHandler::GetRawInputDownEvent(char Key)
{
	return RawInput[Key] && !PrevRawInput[Key];
}

////
//	Return the event of a raw boolean input
////
bool RawInputHandler::GetRawInputUpEvent(char Key)
{
	return !RawInput[Key] && PrevRawInput[Key];
}

////
//	Assign the state of a raw boolean input
////
bool RawInputHandler::SetRawInputState(char Key, bool State)
{
	return RawInput[Key]=State;
}

////
//	Assign the state of a raw key input
////
bool RawInputHandler::SetRawKeyInputState(char Key, bool State)
{
	return RawInput[Key]=State;
}

////
//	Assign the state of a raw mouse button input
////
bool RawInputHandler::SetRawMouseInputState(UINT uMsg, bool State)
{
	switch (uMsg)
	{
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		RawInput[VK_LBUTTON]=State;
		break;

	case WM_RBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
		RawInput[VK_RBUTTON]=State;
		break;

		//case MK_SHIFT:
		//	RawInput[VK_SHIFT]=State;
		//	break;

		//case MK_CONTROL:
		//	RawInput[VK_CONTROL]=State;
		//	break;

	case WM_MBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
		RawInput[VK_MBUTTON]=State;
		break;

		//case MK_XBUTTON1:
		//	RawInput[VK_XBUTTON1]=State;
		//	break;

		//case MK_XBUTTON2:
		//	RawInput[VK_XBUTTON2]=State;
		//	break;
	default:
		return false;
	}
	return true;
}

////
//	Return the raw position of the mouse as a vector
////
Vector RawInputHandler::GetRawMousePos(void)
{
	return RawMousePos;
}

////
//	Return the client position of the mouse as a vector
////
Vector RawInputHandler::GetClientMousePos(HWND hWnd)
{
	POINT ptCursorPos;
	GetCursorPos(&ptCursorPos);
	ScreenToClient(hWnd, &ptCursorPos);

	return Vector(ptCursorPos.x, ptCursorPos.y, 0);
}

////
//	Assign the raw position of the mouse with LPARAM
////
bool RawInputHandler::SetMousePos(LPARAM lParam)
{
	RawMousePos = Vector(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
	return true;
}

////
//	Assign the raw position of the mouse with a vector
////
bool RawInputHandler::SetMousePos(Vector pos)
{
	RawMousePos=pos;
	return true;
}

////
//	Assign the raw position of the mouse with x and y
////
bool RawInputHandler::SetMousePos(int x, int y)
{
	RawMousePos.x = float(x);
	RawMousePos.y = float(y);
	return true;
}

bool RawInputHandler::KillInput()
{
	for (int i = 0; i<256; i++)
	{
		RawInput[i] = false;
		PrevRawInput[i] = false;
	}
	return true;
}
bool RawInputHandler::PollInput()
{
	for (int i = 0; i<256; i++)
	{
		RawInput[i] = (GetAsyncKeyState(i) & 0x8000);
		PrevRawInput[i] = RawInput[i];
	}
	return true;
}

bool RawInputHandler::CheckInput(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	MSG msg;
	msg.message = uMsg;
	msg.wParam = wParam;
	msg.lParam = lParam;

	switch (msg.message)
	{
		// key pressed
	case WM_KEYDOWN:
		{
			SetRawKeyInputState(msg.wParam, true);
			return true;
		}break;

		// key released
	case WM_KEYUP:
		{
			SetRawKeyInputState(msg.wParam, false);
			return true;
		}break;

		// mouse button pressed
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
		{
			SetRawMouseInputState(msg.message, true);
			return true;
		}break;

		// mouse button released
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		{
			SetRawMouseInputState(msg.message, false);
			return true;
		}break;

		// mouse moved
	case WM_MOUSEMOVE:
		{
			SetMousePos(msg.lParam);
			return true;
		}break;

		// window focus handling
	case WM_ACTIVATE:
		{
			switch(LOWORD(msg.wParam))
			{
				// window (re)gains focus
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				// poll entire keyboard/mouse for input
				PollInput();
				return true;
				break;

				// window loses focus
			case WA_INACTIVE:
				// kill all input
				KillInput();
				return true;
				break;
			}
		}break;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
//
// END RawInputHandler Methods and Functions
//
////////////////////////////////////////////////////////////////////////////////
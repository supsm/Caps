#include <algorithm>
#include <iostream>
#include <windows.h>
#include <WinUser.h>
using namespace std;

int shiftnum;
int specials[19] = { 106, 107, 108, 109, 110, 111, 186, 187, 188, 189, 190, 191, 192, 219, 220, 221, 222, 223, 226 };
bool num = false, shift = false, shiftdown = false, usershift = false, alpha = false, spec = false, sysshiftdown = false, capsdown = false, capslock = false;
HHOOK hook;

BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
	switch (CEvent)
	{
	case CTRL_C_EVENT:
		break;
	case CTRL_BREAK_EVENT:
		break;
	case CTRL_CLOSE_EVENT:
		break;
	}
	INPUT sinput;
	sinput.type = INPUT_KEYBOARD;
	sinput.ki.wScan = 0;
	sinput.ki.dwFlags = KEYEVENTF_KEYUP;
	sinput.ki.time = 0;
	sinput.ki.dwExtraInfo = 0;
	sinput.ki.wVk = 16;
	if (shift)
	{
		shift = false;
	}
	return TRUE;
}

LRESULT CALLBACK keyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	//cout << num << shift << alpha << spec << " " << capslock << endl;
	for (int i = 1; i < 255; i++)
	{
		if (GetAsyncKeyState(i) != 0)
		{
			//cout << "down " << i << endl;
		}
	}
	KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)(lParam);
	DWORD newVkCode;
	INPUT input, sinput, cinput, sysshift, syscaps;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.dwFlags = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	sinput = input;
	cinput = input;
	sysshift = input;
	syscaps = input;
	sinput.ki.wVk = 16;
	cinput.ki.wVk = 20;
	sysshift.ki.wVk = 14;
	syscaps.ki.wVk = 15;
	/*cout << p->vkCode;
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		cout << " down" << endl;
	}
	else
	{
		cout << " up" << endl;
	}*/
	if (GetAsyncKeyState(16) == 0 && GetAsyncKeyState(14) == 0)
	{
		shiftdown = false;
	}
	if (shift && !shiftdown && !usershift)
	{
		sysshift.ki.dwFlags = 0;
		SendInput(1, &sysshift, sizeof(INPUT));
		if (GetAsyncKeyState(14) != 0)
		{
			SendInput(1, &sinput, sizeof(INPUT));
		}
		sysshift.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &sysshift, sizeof(INPUT));
		shiftdown = true;
	}
	if (!shift && shiftdown)
	{
		sysshift.ki.dwFlags = 0;
		SendInput(1, &sysshift, sizeof(INPUT));
		sinput.ki.dwFlags = KEYEVENTF_KEYUP;
		if (GetAsyncKeyState(14) != 0)
		{
			SendInput(1, &sinput, sizeof(INPUT));
		}
		sysshift.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &sysshift, sizeof(INPUT));
		sinput.ki.dwFlags = 0;
		shiftdown = false;
	}
	if (capsdown && !shift && !capslock)
	{
		if (GetAsyncKeyState(16) != 0 && GetAsyncKeyState(14) == 0)
		{
			shift = true;
			num = false;
			spec = false;
			alpha = false;
		}
		else if (p->vkCode > 47 && p->vkCode < 58)
		{
			num = true;
			if (wParam == WM_SYSKEYUP || wParam == WM_KEYUP)
			{
				if (GetAsyncKeyState(p->vkCode))
				{
					return 1;
				}
				else
				{
					return CallNextHookEx(hook, nCode, wParam, lParam);
				}
			}
			else
			{
				return 1;
			}
		}
		else if (p->vkCode > 64 && p->vkCode < 91)
		{
			alpha = true;
			if (wParam == WM_SYSKEYUP || wParam == WM_KEYUP)
			{
				if (GetAsyncKeyState(p->vkCode))
				{
					return 1;
				}
				else
				{
					return CallNextHookEx(hook, nCode, wParam, lParam);
				}
			}
			else
			{
				return 1;
			}
		}
		else if (binary_search(specials, specials + 19, p->vkCode))
		{
			spec = true;
			if (wParam == WM_SYSKEYUP || wParam == WM_KEYUP)
			{
				if (GetAsyncKeyState(p->vkCode))
				{
					return 1;
				}
				else
				{
					return CallNextHookEx(hook, nCode, wParam, lParam);
				}
			}
			else
			{
				return 1;
			}
		}
	}
	if (p->vkCode == 20)
	{
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			capsdown = true;
		}
		else
		{
			if (!num && !shift && !alpha && !spec && !capslock)
			{
				num = true;
				alpha = true;
				spec = true;
			}
			else if (capslock)
			{
				num = false;
				shift = false;
				alpha = false;
				spec = false;
			}
			capsdown = false;
			capslock = !capslock;
		}
	}
	if (num || shift || alpha || spec)
	{
		////cout << "pass" << endl;
		if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
		{
			input.ki.dwFlags = KEYEVENTF_KEYUP;
		}
		if ((p->vkCode == 160 || p->vkCode == 161) && GetAsyncKeyState(14) == 0)
		{
			if (input.ki.dwFlags == KEYEVENTF_KEYUP)
			{
				usershift = false;
				//cout << "shift up" << endl;
			}
			else
			{
				usershift = true;
				//cout << "shift down " << p->vkCode << " " << GetAsyncKeyState(14) << endl;
			}
			shiftnum = p->vkCode;
		}
		if (p->vkCode == 20 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) && GetAsyncKeyState(15) == 0)
		{
			if (num)
			{
				num = false;
			}
			if (shift)
			{
				shift = false;
			}
			if (alpha)
			{
				alpha = false;
			}
			if (spec)
			{
				spec = false;
			}
		}
		if (p->vkCode > 47 && p->vkCode < 58 && (p->flags & LLKHF_INJECTED) == 0 && num)
		{
			if (usershift)
			{
				input.ki.wVk = p->vkCode;
				sinput.ki.dwFlags = KEYEVENTF_KEYUP;
				sinput.ki.wVk = shiftnum;
				SendInput(1, &sysshift, sizeof(INPUT));
				if (GetAsyncKeyState(14) != 0)
				{
					SendInput(1, &sinput, sizeof(INPUT));
				}
				SendInput(1, &input, sizeof(INPUT));
				sinput.ki.dwFlags = 0;
				if (GetAsyncKeyState(14) != 0)
				{
					SendInput(1, &sinput, sizeof(INPUT));
				}
				sysshift.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &sysshift, sizeof(INPUT));
				return 1;
			}
			input.ki.wVk = p->vkCode;
			SendInput(1, &sysshift, sizeof(INPUT));
			if (GetAsyncKeyState(14) != 0)
			{
				SendInput(1, &sinput, sizeof(INPUT));
			}
			SendInput(1, &input, sizeof(INPUT));
			sinput.ki.dwFlags = KEYEVENTF_KEYUP;
			if (GetAsyncKeyState(14) != 0)
			{
				SendInput(1, &sinput, sizeof(INPUT));
			}
			sysshift.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &sysshift, sizeof(INPUT));
			return 1;
		}
		if (p->vkCode > 64 && p->vkCode < 91 && (p->flags & LLKHF_INJECTED) == 0)
		{
			if (alpha)
			{
				return CallNextHookEx(hook, nCode, wParam, lParam);
			}
			if ((num || spec) && !alpha)
			{
				if (usershift)
				{
					input.ki.wVk = p->vkCode;
					SendInput(1, &syscaps, sizeof(INPUT));
					SendInput(1, &cinput, sizeof(INPUT));
					cinput.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &cinput, sizeof(INPUT));
					SendInput(1, &sysshift, sizeof(INPUT));
					if (GetAsyncKeyState(14) != 0)
					{
						SendInput(1, &sinput, sizeof(INPUT));
					}
					SendInput(1, &input, sizeof(INPUT));
					sinput.ki.dwFlags = KEYEVENTF_KEYUP;
					if (GetAsyncKeyState(14) != 0)
					{
						SendInput(1, &sinput, sizeof(INPUT));
					}
					sysshift.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &sysshift, sizeof(INPUT));
					cinput.ki.dwFlags = 0;
					SendInput(1, &cinput, sizeof(INPUT));
					cinput.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &cinput, sizeof(INPUT));
					syscaps.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &syscaps, sizeof(INPUT));
					return 1;
				}
				if (GetAsyncKeyState(17) == 0 && GetAsyncKeyState(18) == 0)
				{
					input.ki.wVk = p->vkCode;
					sysshift.ki.dwFlags = 0;
					SendInput(1, &sysshift, sizeof(INPUT));
					if (GetAsyncKeyState(14) != 0)
					{
						SendInput(1, &sinput, sizeof(INPUT));
					}
					SendInput(1, &input, sizeof(INPUT));
					sinput.ki.dwFlags = KEYEVENTF_KEYUP;
					if (GetAsyncKeyState(14) != 0)
					{
						SendInput(1, &sinput, sizeof(INPUT));
					}
					sysshift.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &sysshift, sizeof(INPUT));
					return 1;
				}
			}
			else if (shift)
			{
				if (GetAsyncKeyState(14) != 0)
				{
					return 1;
				}
				if (GetAsyncKeyState(17) == 0 && GetAsyncKeyState(18) == 0 && !usershift)
				{
					input.ki.wVk = p->vkCode;
					sysshift.ki.dwFlags = 0;
					SendInput(1, &sysshift, sizeof(INPUT));
					sinput.ki.dwFlags = KEYEVENTF_KEYUP;
					sysshiftdown = true;
					SendInput(1, &sinput, sizeof(INPUT));
					SendInput(1, &input, sizeof(INPUT));
					sinput.ki.dwFlags = 0;
					SendInput(1, &sysshift, sizeof(INPUT));
					SendInput(1, &sinput, sizeof(INPUT));
					sysshift.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &sysshift, sizeof(INPUT));
					sysshiftdown = false;
					return 1;
				}
			}
		}
		if (binary_search(specials, specials + 19, p->vkCode) && spec && (p->flags & LLKHF_INJECTED) == 0)
		{
			if (usershift)
			{
				input.ki.wVk = p->vkCode;
				sinput.ki.dwFlags = KEYEVENTF_KEYUP;
				sinput.ki.wVk = shiftnum;
				SendInput(1, &sysshift, sizeof(INPUT));
				if (GetAsyncKeyState(14) != 0)
				{
					SendInput(1, &sinput, sizeof(INPUT));
				}
				SendInput(1, &input, sizeof(INPUT));
				sinput.ki.dwFlags = 0;
				if (GetAsyncKeyState(14) != 0)
				{
					SendInput(1, &sinput, sizeof(INPUT));
				}
				sysshift.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &sysshift, sizeof(INPUT));
				return 1;
			}
			input.ki.wVk = p->vkCode;
			SendInput(1, &sysshift, sizeof(INPUT));
			if (GetAsyncKeyState(14) != 0)
			{
				SendInput(1, &sinput, sizeof(INPUT));
			}
			SendInput(1, &input, sizeof(INPUT));
			sinput.ki.dwFlags = KEYEVENTF_KEYUP;
			if (GetAsyncKeyState(14) != 0)
			{
				SendInput(1, &sinput, sizeof(INPUT));
			}
			sysshift.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &sysshift, sizeof(INPUT));
			return 1;
		}
		if ((p->vkCode < 65 || p->vkCode > 90) && (p->flags & LLKHF_INJECTED) == 0 && shift)
		{
			if (usershift)
			{
				input.ki.wVk = p->vkCode;
				sysshift.ki.dwFlags = 0;
				sinput.ki.wVk = shiftnum;
				SendInput(1, &sysshift, sizeof(INPUT));
				sinput.ki.dwFlags = KEYEVENTF_KEYUP;
				sysshiftdown = true;
				SendInput(1, &sinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				sinput.ki.dwFlags = 0;
				SendInput(1, &sysshift, sizeof(INPUT));
				SendInput(1, &sinput, sizeof(INPUT));
				sysshift.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &sysshift, sizeof(INPUT));
				sysshiftdown = false;
				return 1;
			}
		}
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main()
{

	ShowWindow(GetConsoleWindow(), SW_HIDE);
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.time = 0;
	input.ki.wVk = 20;
	input.ki.dwFlags = 0;
	if ((GetKeyState(20) & 0x0001) != 0)
	{
		SendInput(1, &input, sizeof(INPUT));
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
	hook = SetWindowsHookExA(WH_KEYBOARD_LL, keyboardHook, NULL, 0);
	if (hook == NULL)
	{
		Beep(800, 250);
		Sleep(60);
		Beep(600, 300);
		//uh oh
		return 1;
	}
	MSG message;
	while (true)
	{
		GetMessageA(&message, NULL, 0, 0);
		TranslateMessage(&message);
		DispatchMessageA(&message);
		Sleep(1);
	}
}
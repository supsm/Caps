#include <algorithm>
#include <iostream>
#include <windows.h>
#include <WinUser.h>
using namespace std;

int shiftnum, ctrlnum, altnum, winnum;
int specials[19] = { 106, 107, 108, 109, 110, 111, 186, 187, 188, 189, 190, 191, 192, 219, 220, 221, 222, 223, 226 };
bool num = false, shift = false, keydown = false, usershift = false, alpha = false, spec = false, capsdown = false, capslock = false, ctrl = false, alt = false, win = false, userctrl = false, useralt = false, userwin = false, sysshift = false, sysctrl = false, sysalt = false, syswin = false, syscaps = false;
HHOOK hook;

LRESULT CALLBACK keyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	cout << num << shift << alpha << spec << " " << ctrl << alt << win << " " << keydown << " " << usershift << userctrl << useralt << userwin << " " << capslock << endl;
	for (int i = 1; i < 255; i++)
	{
		if (GetAsyncKeyState(i) != 0)
		{
			//cout << "down " << i << endl;
		}
	}
	KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)(lParam);
	DWORD newVkCode;
	INPUT input, keyinput, capinput;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.dwFlags = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	keyinput = input;
	capinput = input;
	capinput.ki.wVk = 20;
	/*cout << p->vkCode;
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		cout << " down" << endl;
	}
	else
	{
		cout << " up" << endl;
	}*/
	if (GetAsyncKeyState(16) == 0 && !sysshift && !ctrl && !alt && !win)
	{
		keydown = false;
	}
	if (GetAsyncKeyState(17) == 0 && ctrl && !sysctrl)
	{
		keydown = false;
	}
	if (GetAsyncKeyState(18) == 0 && alt && !sysalt)
	{
		keydown = false;
	}
	if ((GetAsyncKeyState(91) == 0 || GetAsyncKeyState(92) == 0) && win && !syswin)
	{
		keydown = false;
	}
	if (shift && !keydown && !usershift)
	{
		if (!ctrl && !alt && !win)
		{
			sysshift = true;
			keyinput.ki.wVk = 16;
			SendInput(1, &keyinput, sizeof(INPUT));
			sysshift = false;
		}
		if (ctrl)
		{
			sysctrl = true;
			keyinput.ki.wVk = 17;
			SendInput(1, &keyinput, sizeof(INPUT));
			sysctrl = false;
		}
		if (alt)
		{
			sysalt = true;
			keyinput.ki.wVk = 18;
			SendInput(1, &keyinput, sizeof(INPUT));
			sysalt = false;
		}
		if (win)
		{
			syswin = true;
			keyinput.ki.wVk = 92;
			SendInput(1, &keyinput, sizeof(INPUT));
			syswin = false;
		}
		keydown = true;
	}
	if (!shift && keydown)
	{
		keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
		if (!ctrl && !alt && !win)
		{
			sysshift = true;
			keyinput.ki.wVk = 16;
			SendInput(1, &keyinput, sizeof(INPUT));
			sysshift = false;
		}
		if (ctrl)
		{
			sysctrl = true;
			keyinput.ki.wVk = 17;
			SendInput(1, &keyinput, sizeof(INPUT));
			sysctrl = false;
		}
		if (alt)
		{
			sysalt = true;
			keyinput.ki.wVk = 18;
			SendInput(1, &keyinput, sizeof(INPUT));
			sysalt = false;
		}
		if (win)
		{
			syswin = true;
			keyinput.ki.wVk = 92;
			SendInput(1, &keyinput, sizeof(INPUT));
			syswin = false;
		}
		keyinput.ki.dwFlags = 0;
		keydown = false;
	}
	cout << capsdown << shift << ctrl << alt << win << capslock << " " << sysshift << sysctrl << sysalt << syswin << endl;
	if (capsdown && (!shift || ctrl || alt || win) && !capslock && !sysshift && !sysctrl && !sysalt && !syswin)
	{
		if (GetAsyncKeyState(17) != 0 && !alt && !win)
		{
			ctrl = true;
		}
		if (GetAsyncKeyState(18) != 0 && !ctrl && !win)
		{
			alt = true;
		}
		if ((GetAsyncKeyState(91) != 0 || GetAsyncKeyState(92) != 0) && !ctrl && !alt)
		{
			win = true;
		}
		if (GetAsyncKeyState(16) != 0)
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
	if (num || shift || alpha || spec || ctrl || alt || win)
	{
		////cout << "pass" << endl;
		if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
		{
			input.ki.dwFlags = KEYEVENTF_KEYUP;
		}
		if ((p->vkCode == 160 || p->vkCode == 161) && !sysshift)
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
		if ((p->vkCode == 162 || p->vkCode == 163) && !sysctrl)
		{
			if (input.ki.dwFlags == KEYEVENTF_KEYUP)
			{
				userctrl = false;
			}
			else
			{
				userctrl = true;
			}
			ctrlnum = p->vkCode;
		}
		if ((p->vkCode == 164 || p->vkCode == 165) && !sysalt)
		{
			if (input.ki.dwFlags == KEYEVENTF_KEYUP)
			{
				useralt = false;
			}
			else
			{
				useralt = true;
			}
			altnum = p->vkCode;
		}
		if ((p->vkCode == 91 || p->vkCode == 92) && !syswin)
		{
			if (input.ki.dwFlags == KEYEVENTF_KEYUP)
			{
				userwin = false;
			}
			else
			{
				userwin = true;
			}
			winnum = p->vkCode;
		}
		if (p->vkCode == 20 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) && !syscaps)
		{
			num = false;
			shift = false;
			alpha = false;
			spec = false;
			ctrl = false;
			alt = false;
			win = false;
		}
		if (p->vkCode > 47 && p->vkCode < 58 && (p->flags & LLKHF_INJECTED) == 0 && num)
		{
			input.ki.wVk = p->vkCode;
			keyinput.ki.wVk = 16;
			if (usershift && shift && !ctrl && !alt && !win)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = shiftnum;
				sysshift = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				sysshift = false;
				return 1;
			}
			if (userctrl && ctrl && shift)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = ctrlnum;
				sysctrl = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				sysctrl = false;
				return 1;
			}
			if (useralt && alt && shift)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = altnum;
				sysalt = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				sysalt = false;
				return 1;
			}
			if (useralt && win && shift)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = winnum;
				syswin = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				syswin = false;
				return 1;
			}
			if (shift)
			{
				return CallNextHookEx(hook, nCode, wParam, lParam);
			}
			if (ctrl)
			{
				sysctrl = true;
				keyinput.ki.wVk = 17;
			}
			if (alt)
			{
				sysalt = true;
				keyinput.ki.wVk = 18;
			}
			if (win)
			{
				syswin = true;
				keyinput.ki.wVk = 92;
			}
			SendInput(1, &keyinput, sizeof(INPUT));
			SendInput(1, &input, sizeof(INPUT));
			keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &keyinput, sizeof(INPUT));
			sysctrl = false;
			sysalt = false;
			syswin = false;
			return 1;
		}
		if (p->vkCode > 64 && p->vkCode < 91 && (p->flags & LLKHF_INJECTED) == 0)
		{
			if (alpha && !ctrl && !alt && !win)
			{
				return CallNextHookEx(hook, nCode, wParam, lParam);
			}
			if (num || spec || (alpha && (ctrl || alt || win)))
			{
				if (usershift && shift && !ctrl && !alt && !win)
				{
					input.ki.wVk = p->vkCode;
					keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
					keyinput.ki.wVk = shiftnum;
					sysshift = true;
					SendInput(1, &keyinput, sizeof(INPUT));
					SendInput(1, &input, sizeof(INPUT));
					keyinput.ki.dwFlags = 0;
					SendInput(1, &keyinput, sizeof(INPUT));
					sysshift = false;
					return 1;
				}
				if (userctrl && ctrl && shift)
				{
					input.ki.wVk = p->vkCode;
					keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
					keyinput.ki.wVk = ctrlnum;
					sysctrl = true;
					SendInput(1, &keyinput, sizeof(INPUT));
					SendInput(1, &input, sizeof(INPUT));
					keyinput.ki.dwFlags = 0;
					SendInput(1, &keyinput, sizeof(INPUT));
					sysctrl = false;
					return 1;
				}
				if (useralt && alt && shift)
				{
					input.ki.wVk = p->vkCode;
					keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
					keyinput.ki.wVk = altnum;
					sysalt = true;
					SendInput(1, &keyinput, sizeof(INPUT));
					SendInput(1, &input, sizeof(INPUT));
					keyinput.ki.dwFlags = 0;
					SendInput(1, &keyinput, sizeof(INPUT));
					sysalt = false;
					return 1;
				}
				if (userwin && win && shift)
				{
					input.ki.wVk = p->vkCode;
					keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
					keyinput.ki.wVk = winnum;
					syswin = true;
					SendInput(1, &keyinput, sizeof(INPUT));
					SendInput(1, &input, sizeof(INPUT));
					keyinput.ki.dwFlags = 0;
					SendInput(1, &keyinput, sizeof(INPUT));
					syswin = false;
					return 1;
				}
				if ((userctrl && !ctrl) || (useralt && !alt) || (userwin && !win) || (usershift && (ctrl || alt || win)))
				{
					input.ki.wVk = p->vkCode;
					if (ctrl)
					{
						sysctrl = true;
						keyinput.ki.wVk = 17;
					}
					if (alt)
					{
						sysalt = true;
						keyinput.ki.wVk = 18;
					}
					if (win)
					{
						syswin = true;
						keyinput.ki.wVk = 92;
					}
					SendInput(1, &keyinput, sizeof(INPUT));
					SendInput(1, &input, sizeof(INPUT));
					keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &keyinput, sizeof(INPUT));
					sysctrl = false;
					sysalt = false;
					syswin = false;
					return 1;
				}
			}
		}
		if (binary_search(specials, specials + 19, p->vkCode) && spec && (p->flags & LLKHF_INJECTED) == 0)
		{
			input.ki.wVk = p->vkCode;
			keyinput.ki.wVk = 16;
			if (usershift && shift && !ctrl && !alt && !win)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = shiftnum;
				sysshift = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				sysshift = false;
				return 1;
			}
			if (userctrl && ctrl && shift)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = ctrlnum;
				sysctrl = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				sysctrl = false;
				return 1;
			}
			if (useralt && alt && shift)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = altnum;
				sysalt = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				sysalt = false;
				return 1;
			}
			if (useralt && win && shift)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = winnum;
				syswin = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				syswin = false;
				return 1;
			}
			if (shift)
			{
				return CallNextHookEx(hook, nCode, wParam, lParam);
			}
			if (ctrl)
			{
				sysctrl = true;
				keyinput.ki.wVk = 17;
			}
			if (alt)
			{
				sysalt = true;
				keyinput.ki.wVk = 18;
			}
			if (win)
			{
				syswin = true;
				keyinput.ki.wVk = 92;
			}
			SendInput(1, &keyinput, sizeof(INPUT));
			SendInput(1, &input, sizeof(INPUT));
			keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &keyinput, sizeof(INPUT));
			sysctrl = false;
			sysalt = false;
			syswin = false;
			return 1;
		}
		if ((p->vkCode < 65 || p->vkCode > 90) && (p->flags & LLKHF_INJECTED) == 0 && shift)
		{
			if (usershift && shift && !ctrl && !alt && !win)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = shiftnum;
				sysshift = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				sysshift = false;
				return 1;
			}
			if (userctrl && ctrl && shift)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = ctrlnum;
				sysctrl = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				sysctrl = false;
				return 1;
			}
			if (useralt && alt && shift)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = altnum;
				sysalt = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				sysalt = false;
				return 1;
			}
			if (useralt && win && shift)
			{
				input.ki.wVk = p->vkCode;
				keyinput.ki.dwFlags = KEYEVENTF_KEYUP;
				keyinput.ki.wVk = winnum;
				syswin = true;
				SendInput(1, &keyinput, sizeof(INPUT));
				SendInput(1, &input, sizeof(INPUT));
				keyinput.ki.dwFlags = 0;
				SendInput(1, &keyinput, sizeof(INPUT));
				syswin = false;
				return 1;
			}
		}
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main()
{

	ShowWindow(GetConsoleWindow(), SW_SHOW);
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
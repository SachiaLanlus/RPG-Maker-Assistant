// xinputTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#define WINVER 0x0A00 
//#define _WIN32_WINNT 0x0A00  

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <string>
#include <sstream>
#include <conio.h>
#include <Windows.h>
#include <winuser.h>
#include <Xinput.h>
#include <iostream>
#include <fstream>

#define getMouse false
#define directx true
/*
define::
a=z,
b=x,
x=autoBattle,
y=,
l1=ctrl,
r1=shift,
l2=,
r2=keepRun,
*/

//Huang-Ku Yeh
using namespace std;

//void pause(int);//<- very cost, seems not get much better accuracy than Sleep, so abandoned
void click(int);
void clickWithDelay(int, int);
void mgoto(int, int);
void mgotor(int, int);
void getCursorMode();
void press(int);
void release(int);

void mouseClick(int);//left:key=0,right:key=1
void mouseClickWithDelay(int, int);

void autoBattle();

void saveMacro();
void readMacro();

bool autoBattleSwitch = false;
bool keepRun = false;
bool upPressed = false;
bool downPressed = false;
bool leftPressed = false;
bool rightPressed = false;
bool l1Pressed = false;
bool r1Pressed = false;
bool l2Pressed = false;
bool r2Pressed = false;

bool macroExit = false;

int macroLineNum = 0;
string macroStr[30];

int main()
{
	//system("chcp 65001");
	XINPUT_KEYSTROKE xk = { 0 };
	XINPUT_STATE xs = { 0 };
	thread autoBattleThread;
	string str;
	ifstream f;
	f.open("saveMacroConfig.txt", std::fstream::in);
	if (f)
	{
		string bufStr;
		macroExit = true;
		while(getline(f, macroStr[macroLineNum++]))
		{
			if (macroLineNum >= 30)
			{
				cout << "macro file can not over 30 lines" << endl;
				macroExit = false;
				break;
			}
		}
		macroLineNum--;//because last line is empty
	}
	f.close();
	while (true)
	{
		if (XInputGetKeystroke(0, 0, &xk) != ERROR_EMPTY&&xk.Flags == XINPUT_KEYSTROKE_KEYDOWN)
		{
			switch (xk.VirtualKey)
			{
			case VK_PAD_A:
				cout << "Ｘ" << endl;
				clickWithDelay('Z', 30);
				break;
			case VK_PAD_B:
				cout << "○" << endl;
				clickWithDelay('X', 30);
				break;
			case VK_PAD_X:
				cout << "□" << endl;
				break;
			case VK_PAD_Y:
				cout << "△" << endl;
				if (!autoBattleSwitch)
				{
					autoBattleSwitch = true;
					autoBattleThread = thread(autoBattle);
				}
				else
				{
					autoBattleSwitch = false;
					autoBattleThread.join();
				}
				break;
			case VK_PAD_DPAD_UP:
			//case 22560:
				cout << "↑" << endl;
				upPressed = true;
				press(VK_UP);
				break;
			case VK_PAD_DPAD_DOWN:
			//case 22561:
				cout << "↓" << endl;
				downPressed = true;
				press(VK_DOWN);
				break;
			case VK_PAD_DPAD_LEFT:
			//case 22563:
				cout << "←" << endl;
				leftPressed = true;
				press(VK_LEFT);
				break;
			case VK_PAD_DPAD_RIGHT:
			//case 22562:
				cout << "→" << endl;
				rightPressed = true;
				press(VK_RIGHT);
				break;
			case VK_PAD_LSHOULDER:
				cout << "L1" << endl;
				l1Pressed = true;
				press(VK_CONTROL);
				break;
			case VK_PAD_RSHOULDER:
				cout << "R1" << endl;
				r1Pressed = true;
				//press(VK_SHIFT);
				break;
			case VK_PAD_LTRIGGER:
				cout << "L2" << endl;
				l2Pressed = true;
				break;
			case VK_PAD_RTRIGGER:
				cout << "R2" << endl;
				r2Pressed = true;
				if (!keepRun)
				{
					cout << "into keep run mode" << endl;
					keepRun = true;
					press(VK_SHIFT);
				}
				else
				{
					cout << "out of keep run mode" << endl;
					keepRun = false;
					release(VK_SHIFT);
				}
				break;
			case VK_PAD_START:
				cout << "START" << endl;
				saveMacro();
				break;
			case VK_PAD_BACK:
				cout << "BACK" << endl;
				clickWithDelay(VK_F12, 30);
				break;
			default:
				cout << xk.VirtualKey << endl;
			}
		}
		XInputGetState(0, &xs);

		if ((xs.Gamepad.wButtons & 0x0100) == 0 && l1Pressed)
		{
			l1Pressed = false;
			release(VK_CONTROL);
			cout << "L1 up" << endl;
		}
		if ((xs.Gamepad.wButtons & 0x0001) == 0 && upPressed)
		{
			upPressed = false;
			release(VK_UP);
			cout << "↑ up" << endl;
		}
		if ((xs.Gamepad.wButtons & 0x0002) == 0 && downPressed)
		{
			downPressed = false;
			release(VK_DOWN);
			cout << "↓ up" << endl;
		}
		if ((xs.Gamepad.wButtons & 0x0004) == 0 && leftPressed)
		{
			leftPressed = false;
			release(VK_LEFT);
			cout << "← up" << endl;
		}
		if ((xs.Gamepad.wButtons & 0x0008) == 0 && rightPressed)
		{
			rightPressed = false;
			release(VK_RIGHT);
			cout << "→ up" << endl;
		}
		if (keepRun) press(VK_SHIFT);
		Sleep(10);
	}
	return 0;
}

void autoBattle()
{
	cout << "into auto battle mode" << endl;
	int counter = 1;
	autoBattleSwitch;
	XINPUT_VIBRATION xv = { 0 };
	xv.wLeftMotorSpeed = 10000;
	xv.wRightMotorSpeed = 10000;
	press(VK_SHIFT);
	XInputSetState(0, &xv);
	Sleep(100);
	xv.wLeftMotorSpeed = 0;
	xv.wRightMotorSpeed = 0;
	XInputSetState(0, &xv);
	while (autoBattleSwitch)
	{
		if (counter++ % 25 == 0)cout << "battle mode ..." << endl;
		Sleep(20);
		clickWithDelay('Z', 20);
	}
	cout << "out of auto battle mode" << endl;
	release(VK_SHIFT);
}

void getCursorMode()
{
	string str, buf;
	stringstream ss;
	POINT pt = { 0 };
	while (true)
	{
		GetCursorPos(&pt);
		str = "X: ";
		ss << pt.x << " " << pt.y;
		ss >> buf;
		str += buf;
		ss >> buf;
		str += " Y:";
		str += buf;
		ss.str("");
		ss.clear();
		SetConsoleTitle(str.c_str());
		Sleep(200);
	}
}

void mouseClick(int key)
{
	INPUT ip[2] = { 0 };
	ip[0].type = INPUT_MOUSE;
	ip[1].type = INPUT_MOUSE;
	ip[0].mi.mouseData = 0;
	ip[1].mi.mouseData = 0;
	ip[0].mi.time = 0;
	ip[1].mi.time = 0;
	if (key == 0)
	{
		ip[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		ip[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
	}
	else if (key == 1)
	{
		ip[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		ip[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	}
	SendInput(2, ip, sizeof(INPUT));
}

void mouseClickWithDelay(int key, int t)
{
	if (t < 0)return;
	INPUT ip[2] = { 0 };
	ip[0].type = INPUT_MOUSE;
	ip[1].type = INPUT_MOUSE;
	ip[0].mi.mouseData = 0;
	ip[1].mi.mouseData = 0;
	ip[0].mi.time = 0;
	ip[1].mi.time = 0;
	if (key == 0)
	{
		ip[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		ip[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
	}
	else if (key == 1)
	{
		ip[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		ip[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	}
	SendInput(1, ip, sizeof(INPUT));
	//pause(t);
	Sleep(t);
	SendInput(1, &ip[1], sizeof(INPUT));
	Sleep(t);
}

void click(int key)
{
	INPUT ip[2] = { 0 };
	ip[0].type = INPUT_KEYBOARD;
	ip[1].type = INPUT_KEYBOARD;
	ip[0].ki.wVk = key;
	ip[1].ki.wVk = key;
	ip[1].ki.dwFlags = KEYEVENTF_KEYUP;
	if (directx)
	{
		ip[0].ki.wScan = MapVirtualKey(key, 0);//MAPVK_VK_TO_VSC=0
		ip[1].ki.wScan = MapVirtualKey(key, 0);
		ip[0].ki.dwFlags |= KEYEVENTF_SCANCODE;
		ip[1].ki.dwFlags |= KEYEVENTF_SCANCODE;
	}
	SendInput(2, ip, sizeof(INPUT));
}

void clickWithDelay(int key, int t)
{
	if (t<0)return;
	INPUT ip[2] = { 0 };
	ip[0].type = INPUT_KEYBOARD;
	ip[1].type = INPUT_KEYBOARD;
	ip[0].ki.wVk = key;
	ip[1].ki.wVk = key;
	ip[1].ki.dwFlags = KEYEVENTF_KEYUP;
	if (directx)
	{
		ip[0].ki.wScan = MapVirtualKey(key, 0);//MAPVK_VK_TO_VSC=0
		ip[1].ki.wScan = MapVirtualKey(key, 0);
		ip[0].ki.dwFlags |= KEYEVENTF_SCANCODE;
		ip[1].ki.dwFlags |= KEYEVENTF_SCANCODE;
	}
	SendInput(1, ip, sizeof(INPUT));
	//pause(t);
	Sleep(t);
	SendInput(1, &ip[1], sizeof(INPUT));
	Sleep(t);
}

void press(int key)
{
	INPUT ip = { 0 };
	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = key;
	if (directx)
	{
		ip.ki.wScan = MapVirtualKey(key, 0);
		ip.ki.dwFlags |= KEYEVENTF_SCANCODE;
	}
	SendInput(1, &ip, sizeof(INPUT));
}

void release(int key)
{
	INPUT ip = { 0 };
	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = key;
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	if (directx)
	{
		ip.ki.wScan = MapVirtualKey(key, 0);
		ip.ki.dwFlags |= KEYEVENTF_SCANCODE;
	}
	SendInput(1, &ip, sizeof(INPUT));
}

void mgoto(int x, int y)
{
	if (x<0)x = 0;
	if (y<0)y = 0;

	INPUT ip = { 0 };
	ip.type = INPUT_MOUSE;
	ip.mi.dx = x * (65536 / GetSystemMetrics(SM_CXSCREEN));
	ip.mi.dy = y * (65536 / GetSystemMetrics(SM_CYSCREEN));
	ip.mi.mouseData = 0;
	ip.mi.time = 0;
	ip.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	SendInput(1, &ip, sizeof(INPUT));
}

void mgotor(int x, int y)
{
	if (x<0)x = 0;
	if (y<0)y = 0;

	INPUT ip = { 0 };
	ip.type = INPUT_MOUSE;
	ip.mi.dx = x;
	ip.mi.dy = y;
	ip.mi.mouseData = 0;
	ip.mi.time = 0;
	ip.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &ip, sizeof(INPUT));
}

void saveMacro()
{
	if (!macroExit)
	{
		cout << "macro not found" << endl;
		/*clickWithDelay('X', 50);
		Sleep(500);
		clickWithDelay(VK_LEFT, 50);
		Sleep(100);
		clickWithDelay(VK_LEFT, 50);
		Sleep(100);
		clickWithDelay('Z', 50);
		Sleep(300);
		clickWithDelay('Z', 50);
		Sleep(300);
		clickWithDelay('X', 50);
		Sleep(100);*/
	}
	else
	{
		cout << "macro found" << endl;
		readMacro();
	}
}

void readMacro()
{
	int i = 0,t;
	stringstream ss;
	cout << "save macro start" << endl;
	for(i=0;i<macroLineNum;i++)
	{
		ss << macroStr[i];
		for(int j=0;j<ss.str().length();j++)
		{
			if (ss.str()[j] >= 'a'&&ss.str()[j] <= 'z') ss.str()[j] -= 32;
		}
		if (ss.str() == "X")
		{
			cout << "X" << endl;
			clickWithDelay('X', 50);
		}
		else if (ss.str() == "Z")
		{
			cout << "Z" << endl;
			clickWithDelay('Z', 50);
		}
		else if (ss.str() == "LEFT")
		{
			cout << "VK_LEFT" << endl;
			clickWithDelay(VK_LEFT, 50);
		}
		else if (ss.str() == "RIGHT")
		{
			cout << "VK_RIGHT" << endl;
			clickWithDelay(VK_RIGHT, 50);
		}
		else if (ss.str() == "UP")
		{
			cout << "VK_UP" << endl;
			clickWithDelay(VK_UP, 50);
		}
		else if (ss.str() == "DOWN")
		{
			cout << "VK_DOWN" << endl;
			clickWithDelay(VK_DOWN, 50);
		}
		else
		{
			t = 0;
			for(int k=0;k<ss.str().length();k++)
			{
				t *= 10;
				t += ss.str()[k]-'0';
			}
			if (t < 0)t = 0;
			ss >> t;
			cout << "sleep for " << t << endl;
			Sleep(t);
		}
		ss.str("");
		ss.clear();
	}
	cout << "save macro finish" << endl;
}
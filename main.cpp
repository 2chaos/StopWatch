// hellomsg.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "hellomsg.h"
#include <cwchar>

#define MAX_LOADSTRING 100
#define ID_TIMER 1
// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
WCHAR szListClass[MAX_LOADSTRING];              // list窗口类名
enum BUTTON_STATE {								// 按钮状态
	up = 1,
	down = 2,
	enable = 4,
	disable = 8
};
HWND hbutton1, hbutton2, hlist;
BUTTON_STATE button1_state, button2_state;
SYSTEMTIME time_exist, time_beg, time_now;
int show_min, show_sec, show_msec;


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterWatchClass(HINSTANCE hInstance);
ATOM                MyRegisterListClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    LisProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	wsprintf(szTitle, L"%s", L"Stopwatch");
	wsprintf(szWindowClass, L"%s", L"Watch");
	wsprintf(szListClass, L"%s", L"List");
    MyRegisterWatchClass(hInstance);
	MyRegisterListClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HELLOMSG));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterWatchClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;		//窗口过程
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLOMSG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName   = 0;			// 秒表没有菜单  MAKEINTRESOURCEW(IDC_HELLOMSG);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
ATOM MyRegisterListClass(HINSTANCE hInstance) {
	WNDCLASSEXW lcex;

	lcex.cbSize = sizeof(WNDCLASSEX);

	lcex.style = CS_HREDRAW | CS_VREDRAW;
	lcex.lpfnWndProc    = LisProc;		//窗口过程
	lcex.cbClsExtra		= 0;
	lcex.cbWndExtra		= 0;
	lcex.hInstance		= hInstance;
	lcex.hIcon			= nullptr;		//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLOMSG));
	lcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	lcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 2);
	lcex.lpszMenuName   = 0;			// 秒表没有菜单  MAKEINTRESOURCEW(IDC_HELLOMSG);
	lcex.lpszClassName	= szListClass;
	lcex.hIconSm		= LoadIcon(lcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&lcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW^WS_THICKFRAME,	//大小不可改变
      800, 300, 400, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK LisProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message)
	{
	case WM_CREATE:
		{

		}
		break;
	case WM_PAINT:
		{

		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			hbutton1 = CreateWindow(TEXT("button"), TEXT("计次"), WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_DEFPUSHBUTTON,
				70, 130, 60, 40, hWnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
			hbutton2 = CreateWindow(TEXT("button"), TEXT("启动"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				250, 130, 60, 40, hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
			hlist    = CreateWindow(szListClass, TEXT("List"), WS_CHILDWINDOW| WS_VISIBLE | WS_VSCROLL,
				50, 200, 300, 300, hWnd, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
			button1_state = (BUTTON_STATE)(up | disable);
			button2_state = up;
			time_exist.wMinute = time_exist.wSecond = time_exist.wMilliseconds = 0;
			time_beg.wMinute   = time_beg.wSecond   = time_beg.wMilliseconds   = 0;
			time_now.wMinute   = time_now.wSecond   = time_now.wMilliseconds   = 0;

			ShowWindow(hlist, SW_SHOW);
			UpdateWindow(hlist);		
		}
		break;
    case WM_COMMAND:
        {
            int button_id = LOWORD(wParam);
			HWND hbutton = GetDlgItem(hWnd, button_id);
			HWND hlist = GetDlgItem(hWnd, 3);
            // 分析菜单选择:
            switch (button_id)
            {
				case 1:				//点击左按钮
					{
						if (button1_state & down) {							//复位
							KillTimer(hWnd, ID_TIMER);
							InvalidateRect(hWnd, nullptr, FALSE);
							time_exist.wMinute = time_exist.wSecond = time_exist.wMilliseconds = 0;
							time_beg.wMinute = time_beg.wSecond = time_beg.wMilliseconds = 0;
							time_now.wMinute = time_now.wSecond = time_now.wMilliseconds = 0;
							button1_state = (BUTTON_STATE)(up | disable);
							SetWindowText(hbutton1, TEXT("计次"));
							EnableWindow(hbutton1, FALSE);

							button2_state = up;
							SetWindowText(hbutton2, TEXT("启动"));
						}
						else {												//计次，保存记录，并在子窗口显示

						}
					}
					break;
				case 2:				//点击右按钮
					{
						if (button2_state & up) {							//“启动”
							GetLocalTime(&time_beg);						//获取计时开始时刻时间
							if (button1_state & disable)					//若左按钮灰色，启用左按钮
								EnableWindow(hbutton1, TRUE);
							SetTimer(hWnd, ID_TIMER, 10, nullptr);			//启用计时器
							button1_state = (BUTTON_STATE)(up | enable);
							SetWindowText(hbutton1, TEXT("计次"));
							button2_state = down;
							SetWindowText(hbutton2, TEXT("停止"));							
						}
						else if (button2_state & down) {					//“停止”
							KillTimer(hWnd, ID_TIMER);							
							button2_state = up;
							SetWindowText(hbutton2, TEXT("启动"));
							button1_state = down;
							SetWindowText(hbutton1, TEXT("复位"));
							time_exist.wMinute = show_min;
							time_exist.wSecond = show_sec;
							time_exist.wMilliseconds = show_msec;
						}
					}                
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_TIMER:
		{
			InvalidateRect(hWnd, nullptr, FALSE);
		}
    case WM_PAINT:
        {
			if (button2_state & down) {
				GetLocalTime(&time_now);
				show_min = time_exist.wMinute + time_now.wMinute - time_beg.wMinute;
				show_sec = time_exist.wSecond + time_now.wSecond - time_beg.wSecond;
				show_msec = time_exist.wMilliseconds + time_now.wMilliseconds - time_beg.wMilliseconds;
			}
			else {
				show_min = time_exist.wMinute;
				show_sec = time_exist.wSecond;
				show_msec = time_exist.wMilliseconds;
			}
				
			if (show_msec < 0) {
				show_msec += 1000;
				--show_sec;
			}
			if (show_msec >= 1000) {
				show_msec -= 1000;
				++show_sec;
			}
			if (show_sec < 0) {
				show_sec += 60;
				--show_min;
			}
			if (show_sec >= 60) {
				show_sec -= 60;
				++show_min;
			}
		
            HDC hdc = GetDC(hWnd);	
			TCHAR time_str[30];
			TextOut(hdc, 160, 60, time_str, wsprintf(time_str, TEXT("%02d:%02d.%02d"), show_min, show_sec, show_msec / 10));
            ReleaseDC(hWnd, hdc);
        }
        break;
    case WM_DESTROY:
		{
			KillTimer(hWnd, ID_TIMER);
			PostQuitMessage(0);
		}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


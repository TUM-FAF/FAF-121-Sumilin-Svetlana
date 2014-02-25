#include <windows.h>
#define WM_SETFONT  0x0030
#define IDC_OK_BUTTON 101
#define IDC_CANCEL_BUTTON 102
#define IDC_EDIT2 103
#define IDC_APPLY_BUTTON 104

HWND hEdit, hEdit2;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, 
					int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SVETAs");
	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	typedef struct tagDRAWITEMSTRUCT {
    UINT CtlType;
    UINT CtlID;
    UINT itemID;
    UINT itemAction;
    UINT itemState;
    HWND hwndItem;
    HDC hDC;
    RECT rcItem;
    ULONG_PTR itemData;
	} DRAWITEMSTRUCT;
	
	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This requires Windows NT!"), szAppName, 
					MB_ICONERROR);
		return 0;
	}
	
	hWnd = CreateWindow (szAppName,
						TEXT ("First of Sveta"),
						WS_OVERLAPPEDWINDOW,
						100,
						50,
						600,
						600,
						NULL,
						NULL,
						hInstance,
						NULL);
	ShowWindow (hWnd, iCmdShow);
	UpdateWindow (hWnd);

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	return msg.wParam;


}


LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	RECT rect;
	GetClientRect(hWnd, &rect);
	static bool reset;
	
	switch (message)
	{
		HFONT hFont;
		LOGFONT LogFont;
		PAINTSTRUCT Ps;

		case WM_PAINT:
		{	
			hdc = BeginPaint (hWnd, &Ps);
			RECT rct;
			GetClientRect (hWnd, &rct);
			if (reset) SetTextColor(hdc, 99999);
			DrawText (hdc,
				TEXT ("Hello from Sveta!"), 
				-1, 
				&rect,
				DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	
			hFont = CreateFont(46, 28, 215, 0,
							FW_NORMAL, FALSE, FALSE, FALSE,
							ANSI_CHARSET, OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
							DEFAULT_PITCH | FF_ROMAN,
							"Times New Roman");

			SelectObject(hdc, hFont);
			TextOut(hdc, (rct.right - rct.left) / 2, 128, TEXT("O___O"), 6);
		        
			EndPaint(hWnd, &Ps);
		}break;
	
			 
		case WM_CREATE:
		{
				hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
								TEXT("Edit"),
								"",
								WS_CHILD | WS_VISIBLE | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL,
								10, 10, 200, 50,
								hWnd, NULL, NULL, NULL);

			HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
		
			SendMessage(hEdit,
					WM_SETFONT,
					(WPARAM)hfDefault,
					MAKELPARAM(FALSE,0));
			SendMessage(hEdit,
					WM_SETTEXT,
					NULL,
					(LPARAM)"GO!");
			
			hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE,
							TEXT("Edit"),
							"",
							WS_CHILD | WS_VISIBLE | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL,
							10, 100, 200, 50,
							hWnd, (HMENU)IDC_EDIT2, GetModuleHandle(NULL), NULL);
				
			HWND okButton = CreateWindowEx(NULL, 
									"BUTTON",
									"OK",
									WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
									50,
									220,
									100,
									24,
									hWnd,
									(HMENU)IDC_OK_BUTTON,
									GetModuleHandle(NULL),
									NULL);

			SendMessage(okButton,
					WM_SETFONT,
					(WPARAM)hfDefault,
					MAKELPARAM(FALSE,0));

			HWND cancelButton = CreateWindowEx(NULL, 
										"BUTTON",
										"CANCEL",
										WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WM_SETFONT,
										250,
										220,
										100,
										24,
										hWnd,
										(HMENU)IDC_CANCEL_BUTTON,
										GetModuleHandle(NULL),
										NULL);
		
			hFont = CreateFont(18,
							0,
							0,
							0,
							FW_BOLD,
							0,
							0,
							0,
							ANSI_CHARSET,
							OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY,
							DEFAULT_PITCH | FF_DONTCARE,
							"Lucida Handwriting");
			SendMessage(GetDlgItem(hWnd, IDC_CANCEL_BUTTON), WM_SETFONT, (WPARAM)hFont, TRUE);
		
			HWND applyButton = CreateWindowEx(NULL, 
										"BUTTON",
										"APPLY",
										WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
										450,
										220,
										100,
										24,
										hWnd,
										(HMENU)IDC_APPLY_BUTTON,
										GetModuleHandle(NULL),
										NULL);
			hFont = CreateFont(10,
							0,
							0,
							0,
							FW_BOLD,
							0,
							0,
							0,
							ANSI_CHARSET,
							OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY,
							DEFAULT_PITCH | FF_DONTCARE,
							"Lucida Handwriting");
			SendMessage(GetDlgItem(hWnd, IDC_APPLY_BUTTON), WM_SETFONT, (WPARAM)hFont, TRUE);

		}break;

		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* mmi = (MINMAXINFO*)lParam;
			mmi->ptMinTrackSize.x = 450;
			mmi->ptMinTrackSize.y = 450;
			mmi->ptMaxTrackSize.x = 740;
			mmi->ptMaxTrackSize.y = 740;
			return 0;
		}

		case WM_SIZE:
		{
		   RECT rWindow;
		   GetWindowRect(hWnd, &rWindow);

		   SetWindowPos(GetDlgItem(hWnd, IDC_OK_BUTTON), 
					HWND_TOP, 
					(rWindow.right - rWindow.left)/2 - 50,
					(rWindow.bottom - rWindow.top) - 100, 
					0, 
					0, 
					SWP_NOSIZE);
   
		   SetWindowPos(GetDlgItem(hWnd, IDC_CANCEL_BUTTON), 
					HWND_TOP, 
					(rWindow.right - rWindow.left)/2 - 220,
					(rWindow.bottom - rWindow.top) - 100,  
					0, 
					0, 
					SWP_NOSIZE);
     
			SetWindowPos(GetDlgItem(hWnd, IDC_APPLY_BUTTON), 
					HWND_TOP, 
					(rWindow.right - rWindow.left)/2 + 100,
					(rWindow.bottom - rWindow.top) - 100, 
					0, 
					0, 
					SWP_NOSIZE);
		}break;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_CANCEL_BUTTON:
				{
					MessageBox (NULL, TEXT("Push it harder!"), TEXT("Push"), 0);
					return 0;
				}break;
			
				case IDC_OK_BUTTON:
				{
					char buffer[256];
					GetWindowText(hEdit, buffer, 256);
					MessageBox(NULL,
							buffer,
							"Info",
							MB_ICONINFORMATION);
				}break;
				
				case IDC_APPLY_BUTTON:
				{
					reset = !reset;
					RECT rect;
					GetClientRect(hWnd, &rect);
					HBRUSH brush = CreateSolidBrush(RGB(255, 250, 240));
					SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)brush);
					SetClassLongPtr(hEdit, GCLP_HBRBACKGROUND, (LONG)brush);
					InvalidateRect(hWnd, &rect, TRUE);
				}break;
			}
		}break;

		case WM_DESTROY:
			PostQuitMessage (0);
			return 0;
	}
	
	return DefWindowProc (hWnd, message, wParam, lParam);
}

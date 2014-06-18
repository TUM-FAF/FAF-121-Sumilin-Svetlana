#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ScrollProc (HWND, UINT, WPARAM, LPARAM) ;

char szAppName[] = "Nonono";
WNDPROC fnOldScr[3] ;
HWND hEdit, hEdit2, hwndScrol[3], hwndLabel[3], hwndValue[3], hwndRect ;
int     color[3], iFocus ;
HINSTANCE hInst ;
char      szCaption[35] ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, 
					int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SVETA2");
	static char *szColorLabel[] = { "R", "G", "B" };
	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;
	int i;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON));
	wndclass.hCursor = LoadCursor (hInstance, MAKEINTRESOURCE(IDI_CURSOR));
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wndclass.lpszClassName = szAppName;
	
	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This requires Windows NT!"), szAppName, 
					MB_ICONERROR);
		return 0;
	}
	
	hWnd = CreateWindow (szAppName,
						TEXT ("Second of Sveta"),
						WS_OVERLAPPEDWINDOW|WS_HSCROLL,
						100,
						50,
						600,
						600,
						NULL,
						NULL,
						hInstance,
						NULL);

	RegisterHotKey(hWnd, IDK_HOTKEY, MOD_CONTROL, 0x51);
	RegisterHotKey(hWnd, IDK_HOTKEY2, MOD_CONTROL, 0x41);
/*

	hwndRect = CreateWindow ("static", NULL,
                              WS_CHILD | WS_VISIBLE | SS_WHITERECT,
                              0, 0, 0, 0,
                              hWnd, (HMENU) 9, hInstance, NULL) ;
*/
	for (i = 0 ; i < 3 ; i++)
          {
          hwndScrol[i] = CreateWindow ("scrollbar", NULL,
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT,
                              0, 0, 0, 0,
                              hWnd, (HMENU) i, hInstance, NULL) ;

          hwndLabel[i] = CreateWindow ("static", szColorLabel[i],
                              WS_CHILD | WS_VISIBLE | SS_CENTER,
                              0, 0, 0, 0,
                              hWnd, (HMENU) (i + 3), hInstance, NULL) ;

          hwndValue[i] = CreateWindow ("static", "0",
                              WS_CHILD | WS_VISIBLE | SS_CENTER,
                              0, 0, 0, 0,
                              hWnd, (HMENU) (i + 6), hInstance, NULL) ;

          fnOldScr[i] = (WNDPROC) SetWindowLong (hwndScrol[i], GWL_WNDPROC,
                                                 (LONG) ScrollProc) ;

          SetScrollRange (hwndScrol[i], SB_CTL, 0, 255, FALSE) ;
          SetScrollPos   (hwndScrol[i], SB_CTL, 0, FALSE) ;
          }

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
	HMENU hMenu;
	static int Color [3] = { WHITE_BRUSH, GRAY_BRUSH, BLACK_BRUSH };
	static int Selected = IDM_WHITE;

	 static COLORREF crPrim[3] = { RGB (0, 0, 0)} ;
     static HBRUSH   hBrush[3], hBrushStatic ;
     static int      cyChar ;
     static RECT     rcColor ;
     char            szbuffer[10] ;
     int             i, cxClient, cyClient ;
	 		
	switch (message)
	{
		case WM_COMMAND : 
			hMenu = GetMenu(hWnd);

			switch(LOWORD(wParam)) 
				{ 
				case IDM_ABOUT : 
					MessageBox(hWnd, TEXT (" Laboratory Work Number 2 \n Svetlana Sumilin"), 
					szAppName, MB_ICONEXCLAMATION | MB_OK); 
					return 0;
				case IDM_EXIT : 
					SendMessage(hWnd, WM_CLOSE, 0, 0); 
					return 0; 

				case IDM_WHITE :
				case IDM_GRAY :
				case IDM_BLACK :

					CheckMenuItem(hMenu, Selected, MF_UNCHECKED); 
					Selected = LOWORD(wParam); 
					CheckMenuItem(hMenu, Selected, MF_CHECKED); 

					SetClassLong(hWnd, GCL_HBRBACKGROUND, (LONG) 
						GetStockObject 
								(Color[LOWORD(wParam) - IDM_WHITE])); 

				InvalidateRect(hWnd, NULL, TRUE); 
				return 0; 

				case IDM_HELP : 
					MessageBox(hWnd, TEXT (" CTRL+Q - to quit! \n CTRL+A - to output a dialog box \n" 
						" TAB - to switch between scrolls \n pg up, pg down, end, home, up, down - to interact with scrolls"), 
					szAppName, MB_ICONEXCLAMATION | MB_OK); 
					return 0;

				case IDM_PEACH :
					color[0] = 255;
					color[1] = RGB(0, 218, 0); 
					color[2] = RGB(0, 0, 185); 

			}
			break;

				/*PAINTSTRUCT Ps;
				HDC hdc;
				RECT rect;*/
		case WM_PAINT:
		{	
			
		}break;

		case WM_HOTKEY:
			{
				switch(LOWORD(wParam))
				{
				case IDK_HOTKEY:
					SendMessage(hWnd, WM_DESTROY , 0,0);
					break;

				case IDK_HOTKEY2:
					MessageBox(hWnd, TEXT ("DON'T!"), 
					szAppName, MB_ICONEXCLAMATION | MB_OK); 
					break;
				}
			}break;
				 
		case WM_CREATE:
		{
			HWND hWndList = CreateWindowEx (NULL,
				TEXT ("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL, 200, 20, 150, 60, hWnd, NULL, hInst, NULL);  
						
			for (i = 0 ; i < 3 ; i++)
               hBrush[i] = CreateSolidBrush (crPrim[i]) ;

               hBrushStatic = CreateSolidBrush ( GetSysColor (COLOR_BTNHIGHLIGHT)) ;

               cyChar = HIWORD (GetDialogBaseUnits ()) ;
			   SendMessage(hWndList, LB_ADDSTRING,TRUE, (LPARAM)"Peach Puff"); //255-218-185
			   SendMessage(hWndList, LB_ADDSTRING,TRUE, (LPARAM)"Turquoise"); // 64-224-208
			   SendMessage(hWndList, LB_ADDSTRING,TRUE, (LPARAM)"Firebrick");// 178-34-34
			   SendMessage(hWndList, LB_ADDSTRING,TRUE, (LPARAM)"Tan");// 210-180-140
     	}

		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* mmi = (MINMAXINFO*)lParam;
			mmi->ptMinTrackSize.x = 550;
			mmi->ptMinTrackSize.y = 550;
			mmi->ptMaxTrackSize.x = 850;
			mmi->ptMaxTrackSize.y = 850;
			return 0;
		}

		case WM_SIZE:
		{
			cxClient = LOWORD (lParam) ;
            cyClient = HIWORD (lParam) ;

            SetRect (&rcColor, cxClient , 0, cxClient/1000, cyClient) ;

            MoveWindow (hwndRect, 0, 0, cxClient /1000, cyClient, TRUE) ;

            for (i = 0 ; i < 3 ; i++)
                {
                 MoveWindow (hwndScrol[i],
                 (i + 1) * cxClient / 14, 2 * cyChar,
                 20, cyClient - 4 * cyChar, TRUE) ;

                 MoveWindow (hwndLabel[i],
				 (2 * i + 1) * cxClient / 28, cyChar / 2,
                 35, cyChar, TRUE) ;

                 MoveWindow (hwndValue[i],
                 (2 * i + 1) * cxClient / 28, cyClient - 3 * cyChar / 2,
                 35, cyChar, TRUE) ;
                 }
               SetFocus (hWnd) ;
               return 0 ;
		}break;

		case WM_SETFOCUS :
               SetFocus (hwndScrol[iFocus]) ;
               return 0 ;

		case WM_VSCROLL :
        i = GetWindowLong ((HWND) lParam, GWL_ID) ;

               switch (LOWORD (wParam))
                    {
                    case SB_PAGEDOWN :
                         color[i] += 15 ;
                                                 
                    case SB_LINEDOWN :
                         color[i] = min (255, color[i] + 1) ;
                         break ;

                    case SB_PAGEUP :
                         color[i] -= 15 ;
                                                  
                    case SB_LINEUP :
                         color[i] = max (0, color[i] - 1) ;
                         break ;

                    case SB_TOP :
                         color[i] = 0 ;
                         break ;

                    case SB_BOTTOM :
                         color[i] = 255 ;
                         break ;

                    case SB_THUMBPOSITION :
                    case SB_THUMBTRACK :
                         color[i] = HIWORD (wParam) ;
                         break ;

                    default :
                         break ;
                    }
               SetScrollPos  (hwndScrol[i], SB_CTL, color[i], TRUE) ;
               SetWindowText (hwndValue[i], itoa (color[i], szbuffer, 10)) ;

               DeleteObject ((HBRUSH)
                    SetClassLong (hWnd, GCL_HBRBACKGROUND,
                         (LONG) CreateSolidBrush (
                                     RGB (color[0], color[1], color[2])))) ;

               InvalidateRect (hWnd, &rcColor, TRUE) ;
               return 0 ;

          case WM_CTLCOLORSCROLLBAR :
               i = GetWindowLong ((HWND) lParam, GWL_ID) ;

               return (LRESULT) hBrush[i] ;

          case WM_CTLCOLORSTATIC :
               i = GetWindowLong ((HWND) lParam, GWL_ID) ;

               if (i >= 3 && i <= 8)    
                    {
                    SetTextColor ((HDC) wParam, crPrim[i % 3]) ;
                    SetBkColor ((HDC) wParam, GetSysColor (COLOR_BTNHIGHLIGHT));

                    return (LRESULT) hBrushStatic ;
                    }
               break ;

          case WM_SYSCOLORCHANGE :
               DeleteObject (hBrushStatic) ;

               hBrushStatic = CreateSolidBrush (
                                   GetSysColor (COLOR_BTNHIGHLIGHT)) ;
               return 0 ;

		case WM_DESTROY:
			DeleteObject ((HBRUSH)
                    SetClassLong (hWnd, GCL_HBRBACKGROUND,
                         (LONG) GetStockObject (WHITE_BRUSH))) ;

               for (i = 0 ; i < 3 ; DeleteObject (hBrush[i++])) ;

               DeleteObject (hBrushStatic) ;
			   UnregisterHotKey(hWnd, IDK_HOTKEY);
			PostQuitMessage (0);
			return 0;
	}
	return DefWindowProc (hWnd, message, wParam, lParam);
}

LRESULT CALLBACK ScrollProc (HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     int i = GetWindowLong (hWnd, GWL_ID) ;

     switch (iMsg)
          {
          case WM_KEYDOWN :
            if (wParam == VK_TAB)
                SetFocus (hwndScrol[(i +
                      (GetKeyState (VK_SHIFT) < 0 ? 2 : 1)) % 3]) ;
               
			if (wParam == VK_F1)
						MessageBox(hWnd, TEXT ("You pressed f1"), 
						szAppName, MB_ICONEXCLAMATION | MB_OK); 
						break;

						
			if (wParam == VK_CONTROL && 0x41)
				   MessageBox(hWnd, TEXT ("Exit?"), 
						szAppName, MB_ICONEXCLAMATION | MB_OKCANCEL); 


          case WM_SETFOCUS :
               iFocus = i ;
               break ;
          }
     return CallWindowProc (fnOldScr[i], hWnd, iMsg, wParam, lParam) ;
     }
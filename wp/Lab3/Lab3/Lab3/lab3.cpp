#include <windows.h>
#include <windowsx.h>

#define ID_PEN 200
#define ID_LINE 300
#define ID_POLYGON 400
#define ID_ELLIPSE 500
#define ID_BEZIER 600
#define ID_ERASE 700

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void updateColorPreview(HDC, COLORREF, int, int);
int getWeight(HWND);
POINT ScreenLimits(int, int, RECT, int);
HRGN LastRectangle(RECT, int, BOOL*);
HRGN LastEllipse(RECT, int, BOOL*);

char szClassName[ ] = "My Lab 3";
HINSTANCE hInstance;

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = 0;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = CreateSolidBrush(RGB( 140, 202, 167)); 
	
	if(!RegisterClassEx(&wincl)) return 0;

    hwnd = CreateWindowEx ( 0, szClassName, "My Lab 3",
        (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX),
        CW_USEDEFAULT, CW_USEDEFAULT,
        1050, 770,
        HWND_DESKTOP, NULL, hThisInstance, NULL);

    ShowWindow (hwnd, nCmdShow);

    while (GetMessage (&messages, NULL, 0, 0)) {
      TranslateMessage(&messages);
      DispatchMessage(&messages);
    }
	return messages.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    static HWND hwndEraser, hwndThickLine, hwndPen, hwndLines, hwndPolygon, hwndEllipse, hwndBeziers, hwndErased, hwndifFilled;
	static POINT pen, newLine, BezierPoints[4] ;
    static BOOL LineFirst, PolygonFirst, EllipseFirst, BezierFirst, BezierSecond, BezierThird, DeletedRegion;
    static RECT newPolygon, newEllipse;
    static HRGN lastRegion;
    
    int MouseOnX, MouseOnY;

	static RECT drawingArea = {5, 180, 795, 728};

    RECT rectRED = {390, 55, 530, 75};
    RECT rectGREEN = {390, 95, 530, 115};
    RECT rectBLUE = {390, 135, 530, 155};
    RECT rectTemp;

    HDC hdc = GetDC(hwnd);
    COLORREF OurLineRGB;
    int ThickLine;
    COLORREF fillRGB;
    HBRUSH fillBrush;
    PAINTSTRUCT ps;
    HPEN OurLinePen;
    POINT point;
    RECT rect;

	// Our RGB on screen, and our preview boxes on screen//

    HBRUSH hBrush;
    int xFillPreview = 320 ;
    int yFillPreview = 30;
    int xOurLinePreview = 320;
    int yOurLinePreview = 80;
    UINT fillRED = 255;
    UINT fillGREEN = 255;
    UINT fillBLUE = 255;
    UINT OurLineRED = 0;
    UINT OurLineGREEN = 0;
    UINT OurLineBLUE = 0;

    HDC hdcMem;
    BITMAP bitmap;
    HBITMAP hbmpimage = NULL;
    hbmpimage = (HBITMAP)LoadImage(hInstance, "anime.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmpimage, sizeof(bitmap), &bitmap);

    switch(message) {

            case WM_LBUTTONDOWN:
            MouseOnX = GET_X_LPARAM(lParam);
            MouseOnY = GET_Y_LPARAM(lParam);

			// Set in preview- our new RGB color
            if((MouseOnX > rectRED.left)&&(MouseOnX <= rectRED.right)) {
                OurLineRGB = GetPixel(hdc, xOurLinePreview + 20, yOurLinePreview + 20);

                if((MouseOnY > rectRED.top)&&(MouseOnY <= rectRED.bottom)) {
                    OurLineRED = (MouseOnX - rectRED.left) * 255 / (rectRED.right - rectRED.left);
                    OurLineGREEN = GetGValue(OurLineRGB);
                    OurLineBLUE = GetBValue(OurLineRGB);
                    updateColorPreview(hdc, RGB(OurLineRED, OurLineGREEN, OurLineBLUE), xOurLinePreview, yOurLinePreview);
                }

                else if((MouseOnY > rectGREEN.top)&&(MouseOnY <= rectGREEN.bottom)) {
                    OurLineRED = GetRValue(OurLineRGB);
                    OurLineGREEN = (MouseOnX - rectGREEN.left) * 255 / (rectGREEN.right - rectGREEN.left);
                    OurLineBLUE = GetBValue(OurLineRGB);
                    updateColorPreview(hdc, RGB(OurLineRED, OurLineGREEN, OurLineBLUE), xOurLinePreview, yOurLinePreview);
                }

                else if((MouseOnY > rectBLUE.top)&&(MouseOnY <= rectBLUE.bottom)) {
                    OurLineRED = GetRValue(OurLineRGB);
                    OurLineGREEN = GetGValue(OurLineRGB);
                    OurLineBLUE = (MouseOnX - rectBLUE.left) * 255 / (rectBLUE.right - rectBLUE.left);
                    updateColorPreview(hdc, RGB(OurLineRED, OurLineGREEN, OurLineBLUE), xOurLinePreview, yOurLinePreview);
                }
                return 0;
            }

                if((MouseOnX > drawingArea.left)&&(MouseOnX < drawingArea.right)&&(MouseOnY > drawingArea.top)&&(MouseOnY < drawingArea.bottom)) {
                ThickLine = getWeight(hwndThickLine);
				point = ScreenLimits(MouseOnX, MouseOnY, drawingArea, ThickLine);
                MouseOnX = point.x;
                MouseOnY = point.y;

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndPen) == BST_CHECKED)) {
                    pen.x = MouseOnX;
                    pen.y = MouseOnY;
                }

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndLines) == BST_CHECKED)) {
                    newLine.x = MouseOnX;
                    newLine.y = MouseOnY;
                    LineFirst = true;
                }

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndPolygon) == BST_CHECKED)) {
                    newPolygon.left = MouseOnX;
                    newPolygon.top = MouseOnY;
                    PolygonFirst = true;
                }

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndEllipse) == BST_CHECKED)) {
                    newEllipse.left = MouseOnX;
                    newEllipse.top = MouseOnY;
                    EllipseFirst = true;
                }

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndBeziers) == BST_CHECKED)) {
                    BezierPoints[0] = point;
                    BezierFirst = true;
                    BezierSecond = false;
                    BezierThird = false;
                }
            }
            return 0;

        case WM_LBUTTONUP:

            MouseOnX = GET_X_LPARAM(lParam);
            MouseOnY = GET_Y_LPARAM(lParam);

            OurLineRGB = GetPixel(hdc, xOurLinePreview + 20, yOurLinePreview + 20);
            fillRGB = GetPixel(hdc, xFillPreview + 20, yFillPreview + 20);
            ThickLine = getWeight(hwndThickLine);

            if(LineFirst) {
                point = ScreenLimits(MouseOnX, MouseOnY, drawingArea, ThickLine);
                MouseOnX = point.x;
                MouseOnY = point.y;

                OurLinePen = CreatePen(PS_SOLID, ThickLine, OurLineRGB);
                SelectObject(hdc, OurLinePen);
                MoveToEx(hdc, MouseOnX, MouseOnY, NULL);
                LineTo(hdc, newLine.x, newLine.y);
                DeleteObject(OurLinePen);

                LineFirst = false;
            }

            if(PolygonFirst) {
                point = ScreenLimits(MouseOnX, MouseOnY, drawingArea, ThickLine);
                newPolygon.right = point.x;
                newPolygon.bottom = point.y;

                OurLinePen = CreatePen(PS_SOLID, ThickLine, OurLineRGB);
                fillBrush = (Button_GetCheck(hwndifFilled) == BST_CHECKED)? CreateSolidBrush(fillRGB) : (HBRUSH)GetStockObject(NULL_BRUSH);
                SelectObject(hdc, OurLinePen);
                SelectObject(hdc, fillBrush);
                Rectangle(hdc, newPolygon.left, newPolygon.top, newPolygon.right, newPolygon.bottom);
                DeleteObject(OurLinePen);
                DeleteObject(fillBrush);

                lastRegion = LastRectangle(newPolygon, ThickLine, &DeletedRegion);

                PolygonFirst = false;
            }

            if(EllipseFirst) {
                point = ScreenLimits(MouseOnX, MouseOnY, drawingArea, ThickLine);
                newEllipse.right = point.x;
                newEllipse.bottom = point.y;

                OurLinePen = CreatePen(PS_SOLID, ThickLine, OurLineRGB);
                fillBrush = (Button_GetCheck(hwndifFilled) == BST_CHECKED)? CreateSolidBrush(fillRGB) : (HBRUSH)GetStockObject(NULL_BRUSH);
                SelectObject(hdc, OurLinePen);
                SelectObject(hdc, fillBrush);
                Ellipse(hdc, newEllipse.left, newEllipse.top, newEllipse.right, newEllipse.bottom);
                DeleteObject(OurLinePen);
                DeleteObject(fillBrush);

                lastRegion = LastEllipse(newEllipse, ThickLine, &DeletedRegion);

                EllipseFirst = false;
            }

            if(BezierFirst) {
                point = ScreenLimits(MouseOnX, MouseOnY, drawingArea, ThickLine);
                BezierPoints[1] = point;
                BezierFirst = false;
                BezierSecond = true;
                BezierThird = false;
            }
            return 0;

        case WM_RBUTTONDOWN:
            MouseOnX = GET_X_LPARAM(lParam);
            MouseOnY = GET_Y_LPARAM(lParam);

            if((MouseOnX > rectRED.left)&&(MouseOnX <= rectRED.right)) {
                fillRGB = GetPixel(hdc, xFillPreview + 20, yFillPreview + 20);

                if((MouseOnY > rectRED.top)&&(MouseOnY <= rectRED.bottom)) {
                    fillRED = (MouseOnX - rectRED.left) * 255 / (rectRED.right - rectRED.left);
                    fillGREEN = GetGValue(fillRGB);
                    fillBLUE = GetBValue(fillRGB);
                    updateColorPreview(hdc, RGB(fillRED, fillGREEN, fillBLUE), xFillPreview, yFillPreview);
                }

                else if((MouseOnY > rectGREEN.top)&&(MouseOnY <= rectGREEN.bottom)) {
                    fillRED = GetRValue(fillRGB);
                    fillGREEN = (MouseOnX - rectGREEN.left) * 255 / (rectGREEN.right - rectGREEN.left);
                    fillBLUE = GetBValue(fillRGB);
                    updateColorPreview(hdc, RGB(fillRED, fillGREEN, fillBLUE), xFillPreview, yFillPreview);
                }

                else if((MouseOnY > rectBLUE.top)&&(MouseOnY <= rectBLUE.bottom)) {
                    fillRED = GetRValue(fillRGB);
                    fillGREEN = GetGValue(fillRGB);
                    fillBLUE = (MouseOnX - rectBLUE.left) * 255 / (rectBLUE.right - rectBLUE.left);
                    updateColorPreview(hdc, RGB(fillRED, fillGREEN, fillBLUE), xFillPreview, yFillPreview);
                }
                return 0;
            }

            if((MouseOnX > drawingArea.left)&&(MouseOnX < drawingArea.right)&&(MouseOnY > drawingArea.top)&&(MouseOnY < drawingArea.bottom)) {
                ThickLine = getWeight(hwndThickLine);
                point = ScreenLimits(MouseOnX, MouseOnY, drawingArea, ThickLine);
                MouseOnX = point.x;
                MouseOnY = point.y;

                if((wParam == MK_RBUTTON)&&(Button_GetCheck(hwndBeziers) == BST_CHECKED)&&(BezierSecond)) {
                    BezierPoints[2] = point;
                    BezierFirst = false;
                    BezierSecond = false;
                    BezierThird = true;
                }
            }
            if(DeletedRegion) {
                InvalidateRgn(hwnd, lastRegion, TRUE);
                DeleteObject(lastRegion);
                DeletedRegion = false;
            }
            return 0;

        case WM_RBUTTONUP:
            MouseOnX = GET_X_LPARAM(lParam);
            MouseOnY = GET_Y_LPARAM(lParam);
            OurLineRGB = GetPixel(hdc, xOurLinePreview + 20, yOurLinePreview + 20);
            fillRGB = GetPixel(hdc, xFillPreview + 20, yFillPreview + 20);
            ThickLine = getWeight(hwndThickLine);
            point = ScreenLimits(MouseOnX, MouseOnY, drawingArea, ThickLine);
            MouseOnX = point.x;
            MouseOnY = point.y;

            if(BezierThird) {
                BezierPoints[3] = point;
                OurLinePen = CreatePen(PS_SOLID, ThickLine, OurLineRGB);
                SelectObject(hdc, OurLinePen);
                PolyBezier(hdc, BezierPoints, 4);
                DeleteObject(OurLinePen);
                BezierFirst = false;
                BezierSecond = false;
                BezierThird = false;
            }
            return 0;

        case WM_MOUSEMOVE:
            MouseOnX = GET_X_LPARAM(lParam);
            MouseOnY = GET_Y_LPARAM(lParam);

            if((MouseOnX > drawingArea.left)&&(MouseOnX < drawingArea.right)
                &&(MouseOnY > drawingArea.top)&&(MouseOnY < drawingArea.bottom)) {
                OurLineRGB = GetPixel(hdc, xOurLinePreview + 20, yOurLinePreview + 20);
                fillRGB = GetPixel(hdc, xOurLinePreview + 20, yOurLinePreview + 20);

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndPen) == BST_CHECKED)) {
                    OurLinePen = CreatePen(PS_SOLID, 1, OurLineRGB);
                    SelectObject(hdc, OurLinePen);
                    MoveToEx(hdc, MouseOnX, MouseOnY, NULL);
                    LineTo(hdc, pen.x, pen.y);
                    DeleteObject(OurLinePen);
                    pen.x = MouseOnX;
                    pen.y = MouseOnY;
                }

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndErased) == BST_CHECKED)) {
                    ThickLine = getWeight(hwndEraser);
                    point = ScreenLimits(MouseOnX, MouseOnY, drawingArea, ThickLine);
                    MouseOnX = point.x;
                    MouseOnY = point.y;
                    rect.left = point.x - (ThickLine/2);
                    rect.right = point.x + (ThickLine/2);
                    rect.top = point.y - (ThickLine/2);
                    rect.bottom = point.y + (ThickLine/2);
                    InvalidateRect(hwnd, &rect, FALSE);
                    SendMessage(hwnd, WM_PAINT, 0, 0);
                    ValidateRect(hwnd, &rect);
                }
            }
            return 0;

        case WM_CREATE:

			// Create everything on screen(all text)//
            
			CreateWindowEx( 0, "Static", "Fill",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                270, 45, 45, 20,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx( 0, "Static", "Line",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                270, 95, 45, 20,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx( 0, "Static", "Red",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                390, 35, 45, 17,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx( 0, "Static", "Green",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                390, 75, 45, 17,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx( 0, "Static", "Blue",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                390, 115, 45, 17,
                hwnd, (HMENU)0, hInstance, NULL);

            hwndPen = CreateWindowEx( 0, "Button", "Pencil",
                WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON,
                170, 40, 80, 20,
                hwnd, (HMENU)ID_PEN, hInstance, NULL);

            Button_SetCheck(hwndPen, BST_CHECKED);

            hwndLines = CreateWindowEx( 0, "Button", "Line",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                170, 80, 80, 20,
                hwnd, (HMENU)ID_LINE, hInstance, NULL);

            hwndPolygon = CreateWindowEx( 0, "Button", "Polygon",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                170, 100, 80, 20,
                hwnd, (HMENU)ID_POLYGON, hInstance, NULL);

            hwndEllipse = CreateWindowEx( 0, "Button", "Ellipse",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                170, 120, 80, 20,
                hwnd, (HMENU)ID_ELLIPSE, hInstance, NULL);

            hwndBeziers = CreateWindowEx( 0, "Button", "Bezier",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                170, 60, 80, 20,
                hwnd, (HMENU)ID_BEZIER, hInstance, NULL);

            hwndifFilled = CreateWindowEx( 0, "Button", "Fill",
                WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                30, 50, 80, 20,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx( 0, "Static", "OurLine",
                WS_VISIBLE | WS_CHILD,
                30, 70, 80, 20,
                hwnd, (HMENU)0, hInstance, NULL);

            hwndThickLine = CreateWindowEx( 0, "Edit", "1",
                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                120, 70, 20, 20,
                hwnd, (HMENU)0, hInstance, NULL);

            hwndErased = CreateWindowEx( 0, "Button", "Eraser",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 90, 80, 20,
                hwnd, (HMENU)ID_ERASE, hInstance, NULL);

            hwndEraser = CreateWindowEx( 0, "Edit", "1",
                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                120, 90, 20, 20,
                hwnd, (HMENU)0, hInstance, NULL);
            return 0;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                default:
                    DefWindowProc(hwnd, WM_COMMAND, wParam, lParam);
                    break;
            }
            return 0;
		case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            updateColorPreview(hdc, RGB(255, 255, 255), xFillPreview, yFillPreview);

            updateColorPreview(hdc, RGB(0, 0, 0), xOurLinePreview, yOurLinePreview);

            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hbmpimage);
            BitBlt(hdc, 800, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);

            rectTemp.top = rectRED.top;
            rectTemp.bottom = rectRED.bottom;
            for(int i = 0; i < (rectRED.right - rectRED.left); i++) {
                int r;
                r = i * 255 / (rectRED.right - rectRED.left);
                rectTemp.left = rectRED.left + i;
                rectTemp.right = rectRED.left + i + 1;
                hBrush = CreateSolidBrush(RGB(r, 0, 0));
                FillRect(hdc, &rectTemp, hBrush);
                DeleteObject(hBrush);
            }

            rectTemp.top = rectGREEN.top;
            rectTemp.bottom = rectGREEN.bottom;
            for(int i = 0; i < (rectGREEN.right - rectGREEN.left); i++) {
                int g;
                g = i * 255 / (rectGREEN.right - rectGREEN.left);
                rectTemp.left = rectGREEN.left + i;
                rectTemp.right = rectGREEN.left + i + 1;
                hBrush = CreateSolidBrush(RGB(0, g, 0));
                FillRect(hdc, &rectTemp, hBrush);
                DeleteObject(hBrush);
            }

            rectTemp.top = rectBLUE.top;
            rectTemp.bottom = rectBLUE.bottom;
            for(int i = 0; i < (rectBLUE.right - rectBLUE.left); i++) {
                int b;
                b = i * 255 / (rectBLUE.right - rectBLUE.left);
                rectTemp.left = rectBLUE.left + i;
                rectTemp.right = rectBLUE.left + i + 1;
                hBrush = CreateSolidBrush(RGB(0, 0, b));
                FillRect(hdc, &rectTemp, hBrush);
                DeleteObject(hBrush);
            }

            SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0,0,0)));
            SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            Rectangle(hdc, drawingArea.left, drawingArea.top, drawingArea.right, drawingArea.bottom);

            EndPaint(hwnd, &ps);
            return 0;

        case WM_DESTROY:
            PostQuitMessage (0);
            return 0;

        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

POINT ScreenLimits(int MouseOnX, int MouseOnY, RECT limit, int OurLine) {
    POINT result;
    OurLine = OurLine / 2 + 1;

    if(MouseOnX - OurLine < limit.left) {
        result.x = limit.left + OurLine;
    } else if(MouseOnX + OurLine > limit.right) {
        result.x = limit.right - OurLine;
    } else result.x = MouseOnX;

    if(MouseOnY - OurLine < limit.top) {
        result.y = limit.top + OurLine;
    } else if(MouseOnY + OurLine > limit.bottom) {
        result.y = limit.bottom - OurLine;
    } else result.y = MouseOnY;

    return result;
}

void updateColorPreview(HDC hdc, COLORREF rgb, int xLeft, int yTop) {
    HBRUSH hBrush = CreateSolidBrush(rgb);
    SelectObject(hdc, hBrush);
    Rectangle(hdc, xLeft, yTop, xLeft + 60, yTop + 35);
    DeleteObject(hBrush);
}

HRGN LastEllipse(RECT rect, int OurLine, BOOL* flag) {
    HRGN result = CreateEllipticRgn(
        rect.left - (OurLine / 2) - 1,
        rect.top - (OurLine / 2) - 1,
        rect.right + (OurLine / 2) + 1,
        rect.bottom + (OurLine / 2) + 1);
    *flag = TRUE;
    return result;
}

HRGN LastRectangle(RECT rect, int OurLine, BOOL* flag) {
    HRGN result = CreateRectRgn(
        rect.left - (OurLine / 2) - 1,
        rect.top - (OurLine / 2) - 1,
        rect.right + (OurLine / 2) + 1,
        rect.bottom + (OurLine / 2) + 1);
    *flag = TRUE;
    return result;
}

int getWeight(HWND input) {
    int result;
    int iLength = SendMessage(input, WM_GETTEXTLENGTH, 0, 0);
    char* szText = (char*)malloc(iLength+1);
    SendMessage(input, WM_GETTEXT, iLength+1, (LPARAM)szText);
    result = atoi(szText);
    _itoa(result, szText, 10);
    SendMessage(input, WM_SETTEXT, 0, (LPARAM)szText);
    free(szText);
    return result;
}
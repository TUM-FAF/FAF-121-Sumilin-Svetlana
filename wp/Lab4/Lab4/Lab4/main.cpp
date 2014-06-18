#include <iostream>
#include <windows.h>
#include <cmath>
#define ID_TIMER 022
using namespace std;

class animationCirc{
public:
    POINT center;
    int speedOnX,speedOnY;
	bool circleInteract(const RECT &rect);
	bool Accelerate(const int& speedX,const int& speedY);
    animationCirc(POINT center,const int &speed);
    bool Color(const COLORREF &colorFirst);
    
    virtual bool Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush) = 0;
protected:
    COLORREF colorFirst;
};

class Circle : public animationCirc {
public:
    Circle(POINT center,const int &speed): animationCirc(center,speed)
    {
    }
    bool Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush);
};
bool Interaction(animationCirc &first,animationCirc &second);


LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
static animationCirc *obj[100];

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = "Lab4";
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS; 
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	char szClassName[ ] = "Lab4";


    if (!RegisterClassEx (&wincl))
        return 0;

       hwnd = CreateWindowEx (
           0, szClassName, "Lab 4",
           WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
           800, 500,
           HWND_DESKTOP,
           NULL, hThisInstance, NULL
           );

    ShowWindow (hwnd, nCmdShow);
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;
}


LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc, hdcMem;
    static PAINTSTRUCT ps;
    static RECT rect;
    static HBRUSH hBrush;
    static HBITMAP hBitm;
    static HANDLE hndl;
	static int ourSpeed = 40;
    static int nmbCircles = 0;

    switch (message)
    {
          case WM_CREATE:
            hdc = GetDC(hwnd);
            GetClientRect(hwnd,&rect);
            hdcMem = CreateCompatibleDC(hdc);
            hBitm = CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
            hndl = SelectObject(hdcMem,hBitm);
            SetTimer(hwnd,ID_TIMER,ourSpeed,NULL);
            break;

        case WM_SIZE:
            SelectObject(hdcMem,hndl);
            DeleteObject(hBitm);
            DeleteDC(hdcMem);
            hdc = GetDC(hwnd);
            GetClientRect(hwnd,&rect);
            hdcMem = CreateCompatibleDC(hdc);
            hBitm = CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
            hndl = SelectObject(hdcMem,hBitm);
            break;

        case WM_LBUTTONDOWN:
			POINT ourCenter;
            ourCenter.x = LOWORD(lParam);
            ourCenter.y = HIWORD(lParam);

            obj[nmbCircles] = new Circle(ourCenter,5 + ourCenter.x%5);
            obj[nmbCircles]->Color(RGB((BYTE)(rand() % 255), (BYTE)(rand() % 255), (BYTE)(rand() % 255)));
			nmbCircles++;
            break;

        case WM_MOUSEWHEEL:
            if((short)HIWORD(wParam)<0) {
                ourSpeed+= 5;
			}else {
                ourSpeed-= 5;
            if (ourSpeed<10) ourSpeed = 15;
            }
            
            KillTimer(hwnd,ID_TIMER);
            SetTimer(hwnd,ID_TIMER,ourSpeed,NULL);
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd,&ps);
            GetClientRect(hwnd,&rect);
			//if circles interact
            for(int i = 0; i < nmbCircles-1; i++) 
			{
                for(int j = i + 1; j<nmbCircles; j++) 
				{
                    Interaction(*obj[i],*obj[j]);
                }
            }

            FillRect(hdcMem,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));
			//Redraws all 
            for(int i = 0;i<nmbCircles;i++) 
			{
              obj[i]->Move(hdcMem,rect,hBrush);
            }
			//flickering
            BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
            EndPaint(hwnd,&ps);
            break;
			
        case WM_TIMER:
			InvalidateRect(hwnd,NULL,FALSE);
            break;

        case WM_DESTROY:
            SelectObject(hdcMem,hndl);
            DeleteObject(hBitm);
            DeleteDC(hdcMem);
            KillTimer(hwnd,ID_TIMER);

            PostQuitMessage (0);
            break;

        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
//The direction on x\y different angle
animationCirc::animationCirc(POINT center,const int &speed) 
{
	this->center = center;
    this->speedOnX = speed - ((BYTE)(rand() % 20));
    this->speedOnY = speed + ((BYTE)(rand() % 20));
    colorFirst = RGB(0,0,0);
}
//The speed of circles
bool animationCirc::Accelerate(const int& speedX,const int& speedY) 
{
    speedOnX = speedX;
    speedOnY = speedY;
    return TRUE;
}
//The color forcircles
bool animationCirc::Color(const COLORREF &colorFirst)
{
    this->colorFirst = colorFirst;
    return TRUE;
}
//Redraw circle
bool Circle::Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush) 
{
    hBrush = CreateSolidBrush(colorFirst);
	SelectObject(hdc,hBrush);
    circleInteract(rect); //If interacts
    center.x+= speedOnX; //Change position
    center.y+= speedOnY;
	Ellipse(hdc,center.x - 20,center.y-20,center.x + 20,center.y + 20); // Drawing circle
	SelectObject(hdc,GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);
	 return TRUE;
}
//Changes the direction
bool animationCirc::circleInteract(const RECT &rect) 
{
    if(center.x+5 > rect.right - 20)
        { speedOnX = -abs(speedOnX); }
    if(center.x-5 < rect.left + 20)
        { speedOnX = abs(speedOnX); }
	if(center.y+5 > rect.bottom - 20)
        { speedOnY = -abs(speedOnY); }
    if(center.y-5 < rect.top + 20)
        { speedOnY = abs(speedOnY); }
	return TRUE;
}

bool Interaction(animationCirc &first,animationCirc &second) 
{
    float distance;
    POINT totalSpeed;
    distance = sqrt( pow(first.center.x-second.center.x,2)+ pow(first.center.y-second.center.y,2) );
   
    if ( distance < 42 && distance > 30) {
	first.Color(RGB((BYTE)(rand() % 255), (BYTE)(rand() % 255), (BYTE)(rand() % 255)));
	second.Color(RGB(0,0,0));
    totalSpeed.x = (abs(first.speedOnX) + abs(second.speedOnX))/2;
    totalSpeed.y = (abs(first.speedOnY) + abs(second.speedOnY))/2;
	first.Accelerate(-first.speedOnX+1,-first.speedOnY-1);
    second.Accelerate(-second.speedOnX-1,-second.speedOnY+1);
    }
	return TRUE;
}
#define _CRT_SECURE_NO_WARNINGS
#define ANSI_COLOR_RED 
#define ID_START 3000
#define ID_STOP 3001
#define EDIT 3002
#define ID_STAT_WND 3006
#define TB_HWND 3007
#include <windows.h>
#include "resource.h"
#include <string> 
#include <time.h>
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <cctype>
#include  <commctrl.h>
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
# pragma comment (lib,"comctl32.lib")
using namespace std;

HINSTANCE hInst;   
HWND hWnd0, hWnd1, hWnd2, hWnd3;  
TCHAR name0[] = "Win0";
TCHAR name1[] = "Win1";
TCHAR name2[] = "Win2";
TCHAR name3[] = "Win3";
TCHAR name4[] = "Win4";

int count_right = 2;
int cnt = 0;

BOOL CALLBACK DlgProc6(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL NewClass(WNDPROC Proc, TCHAR szName[], UINT brBackground, UINT icon, UINT cursor, UINT menu);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc1(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2);
BOOL CALLBACK DlgProc1(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Avtorization(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgRules(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcF(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgTeacher(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK res(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class gamer {
    char login[20];
    char pass[20];
public:
    int cw = 0;
    char* getLog() {
        return login;
    }
    char* getPas() {
        return pass;
    }
    gamer(char l[], char p[]) {
        strcpy_s(login, l);
        strcpy_s(pass, p);
        cw = 0;
    }
    gamer() {
        strcpy_s(login, "\0");
        strcpy_s(pass, "\0");
        cw = 0;
    }
 
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    PlaySound(MAKEINTRESOURCE(IDR_WAVE1), 0, SND_RESOURCE | SND_ASYNC | SND_MEMORY | SND_LOOP);
    srand(time(NULL));
    hInst = hInstance;         


    if (!NewClass(WndProc, name0, COLOR_WINDOW, ICON, IDC_CURSOR1, IDR_MENU1)) return FALSE;
    if (!NewClass(WndProc1, name1, COLOR_WINDOW, ICON, IDC_CURSOR1, NULL)) return FALSE; 
    if (!NewClass(res, name4, COLOR_WINDOW, ICON, IDC_CURSOR1, NULL)) return FALSE;  
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, Avtorization);
    HACCEL hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR2));

    if (hAccel == NULL) {
        MessageBox(NULL, "Accelerator Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    MSG msg;                                //  Ц О П - цикл обробки повідомлень
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(hWnd0, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)         
{

    static HWND hTb;
    static TBBUTTON but[2];
    but[0].iBitmap = STD_FIND;
    but[0].fsStyle = TBSTYLE_BUTTON;
    but[0].fsState = TBSTATE_ENABLED;
    but[0].idCommand = TEACHER;

    but[1].iBitmap = STD_HELP;
    but[1].fsStyle = TBSTYLE_BUTTON;
    but[1].fsState = TBSTATE_ENABLED;
    but[1].idCommand = AUTHOR;

    static HWND hSt;
    static OPENFILENAME ofn;  // panel
    static char FullName[512];
    static char FileName[512];
    static char Filter[256] = "Results\0*.txt\0Все форматы\0*.*\0";

    static int x = 0;
    static int speed = 10;
    HWND but1, but2;

    switch (msg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case EXIT:
            if (MessageBox(hWnd, "Ви впевнені?", "Вихід", MB_YESNO | MB_ICONQUESTION) == IDYES)     //   MB_YESNO ( buttons 2) 
                PostQuitMessage(0);
            break;
        case AUTHOR:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc1);
            //WinExec((LPCSTR)"Inet.bat", SW_SHOW);
            break;
        case RULES:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, DlgRules);
            break;
        case WM_NOTIFY:
            LPTOOLTIPTEXT Tstr;
            Tstr = (LPTOOLTIPTEXT)lParam;
            if (Tstr->hdr.code != TTN_NEEDTEXT) return 0;
            switch (Tstr->hdr.idFrom)
            {
            case TEACHER:
                strcpy(Tstr->lpszText, "Перегляд інформації про завдання");
                break;
            case AUTHOR:
                SendMessage(hSt, SB_SETTEXT, 0, (LPARAM)"Інформація про автора");
                strcpy(Tstr->lpszText, "Перегляд інформації про автора додатку");
                break;
            }
            break;
        case START:case ID_START:
            cnt = 0;
            count_right = 2;
            if (!IsWindow(hWnd1))
                hWnd1 = CreateWindow("Win1", "Shibenicha", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME | WS_VISIBLE,
                    100, 100, 800, 800, NULL, NULL, hInst, NULL);
            break;
        case TEACHER:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG5), hWnd, DlgTeacher);
            break;
        case RESULTS:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6), hWnd, DlgProc6);
            break;
        }
        break;
    case WM_TIMER:
        RECT r;
        GetClientRect(hWnd, &r);
        x++;

        if (x >= r.right) x = 0;
        RECT r1;
        r1.left = 0, r1.right = r.right, r1.top = 60, r1.bottom = 130;      // перемалювуємо кусочок з рухом тексту
        InvalidateRect(hWnd, &r1, FALSE);
        break;
    case WM_CREATE:
    {
        hTb = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_DLGFRAME | TBS_TOOLTIPS,
            TB_HWND, 0, HINST_COMMCTRL, IDB_STD_SMALL_COLOR,
            but, 2, 0, 0, 0, 0, sizeof(TBBUTTON));

        static int prt[2];
        hSt = CreateStatusWindow(WS_CHILD | WS_VISIBLE, "Гра Шибениця, головне вікно", hWnd, ID_STAT_WND);
        SendMessage(hSt, SB_SETPARTS, 3, (LPARAM)prt);
        int w = LOWORD(lParam);
        prt[0] = w / 3; prt[1] = w * 2 / 3; prt[2] = w;
        SendMessage(hSt, SB_SETPARTS, 3, (LPARAM)prt);
        but1 = CreateWindow("button", "START", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            130, 200, 100, 50, hWnd, (HMENU)ID_START, hInst, NULL);
        but2 = CreateWindow("button", "STOP", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            250, 200, 100, 50, hWnd, (HMENU)ID_STOP, hInst, NULL);
        SetTimer(hWnd, 2, speed, NULL);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT r;
        GetClientRect(hWnd, &r);

        HANDLE hBitmap = LoadImage(NULL,      
            "my_fon.bmp",   
            IMAGE_BITMAP,   
            0, 0,        
            LR_LOADFROMFILE);  
        BITMAP bitmap;
        GetObject(hBitmap, sizeof(BITMAP), &bitmap);  
        HDC hCompDC = CreateCompatibleDC(hdc);  
        SelectObject(hCompDC, hBitmap);  
        StretchBlt(hdc, r.left, r.top, r.right, r.bottom,             
            hCompDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight,   
            SRCCOPY);                      
        DeleteObject(hBitmap);  
        DeleteDC(hCompDC);    
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 250, 255));
        SetTextAlign(hdc, TA_CENTER);
        LOGFONT lf;
        lf.lfHeight = 36;
        lf.lfEscapement = lf.lfStrikeOut = lf.lfUnderline = lf.lfOrientation = 0;
        HFONT hf = CreateFontIndirect(&lf);
        SelectObject(hdc, hf);
        TextOut(hdc, r.right / 2, 300, "Давайте пограємо у Шибеницю!", strlen("Давайте пограємо у Шибеницю!"));  
        TextOut(hdc, x + 100, 100, "Вітаю!", strlen("Вітаю!"));
        DeleteObject(hf);

        EndPaint(hWnd, &ps);  
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);                 
        break;
        KillTimer(hWnd, 1);
        break;
        break;
    case EXIT:
    {
        KillTimer(hWnd, 2);
        if (MessageBox(hWnd, "Ви впевнені у своєму виборі ?", "Вихід", MB_YESNOCANCEL | MB_ICONSTOP) == IDYES)
            PostQuitMessage(0);

        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
        break;
    }
    return 0;
}


BOOL NewClass(WNDPROC Proc, TCHAR szName[], UINT brBackground, UINT icon, UINT cursor, UINT menu)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Proc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon(hInst, (LPCSTR)icon);
    wcex.hIconSm = LoadIcon(hInst, (LPCSTR)icon);
    wcex.hCursor = LoadCursor(hInst, (LPCSTR)cursor);
    wcex.hbrBackground = (HBRUSH)(brBackground + 1);
    wcex.lpszMenuName = (LPCSTR)menu;
    wcex.lpszClassName = szName;

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, ("Call to RegisterClassEx failed!"), szName, NULL);
        return 0;
    }
    else return 1;
}

LRESULT CALLBACK res(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        return 1;
    }
}

BOOL CALLBACK DlgProc1(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:           
        return 0;
    case WM_COMMAND:			
        switch (wParam)
        {
        case MYOK:
            if (!IsWindow(hWnd0))
                hWnd0 = CreateWindow("Win0", "Shibenicha", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                    200, 200, 500, 400, NULL, NULL, hInst, NULL);
            if (!hWnd0)                             
            {
                MessageBox(NULL, "Window not created!", "ERROR...", NULL);
                return 1;
            }
            EndDialog(hdWnd, NULL);
            return 1;


        }
        return 1;
    case WM_CLOSE:
        EndDialog(hdWnd, NULL);
        return 1;
    }
    return 0;

}
BOOL CALLBACK DlgTeacher(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        EndDialog(hdWnd, 0);
        return 1;
    }
    return 0;

}
BOOL CALLBACK Dlgloose(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    switch (msg)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case CLS:
            //MessageBox(NULL, "Zahodim", "", NULL);
            EndDialog(hdWnd, 0);
            return 1;
        case CLS2:
            PostQuitMessage(0);
            break;
        }
        return 1;

    case WM_CLOSE:
        EndDialog(hdWnd, 0);
        return 1;
    }
    return 0;

}
BOOL CALLBACK DlgRules(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //CloseWindow(hWnd1);
    switch (msg)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case VPERED:
            if (!IsWindow(hWnd1))
                hWnd1 = CreateWindow("Win1", "Shibenicha", WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
                    100, 100, 900, 900, NULL, NULL, hInst, NULL);
            //ShowWindow(hWnd1);
            return 1;
        }
        EndDialog(hdWnd, 0);
        return 1;
    case WM_CLOSE:
        EndDialog(hdWnd, 0);
        return 1;

    }
    return 0;

}

char bufl[20], bufp[20];
BOOL CALLBACK Avtorization(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hlog, hpas;
    switch (msg)
    {
    case WM_INITDIALOG:           // ініціалізація елементів керування
        hlog = GetDlgItem(hdWnd, LOGIN);
        hpas = GetDlgItem(hdWnd, PSW);
        return false;
    case WM_CLOSE:
        EndDialog(hdWnd, NULL);
        PostQuitMessage(0);
        return true;
    case WM_COMMAND:      // Обробка елементів керування
        switch (wParam)
        {
        case B_REG:
            GetWindowText(hlog, bufl, 20);
            GetWindowText(hpas, bufp, 20);
            if (strlen(bufl) != 0 && strlen(bufp) != 0) {
                gamer g(bufl, bufp);
                ofstream f("MyResults.txt", ios::app);
                f.write((char*)&g, sizeof(gamer));
                f.close();
                MessageBox(NULL, "Ви зареєстровані\nГРАЙТЕ!!!!", "", MB_OK);
            }
            return true;
        case YES: {

            GetWindowText(hlog, bufl, 20);
            GetWindowText(hpas, bufp, 20);
            ifstream f("MyResults.txt");
            gamer g;
            bool b = false;
            while (true) {
                f.read((char*)&g, sizeof(gamer));
                if (f.eof()) break;
                if (strcmp(g.getLog(), bufl) == 0 && strcmp(g.getPas(), bufp) == 0) {
                    MessageBox(NULL, "Авторизацію проййдено!\nУРА!!!!", "", MB_OK);
                    EndDialog(hdWnd, NULL);
                    b = true;
                    hWnd0 = CreateWindow(name0, "Shibenicha", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME | WS_VISIBLE,
                        200, 200, 500, 450, NULL, NULL, hInst, NULL);
                    // 200 - 200 ( location )   500  400 ( size)            h - inst ( dodatok)
                    if (!hWnd0)                             // перевірка правильності створення вікна
                    {
                        MessageBox(NULL, "Window not created!", "ERROR...", NULL);
                        return 1;
                    }
                    break;
                }
            }
            f.close();

            if (b == false) {
                MessageBox(NULL, "Авторизацію HE проййдено!\nERROR!!!!", "", MB_OK);
                PostQuitMessage(0);
            }
        }
                return 1;
        case CLEAR:
            SetWindowText(hlog, "\0");
            SetWindowText(hpas, "\0");
            return true;
        }
        return 1;
    }
    return 0;
}

LRESULT CALLBACK WndProc1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HBRUSH brush = CreateSolidBrush(RGB(0, 120, 230));
    SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)brush);
    static HWND field;
    static COLORREF colText = RGB(125, 125, 125), colText1 = RGB(0, 0, 0);
    static CHOOSEFONT cf;
    static LOGFONT lf;
    // static HFONT hf;
    lf.lfCharSet = RUSSIAN_CHARSET;
    lf.lfHeight = 32;
    lf.lfOrientation = lf.lfStrikeOut = lf.lfEscapement = lf.lfUnderline = lf.lfItalic = 0;
    strcpy(lf.lfFaceName, "Arial");         // check lfs
    static  HMENU hMenu, hPopupMenu;
    static CHOOSECOLOR cc;
    static COLORREF defCol[16];
    static char s[2];
    static char* word = new char[30];
    static char* buf;
    static RECT r;

    char Fonts[LF_FACESIZE];

    HPEN pen = CreatePen(PS_SOLID, 5, RGB(0, 255, 255));
    int v[12][4] = { {400,50,400,625},
        {350,625,400,575},
        {450,625,400,575},
        {400,50,550,50},
        {400,100,450,50},
        {550,50,550,150},
        {525,150,575,200},
        {525,200,575,300},
        {540,200,500,250},
        {560,200,600,250},
        {540,300,520,375},
        {560,300,580,375} };

    static int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    switch (msg)
    {
    case WM_CREATE:
    {
        srand(time(NULL));
        ifstream fin("slova.txt");

        int number = rand() % 2004;
        int k = 0;
        while (true)
        {
            fin.getline(word, 30);
            if (fin.eof() || k == number)
                break;
            k++;
        }
        fin.close();
        // MessageBox(NULL, word, "", NULL);        
        buf = new char[strlen(word)];
        for (int i = 0; i < strlen(word); i++)
            buf[i] = '-';
        buf[0] = word[0];
        buf[strlen(word) - 1] = word[strlen(word) - 1];

        break;
    }
    case WM_CONTEXTMENU:
    {
        hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU2));
        hPopupMenu = GetSubMenu(hMenu, 0);
        POINT pt;
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        TrackPopupMenu(hPopupMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
        break;
    }
    case WM_COMMAND:
        switch (wParam)
        {
        case FON:
        {
            cc.lStructSize = sizeof(CHOOSECOLOR);
            cc.Flags = CC_ANYCOLOR;
            cc.lpCustColors = defCol;
            if (ChooseColor(&cc))
                colText = cc.rgbResult;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
        case SHRIFT:
        {
            cf.lStructSize = sizeof(CHOOSEFONT);
            cf.lpLogFont = &lf;
            cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
            cf.lpszStyle = (LPSTR)Fonts;
            if (ChooseFont(&cf))
                colText1 = cf.rgbColors;
            InvalidateRect(hWnd, NULL, TRUE);

        }
        break;
        }
        break;
    case WM_LBUTTONDOWN: {
        break;
    }
    case WM_CHAR:
    {
       
        bool b = false;

        s[0] = (char)wParam;
        s[0] = toupper(s[0]);
        MessageBox(NULL, s, "", NULL);
        bool flag = false;
        // check otladkoi ()

        for (int i = 1; i < strlen(word - 1); i++)
        {
            if (word[i] == s[0])
            {
                //if () && s[0] != s[i] )

                buf[i] = s[0];
                flag = true;
                count_right++;
            }
        }
        HDC hdc1 = GetDC(hWnd);
        if (flag == true)
        {
            HFONT hf = CreateFontIndirect(&lf);
            SelectObject(hdc1, hf);
            SetTextAlign(hdc1, TA_CENTER);
            SetBkColor(hdc1, GetPixel(hdc1, r.right / 2, 700));

            TextOut(hdc1, r.right / 2, 700, buf, strlen(word)); // + рухатися   
            DeleteObject(hf);
        }
        else
        {
            SelectObject(hdc1, pen);
            if (cnt == 6 || cnt == 7)
                Ellipse(hdc1, v[cnt][0], v[cnt][1], v[cnt][2], v[cnt][3]);
            else
                DrawLine(hdc1, v[cnt][0], v[cnt][1], v[cnt][2], v[cnt][3]);
            DeleteObject(pen);
            cnt++;
            if (cnt == 12)
            {
                MessageBox(NULL, word, "", NULL);
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), NULL, Dlgloose);

                DestroyWindow(hWnd1);
            }
        }
        if (count_right == strlen(word))
        {
            MessageBox(NULL, "ПЕРЕМОГА!!!", "", MB_OK);
            string path = "MyResults.txt";
            gamer g;
            ifstream fin(path);
            ofstream tmp("Tmp.txt");
            while (true)
            {
                fin.read((char*)&g, sizeof(g));
                if (fin.eof())
                    break;
                if (strcmp(g.getPas(),bufp) == 0 && strcmp(g.getLog(), bufl) == 0)
                    g.cw++;
                tmp.write((char*)&g, sizeof(g));
                
            }
            fin.close();
            tmp.close();
            remove("MyResults.txt");
            rename("Tmp.txt", "MyResults.txt");
        }
        ReleaseDC(hWnd, hdc1);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &r);
        HBRUSH hbr = CreateSolidBrush(colText);
        SelectObject(hdc, hbr);
        FillRect(hdc, &r, hbr);
        x1 = r.left + 40;
        x2 = r.right - 40;
        y1 = 50;
        y2 = r.bottom * 2 / 3;

        SetBkMode(hdc, TRANSPARENT);
        int s1 = strlen(word);
        char buf1[25];
        strcpy(buf1, "Кількість символів = ");
        char buf2[3];
        _itoa(s1, buf2, 10);
        strcat(buf1, buf2);
        char* count_w = new char[s1];
        HFONT hf = CreateFontIndirect(&lf);
        SelectObject(hdc, hf);
        char info[83] = "У вас есть 12 попыток. Нажмите правую кнопку мыши, чтобы изменить фон или шрифт";
        RECT r1;
        r1.left = 10, r1.right = 400, r1.top = 200, r1.bottom = 500;
        SetTextAlign(hdc, TA_LEFT);
        DrawText(hdc, info, strlen(info), &r1, DT_LEFT | DT_WORDBREAK);
        SetTextAlign(hdc, TA_CENTER);
        TextOut(hdc, r.right / 2, 700, buf, strlen(word));
        TextOut(hdc, r.right / 2, 1, buf1, strlen(buf1));
        //DeleteObject(hf);
        DeleteObject(hbr);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
    {
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
        break;
    }

    return 0;
}


BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
    POINT p;
    MoveToEx(hdc, x1, y1, &p);
    LineTo(hdc, x2, y2);
    return true;
}



BOOL CALLBACK DlgProc6(HWND hdWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  
    char buf[45];
    switch (msg)
    {
    case WM_INITDIALOG: {
        gamer g;
        ifstream fp("MyResults.txt");
        while (true) {
            fp.read((char*)&g, sizeof(gamer));
            if (fp.eof()) break;
            sprintf(buf, "%-20s%20s%2i", g.getLog(), g.getPas(), g.cw);
            SendDlgItemMessage(hdWnd, IDC_LIST1, LB_ADDSTRING, 0, LPARAM(buf));
        }
        fp.close();

        return 0;
    }
    case WM_CLOSE:
        EndDialog(hdWnd, 0);
        return 1;
    }
    return 0;

}
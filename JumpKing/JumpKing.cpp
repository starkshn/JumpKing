// JumpKing.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include "JumpKing.h"

#include "SceneManager.h"

#include "Core.h"
#include "Scene.h"
#include "StageScene.h"

#include "Object.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND g_hwnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK CheckGroundType(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CheckGroundRender(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JUMPKING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // Core초기화
    if (FAILED(Core::GetInstance()->Init(g_hwnd, POINT{ 1200, 900 })))
    {
        MessageBox(nullptr, L"Core 객체 초기화 실패", L"EROOR", MB_OK);

        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JUMPKING));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // hAccelTable : 단축키 정보 확인한다.

            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // Game코드 수행
            // 디자인 패턴(설계유형)
            // Single Tone
            Core::GetInstance()->Progress();
        }

    }

    return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JUMPKING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_JUMPKING);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hwnd)
   {
      return FALSE;
   }

   ShowWindow(g_hwnd, nCmdShow);
   UpdateWindow(g_hwnd);

   return TRUE;
}

POINT g_gameObjectPos = { 500 ,300 };
POINT g_gameObjectScale = { 100 , 100 };

POINT g_leftTopPos;
POINT g_rightBottomPos;

struct ObjInfo
{
    POINT _objPosInfo;
    POINT _objScaleInfo;
};

bool g_lButtonDown = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 32774 132
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case ID_GROUND_TYPE:
                DialogBox(hInst, MAKEINTRESOURCE(ID_CHECK_GROUND_TYPE), hWnd, CheckGroundType);
                break;
            case SELECT_RENDERING:
                DialogBox(hInst, MAKEINTRESOURCE(CHECK_GROUND_RENDER), hWnd, CheckGroundRender);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            // EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// =========================
// Ground Create Window Proc
// =========================
INT_PTR CALLBACK CheckGroundType(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            // 사각형, 삼각형, 선인지 Check Box확인

            if (IsDlgButtonChecked(hDlg, IDC_CHECK1))
            {
                COLLIDER_TYPE_INFO typeInfo;
                typeInfo._check = IsDlgButtonChecked(hDlg, IDC_CHECK1);
                typeInfo._type = COLLIDER_TYPE::SQUARE;

                StageScene* stagePtr = dynamic_cast<StageScene*>(SceneManager::GetInstance()->GetSceneByVec());

                stagePtr->SetGroundTypeInfo(typeInfo);  
            }

            if (IsDlgButtonChecked(hDlg, IDC_CHECK2))
            {
                COLLIDER_TYPE_INFO typeInfo;
                typeInfo._check = IsDlgButtonChecked(hDlg, IDC_CHECK1);
                typeInfo._type = COLLIDER_TYPE::TRIANGLE;

                StageScene* stagePtr = dynamic_cast<StageScene*>(SceneManager::GetInstance()->GetSceneByVec());

                stagePtr->SetGroundTypeInfo(typeInfo);
            }

            if (IsDlgButtonChecked(hDlg, IDC_CHECK3))
            {
                COLLIDER_TYPE_INFO typeInfo;
                typeInfo._check = IsDlgButtonChecked(hDlg, IDC_CHECK1);
                typeInfo._type = COLLIDER_TYPE::LINE;

                StageScene* stagePtr = dynamic_cast<StageScene*>(SceneManager::GetInstance()->GetSceneByVec());

                stagePtr->SetGroundTypeInfo(typeInfo);
            }

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// =========================
// Ground Render Check Box
// =========================
INT_PTR CALLBACK CheckGroundRender(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            // Ground Render 할것인지 안할 것인지 설정

            if (IsDlgButtonChecked(hDlg, RenderON))
            {
                bool isRender;
                isRender = IsDlgButtonChecked(hDlg, RenderON);

                SceneManager::GetInstance()->SetColliderIsRender(isRender);
            }

            if (IsDlgButtonChecked(hDlg, RenderOFF))
            {
                bool isRender;
                isRender = IsDlgButtonChecked(hDlg, RenderOFF);

                if (isRender != false)
                    isRender = false;

                SceneManager::GetInstance()->SetColliderIsRender(isRender);
            }

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
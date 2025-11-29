
// gRimApply.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "gRimApply.h"
#include "gRimApplyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CgRimApplyApp

BEGIN_MESSAGE_MAP(CgRimApplyApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CgRimApplyApp 생성

CgRimApplyApp::CgRimApplyApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	_circle = nullptr;
	//time seed로 rand 초기화
	srand((unsigned int)time(NULL));
}


// 유일한 CgRimApplyApp 개체입니다.

CgRimApplyApp theApp;


// CgRimApplyApp 초기화

BOOL CgRimApplyApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));

	CgRimApplyDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}
/// @brief _dots 에 입력된 좌표로 circle형 도트를 추가한다.
/// @param x 좌표
/// @param y 좌표
/// @return 성공/ 실패
bool CgRimApplyApp::AddDot(int x, int y, int radius)
{
	if (3 > _dots.GetCount())
	{
		int i = _dots.GetCount();
		return _dots.AddDot(x, y, radius);
	}
	return false;
}
/// @brief 입력된 정보로 _circle 생성
/// @param x 중점
/// @param y 중점
/// @param radius 반지름 
/// @param thick 선 두께
void CgRimApplyApp::CreateCircle(int x, int y, double radius, int thick = 2)
{
	DeleteCircle();
	_circle = new LineCircle(x, y, radius, thick);
}

CgRimApplyApp::~CgRimApplyApp()
{
	if (_circle)
	{
		delete _circle;
		_circle = nullptr;
	}
}
/// @brief _dots 반환
/// @return Dots 인스턴스
Dots CgRimApplyApp::GetDots()
{
	return Dots(_dots);
}
/// @brief 클릭 시 포인터가 도트 영역에 있는지 확인, 있다면 해당 인덱스 반환
/// @param x 포인터 x
/// @param y 포인터 y
/// @return 인덱스/ 없으면 -1;
int CgRimApplyApp::IsInTheDot(int x, int y)
{
	double dx = 0;
	double dy = 0;
	double distance = 0;

	for (int i = 0; i < 3; i++)
	{
		Circle dot = _dots.GetDot(i);

		dx = x - dot.GetX();
		dy = y - dot.GetY();
		distance = sqrt((dx * dx) + (dy * dy));
		if (distance < dot.GetRadius())
			return i;
	}
	return -1;
}
/// @brief 입력된 좌표의 도트가 존재하는지 확인
/// @param x 좌표
/// @param y 좌표
/// @return 존재:true / 부재:false
bool CgRimApplyApp::IsDotExist(int x, int y)
{
	for (int i = 0; i < _dots.GetCount(); i++)
	{
		int dx = x - (_dots.GetDot(i)).GetX();
		int dy = y - (_dots.GetDot(i)).GetY();
		if (dx == 0 && dy == 0)
			return true;
	}
	return false;
}
/// @brief 무작위로 도트 셋을 생성
/// @return 성공/ 실패
bool CgRimApplyApp::GenRandDots(int dotRad)
{
	// 랜덤 생성 
	int x = 0;
	int y = 0;
	bool isRegen = true;

	if (0 < _dots.GetCount())
	{
		_dots.Clear();
	}

	//삼각형 한개 무작위 생성
	for (int i = 0; i < 3; i++)
	{
		do
		{
			x = rand() % RES_WIDTH;
			y = rand() % RES_HEIGHT;

			isRegen = IsDotExist(x, y);
		} while (isRegen);
		AddDot(x, y, dotRad);
		isRegen = true;
	}
	return true;
}
/// @brief _dots의 원 반지름을 일괄 설정
/// @param radius 
void CgRimApplyApp::SetDotsRad(int radius)
{
	for (int i = 0; i < _dots.GetCount(); i++)
	{
		_dots.SetDotRad(i, (double)radius);
	}
}
/// @brief _dots 3개 점으로 외접원을 생성.
/// @return 성공/ 실패
bool CgRimApplyApp::GenCircumccl()
{
	if (3 != _dots.GetCount())
		return false; //원을 생성할 수 없다. 
	if (nullptr == _circle)
		return false; //인스턴스가 할당되지 않았다. 

	int x1 = (_dots.GetDot(0)).GetX();
	int y1 = (_dots.GetDot(0)).GetY();
	int x2 = (_dots.GetDot(1)).GetX();
	int y2 = (_dots.GetDot(1)).GetY();
	int x3 = (_dots.GetDot(2)).GetX();
	int y3 = (_dots.GetDot(2)).GetY();

	int cX = 0; 
	int cY = 0;
	double radius = 0.0;
	// 외접원의 중점 계산
	cX = (int)( (x1*x1 + y1*y1)*(y2 - y3) + (x2*x2 + y2*y2)*(y3 - y1) + (x3*x3 + y3*y3)*(y1 - y2) ) 
		/ (2 * ( x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2)) );
	cY = (int)( (x1*x1 + y1*y1)*(x3 - x2) + (x2*x2 + y2*y2)*(x1 - x3) + (x3*x3 + y3*y3)*(x2 - x1) ) 
		/ (2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) );
	radius = sqrt(((x1 - cX) * (x1 - cX)) + ((y1 - cY) * (y1 - cY)));
	
	if (80000 < radius)
		return false; // 반지름이 무한대가 나오지 않도록 예외처리

	_circle->SetCircle(cX, cY, radius);

	return true;
}
/// @brief _circle 해제 후 소거
void CgRimApplyApp::DeleteCircle()
{
	if (_circle)
	{
		delete _circle;
		_circle = nullptr;
	}
}



// gRimApplyDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "gRimApply.h"
#include "gRimApplyDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CgRimApplyDlg 대화 상자



CgRimApplyDlg::CgRimApplyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRIMAPPLY_DIALOG, pParent)
	, _editThick(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_frameBuffer.Create(RES_WIDTH, -RES_HEIGHT, DEPTH);
	//컬러테이블 설정. 256단계 그레이스케일
	static RGBQUAD rgb[256];
	for (int i = 0; i < 256; i++)
	{
		rgb[i].rgbRed = i;
		rgb[i].rgbGreen = i;
		rgb[i].rgbBlue = i;
	}
	_frameBuffer.SetColorTable(0, 256, rgb);
}
/// @brief 화면의 점을 렌더링한다.
void CgRimApplyDlg::RenderDots()
{
	//Dots를 확인하고 해당 위치에 그림.
	Dots dots = theApp._dots;
	int count = dots.GetCount();
	for (int i = 0; i < count; i++)
	{
		Circle circle = dots.GetDot(i);
		RenderCircle(circle);
	}
}
/// @brief 정원 렌더링. 
void CgRimApplyDlg::RenderLineCcl()
{
	if (nullptr == theApp._circle)
		return;

	Circle pCircle = *(theApp._circle->GetCircle()); //정원을 가져오기 전 할당여부를 확인해야한다.
	double thickness = (double)theApp._circle->GetThickness();

	pCircle.SetRadius(pCircle.GetRadius() + (thickness / 2)); //선을 위한 겹쳐그리기.
	RenderCircle(pCircle);
	pCircle.SetColor(0xFF);
	pCircle.SetRadius(pCircle.GetRadius() - thickness); //선을 위한 겹쳐그리기.
	RenderCircle(pCircle);
}
/// @brief 원을 렌더링한다.
/// @param circle 원의 정의 
/// @param thick 원주의 두께. 선 두께가 0이면 꽉찬 원/점으로 판정.
void CgRimApplyDlg::RenderCircle(Circle circle)
{
	BYTE* frame = (BYTE*)_frameBuffer.GetBits();
	int pitch = _frameBuffer.GetPitch();
	int top = 0;
	int bottom = RES_HEIGHT;
	int left = 0;
	int right = RES_WIDTH;

	if (nullptr == frame)
	{
		return;
	}
	int x = circle.GetX();
	int y = circle.GetY();
	double radius = circle.GetRadius();
	int color = circle.GetColor();

	//원의 박스영역을 구한다. 
	left = (int)(x - radius);
	right = (int)(x + radius);
	top = (int)(y - radius);
	bottom = (int)(y + radius);
	//박스영역이 프레임버퍼의 메모리 영역을 벗어나는 것 방지
	if (left < 0)
		left = 0;
	if (right >= RES_WIDTH)
		right = RES_WIDTH;
	if (top < 0)
		top = 0;
	if (bottom >= RES_HEIGHT)
		bottom = RES_HEIGHT;

	for (int j = top; j < bottom; j++)
	{
		for (int i = left; i < right; i++)
		{
			if (IsInCircle(i, j, x, y, radius))
				frame[(j * pitch) + i] = color;
		}
	}
}
/// @brief 프레임버퍼를 화면에 그린다.
void CgRimApplyDlg::DrawFrame()
{
	CClientDC dc(this);
	_frameBuffer.Draw(dc, 0, 0);
}
/// @brief 프레임버퍼를 소거
void CgRimApplyDlg::ClearFrame()
{
	BYTE* frame = (BYTE*)_frameBuffer.GetBits();
	int pitch = _frameBuffer.GetPitch();
	int height = _frameBuffer.GetHeight();

	if (!frame)
		return;

	memset(frame, 0xFF, pitch * height);
}
/// @brief 입력된 좌표가 원의 내부에 있는지 판별하는 메서드
/// @param x 입력 x좌표
/// @param y 입력 y좌표
/// @param cx 원 중점 x
/// @param cy 원 중점 y
/// @param radius 원 반지름
/// @return 원내부 true/ 원 바깥 false
bool CgRimApplyDlg::IsInCircle(int x, int y, int cx, int cy, double radius)
{
	double dX = x - cx;
	double dY = y - cy;
	double distance = sqrt((dX * dX) + (dY * dY));

	if (distance < radius)
		//if (4.0 > fabs(distance - radius))
		return true;
	else
		return false;
}
/// @brief 점 세개를 무작위로 생성하여 외접원을 생성하고 화면에 그리는 작업을 초당 2회의 주기로 10회 반복
/// @brief 스레드에서 작업
void CgRimApplyDlg::RandomCreate()
{
	for (int i = 0; i < 10; i++)
	{
		theApp.GenRandDots();
		theApp.GenCircumccl();

		PostMessage(WM_UPDATE_FRAME, 0, 0);

		Sleep(500);
	}
}

void CgRimApplyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THK, _editThick);
	DDV_MinMaxInt(pDX, _editThick, 1, 64);
}

BEGIN_MESSAGE_MAP(CgRimApplyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_INIT, &CgRimApplyDlg::OnBnClickedBtnInit)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(IDC_EDIT_THK, &CgRimApplyDlg::OnChangeEditThk)
	ON_BN_CLICKED(IDC_BTN_RNDMOV, &CgRimApplyDlg::OnBnClickedBtnRndmov)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_UPDATE_FRAME, &CgRimApplyDlg::OnUpdateFrame)
END_MESSAGE_MAP()


// CgRimApplyDlg 메시지 처리기

BOOL CgRimApplyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	BYTE* frame = (BYTE*)_frameBuffer.GetBits();
	int width = _frameBuffer.GetWidth();
	int height = _frameBuffer.GetHeight();
	int pitch = _frameBuffer.GetPitch();

	_editThick = 2;
	UpdateData(FALSE);
	// clear frameBuffer
	memset(frame, 0xFF, pitch * height);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CgRimApplyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CgRimApplyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawFrame();
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CgRimApplyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/// @brief 화면 초기화 버튼 이벤트핸들러
void CgRimApplyDlg::OnBnClickedBtnInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	BYTE* frame = (BYTE*)_frameBuffer.GetBits();
	int width = _frameBuffer.GetWidth();
	int height = _frameBuffer.GetHeight();
	int pitch = _frameBuffer.GetPitch();

	// clear frameBuffer
	memset(frame, 0xFF, pitch * height);
	theApp._dots.Clear();
	theApp.DeleteCircle();
	Invalidate(FALSE);
	//DrawFrame();
}

/// @brief L버튼 누름. 핵심 이벤트 핸들러.
/// @param nFlags 
/// @param point 
void CgRimApplyDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 클릭해서 도트를 그리고 선택하는 루틴
	TRACE(_T("%d, %d\n"), point.x, point.y);

	//클릭한 위치에 도트가 있다면 도트를 드래그 할 수 있도록 한다
	Dots dots = theApp.GetDots();
	int index = theApp.IsInTheDot((int)point.x, (int)point.y);
	if (-1 != index)
	{
		TRACE(_T("dot %d selected\n"), index);
		if (!_isAbleToDrag)
			_isAbleToDrag = true;
		_selected = index;
		CDialogEx::OnLButtonDown(nFlags, point);
		return;
	}
	bool res = theApp.AddDot((int)point.x, (int)point.y);
	// clear buffer	
	ClearFrame();	

	if (!res)
	{//도트가 3개 모였다면 정원을 활성화 하고 정원을 렌더링.
		// 도트 3개의 정보로 삼각형을 만들고 외심과 외접원을 계산한다. 
		theApp.CreateCircle(0, 0, 0.0, _editThick);
		res = theApp.GenCircumccl();
		RenderLineCcl();
	}
	//도트가 있다면 도트를 렌더링.
	RenderDots();

	Invalidate(FALSE);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CgRimApplyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (_isAbleToDrag)
	{
		// 선택한 도트를 드래그 하는 루틴
		ClearFrame();
		TRACE(_T("d(%d, %d)\n"), point.x, point.y);
		theApp._dots.SetDot(_selected, (int)point.x, (int)point.y);
		bool res = theApp.GenCircumccl();
		RenderLineCcl();
		RenderDots();
		Invalidate(FALSE);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CgRimApplyDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	_isAbleToDrag = false;
	_selected = -1;
	// 도트가 움직인 위치를 최종 결정

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CgRimApplyDlg::OnChangeEditThk()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	if (theApp._circle)
	{
		theApp._circle->SetThickness(_editThick);
		ClearFrame();
		RenderLineCcl();
		RenderDots();
	}

	Invalidate(FALSE);
}

void CgRimApplyDlg::OnBnClickedBtnRndmov()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (_thread.joinable())
	{
		_isStopThread = true;
		_thread.join();
		_isStopThread = false;
	}
	_thread = thread(&CgRimApplyDlg::RandomCreate, this);

	ClearFrame();
	if (nullptr == theApp._circle)
	{
		theApp.CreateCircle(0, 0, 0.0, _editThick);
	}
	Invalidate(FALSE);
	//RandomCreate();
}


void CgRimApplyDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (_thread.joinable())
	{
		_isStopThread = true;
		_thread.join();
	}
}

afx_msg LRESULT CgRimApplyDlg::OnUpdateFrame(WPARAM wParam, LPARAM lParam)
{
	ClearFrame();
	RenderLineCcl();
	RenderDots();
	Invalidate(FALSE);
	return 0;
}

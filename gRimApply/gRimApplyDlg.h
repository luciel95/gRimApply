
// gRimApplyDlg.h: 헤더 파일
//
#include <thread>

using namespace std;

#pragma once

#define WM_UPDATE_FRAME (WM_USER + 100)

const int RES_WIDTH = 640;
const int RES_HEIGHT = 480;
const int DEPTH = 8;

// CgRimApplyDlg 대화 상자
class CgRimApplyDlg : public CDialogEx
{
// 생성입니다.
private:
	thread _thread;
	bool _isStopThread = false;
public:
	CgRimApplyDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	void RenderDots();
	void RenderLineCcl();
	void RenderCircle(Circle circle);
	void DrawFrame();
	void ClearFrame();
	bool IsInCircle(int x, int y, int cx, int cy, double radius);
	void RandomCreate();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRIMAPPLY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	CImage _frameBuffer;
	bool _isAbleToDrag = false;
	int _selected = -1;

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int _editThick;
	int _editDotRad;

	afx_msg void OnEnChangeEditDotrad();
	afx_msg void OnBnClickedBtnInit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnChangeEditThk();
	afx_msg void OnBnClickedBtnRndmov();
	afx_msg void OnDestroy();
protected:
	afx_msg LRESULT OnUpdateFrame(WPARAM wParam, LPARAM lParam);
public:
	// 점들의 좌표를 출력.
	CStatic _staticDotPos;
};

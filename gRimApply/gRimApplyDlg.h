
// gRimApplyDlg.h: 헤더 파일
//

#pragma once

const int RES_WIDTH = 640;
const int RES_HEIGHT = 480;
const int DEPTH = 8;

// CgRimApplyDlg 대화 상자
class CgRimApplyDlg : public CDialogEx
{
// 생성입니다.
public:
	CgRimApplyDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRIMAPPLY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	CImage _frameBuffer;

// 구현입니다.
protected:
	HICON m_hIcon;
	bool _isAbleToDrag = false;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnInit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

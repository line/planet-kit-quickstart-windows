
// GroupAudioCallMFCDlg.h : header file
//

#pragma once

#include "../src/GroupAudioCallController.h"
#include "EventListener.h"

enum CONFERENCE_EVENT {
	CONFERENCE_CONNECTED = WM_USER + 1,
	CONFERENCE_DISCONNECTED,
	CONFERENCE_PARTICIPANTS_UPDATED
};

// CGroupAudioCallMFCDlg dialog
class CGroupAudioCallMFCDlg : public CDialogEx {
// Construction
public:
	CGroupAudioCallMFCDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GROUPAUDIOCALLMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClickJoin();
	afx_msg void OnClickLeave();
	afx_msg LRESULT OnConferenceConnected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConferenceDisconnected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConferenceParticipantsUpdated(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void InitializeConstants();	

private :
	std::shared_ptr<EventListener> m_pEventListener{ new EventListener{this} };
	GroupAudioCall::GroupAudioCallController m_cGroupAudioCallController{ m_pEventListener };
	
};


// GroupAudioCallMFCDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "GroupAudioCallMFC.h"
#include "GroupAudioCallMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../src/Constants.h"

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx {
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGroupAudioCallMFCDlg dialog



CGroupAudioCallMFCDlg::CGroupAudioCallMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GROUPAUDIOCALLMFC_DIALOG, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGroupAudioCallMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGroupAudioCallMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BUTTON_JOIN, &CGroupAudioCallMFCDlg::OnClickJoin)
	ON_BN_CLICKED(ID_BUTTON_LEAVE, &CGroupAudioCallMFCDlg::OnClickLeave)
	ON_MESSAGE(CONFERENCE_CONNECTED, OnConferenceConnected)
	ON_MESSAGE(CONFERENCE_DISCONNECTED, OnConferenceDisconnected)
	ON_MESSAGE(CONFERENCE_PARTICIPANTS_UPDATED, OnConferenceParticipantsUpdated)
END_MESSAGE_MAP()


// CGroupAudioCallMFCDlg message handlers

BOOL CGroupAudioCallMFCDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize constant values.
	InitializeConstants();

	// Initialize PlanetKit.
	m_cGroupAudioCallController.InitializePlanetKit();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGroupAudioCallMFCDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGroupAudioCallMFCDlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGroupAudioCallMFCDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

void CGroupAudioCallMFCDlg::OnClickJoin() {
	CString strRoomId;
	GetDlgItemText(ID_EDIT_ROOM_ID, strRoomId);

	if (strRoomId.GetLength() == 0) {
		MessageBox(L"Enter room id");

		return;
	}

	m_cGroupAudioCallController.JoinConference(strRoomId.GetBuffer());
}

void CGroupAudioCallMFCDlg::OnClickLeave() {
	m_cGroupAudioCallController.LeaveConference();
}

LRESULT CGroupAudioCallMFCDlg::OnConferenceConnected(WPARAM wParam, LPARAM lParam) {
	SetDlgItemText(ID_STATIC_STATE, L"Connected");
	SetDlgItemText(ID_STATIC_PARTICIPANT_COUNT, L"1");

	return 0;
}

LRESULT CGroupAudioCallMFCDlg::OnConferenceDisconnected(WPARAM wParam, LPARAM lParam) {
	std::wstring* pstrDisconnectReason = (std::wstring*)wParam;
	SetDlgItemText(ID_STATIC_STATE, pstrDisconnectReason->c_str());
	SetDlgItemText(ID_STATIC_PARTICIPANT_COUNT, L"0");

	delete pstrDisconnectReason;

	return 0;
}

LRESULT CGroupAudioCallMFCDlg::OnConferenceParticipantsUpdated(WPARAM wParam, LPARAM lParam) {
	size_t nTotalParticipantCount = (size_t)wParam + 1;
	SetDlgItemText(ID_STATIC_PARTICIPANT_COUNT, std::to_wstring(nTotalParticipantCount).c_str());
	
	return 0;
}

void CGroupAudioCallMFCDlg::InitializeConstants() {
	SetDlgItemText(ID_STATIC_USER_ID, GroupAudioCall::USER_ID);
}

// Copyright 2025 LINE Plus Corporation
//
// LINE Plus Corporation licenses this file to you under the Apache License,
// version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at:
//
//   https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.

// GroupVideoCallMFCDlg.cpp : implementation file
//

#include "framework.h"
#include "GroupVideoCallMFC.h"
#include "GroupVideoCallMFCDlg.h"
#include "afxdialogex.h"
#include "../PlanetKit/src/GroupVideoCallController.h"
#include "../PlanetKit/src/Constants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CGroupVideoCallMFCDlg dialog



CGroupVideoCallMFCDlg::CGroupVideoCallMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GROUPVIDEOCALLMFC_DIALOG, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CGroupVideoCallMFCDlg::~CGroupVideoCallMFCDlg() {
    if (m_pGroupVideoCallDlg != nullptr) {
        m_pGroupVideoCallDlg->DestroyWindow();
        delete m_pGroupVideoCallDlg;
        m_pGroupVideoCallDlg = nullptr;
    }
}

void CGroupVideoCallMFCDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGroupVideoCallMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BUTTON_JOIN, &CGroupVideoCallMFCDlg::OnClickJoin)
	ON_MESSAGE(CONFERENCE_CONNECTED, OnConferenceConnected)
	ON_MESSAGE(CONFERENCE_DISCONNECTED, OnConferenceDisconnected)
END_MESSAGE_MAP()


// CGroupVideoCallMFCDlg message handlers

BOOL CGroupVideoCallMFCDlg::OnInitDialog() {
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

    GroupVideoCall::GroupVideoCallController::GetInstance()->SetEventListener(m_pEventListener);
	// Initialize PlanetKit.
	GroupVideoCall::GroupVideoCallController::GetInstance()->InitializePlanetKit();

	SetDlgItemText(ID_STATIC_USER_ID, GroupVideoCall::USER_ID);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGroupVideoCallMFCDlg::OnSysCommand(UINT nID, LPARAM lParam) {
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

void CGroupVideoCallMFCDlg::OnPaint() {
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
HCURSOR CGroupVideoCallMFCDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

void CGroupVideoCallMFCDlg::OnClickJoin() {
	CString strRoomId;
	GetDlgItemText(ID_EDIT_ROOM_ID, strRoomId);

	if (strRoomId.GetLength() == 0) {
		MessageBox(L"Enter room id");

		return;
	}

	GroupVideoCall::GroupVideoCallController::GetInstance()->JoinConference(strRoomId.GetBuffer());
}


LRESULT CGroupVideoCallMFCDlg::OnConferenceConnected(WPARAM wParam, LPARAM lParam) {
	SetDlgItemText(ID_STATIC_STATE, L"Connected");

	if (m_pGroupVideoCallDlg == nullptr) {
		m_pGroupVideoCallDlg = new GroupVideoCallDlg(
			this
		);
		m_pGroupVideoCallDlg->Create(IDD_DIALOG_GROUP_VIDEO_CALL_JOIN, this);

		m_pGroupVideoCallDlg->ShowWindow(SW_SHOW);
	}

	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CGroupVideoCallMFCDlg::OnConferenceDisconnected(WPARAM wParam, LPARAM lParam) {
	ShowWindow(SW_SHOW);
	if (m_pGroupVideoCallDlg != nullptr) {
		m_pGroupVideoCallDlg->DestroyWindow();
		delete m_pGroupVideoCallDlg;
		m_pGroupVideoCallDlg = nullptr;
	}
	std::wstring* pstrDisconnectReason = (std::wstring*)wParam;
	SetDlgItemText(ID_STATIC_STATE, pstrDisconnectReason->c_str());

	delete pstrDisconnectReason;

	return 0;
}
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

#include "resource.h"
#include "GroupVideoCallDlg.h"
#include "afxdialogex.h"
#include "PlanetKitManager.h"
#include "../PlanetKit/src/GroupVideoCallController.h"

constexpr int DLG_PADDING = 10;

constexpr int PEER_MIN_WIDTH = 320;
constexpr int PEER_MIN_HEIGHT = 213;

constexpr int DLG_MIN_WIDTH = (PEER_MIN_WIDTH + DLG_PADDING) * 3 + DLG_PADDING;
constexpr int DLG_MIN_HEIGHT = (PEER_MIN_HEIGHT + DLG_PADDING * 4) * 2;

constexpr int LIST_WIDTH = 200;

constexpr int BUTTON_HEIGHT = 40;

constexpr wchar_t COLUMN_PEER_ID[] = L"Peer list";


// GroupVideoCallDlg dialog

IMPLEMENT_DYNAMIC(GroupVideoCallDlg, CDialogEx)

GroupVideoCallDlg::GroupVideoCallDlg(
    CWnd* pParent /*= nullptr*/
)
    : CDialogEx(IDD_DIALOG_GROUP_VIDEO_CALL_JOIN, pParent) {
    InitializeCriticalSection(&m_cs);    

    m_pPeerListUpdateEventListener = std::make_shared<PeerListUpdateEventListener>(this);

    GroupVideoCall::GroupVideoCallPeerList::GetInstance()->SetEvent(m_pPeerListUpdateEventListener);
}

GroupVideoCallDlg::~GroupVideoCallDlg() {
    DeleteCriticalSection(&m_cs);

    GroupVideoCall::GroupVideoCallPeerList::GetInstance()->UnsetEvent();
}

void GroupVideoCallDlg::PositionControls(int cx, int cy) {

    // List control position
    int listCtrlLeft = cx - LIST_WIDTH - DLG_PADDING;
    int listCtrlTop = DLG_PADDING;
    int listCtrlHeight = cy - (DLG_PADDING * 2) - BUTTON_HEIGHT - DLG_PADDING;

    m_listParticipant.MoveWindow(listCtrlLeft, listCtrlTop, LIST_WIDTH, listCtrlHeight);

    // Button position
    int buttonLeft = listCtrlLeft;
    int buttonTop = listCtrlTop + listCtrlHeight + DLG_PADDING;

    m_btnLeave.MoveWindow(buttonLeft, buttonTop, LIST_WIDTH, BUTTON_HEIGHT);

    // MyView position
    int nViewWidth = cx - LIST_WIDTH - (DLG_PADDING * 3);
    int nViewTop = DLG_PADDING + 32;
    int nViewLeft = DLG_PADDING;
    int nViewHeight = (cy - (DLG_PADDING * 3 + 32)) / 2;

    m_cMyView.MoveWindow(nViewLeft, nViewTop, nViewWidth, nViewHeight);

    // PeerView position
    nViewTop += (DLG_PADDING + nViewHeight);
    m_cPeerView.MoveWindow(nViewLeft, nViewTop, nViewWidth, nViewHeight);

    FillWindowWithBlack(m_cPeerView);
    FillWindowWithBlack(m_cMyView);
}

void GroupVideoCallDlg::FillWindowWithBlack(CWnd& wnd) {
    if (!wnd.GetSafeHwnd())
        return;

    CDC* pDC = wnd.GetDC();

    CBrush blackBrush(RGB(0, 0, 0));

    CRect rect;
    wnd.GetClientRect(&rect);

    pDC->FillRect(&rect, &blackBrush);

    wnd.ReleaseDC(pDC);
}

void GroupVideoCallDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_LIST_GROUP_VIDEO_CALL_PARTICIPANT, m_listParticipant);
    DDX_Control(pDX, ID_BUTTON_LEAVE, m_btnLeave);
}

BEGIN_MESSAGE_MAP(GroupVideoCallDlg, CDialogEx)
    ON_WM_GETMINMAXINFO()
    ON_WM_PAINT()
    ON_WM_DESTROY()

    ON_BN_CLICKED(ID_BUTTON_LEAVE, OnBnClickedGroupVideoCallLeave)

    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_GROUP_VIDEO_CALL_PARTICIPANT, OnChangePeer)

    ON_MESSAGE(WM_PEER_LIST_UPDATE, OnPeerListUpdate)


END_MESSAGE_MAP()


void GroupVideoCallDlg::StartPeerVideo(PlanetKit::PeerControlPtr pPeerControl) {
    EnterCriticalSection(&m_cs);

    int nSelectedIdx = m_listParticipant.GetNextItem(-1, LVNI_SELECTED);

    if (nSelectedIdx < 0) {
        return;
    }

    CString strcurrentSelectedPeer = m_listParticipant.GetItemText(nSelectedIdx, 0);

    if (pPeerControl->GetPeer()->GetUserID()->GetID().c_str() == strcurrentSelectedPeer) {
        pPeerControl->StartVideo(PlanetKit::NullOptional, PlanetKit::EVideoResolution::PLNK_VIDEO_RESOLUTION_RECOMMENDED);
    }
}

// GroupVideoCallDlg message handlers
BOOL GroupVideoCallDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();

    SetBackgroundColor(RGB(255, 255, 255));

    RECT rt;
    GetClientRect(&rt);

    m_cMyView.Create(NULL, NULL, WS_CHILD, rt, this, 0);
    m_cPeerView.Create(NULL, NULL, WS_CHILD, rt, this, 0);

    CenterWindow(this);

    GroupVideoCall::GroupVideoCallController::GetInstance()->AddMyVideoView(m_cMyView.GetSafeHwnd());

    EnterCriticalSection(&m_cs);
    m_listParticipant.InsertColumn(0, COLUMN_PEER_ID, LVCFMT_LEFT, LIST_WIDTH);
    LeaveCriticalSection(&m_cs);

    m_cMyView.ShowWindow(SW_SHOW);
    m_cPeerView.ShowWindow(SW_SHOW);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void GroupVideoCallDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI) {
    lpMMI->ptMinTrackSize.x = DLG_MIN_WIDTH;
    lpMMI->ptMinTrackSize.y = DLG_MIN_HEIGHT;

    CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void GroupVideoCallDlg::OnDestroy() {
    EnterCriticalSection(&m_cs);

    m_listParticipant.DestroyWindow();
    LeaveCriticalSection(&m_cs);
}

void GroupVideoCallDlg::OnPaint() {
    CDialogEx::OnPaint();

    RECT rt;
    GetClientRect(&rt);
    PositionControls(rt.right, rt.bottom);
}

LRESULT GroupVideoCallDlg::OnPeerListUpdate(WPARAM wParam, LPARAM lParam){
    EnterCriticalSection(&m_cs);

    GroupVideoCall::GroupVideoCallPeerUpdate* pGroupVideoCallUpdate = reinterpret_cast<GroupVideoCall::GroupVideoCallPeerUpdate*>(wParam);

    int nSelectedIdx = m_listParticipant.GetNextItem(-1, LVNI_SELECTED);
    CString strcurrentSelectedPeer;
    bool bDeleteSelectedPeer = false;

    if (nSelectedIdx >= 0) {
        strcurrentSelectedPeer = m_listParticipant.GetItemText(nSelectedIdx, 0);
    }

    for (const auto& removedPeer : pGroupVideoCallUpdate->RemovedPeer) {

        for (int idx = 0; idx < m_listParticipant.GetItemCount(); ++idx) {
            CString strPeer = m_listParticipant.GetItemText(idx, 0);

            if (strPeer == removedPeer.c_str()) {
                m_listParticipant.DeleteItem(idx);
            }

            if (nSelectedIdx > 0 && strPeer == strcurrentSelectedPeer) {
                bDeleteSelectedPeer = true;
            }
        }
    }

    for (const auto& addedPeer : pGroupVideoCallUpdate->AddedPeer) {
        CString strPeerId(addedPeer.c_str());
        int nIdx = m_listParticipant.InsertItem(m_listParticipant.GetItemCount(), strPeerId);
    }

    if ((bDeleteSelectedPeer && m_listParticipant.GetItemCount() > 0)
        || m_listParticipant.GetItemCount() == 1) {
        m_listParticipant.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
        m_listParticipant.SetFocus();
    }

    if (m_listParticipant.GetItemCount() == 0) {
        FillWindowWithBlack(m_cPeerView);
    }
    
    delete pGroupVideoCallUpdate;
    LeaveCriticalSection(&m_cs);

    return 0;
}

void GroupVideoCallDlg::OnChangePeer(NMHDR* pNMHDR, LRESULT* pResult) {
    if (m_pMyPeerController != nullptr) {
        m_pMyPeerController->StopVideo();
        m_pMyPeerController = nullptr;
        FillWindowWithBlack(m_cPeerView);
    }

    int nSelectedIdx = m_listParticipant.GetNextItem(-1, LVNI_SELECTED);
    if (nSelectedIdx < 0) {
        return;
    }

    auto strcurrentSelectedPeer = m_listParticipant.GetItemText(nSelectedIdx, 0);

    m_pMyPeerController = GroupVideoCall::GroupVideoCallPeerList::GetInstance()->GetPeer(strcurrentSelectedPeer.GetBuffer());
    if (m_pMyPeerController != nullptr) {
        m_pMyPeerController->StartVideo(m_cPeerView.GetSafeHwnd(), std::bind(&GroupVideoCallDlg::StartPeerVideo, this, std::placeholders::_1));
    }
}


void GroupVideoCallDlg::OnBnClickedGroupVideoCallLeave() {
    GroupVideoCall::GroupVideoCallController::GetInstance()->RemoveMyView(m_cMyView.GetSafeHwnd());
    GroupVideoCall::GroupVideoCallController::GetInstance()->LeaveConference();
}

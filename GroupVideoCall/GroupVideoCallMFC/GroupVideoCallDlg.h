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

#pragma once

#include <afxwin.h>
#include <afxext.h>
#include <afxdialogex.h>
#include <map>
#include <string>

#include "PlanetKitPeer.h"
#include "PlanetKitSubgroupInterface.h"
#include "PeerListHandler.h"
#include "PeerListUpdateEventListener.h"
#include "PlanetKitPeerControl.h"
#include "../PlanetKit/src/GroupVideoCallPeerList.h"

// GroupVideoCallDlg dialog

class GroupVideoCallDlg : public CDialogEx
{
    DECLARE_DYNAMIC(GroupVideoCallDlg)

private:
    CListCtrl m_listParticipant;
    CButton m_btnLeave;

    CRITICAL_SECTION m_cs;

    CWnd m_cMyView;
    CWnd m_cPeerView;

    std::shared_ptr<PeerListUpdateEventListener> m_pPeerListUpdateEventListener;
    std::shared_ptr<GroupVideoCall::GroupVideoCallPeerController> m_pMyPeerController;
public:
    GroupVideoCallDlg( CWnd* pParent = nullptr );   // standard constructor
    virtual ~GroupVideoCallDlg();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_GROUP_VIDEO_CALL_JOIN };
#endif

private:
    void PositionControls(int cx, int cy);
    void FillWindowWithBlack(CWnd& wnd);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()

public:
    void StartPeerVideo(PlanetKit::PeerControlPtr pPeerControl);
    virtual BOOL OnInitDialog();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();

    afx_msg LRESULT OnPeerListUpdate(WPARAM wParam, LPARAM lParam);

    afx_msg void OnChangePeer(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedGroupVideoCallLeave();
};

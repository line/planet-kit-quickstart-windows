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

#include "EventListener.h"
#include "GroupVideoCallMFCDlg.h"
#include "../PlanetKit/src/GroupVideoCallController.h"

EventListener::EventListener(CGroupVideoCallMFCDlg* pDialog) : m_peerListHandler(GroupVideoCall::GroupVideoCallPeerList::GetInstance()){
    m_pParent = pDialog;

    LPCTSTR className = AfxRegisterWndClass(
        CS_HREDRAW | CS_VREDRAW, 
        ::LoadCursor(NULL, IDC_ARROW), 
        (HBRUSH)(COLOR_WINDOW + 1), 
        NULL 
    );

    m_peerListHandler.CreateEx(
        0,               
        className,       
        _T("PeerListHandler"),
        WS_OVERLAPPEDWINDOW,   
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,  
        NULL   
    );
}

void EventListener::FailedToJoin(int nFailReason) {
    std::wstring strLog = L"Failed to join conference. reason code : " + std::to_wstring(nFailReason) + L"\n";
    OutputDebugStringW(strLog.c_str());
}

void EventListener::OnConnected() {
    OutputDebugStringW(L"Connected\n");
    m_pParent->PostMessage(CONFERENCE_CONNECTED);
}

void EventListener::OnDisconnected(const std::wstring& strDisconnectReason) {
    std::wstring strOutputLog = L"Disconnected (" + strDisconnectReason + L")\n";
    OutputDebugStringW(strOutputLog.c_str());

    std::wstring* pstrLog = new std::wstring;
    *pstrLog = L"Disconnected (" + strDisconnectReason + L")";

    GroupVideoCall::GroupVideoCallPeerList::GetInstance()->Clear();
    m_peerListHandler.PostMessage(WM_PEER_LIST_CLEAR, 0, 0);
    m_pParent->PostMessage(CONFERENCE_DISCONNECTED, (WPARAM)pstrLog);
}

void EventListener::OnPeerListUpdate(PlanetKit::ConferencePeerUpdateParamPtr pParam) {
    std::wstring strLog = L"Participant count : " + std::to_wstring(pParam->TotalCount()) + L"\n";
    OutputDebugStringW(strLog.c_str());

    UpdatePeerListParam* pUpdatePeerListParam = new UpdatePeerListParam;
    pUpdatePeerListParam->pParam = pParam;
    m_peerListHandler.PostMessage(WM_PEER_LIST_UPDATE, (WPARAM)pUpdatePeerListParam, 0);
}
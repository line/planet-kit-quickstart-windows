// Copyright 2024 LINE Plus Corporation
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

#include "pch.h"

#include "EventListener.h"
#include "GroupAudioCallMFCDlg.h"

EventListener::EventListener(CGroupAudioCallMFCDlg* pDialog) {
    m_pParent = pDialog;
}

void EventListener::FailedToJoin(int nFailReason) {
    std::wstring strLog = L"Failed to join conference. reason code : " + std::to_wstring(nFailReason) + L"\n";
    OutputDebugStringW(strLog.c_str());
}

void EventListener::OnConnected() {
    OutputDebugStringW(L"Connected\n");
    m_pParent->PostMessage(CONFERENCE_CONNECTED);
}

void EventListener::OnDisconnected(std::wstring strDisconnectReason) {
    std::wstring strOutputLog = L"Disconnected (" + strDisconnectReason + L")\n";
    OutputDebugStringW(strOutputLog.c_str());

    std::wstring* pstrLog = new std::wstring;
    *pstrLog = L"Disconnected (" + strDisconnectReason + L")";
    
    m_pParent->PostMessage(CONFERENCE_DISCONNECTED, (WPARAM)pstrLog);
}

void EventListener::OnPeerListUpdate(size_t nParticipantCount) {
    std::wstring strLog = L"Participant count : " + std::to_wstring(nParticipantCount) + L"\n";
    OutputDebugStringW(strLog.c_str());

    m_pParent->PostMessage(CONFERENCE_PARTICIPANTS_UPDATED, nParticipantCount);
}
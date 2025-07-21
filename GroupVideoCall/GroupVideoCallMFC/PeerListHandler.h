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
#include "../PlanetKit/src/GroupVideoCallPeerList.h"

constexpr UINT WM_PEER_LIST_UPDATE = WM_USER + 100;
constexpr UINT WM_PEER_LIST_CLEAR = WM_PEER_LIST_UPDATE + 1;

typedef struct _UpdatePeerListParam {
    PlanetKit::ConferencePeerUpdateParamPtr pParam;
} UpdatePeerListParam;

class PeerListHandler : public CWnd {
public:
    PeerListHandler(std::shared_ptr<GroupVideoCall::GroupVideoCallPeerList> pGroupCallPeerList);
    afx_msg LRESULT OnPeerListUpdate(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnPeerListClear(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
private:
    std::shared_ptr<GroupVideoCall::GroupVideoCallPeerList> m_pGroupCallPeerList;
};
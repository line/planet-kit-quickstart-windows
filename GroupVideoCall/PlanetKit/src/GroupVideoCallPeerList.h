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

#include <map>
#include <memory>
#include <string>
#include <functional>

#include "PlanetKitConferencePeerUpdateParam.h"
#include "IPeerListUpdateEventListener.h"
#include "GroupVideoCallPeerCotroller.h"

namespace GroupVideoCall {

    class GroupVideoCallPeerList {
    public :
        static std::shared_ptr<GroupVideoCallPeerList> GetInstance();

        void UpdatePeerListParam(PlanetKit::ConferencePeerUpdateParamPtr pParam);
        void Clear();

        void SetEvent(std::shared_ptr<IPeerListUpdateEventListener> pPeerListUpdateEvent);
        void UnsetEvent();

        std::shared_ptr<GroupVideoCallPeerController> GetPeer(LPWSTR strPeerID);
    private:
        GroupVideoCallPeerList() = default;
        static std::shared_ptr<GroupVideoCallPeerList> m_pInstance;
        std::map<std::wstring, std::shared_ptr<GroupVideoCallPeerController>> m_mapPeers;

        std::shared_ptr<IPeerListUpdateEventListener> m_pPeerListUpdateEventListener;
    };
};
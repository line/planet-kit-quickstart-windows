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


#include "GroupVideoCallPeerList.h"

namespace GroupVideoCall {
    std::shared_ptr<GroupVideoCallPeerList> GroupVideoCallPeerList::m_pInstance = nullptr;

    std::shared_ptr<GroupVideoCallPeerList> GroupVideoCallPeerList::GetInstance() {
        if (m_pInstance == nullptr) {
            m_pInstance = std::shared_ptr<GroupVideoCallPeerList>(new GroupVideoCallPeerList());
        }

        return m_pInstance;
    }

    void GroupVideoCallPeerList::UpdatePeerListParam(PlanetKit::ConferencePeerUpdateParamPtr pParam) {
        for (size_t i = 0; i < pParam->GetAddedPeer().Size(); i++) {

            std::wstring strPeerID = pParam->GetAddedPeer().At(i)->GetUserID()->GetID().c_str();

            auto itPeer = m_mapPeers.find(strPeerID);
            if (itPeer == m_mapPeers.end()) {
                m_mapPeers.insert(std::make_pair(strPeerID, std::make_shared<GroupVideoCallPeerController>(pParam->GetAddedPeer().At(i))));
            }
        }

        for (size_t i = 0; i < pParam->GetRemovedPeer().Size(); i++) {

            std::wstring strPeerID = pParam->GetRemovedPeer().At(i)->GetUserID()->GetID().c_str();

            auto itPeer = m_mapPeers.find(strPeerID);
            if (itPeer != m_mapPeers.end()) {
                m_mapPeers.erase(itPeer);
            }
        }

        if (m_pPeerListUpdateEventListener) {
            GroupVideoCallPeerUpdate* pPeerUpdate = new GroupVideoCallPeerUpdate;

            for (size_t i = 0; i < pParam->GetAddedPeer().Size(); i++) {
                pPeerUpdate->AddedPeer.push_back(pParam->GetAddedPeer().At(i)->GetUserID()->GetID().c_str());
            }

            for (size_t i = 0; i < pParam->GetRemovedPeer().Size(); i++) {
                pPeerUpdate->RemovedPeer.push_back(pParam->GetRemovedPeer().At(i)->GetUserID()->GetID().c_str());
            }

            if (m_pPeerListUpdateEventListener != nullptr) {
                m_pPeerListUpdateEventListener->ChangePeerList(pPeerUpdate);
            }
        }

    }
    void GroupVideoCallPeerList::Clear() {
        m_mapPeers.clear();
    }

    void GroupVideoCallPeerList::SetEvent(std::shared_ptr<IPeerListUpdateEventListener> pPeerListUpdateEvent) {
        m_pPeerListUpdateEventListener = pPeerListUpdateEvent;
    }

    void GroupVideoCallPeerList::UnsetEvent() {
        m_pPeerListUpdateEventListener = nullptr;
    }

    std::shared_ptr<GroupVideoCallPeerController> GroupVideoCallPeerList::GetPeer(LPWSTR strPeerID) {
        auto itPeer = m_mapPeers.find(strPeerID);
        if (itPeer != m_mapPeers.end()) {
            return itPeer->second;
        }
        else {
            return nullptr;
        }
    }
}
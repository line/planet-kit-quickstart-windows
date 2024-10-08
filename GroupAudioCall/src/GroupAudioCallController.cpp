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

#include "GroupAudioCallController.h"

#include "PlanetKitManager.h"
#include "Constants.h"

namespace GroupAudioCall {
    GroupAudioCallController::GroupAudioCallController(std::shared_ptr<GroupAudioCallEventListener> pEventListener) {
        m_pEventListener = pEventListener;
    }

    void GroupAudioCallController::InitializePlanetKit() {
        // Initializes PlanetKit.
        auto pConfiguration = PlanetKit::Configuration::Create(L"./", L"./");
        PlanetKit::PlanetKitManager::Initialize(pConfiguration);

        auto pManager = PlanetKit::PlanetKitManager::GetInstance();
        // Updates PlanetKit cloud server URL.
        pManager->UpdateServerUrl(PLANET_CLOUD_URL);
    }

    void GroupAudioCallController::JoinConference(std::wstring strRoomId) {
        if (*m_pConference != nullptr) {
            return;
        }

        // Creates user information.
        
        auto pUserId = PlanetKit::UserId::Create(
            USER_ID,
            SERVICE_ID,
            PlanetKit::WString(L"KR")
        );

        // Creates ConferenceJoinParam.
        auto pConferenceParam = PlanetKit::ConferenceParam::CreateWithAccessToken(
            pUserId,
            strRoomId.c_str(),
            SERVICE_ID,
            ACCESS_TOKEN
        );

        // Attaches a PlanetKit conference event listener.
        pConferenceParam->SetConferenceEvent(&m_cEventListener);

        auto pPlanetKitManager = PlanetKit::PlanetKitManager::GetInstance();
        auto sStartResult = pPlanetKitManager->JoinConference(pConferenceParam, m_pConference);

        if (sStartResult.bSuccess == false) {
            m_pEventListener->FailedToJoin((int)sStartResult.reason);
        }
    }

    void GroupAudioCallController::LeaveConference() {
        if (*m_pConference == nullptr) {
            return;
        }

        m_pConference->LeaveConference();
    }

    void GroupAudioCallController::OnConnected() {
        m_pEventListener->OnConnected();
    }

    void GroupAudioCallController::OnDisconnected(std::wstring strDisconnectReason) {
        m_pEventListener->OnDisconnected(strDisconnectReason);

        m_pConference = nullptr;
    }

    void GroupAudioCallController::OnPeerListUpdate(size_t nParticipantCount) {
        m_pEventListener->OnPeerListUpdate(nParticipantCount);
    }
};
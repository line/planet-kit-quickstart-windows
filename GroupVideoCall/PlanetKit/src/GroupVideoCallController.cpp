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

#include "GroupVideoCallController.h"
#include "ConferenceEventListener.h"
#include "Planetkit.h"
#include "PlanetKitManager.h"
#include "Constants.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif

namespace GroupVideoCall {
    std::shared_ptr<GroupVideoCallController> GroupVideoCallController::m_pInstance = nullptr;

    std::shared_ptr<GroupVideoCallController> GroupVideoCallController::GetInstance() {
        if (m_pInstance == nullptr) {
            m_pInstance = std::shared_ptr<GroupVideoCallController>(new GroupVideoCallController());
            m_pInstance->m_pConferenceEventListener = PlanetKit::MakeAutoPtr<ConferenceEventListener>(GroupVideoCallController::m_pInstance);
        }

        return m_pInstance;
    }

    void GroupVideoCallController::SetEventListener(std::shared_ptr<GroupVideoCallEventListener> pEventListener) {
        m_pEventListener = pEventListener;
    }

    void GroupVideoCallController::InitializePlanetKit() {

#ifdef _DEBUG
        // Disable CRT assert dialogs
        _CrtSetReportMode(_CRT_ASSERT, 0);
#endif

        // Initializes PlanetKit.
        auto pConfiguration = PlanetKit::Configuration::Create(L"./", L"./");
        PlanetKit::PlanetKitManager::Initialize(pConfiguration);

        auto pManager = PlanetKit::PlanetKitManager::GetInstance();
        // Updates PlanetKit cloud server URL.
        pManager->UpdateServerUrl(PLANET_CLOUD_URL);

        // Initializes audio settings.
        InitializeAudio();

        // Initializes camera settings.
        InitializeCamera();
    }

    void GroupVideoCallController::JoinConference(std::wstring strRoomId) {
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
        pConferenceParam->SetConferenceEvent(m_pConferenceEventListener);
        pConferenceParam->SetIsVideoCall(true);

        auto pPlanetKitManager = PlanetKit::PlanetKitManager::GetInstance();

        auto pCurMic = pPlanetKitManager->GetAudioManager()->GetCurrentMic();

        auto sStartResult = pPlanetKitManager->JoinConference(pConferenceParam, pCurMic, m_pConference);

        if (sStartResult.bSuccess == false && m_pEventListener != nullptr) {
            m_pEventListener->FailedToJoin((int)sStartResult.reason);
        }
    }

    void GroupVideoCallController::LeaveConference() {
        if (*m_pConference == nullptr) {
            return;
        }

        m_pConference->LeaveConference();
    }

    bool GroupVideoCallController::AddMyVideoReceiver(PlanetKit::IVideoReceiverPtr pReceiver) {
        if (*m_pConference == nullptr) {
            return false;
        }

        return m_pConference->AddMyVideoReceiver(pReceiver);
    }

    bool GroupVideoCallController::RemoveMyVideoReceiver(PlanetKit::IVideoReceiverPtr pReceiver) {
        if (*m_pConference == nullptr) {
            return false;
        }

        return m_pConference->RemoveMyVideoReceiver(pReceiver);
    }

    bool GroupVideoCallController::AddMyVideoView(HWND hWnd) {
        if (*m_pConference == nullptr) {
            return false;
        }

        return m_pConference->AddMyVideoView(hWnd);
    }

    void GroupVideoCallController::RemoveMyView(HWND hWnd) {
        if (*m_pConference == nullptr) {
            return;
        }

        m_pConference->RemoveMyVideoView(hWnd);
    }

    void GroupVideoCallController::OnConnected() {
        if (m_pEventListener != nullptr) {
            m_pEventListener->OnConnected();
        }
    }

    void GroupVideoCallController::OnDisconnected(std::wstring strDisconnectReason) {
        if (m_pEventListener != nullptr) {
            m_pEventListener->OnDisconnected(strDisconnectReason);
        }

        m_pConference = nullptr;
    }

    void GroupVideoCallController::OnPeerListUpdate(PlanetKit::ConferencePeerUpdateParamPtr pParam) {
        if (m_pEventListener != nullptr) {
            m_pEventListener->OnPeerListUpdate(pParam);
        }
    }

    GroupVideoCallController::GroupVideoCallController() {  }

    void GroupVideoCallController::InitializeAudio() {
        auto pPlanetKitManager = PlanetKit::PlanetKitManager::GetInstance();
        auto pAudioManager = pPlanetKitManager->GetAudioManager();

        auto pDefaultMicInfo = pAudioManager->GetDefaultMicInfo();
        auto pDefaultSpkInfo = pAudioManager->GetDefaultSpeakerInfo();

        pAudioManager->ChangeMic(pDefaultMicInfo.Value());
        pAudioManager->ChangeSpeaker(pDefaultSpkInfo.Value());
    }


    void GroupVideoCallController::InitializeCamera() {
        auto pPlanetKitManager = PlanetKit::PlanetKitManager::GetInstance();

        PlanetKit::CameraInfoArray videoCaptures;
        auto pCameraController = pPlanetKitManager->GetCameraController();

        pCameraController->GetCapturerInfo(videoCaptures);

        if (videoCaptures.Size() > 0) {
            pCameraController->ChangeCamera(videoCaptures[0]);
        }
    }
};
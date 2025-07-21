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

#include <memory>
#include <string>

#include "PlanetKit.h"
#include "PlanetKitConference.h"
#include "GroupVideoCallEventListener.h"
#include "GroupVideoCallPeerList.h"

namespace GroupVideoCall {
    class ConferenceEventListener;

    class GroupVideoCallController {
    public :
        static std::shared_ptr<GroupVideoCallController> GetInstance();
        void SetEventListener(std::shared_ptr<GroupVideoCallEventListener> pEventListener);
        void InitializePlanetKit();

        void JoinConference(std::wstring strRoomId);
        void LeaveConference();

        bool AddMyVideoReceiver(PlanetKit::IVideoReceiverPtr pReceiver);
        bool RemoveMyVideoReceiver(PlanetKit::IVideoReceiverPtr pReceiver);
        bool AddMyVideoView(HWND hWnd);
        void RemoveMyView(HWND hWnd);

        // Events from PlanetKit
        void OnConnected();
        void OnDisconnected(std::wstring strDisconnectReason);
        void OnPeerListUpdate(PlanetKit::ConferencePeerUpdateParamPtr pParam);

    private :
        GroupVideoCallController();
        void InitializeAudio();
        void InitializeCamera();
        
        static std::shared_ptr<GroupVideoCallController> m_pInstance;

        PlanetKit::PlanetKitConferencePtr m_pConference;

        std::shared_ptr<GroupVideoCallEventListener> m_pEventListener;
        PlanetKit::SharedPtr<ConferenceEventListener> m_pConferenceEventListener;
    };
};
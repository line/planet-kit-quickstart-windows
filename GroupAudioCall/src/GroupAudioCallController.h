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

#pragma once

#include <memory>
#include <string>

#include "PlanetKit.h"
#include "PlanetKitConference.h"
#include "GroupAudioCallEventListener.h"
#include "ConferenceEventListener.h"

namespace GroupAudioCall {
    class GroupAudioCallController {
    public :
        GroupAudioCallController(std::shared_ptr<GroupAudioCallEventListener> pEventListener);
        void InitializePlanetKit();

        void JoinConference(std::wstring strRoomId);
        void LeaveConference();


        // Events from PlanetKit
        void OnConnected();
        void OnDisconnected(std::wstring strDisconnectReason);
        void OnPeerListUpdate(size_t nParticipantCount);

    private :
        PlanetKit::PlanetKitConferencePtr m_pConference;
        std::shared_ptr<GroupAudioCallEventListener> m_pEventListener{ nullptr };
        ConferenceEventListener m_cEventListener{ this };
    };
};
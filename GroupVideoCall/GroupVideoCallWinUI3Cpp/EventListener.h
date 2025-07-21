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

#include "../PlanetKit/src/GroupVideoCallEventListener.h"

#include "GroupVideoCallViewModel.h"
#include "ParticipantViewModel.h"

class EventListener : public GroupVideoCall::GroupVideoCallEventListener {
public:
    EventListener(winrt::GroupVideoCallWinUI3Cpp::GroupVideoCallViewModel groupVideoCallViewModel, winrt::GroupVideoCallWinUI3Cpp::ParticipantViewModel participantViewModel);

    void FailedToJoin(int nFailReason) final;
    void OnConnected() final;
    void OnDisconnected(const std::wstring& strDisconnectReason) final;
    void OnPeerListUpdate(PlanetKit::ConferencePeerUpdateParamPtr pParam) final;

private:
    winrt::GroupVideoCallWinUI3Cpp::GroupVideoCallViewModel m_groupVideoCallViewModel{ nullptr };
    winrt::GroupVideoCallWinUI3Cpp::ParticipantViewModel m_participantViewModel{ nullptr };
};
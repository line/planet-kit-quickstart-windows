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

#include "pch.h"
#include "EventListener.h"
#include "GroupVideoCallViewModel.h"


EventListener::EventListener(winrt::GroupVideoCallWinUI3Cpp::GroupVideoCallViewModel groupVideoCallViewModel, winrt::GroupVideoCallWinUI3Cpp::ParticipantViewModel participantViewModel){
    m_groupVideoCallViewModel = groupVideoCallViewModel;
    m_participantViewModel = participantViewModel;
}

void EventListener::FailedToJoin(int nFailReason) {
    m_groupVideoCallViewModel.FailedToJoin(nFailReason);
}

void EventListener::OnConnected() {
    m_groupVideoCallViewModel.GroupCallConnectedState(GroupVideoCallWinUI3Cpp::ConnectedState::connected);
    m_participantViewModel.GroupCallConnectedState(GroupVideoCallWinUI3Cpp::ConnectedState::connected);
}

void EventListener::OnDisconnected(const std::wstring& strDisconnectReason) {
    m_groupVideoCallViewModel.DisconnectReason(strDisconnectReason.c_str());
    m_groupVideoCallViewModel.GroupCallConnectedState(GroupVideoCallWinUI3Cpp::ConnectedState::disconnected);
    m_participantViewModel.GroupCallConnectedState(GroupVideoCallWinUI3Cpp::ConnectedState::disconnected);
}

void EventListener::OnPeerListUpdate(PlanetKit::ConferencePeerUpdateParamPtr pParam) {
    m_participantViewModel.as<winrt::GroupVideoCallWinUI3Cpp::implementation::ParticipantViewModel>()->PeerListUpdate(pParam);
}
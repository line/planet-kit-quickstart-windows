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
#include "ParticipantViewModel.g.h"

#include "VideoReceiver.h"
#include "PeerListUpdateEventListener.h"
#include "PlanetKitPeerControl.h"
#include "PlanetKitConferencePeerUpdateParam.h"

#include <functional>

constexpr const wchar_t* DefaultRoomInfo = L"GroupVideoCall";

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {
    struct ParticipantViewModel : ParticipantViewModelT<ParticipantViewModel> {
        ParticipantViewModel(winrt::Microsoft::UI::Dispatching::DispatcherQueue const& queue);
        virtual ~ParticipantViewModel();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void CreateMyVideoReceiver(SwapChainPanel const& panel);
        void CreatePeerVideoReceiver(SwapChainPanel const& panel);
        
        ConnectedState GroupCallConnectedState();
        void GroupCallConnectedState(ConnectedState connectedState);

        void LeaveConference();

        void PeerListUpdate(PlanetKit::ConferencePeerUpdateParamPtr pParam);

        Windows::Foundation::Collections::IObservableVector<hstring> PeerNames();
        void ChangePeerList(const GroupVideoCall::GroupVideoCallPeerUpdate* pGroupVideoCallUpdate);

        int32_t SelectedPeerIdx();
        void SelectedPeerIdx(int32_t nIdx);

        hstring RoomID();
        void RoomID(const hstring& strRoomID);

        hstring RoomInfo();
    private:
        void StartPeerVideo(PlanetKit::PeerControlPtr pPeerControl);
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;

        PlanetKit::SharedPtr<VideoReceiver> m_pMyVideoReceiver = nullptr;
        PlanetKit::SharedPtr<VideoReceiver> m_pPeerVideoReceiver = nullptr;

        std::shared_ptr<PeerListUpdateEventListener> m_pPeerListUpdateEventListener;

        Windows::Foundation::Collections::IObservableVector<hstring> m_PeerList;
        int32_t m_nSelectedIdx = -1;

        winrt::Microsoft::UI::Dispatching::DispatcherQueue m_queue;

        ConnectedState m_connectedState = ConnectedState::disconnected;

        hstring m_strRoomID;
        hstring m_strRoomInfo = DefaultRoomInfo;
    };
}

namespace winrt::GroupVideoCallWinUI3Cpp::factory_implementation {
    struct ParticipantViewModel : ParticipantViewModelT<ParticipantViewModel, implementation::ParticipantViewModel> {
    };
}

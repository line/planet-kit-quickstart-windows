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
#include "ParticipantViewModel.h"
#if __has_include("ParticipantViewModel.g.cpp")
#include "ParticipantViewModel.g.cpp"
#endif

#include "../PlanetKit/src/GroupVideoCallController.h"
#include "../PlanetKit/src/GroupVideoCallPeerList.h"

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {
    ParticipantViewModel::ParticipantViewModel(winrt::Microsoft::UI::Dispatching::DispatcherQueue const& queue) : m_queue(queue) {
        m_pPeerListUpdateEventListener = std::make_shared<PeerListUpdateEventListener>(std::bind(&ParticipantViewModel::ChangePeerList, this, std::placeholders::_1));
        GroupVideoCall::GroupVideoCallPeerList::GetInstance()->SetEvent(m_pPeerListUpdateEventListener);

        m_PeerList = single_threaded_observable_vector<hstring>();
    }

    ParticipantViewModel::~ParticipantViewModel() {
        GroupVideoCall::GroupVideoCallPeerList::GetInstance()->UnsetEvent();
    }

    int32_t ParticipantViewModel::MyProperty() {
        throw hresult_not_implemented();
    }

    void ParticipantViewModel::MyProperty(int32_t /*value*/) {
        throw hresult_not_implemented();
    }

    void ParticipantViewModel::CreateMyVideoReceiver(SwapChainPanel const& panel) {
        m_pMyVideoReceiver = PlanetKit::MakeAutoPtr<VideoReceiver>();
        m_pMyVideoReceiver->SetSwapChainPanel(panel);

        GroupVideoCall::GroupVideoCallController::GetInstance()->AddMyVideoReceiver(m_pMyVideoReceiver);
    }

    void ParticipantViewModel::CreatePeerVideoReceiver(SwapChainPanel const& panel) {
        m_pPeerVideoReceiver = PlanetKit::MakeAutoPtr<VideoReceiver>();
        m_pPeerVideoReceiver->SetSwapChainPanel(panel);
    }

    ConnectedState ParticipantViewModel::GroupCallConnectedState() {
        return m_connectedState;
    }

    void ParticipantViewModel::GroupCallConnectedState(ConnectedState connectedState) {
        m_queue.TryEnqueue([connectedState, this]() {

            m_connectedState = connectedState;

            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"GroupCallConnectedState"));


            if (connectedState == ConnectedState::disconnected) {

                GroupVideoCall::GroupVideoCallController::GetInstance()->RemoveMyVideoReceiver(m_pMyVideoReceiver);
                if (m_pPeerVideoReceiver.hasValue()) {
                    m_pPeerVideoReceiver->DrawBlack();
                }

                if (m_nSelectedIdx > 0) {
                    hstring curPeerStr = m_PeerList.GetAt(m_nSelectedIdx);
                    auto curPeer = GroupVideoCall::GroupVideoCallPeerList::GetInstance()->GetPeer(const_cast<LPWSTR>(curPeerStr.c_str()));
                    curPeer->StopVideo();
                }

                m_nSelectedIdx = -1;
                m_PeerList.Clear();


                m_strRoomInfo = DefaultRoomInfo;
                m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"RoomInfo"));
            }
        });
    }

    void ParticipantViewModel::LeaveConference() {
        GroupVideoCall::GroupVideoCallController::GetInstance()->LeaveConference();
    }

    void ParticipantViewModel::PeerListUpdate(PlanetKit::ConferencePeerUpdateParamPtr pParam) {
        m_queue.TryEnqueue([pParam, this]() {
            winrt::hstring strLog = L"Participant count : ";
            strLog = strLog + std::to_wstring(pParam->TotalCount());
            strLog = strLog + L"\n";
            OutputDebugStringW(strLog.c_str());

            GroupVideoCall::GroupVideoCallPeerList::GetInstance()->UpdatePeerListParam(pParam);
        });
    }

    Windows::Foundation::Collections::IObservableVector<hstring> ParticipantViewModel::PeerNames() {
        return m_PeerList;
    }

    void ParticipantViewModel::ChangePeerList(const GroupVideoCall::GroupVideoCallPeerUpdate* pGroupVideoCallUpdate) {
        m_queue.TryEnqueue([pGroupVideoCallUpdate, this]() {

            bool bDeleteSelectedPeer = false;
            hstring strCurrentPeer = m_nSelectedIdx > 0 ? m_PeerList.GetAt(m_nSelectedIdx) : L"";

            for (const auto& removedPeer : pGroupVideoCallUpdate->RemovedPeer) {

                for (int idx = 0; idx < m_PeerList.Size(); ++idx) {
                    hstring strPeer = m_PeerList.GetAt(idx);

                    if (strCurrentPeer.size() != 0 && strPeer == strCurrentPeer) {
                        bDeleteSelectedPeer = true;
                        SelectedPeerIdx(-1);
                    }

                    if (strPeer == removedPeer.c_str()) {
                        m_PeerList.RemoveAt(idx);
                        --idx;
                    }
                }
            }

            for (const auto& addedPeer : pGroupVideoCallUpdate->AddedPeer) {
                hstring strPeerId(addedPeer.c_str());
                m_PeerList.Append(strPeerId);
            }

            // Selected peer is removed
            if ((bDeleteSelectedPeer && m_PeerList.Size() > 0)) {
                SelectedPeerIdx(0);
            }

            // There is no currently selected peer, but since the list size is greater than 1, the first item will be selected.
            if (m_PeerList.Size() > 0 && m_nSelectedIdx < 0) {
                SelectedPeerIdx(0);
            }

            // List is empty
            if (m_PeerList.Size() == 0) {
                SelectedPeerIdx(-1);
                m_pPeerVideoReceiver->DrawBlack();
            }

            m_strRoomInfo = m_strRoomID + L"(" + std::to_wstring((m_PeerList.Size() + 1)) + L")";
            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"RoomInfo"));
            delete pGroupVideoCallUpdate;
        });
    }

    int32_t ParticipantViewModel::SelectedPeerIdx() {
        return m_nSelectedIdx;
    }

    void ParticipantViewModel::SelectedPeerIdx(int32_t nIdx) {
        if (m_nSelectedIdx == nIdx) {
            return;
        }

        // Stop peer video before chagne
        if (m_PeerList.Size() > 0 && m_nSelectedIdx >= 0) {
            hstring curPeerStr = m_PeerList.GetAt(m_nSelectedIdx);
            auto curPeer = GroupVideoCall::GroupVideoCallPeerList::GetInstance()->GetPeer(const_cast<LPWSTR>(curPeerStr.c_str()));
            curPeer->StopVideo();
        }

        if (nIdx >= 0) {
            hstring newPeerStr = m_PeerList.GetAt(nIdx);
            auto newPeer = GroupVideoCall::GroupVideoCallPeerList::GetInstance()->GetPeer(const_cast<LPWSTR>(newPeerStr.c_str()));
            newPeer->StartVideo(m_pPeerVideoReceiver, std::bind(&ParticipantViewModel::StartPeerVideo, this, std::placeholders::_1));

        }

        m_nSelectedIdx = nIdx;
        m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"SelectedPeerIdx"));
    }

    void ParticipantViewModel::RoomID(const hstring& strRoomID) {
        m_strRoomID = strRoomID;

        m_strRoomInfo = m_strRoomID + L"(" + std::to_wstring((m_PeerList.Size() + 1)) + L")";
        m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"RoomInfo"));
    }

    hstring ParticipantViewModel::RoomID() {
        return m_strRoomID;
    }

    hstring ParticipantViewModel::RoomInfo() {
        return m_strRoomInfo;
    }

    void ParticipantViewModel::StartPeerVideo(PlanetKit::PeerControlPtr pPeerControl) {
        if (m_nSelectedIdx < 0) {
            return;
        }

        if (pPeerControl->GetPeer()->GetUserID()->GetID() == m_PeerList.GetAt(m_nSelectedIdx)) {
            pPeerControl->StartVideo(PlanetKit::NullOptional, PlanetKit::EVideoResolution::PLNK_VIDEO_RESOLUTION_RECOMMENDED);
        }
    }

    event_token ParticipantViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_PropertyChanged.add(handler);
    }

    void ParticipantViewModel::PropertyChanged(event_token const& token) noexcept {
        m_PropertyChanged.remove(token);
    }
}

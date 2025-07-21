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
#include "ParticipantPage.xaml.h"
#if __has_include("ParticipantPage.g.cpp")
#include "ParticipantPage.g.cpp"
#endif

#include "ParticipantViewModel.h"
#include <winrt/Microsoft.UI.Windowing.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {
    ParticipantPage::~ParticipantPage() {
        if (m_participantViewModel) {
            m_participantViewModel.PropertyChanged(m_callViewModelEventToken);
        }

        m_participantViewModel = nullptr;
        m_mainFrame = nullptr;
    }

    int32_t ParticipantPage::MyProperty() {
        throw hresult_not_implemented();
    }

    void ParticipantPage::MyProperty(int32_t /* value */) {
        throw hresult_not_implemented();
    }

    void ParticipantPage::InitializeComponent() {
        ParticipantPageT::InitializeComponent();
    }

    void ParticipantPage::OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& args) {
        auto parameter = args.Parameter();

        if (auto groupVideoCallJoinPageData = parameter.try_as<Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable>>()) {
            m_mainFrame = groupVideoCallJoinPageData.Lookup(L"mainFrame").as<Microsoft::UI::Xaml::Controls::Frame>();
            m_participantViewModel = groupVideoCallJoinPageData.Lookup(L"participantViewModel").as<GroupVideoCallWinUI3Cpp::ParticipantViewModel>();

            IInspectable roomIdInspectable = groupVideoCallJoinPageData.Lookup(L"RoomID");
            hstring roomId = roomIdInspectable.as<Windows::Foundation::IPropertyValue>().GetString();

            m_participantViewModel.RoomID(roomId);

            m_participantViewModel.CreateMyVideoReceiver(MySwapChainPanel());
            m_participantViewModel.CreatePeerVideoReceiver(PeerSwapChainPanel());

            m_callViewModelEventToken = m_participantViewModel.PropertyChanged({ this, &ParticipantPage::groupVideoCallViewModelProperyChanged });
        }
    }


    GroupVideoCallWinUI3Cpp::ParticipantViewModel ParticipantPage::participantViewModel() {
        return m_participantViewModel;
    }

    void ParticipantPage::groupVideoCallViewModelProperyChanged(IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"GroupCallConnectedState") {
            if (m_participantViewModel.GroupCallConnectedState() == ConnectedState::disconnected) {
                m_mainFrame.GoBack();
            }
        }
    }

    void ParticipantPage::OnClickLeave(::winrt::Windows::Foundation::IInspectable const& sender, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_participantViewModel.LeaveConference();
    }

}

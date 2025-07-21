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
#include "GroupVideoCallJoinPage.xaml.h"
#if __has_include("GroupVideoCallJoinPage.g.cpp")
#include "GroupVideoCallJoinPage.g.cpp"
#endif

#include "GroupVideoCallViewModel.h"
#include "ParticipantViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {
    GroupVideoCallJoinPage::~GroupVideoCallJoinPage() {
        if (m_groupVideoCallViewModel) {
            m_groupVideoCallViewModel.PropertyChanged(m_callViewModelEventToken);
        }

        m_groupVideoCallViewModel = nullptr;
        m_participantViewModel = nullptr;
        m_mainFrame = nullptr;
    }

    int32_t GroupVideoCallJoinPage::MyProperty() {
        throw hresult_not_implemented();
    }

    void GroupVideoCallJoinPage::MyProperty(int32_t /* value */) {
        throw hresult_not_implemented();
    }

    void GroupVideoCallJoinPage::InitializeComponent() {
        GroupVideoCallJoinPageT::InitializeComponent();
    }

    void GroupVideoCallJoinPage::OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& args) {
        auto parameter = args.Parameter();

        if (auto gorupVideoCallJoinPageData = parameter.try_as<Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable>>()) {
            m_mainFrame = gorupVideoCallJoinPageData.Lookup(L"mainFrame").as<Microsoft::UI::Xaml::Controls::Frame>();
            m_groupVideoCallViewModel = gorupVideoCallJoinPageData.Lookup(L"groupVideoCallViewModel").as<GroupVideoCallWinUI3Cpp::GroupVideoCallViewModel>();
            m_participantViewModel = gorupVideoCallJoinPageData.Lookup(L"participantViewModel").as<GroupVideoCallWinUI3Cpp::ParticipantViewModel>();

            m_callViewModelEventToken = m_groupVideoCallViewModel.PropertyChanged({ this, &GroupVideoCallJoinPage::groupVideoCallViewModelProperyChanged });

            // Attaches a `InfoBar` UI that is log message.
            m_groupVideoCallViewModel.LogMessageInfoBar(logMessage());
        }

    }

    GroupVideoCallWinUI3Cpp::GroupVideoCallViewModel GroupVideoCallJoinPage::groupVideoCallViewModel() {
        return m_groupVideoCallViewModel;
    }

    void GroupVideoCallJoinPage::OnClickJoin(::winrt::Windows::Foundation::IInspectable const& sender, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_groupVideoCallViewModel.JoinCoference();
    }

    void GroupVideoCallJoinPage::groupVideoCallViewModelProperyChanged(IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"GroupCallConnectedState") {
            if (m_groupVideoCallViewModel.GroupCallConnectedState() == ConnectedState::connected) {

                Windows::UI::Xaml::Interop::TypeName pageTypeName = winrt::xaml_typename<winrt::GroupVideoCallWinUI3Cpp::ParticipantPage>();
                Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
                slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromRight);

                Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> participantData = winrt::single_threaded_map<hstring, Windows::Foundation::IInspectable>();

                participantData.Insert(L"mainFrame", m_mainFrame);
                participantData.Insert(L"participantViewModel", m_participantViewModel);
                participantData.Insert(L"RoomID", Windows::Foundation::PropertyValue::CreateString(m_groupVideoCallViewModel.RoomID()));

                m_mainFrame.Navigate(pageTypeName, box_value(participantData), slide);
            }
        }
    }
}

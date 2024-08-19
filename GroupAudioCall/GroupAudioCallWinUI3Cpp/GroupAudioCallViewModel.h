#pragma once

#include <memory>

#include "GroupAudioCallViewModel.g.h"

#include "EventListener.h"
#include "../src/GroupAudioCallController.h"

namespace winrt::GroupAudioCallWinUI3Cpp::implementation {
    struct GroupAudioCallViewModel : GroupAudioCallViewModelT<GroupAudioCallViewModel> {
        GroupAudioCallViewModel();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        hstring ConnectedState();
        void ConnectedState(const hstring& strConnectedState);

        int32_t Participants();
        void Participants(int32_t nParticipants);

        hstring UserID();

        hstring RoomID();
        void RoomID(const hstring& strRoomID);

        Microsoft::UI::Xaml::Controls::InfoBar LogMessageInfoBar();
        void LogMessageInfoBar(Microsoft::UI::Xaml::Controls::InfoBar logInfoBar);

        hstring LogTitle();
        void LogTitle(const hstring& strTitle);

        hstring LogMessage();
        void LogMessage(const hstring& strMessage);

        void OpenLogMessage(const hstring& strTitle, const hstring& strMessage);

        void OnClickJoin(::winrt::Windows::Foundation::IInspectable const& sender, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void OnClickLeave(::winrt::Windows::Foundation::IInspectable const& sender, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;

        hstring m_strConnectedState = L"idle";
        int32_t m_nParticipants = 0;
        hstring m_strUserID;
        hstring m_strRoomID;

        Microsoft::UI::Xaml::Controls::InfoBar m_logInfoBar{ nullptr };
        hstring m_strLogTitle;
        hstring m_strLogMessage;



        std::shared_ptr<EventListener> m_pEventListener{ new EventListener{ *this } };
        GroupAudioCall::GroupAudioCallController m_cGroupAudioCallController{ m_pEventListener };
    };
};

namespace winrt::GroupAudioCallWinUI3Cpp::factory_implementation {
    struct GroupAudioCallViewModel : GroupAudioCallViewModelT<GroupAudioCallViewModel, implementation::GroupAudioCallViewModel> {

    };
}
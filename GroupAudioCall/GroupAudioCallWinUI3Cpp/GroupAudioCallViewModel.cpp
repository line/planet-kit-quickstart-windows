#include "pch.h"

#include "GroupAudioCallViewModel.h"

#if __has_include("GroupAudioCallViewModel.g.cpp")
#include "GroupAudioCallViewModel.g.cpp"
#endif

#include "../src/Constants.h"

namespace winrt::GroupAudioCallWinUI3Cpp::implementation {
    GroupAudioCallViewModel::GroupAudioCallViewModel() {
        m_strUserID = GroupAudioCall::USER_ID;

        m_cGroupAudioCallController.InitializePlanetKit();
    }

    event_token GroupAudioCallViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_PropertyChanged.add(handler);
    }

    void GroupAudioCallViewModel::PropertyChanged(event_token const& token) noexcept {
        m_PropertyChanged.remove(token);
    }

    hstring GroupAudioCallViewModel::ConnectedState() {
        return m_strConnectedState;
    }

    void GroupAudioCallViewModel::ConnectedState(const hstring& strConnectedState) {
        if (m_strConnectedState != strConnectedState) {
            m_strConnectedState = strConnectedState;

            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ConnectedState"));
        }
    }

    int32_t GroupAudioCallViewModel::Participants() {
        return m_nParticipants;
    }

    void GroupAudioCallViewModel::Participants(int32_t nParticipants) {
        if (m_nParticipants != nParticipants) {
            m_nParticipants = nParticipants;

            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"Participants"));
        }
    }

    hstring GroupAudioCallViewModel::UserID() {
        return m_strUserID;
    }

    hstring GroupAudioCallViewModel::RoomID() {
        return m_strRoomID;
    }

    void GroupAudioCallViewModel::RoomID(const hstring& strRoomID) {
        if (m_strRoomID != strRoomID) {
            m_strRoomID = strRoomID;

            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"RoomID"));
        }
    }

    Microsoft::UI::Xaml::Controls::InfoBar GroupAudioCallViewModel::LogMessageInfoBar() {
        return m_logInfoBar;
    }

    void GroupAudioCallViewModel::LogMessageInfoBar(Microsoft::UI::Xaml::Controls::InfoBar logInfoBar) {
        if (m_logInfoBar != logInfoBar) {
            m_logInfoBar = logInfoBar;
        }
    }

    hstring GroupAudioCallViewModel::LogTitle() {
        return m_strLogTitle;
    }

    void GroupAudioCallViewModel::LogTitle(const hstring& strTitle) {
        if (m_strLogTitle != strTitle) {
            m_strLogTitle = strTitle;
            
            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"LogTitle"));
        }
    }

    hstring GroupAudioCallViewModel::LogMessage() {
        return m_strLogMessage;
    }

    void GroupAudioCallViewModel::LogMessage(const hstring& strMessage) {
        if (m_strLogMessage != strMessage) {
            m_strLogMessage = strMessage;

            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"LogMessage"));
        }
    }

    void GroupAudioCallViewModel::OpenLogMessage(const hstring& strTitle, const hstring& strMessage) {
        LogTitle(strTitle);
        LogMessage(strMessage);

        m_logInfoBar.IsOpen(true);
    }

    void GroupAudioCallViewModel::OnClickJoin(::winrt::Windows::Foundation::IInspectable const& sender, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_cGroupAudioCallController.JoinConference(m_strRoomID.c_str());
    }

    void GroupAudioCallViewModel::OnClickLeave(::winrt::Windows::Foundation::IInspectable const& sender, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_cGroupAudioCallController.LeaveConference();
    }
}
#include "pch.h"

#include "EventListener.h"

EventListener::EventListener(winrt::GroupAudioCallWinUI3Cpp::GroupAudioCallViewModel parentViewModel) {
    m_parentViewModel = parentViewModel;
}

void EventListener::FailedToJoin(int nFailReason) {
    winrt::hstring strLogMessage;
    strLogMessage = L"Fail reason(" + std::to_wstring(nFailReason) + L")";

    m_parentViewModel.OpenLogMessage(L"Failed to Join", strLogMessage);

    strLogMessage = L"Failed to Join. " + strLogMessage + L"\n";
    OutputDebugStringW(strLogMessage.c_str());
}

void EventListener::OnConnected() {
    winrt::hstring strConnectedState = L"Connected";
    m_parentViewModel.ConnectedState(strConnectedState);
    m_parentViewModel.Participants(1);

    m_parentViewModel.OpenLogMessage(L"Join", strConnectedState);
    strConnectedState = strConnectedState + L"\n";
    OutputDebugStringW(strConnectedState.c_str());
}

void EventListener::OnDisconnected(std::wstring strDisconnectReason) {
    winrt::hstring strConnectedState;
    strConnectedState = L"Disconnected (" + strDisconnectReason + L")";
    m_parentViewModel.ConnectedState(strConnectedState);
    m_parentViewModel.Participants(0);

    m_parentViewModel.OpenLogMessage(L"Leave", strConnectedState);
    strConnectedState = strConnectedState + L"\n";
    OutputDebugStringW(strConnectedState.c_str());
}

void EventListener::OnPeerListUpdate(size_t nParticipantCount) {
    size_t nTotalCount = nParticipantCount + 1;
    m_parentViewModel.Participants((int32_t)nTotalCount);

    std::wstring strLog = L"Participant count : " + std::to_wstring(nTotalCount);
    m_parentViewModel.OpenLogMessage(L"ParticipantCount is updated", strLog);

    strLog = strLog + L"\n";
    OutputDebugStringW(strLog.c_str());
}
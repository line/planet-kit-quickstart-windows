#pragma once

#include "../src/GroupAudioCallEventListener.h"

#include "GroupAudioCallViewModel.g.h"

class EventListener : public GroupAudioCall::GroupAudioCallEventListener {
public:
    EventListener(winrt::GroupAudioCallWinUI3Cpp::GroupAudioCallViewModel parentViewModel);

    void FailedToJoin(int nFailReason) final;
    void OnConnected() final;
    void OnDisconnected(std::wstring strDisconnectReason) final;
    void OnPeerListUpdate(size_t nParticipantCount) final;

private :
    winrt::GroupAudioCallWinUI3Cpp::GroupAudioCallViewModel m_parentViewModel{ nullptr };
};
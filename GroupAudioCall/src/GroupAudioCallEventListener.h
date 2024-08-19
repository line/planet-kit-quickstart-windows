#pragma once

#include <string>

namespace GroupAudioCall {
    class GroupAudioCallEventListener {
    public :
        virtual void FailedToJoin(int nFailReason) = 0;
        virtual void OnConnected() = 0;
        virtual void OnDisconnected(std::wstring strDisconnectReason) = 0;
        virtual void OnPeerListUpdate(size_t nParticipantCount) = 0;
    };
};
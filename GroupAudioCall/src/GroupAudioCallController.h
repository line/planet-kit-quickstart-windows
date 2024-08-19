#pragma once

#include <memory>
#include <string>

#include "PlanetKit.h"
#include "PlanetKitConference.h"
#include "GroupAudioCallEventListener.h"
#include "ConferenceEventListener.h"

namespace GroupAudioCall {
    class GroupAudioCallController {
    public :
        GroupAudioCallController(std::shared_ptr<GroupAudioCallEventListener> pEventListener);
        void InitializePlanetKit();

        void JoinConference(std::wstring strRoomId);
        void LeaveConference();


        // Events from PlanetKit
        void OnConnected();
        void OnDisconnected(std::wstring strDisconnectReason);
        void OnPeerListUpdate(size_t nParticipantCount);

    private :
        PlanetKit::PlanetKitConferencePtr m_pConference;
        std::shared_ptr<GroupAudioCallEventListener> m_pEventListener{ nullptr };
        ConferenceEventListener m_cEventListener{ this };
    };
};
#pragma once

#include <memory>
#include "PlanetKitManager.h"
#include "IPlanetKitConferenceEvent.h"

namespace GroupAudioCall {
    class GroupAudioCallController;

    class ConferenceEventListener : public PlanetKit::IConferenceEvent {
    public :
        ConferenceEventListener(GroupAudioCallController* pAudioCallController);

        void OnConnected(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceConnectedParamPtr pParam) final;

        void OnDisconnected(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceDisconnectedParamPtr pParam) final;

        void OnPeerListUpdate(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferencePeerUpdateParamPtr pParam) final;

        void OnPeersVideoUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceVideoUpdatedParam* pParam) final;

        void OnPublicSubgroupUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::ConferenceSubgroupUpdatePeerArray& arrSubgroupUpdatePeer) final;

        void OnReceivedShortData(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ShortDataParamPtr pParam) final;

        void OnReceivedPeersHold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerHoldArray& arrPeerHold) final;

        void OnReceivedPeersUnhold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& arrPeer) final;
        
        void OnPeersMicMuted(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& peerArray) final;

        void OnPeersMicUnmuted(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& peerArray) final;

        void OnReceivedMuteMyAudioRequest(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::PeerPtr pPeer, bool bEnableMute) final;

        void OnReceivedAppControlMessage(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const void* pMessage, int nMessageSize) final;

        void OnNetworkUnavailable(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, int nWillDisconnectAfterSec) final;

        void OnNetworkReavailable(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) final;

        void OnDataSessionIncoming(const PlanetKit::WStringOptional& strSubgroupName, PlanetKit::DataSessionStreamIdT nStreamId, PlanetKit::EDataSessionType eType) final;

        void OnPeerScreenShareUpdated(PlanetKit::ConferencePeerScreenShareUpdatedParam* pPeerScreenShareUpdateInfo) final;

        void OnException(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::ConferenceExceptionArray& arrExceptionMessage) final;

        void OnMyVideoNoSource(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) final;

        void OnPeersAudioDescriptionUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, unsigned char ucAverageVolumeLevel, const PlanetKit::PeerAudioDescriptionArray& arrPeer) final;

        void OnMyAudioDescriptionUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::MyAudioDescription& sMyAudioDescription) final;

        void OnMyScreenShareStoppedByHold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) final;

    private :
        GroupAudioCallController* m_pAudioCallController{ nullptr };
    };
};
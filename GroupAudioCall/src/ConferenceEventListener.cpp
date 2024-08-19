// Copyright 2024 LINE Plus Corporation
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

#include <string>

#include "ConferenceEventListener.h"
#include "GroupAudioCallController.h"


namespace GroupAudioCall {
    ConferenceEventListener::ConferenceEventListener(GroupAudioCallController* pAudioCallController) {
        m_pAudioCallController = pAudioCallController;
    }

    void ConferenceEventListener::OnConnected(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceConnectedParamPtr pParam) {
        std::ignore = pPlanetKitConference;
        m_pAudioCallController->OnConnected();
    }

    void ConferenceEventListener::OnDisconnected(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceDisconnectedParamPtr pParam) {
        std::ignore = pPlanetKitConference;
        auto eReason = pParam->GetReason();

        std::wstring strReason;

        switch (eReason) {
        case PlanetKit::PLNK_DISCONNECT_REASON_NORMAL:
            strReason = L"NORMAL";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_DECLINE:
            strReason = L"DECLINE";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_INTERNAL_ERROR:
            strReason = L"INTERNAL_ERROR";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_USER_ERROR:
            strReason = L"USER_ERROR";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_INTERNAL_KIT_ERROR:
            strReason = L"INTERNAL_KIT_ERROR";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_AUDIO_TX_NO_SRC:
            strReason = L"AUDIO_NO_SRC";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_AUTH_ERROR:
            strReason = L"AUTH_ERROR";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVER_INTERNAL_ERROR:
            strReason = L"SERVER_INTERNAL_ERROR";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_UNAVAILABLE_NETWORK:
            strReason = L"UNAVAILABLE_NETWORK";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_APP_DESTROY:
            strReason = L"APP_DESTROY";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SYSTEM_SLEEP:
            strReason = L"SYSTEM_SLEEP";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_MTU_EXCEEDED:
            strReason = L"MTU_EXCEEDED";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_APP_SERVER_DATA_ERROR:
            strReason = L"APP_SERVER_DATA_ERROR";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_ROOM_IS_FULL:
            strReason = L"ROOM_IS_FULL";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_ALONE_KICK_OUT:
            strReason = L"ALONE_KICK_OUT";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_ROOM_NOT_FOUND:
            strReason = L"ROOM_NOT_FOUND";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_ANOTHER_INSTANCE_TRY_TO_JOIN:
            strReason = L"ANOTHER_INSTANCE_TRY_TO_JOIN";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_ACCESS_TOKEN_ERROR:
            strReason = L"SERVICE_ACCESS_TOKEN_ERROR";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_INVALID_ID:
            strReason = L"SERVICE_INVALID_ID";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_MAINTENANCE:
            strReason = L"SERVICE_MAINTENANCE";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_BUSY:
            strReason = L"SERVICE_BUSY";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_INTERNAL_ERROR:
            strReason = L"SERVICE_INTERNAL_ERROR";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_HTTP_INVALID_PEER_CERT:
            strReason = L"SERVICE_HTTP_INVALID_PEER_CERT";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_HTTP_ERROR:
            strReason = L"SERVICE_HTTP_ERROR";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_HTTP_CONNECTION_TIME_OUT:
            strReason = L"SERVICE_HTTP_CONNECTION_TIME_OUT";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_HTTP_CONNECT_FAIL:
            strReason = L"SERVICE_HTTP_CONNECT_FAIL";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_HTTP_INVALID_URL:
            strReason = L"SERVICE_HTTP_INVALID_URL";
            break;

        case PlanetKit::PLNK_DISCONNECT_REASON_SERVICE_INCOMPATIBLE_PLANETKIT_VER:
            strReason = L"SERVICE_INCOMPATIBLE_PLANETKIT_VER";
            break;

        default :
            strReason = L"Unknown error";
        }

        m_pAudioCallController->OnDisconnected(strReason);
    }

    void ConferenceEventListener::OnPeerListUpdate(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferencePeerUpdateParamPtr pParam) {
        std::ignore = pPlanetKitConference;
        m_pAudioCallController->OnPeerListUpdate(pParam->TotalCount());
    }

    void ConferenceEventListener::OnPeersVideoUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceVideoUpdatedParam* pParam) {
        std::ignore = pPlanetKitConference;
        std::ignore = pParam;
    }

    void ConferenceEventListener::OnPublicSubgroupUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::ConferenceSubgroupUpdatePeerArray& arrSubgroupUpdatePeer) {
        std::ignore = pPlanetKitConference;
        std::ignore = arrSubgroupUpdatePeer;
    }

    void ConferenceEventListener::OnReceivedShortData(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ShortDataParamPtr pParam) {
        std::ignore = pPlanetKitConference;
        std::ignore = pParam;
    }

    void ConferenceEventListener::OnReceivedPeersHold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerHoldArray& arrPeerHold) {
        std::ignore = pPlanetKitConference;
        std::ignore = arrPeerHold;
    }

    void ConferenceEventListener::OnReceivedPeersUnhold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& arrPeer) {
        std::ignore = pPlanetKitConference;
        std::ignore = arrPeer;
    }

    void ConferenceEventListener::OnPeersMicMuted(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& peerArray) {
        std::ignore = pPlanetKitConference;
        std::ignore = peerArray;
    }

    void ConferenceEventListener::OnPeersMicUnmuted(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& peerArray) {
        std::ignore = pPlanetKitConference;
        std::ignore = peerArray;
    }

    void ConferenceEventListener::OnReceivedMuteMyAudioRequest(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::PeerPtr pPeer, bool bEnableMute) {
        std::ignore = pPlanetKitConference;
        std::ignore = pPeer;
        std::ignore = bEnableMute;
    }

    void ConferenceEventListener::OnReceivedAppControlMessage(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const void* pMessage, int nMessageSize) {
        std::ignore = pPlanetKitConference;
        std::ignore = pMessage;
        std::ignore = nMessageSize;
    }

    void ConferenceEventListener::OnNetworkUnavailable(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, int nWillDisconnectAfterSec) {
        std::ignore = pPlanetKitConference;
        std::ignore = nWillDisconnectAfterSec;
    }

    void ConferenceEventListener::OnNetworkReavailable(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) {
        std::ignore = pPlanetKitConference;
    }

    void ConferenceEventListener::OnDataSessionIncoming(const PlanetKit::WStringOptional& strSubgroupName, PlanetKit::DataSessionStreamIdT nStreamId, PlanetKit::EDataSessionType eType) {
        std::ignore = strSubgroupName;
        std::ignore = nStreamId;
        std::ignore = eType;
    }

    void ConferenceEventListener::OnPeerScreenShareUpdated(PlanetKit::ConferencePeerScreenShareUpdatedParam* pPeerScreenShareUpdateInfo) {
        std::ignore = pPeerScreenShareUpdateInfo;
    }

    void ConferenceEventListener::OnException(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::ConferenceExceptionArray& arrExceptionMessage) {
        std::ignore = pPlanetKitConference;
        std::ignore = arrExceptionMessage;
    }

    void ConferenceEventListener::OnMyVideoNoSource(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) {
        std::ignore = pPlanetKitConference;
    }

    void ConferenceEventListener::OnPeersAudioDescriptionUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, unsigned char ucAverageVolumeLevel, const PlanetKit::PeerAudioDescriptionArray& arrPeer) {
        std::ignore = pPlanetKitConference;
        std::ignore = ucAverageVolumeLevel;
        std::ignore = arrPeer;
    }

    void ConferenceEventListener::OnMyAudioDescriptionUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::MyAudioDescription& sMyAudioDescription) {
        std::ignore = pPlanetKitConference;
        std::ignore = sMyAudioDescription;
    }

    void ConferenceEventListener::OnMyScreenShareStoppedByHold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) {
        std::ignore = pPlanetKitConference;
    }
};
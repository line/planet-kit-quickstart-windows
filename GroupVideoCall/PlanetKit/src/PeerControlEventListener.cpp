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

#include "PeerControlEventListener.h"
#include <string>

namespace GroupVideoCall {
    PeerControlEventListener::PeerControlEventListener(std::function<void(PlanetKit::PeerControlPtr)> peerControlInitialzied) {
        m_peerControlInitialzied = peerControlInitialzied;
    }

    void PeerControlEventListener::OnInitialized(PlanetKit::PeerControlPtr pPeerControl, bool bResult) {
        if (bResult == true) {
            m_peerControlInitialzied(pPeerControl);
        }
    }

    void PeerControlEventListener::OnMuted(PlanetKit::PeerControlPtr pPeerControl) {
        std::ignore = pPeerControl;
    }

    void PeerControlEventListener::OnUnmuted(PlanetKit::PeerControlPtr pPeerControl) {
        std::ignore = pPeerControl;
    }

    void PeerControlEventListener::OnVideoUpdated(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup, const PlanetKit::VideoStatus& videoStatus) {
        std::ignore = pPeerControl;
        std::ignore = pSubgroup;
        std::ignore = videoStatus;
    }

    void PeerControlEventListener::OnScreenShareUpdated(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup, PlanetKit::EScreenShareState eState) {
        std::ignore = pPeerControl;
        std::ignore = pSubgroup;
        std::ignore = eState;
    }

    void PeerControlEventListener::OnSubscribed(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup) {
        std::ignore = pPeerControl;
        std::ignore = pSubgroup;
    }

    void PeerControlEventListener::OnUnsubscribed(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup) {
        std::ignore = pPeerControl;
        std::ignore = pSubgroup;
    }

    void PeerControlEventListener::OnDisconnected(PlanetKit::PeerControlPtr pPeerControl) {
        std::ignore = pPeerControl;
    }

    void PeerControlEventListener::OnHold(PlanetKit::PeerControlPtr pPeerControl, const PlanetKit::WString& strHoldReason) {
        std::ignore = pPeerControl;
        std::ignore = strHoldReason;
    }

    void PeerControlEventListener::OnUnHold(PlanetKit::PeerControlPtr pPeerControl) {
        std::ignore = pPeerControl;
    }

    void PeerControlEventListener::OnSetSharedContents(PlanetKit::PeerControlPtr pPeerControl, unsigned int unElapsedAfterSet, const void* pvData, unsigned int unDataSize) {
        std::ignore = pPeerControl;
        std::ignore = unElapsedAfterSet;
        std::ignore = pvData;
        std::ignore = unDataSize;
    }

    void PeerControlEventListener::OnUnsetSharedContents(PlanetKit::PeerControlPtr pPeerControl) {
        std::ignore = pPeerControl;
    }

    void PeerControlEventListener::OnPeerAudioDescriptionUpdated(PlanetKit::PeerControlPtr pPeerControl, const PlanetKit::PeerAudioDescription& sPeerAudioDescription) {
        std::ignore = pPeerControl;
        std::ignore = sPeerAudioDescription;
    }
}

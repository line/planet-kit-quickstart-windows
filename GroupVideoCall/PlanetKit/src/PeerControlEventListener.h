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

#pragma once

#include "PlanetKitSubgroup.h"
#include "IPlanetKitPeerControlEvent.h"
#include <functional>

namespace GroupVideoCall {
    class PeerControlEventListener : public PlanetKit::IPeerControlEvent {
    public:
        PeerControlEventListener(std::function<void(PlanetKit::PeerControlPtr)> peerControlInitialzied);

        void OnInitialized(PlanetKit::PeerControlPtr pPeerControl, bool bResult) final;

        void OnMuted(PlanetKit::PeerControlPtr pPeerControl) final;

        void OnUnmuted(PlanetKit::PeerControlPtr pPeerControl) final;

        void OnVideoUpdated(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup, const PlanetKit::VideoStatus& videoStatus) final;

        void OnScreenShareUpdated(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup, PlanetKit::EScreenShareState eState) final;

        void OnSubscribed(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup) final;

        void OnUnsubscribed(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup) final;

        void OnDisconnected(PlanetKit::PeerControlPtr pPeerControl) final;

        void OnHold(PlanetKit::PeerControlPtr pPeerControl, const PlanetKit::WString& strHoldReason) final;

        void OnUnHold(PlanetKit::PeerControlPtr pPeerControl) final;

        void OnSetSharedContents(PlanetKit::PeerControlPtr pPeerControl, unsigned int unElapsedAfterSet, const void* pvData, unsigned int unDataSize) final;

        void OnUnsetSharedContents(PlanetKit::PeerControlPtr pPeerControl) final;

        void OnPeerAudioDescriptionUpdated(PlanetKit::PeerControlPtr pPeerControl, const PlanetKit::PeerAudioDescription& sPeerAudioDescription) final;

    private:
        std::function<void(PlanetKit::PeerControlPtr pPeerControl)> m_peerControlInitialzied;
    };
};
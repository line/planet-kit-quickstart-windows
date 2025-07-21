#include "GroupVideoCallPeerCotroller.h"

namespace GroupVideoCall {
    GroupVideoCallPeerController::GroupVideoCallPeerController(PlanetKit::PeerPtr pPeer) {
        m_pCurrentPeer = pPeer;
    }

    bool GroupVideoCallPeerController::StartVideo(PlanetKit::IVideoReceiverPtr pVideoReceiver, std::function<void(PlanetKit::PeerControlPtr)> peerControlInitialzied) {
        if (nullptr == *m_pCurrentPeerControl) {
            m_pCurrentPeerControl = m_pCurrentPeer->CreatePeerControl();
            if(false == m_pCurrentPeerControl->Register(m_pPeerControlEventListener)) {
                return false;
            }
        }

        m_pVideoReceiver = pVideoReceiver;
        m_pCurrentPeerControl->RegisterReceiver(pVideoReceiver);

        m_peerControlInitialzied = peerControlInitialzied;

        return true;
    }

    bool GroupVideoCallPeerController::StartVideo(HWND hWnd, std::function<void(PlanetKit::PeerControlPtr)> peerControlInitialzied) {
        if (nullptr == *m_pCurrentPeerControl) {
            m_pCurrentPeerControl = m_pCurrentPeer->CreatePeerControl();
            if (false == m_pCurrentPeerControl->Register(m_pPeerControlEventListener)) {
                return false;
            }
        }

        m_pCurrentPeerControl->SetView(hWnd);

        m_peerControlInitialzied = peerControlInitialzied;
        return true;
    }

    bool GroupVideoCallPeerController::StopVideo() {
        bool bRt = false;

        if (*m_pCurrentPeerControl != nullptr) {
            bRt = m_pCurrentPeerControl->StopVideo();
            m_pCurrentPeerControl->ClearView();

            if (m_pVideoReceiver != nullptr) {
                m_pCurrentPeerControl->DeregisterReceiver(m_pVideoReceiver);
            }

            m_pCurrentPeerControl->Unregister();
            m_pCurrentPeerControl = nullptr;
        }

        return bRt;
    }

    void GroupVideoCallPeerController::PeerControlInitialized(PlanetKit::PeerControlPtr pPeerControl) {
        if (m_peerControlInitialzied != nullptr) {
            m_peerControlInitialzied(pPeerControl);
        }
    }
}
#include "PeerListUpdateEventListener.h"
#include "PeerListHandler.h"

PeerListUpdateEventListener::PeerListUpdateEventListener(CWnd* pCwnd) {
    m_pCwnd = pCwnd;
}

void PeerListUpdateEventListener::ChangePeerList(const GroupVideoCall::GroupVideoCallPeerUpdate* pGroupVideoCallUpdate) {
    m_pCwnd->PostMessage(WM_PEER_LIST_UPDATE, (WPARAM)pGroupVideoCallUpdate, 0);
}

#include "PeerListHandler.h"

BEGIN_MESSAGE_MAP(PeerListHandler, CWnd)
    ON_MESSAGE(WM_PEER_LIST_UPDATE, OnPeerListUpdate)
END_MESSAGE_MAP()

PeerListHandler::PeerListHandler(std::shared_ptr<GroupVideoCall::GroupVideoCallPeerList> pGroupCallPeerList) {
    m_pGroupCallPeerList = pGroupCallPeerList;  
}

LRESULT PeerListHandler::OnPeerListUpdate(WPARAM wParam, LPARAM lParam) {
    UpdatePeerListParam* pUpdatePeerListParam = reinterpret_cast<UpdatePeerListParam*>(wParam);

    GroupVideoCall::GroupVideoCallPeerList::GetInstance()->UpdatePeerListParam(pUpdatePeerListParam->pParam);

    delete pUpdatePeerListParam;

    return 0;
}

LRESULT PeerListHandler::OnPeerListClear(WPARAM wParam, LPARAM lParam) {
    GroupVideoCall::GroupVideoCallPeerList::GetInstance()->Clear();

    return 0;
}

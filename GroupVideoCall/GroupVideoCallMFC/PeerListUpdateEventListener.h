#pragma once

#include <afxwin.h> 
#include "../PlanetKit/src/IPeerListUpdateEventListener.h"

class PeerListUpdateEventListener : public GroupVideoCall::IPeerListUpdateEventListener {
public:
    PeerListUpdateEventListener(CWnd* pCwnd);
    virtual void ChangePeerList(const GroupVideoCall::GroupVideoCallPeerUpdate* pGroupVideoCallUpdate);
private:
    CWnd* m_pCwnd{ nullptr };
};
#pragma once

#include "../src/GroupAudioCallEventListener.h"

class CGroupAudioCallMFCDlg;

class EventListener : public GroupAudioCall::GroupAudioCallEventListener {
public :
    EventListener(CGroupAudioCallMFCDlg* pDialog);

    void FailedToJoin(int nFailReason) final;
    void OnConnected() final;
    void OnDisconnected(std::wstring strDisconnectReason) final;
    void OnPeerListUpdate(size_t nParticipantCount) final;

private :
    CGroupAudioCallMFCDlg* m_pParent{ nullptr };
};

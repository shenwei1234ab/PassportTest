#include "ClientSession.h"
#include "vnBase64.h"
#include <time.h>
#include <vector>





void ClientSession::_init() {
	Session::_init();
	//m_tmpToken = sWorldManager.createToken();

}
void ClientSession::_destroy() {
	//sPlayerManager.Disconnect(m_tmpToken);
	/*if(m_playerId) {
		sPlayerManager.PlayerDisconnect(m_playerId);
	}*/

	Session::_destroy();
}

vn::PacketPtr ClientSession::createRequest(vn::u32 pid) {
	return vn::PacketFactory<ClientSession>::instance().createRequest(pid);
}


void ClientSession::setPacketHandler(PacketHandler *handler) {
    std::lock_guard<decltype(m_lock)> guard(m_lock);
    m_handler = handler;
   /* if (!m_link && m_handler) {
        m_handler->notify_closed();
    }*/
}






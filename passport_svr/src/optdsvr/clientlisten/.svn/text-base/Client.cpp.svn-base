/*
 * ClientSession.cpp
 *
 *  Created on: 2015年8月14日
 *      Author: leave
 */

//#include "../../passportsvr/clientlisten/ClientSession.h"

#include "Client.h"
#include "../worker/PreLoggingUser.h"
#include "vnBase64.h"
#include <time.h>
#include <vector>
#include "../manager/WorkQueueManager.h"
#include "../../optdsvr/WorldManager.h"
#include "../manager/WorkQueueManager.h"
using namespace vn;
void Client::_init() {
	Session::_init();
	//m_tmpToken = sWorldManager.createToken();

}
void Client::_destroy() {

	Session::_destroy();
}






void Client::setPacketHandler(PacketHandler *handler) {
    std::lock_guard<decltype(m_lock)> guard(m_lock);
    m_handler = handler;
    /*if (!m_link && m_handler) {
        m_handler->notify_closed();
    }*/
}

//重写
void Client::_connected()
{
	m_handler = vnnew PreLoggingUser(this);
	optd::WorkQueue * wq = WorkQueueManager::instance().selectWorkQueue();
	vnassert(wq);
	optd::WorkerPtr worker(m_handler);
	wq->attach(worker);
}

 void Client::_disconnected()
{
	 //关闭
	 Session::_disconnected();
	 if (m_handler)
	 {
	       m_handler->notify_closed();
	 }

}
//重写passport 获取余额
void Client::_received(vn::LinkBufferPtr buffers)
{
	    m_unpacking.append(std::move(buffers));
	    while (m_unpacking.ready()) {
	        try {
	            u32 pid;
	            m_unpacking >> DataStream::cu(pid);
	            vn::PacketPtr pkt = m_handler->createRequest(pid);
	            if (pkt) {
	                pkt->load(m_unpacking);
	                m_lock.lock();
	                if (m_handler) {
	                    m_handler->postPacket(std::move(pkt));
	                    m_lock.unlock();
	                } else {
	                    m_lock.unlock();
	                    close();
	                    break;
	                }
	            } else {
	                _bad_data();
	                break;
	            }
	        } catch (DataStream::Exception &) {
	            _bad_data();
	            break;
	        }
	    }
}



void Client::_mask(const LinkBufferPtr &buffer)
{
  /*  if (!m_bufferMask) {
        return ;
    }
    for (u8 *p = (u8 *)buffer->lock(), *last = p + buffer->size(); p != last; ++p) {
        *p ^= m_bufferMask;
    }
    buffer->unlock();*/
}

void Client::_bad_data()
{
    //VN_LOG_ERROR("Client: [" << m_link->peer().to_string() << "] bad data!");
    close();
}




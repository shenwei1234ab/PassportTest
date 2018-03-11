//
//  DC_File.cpp
//  optd_srv
//
//  Created by Wayne on 6/23/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "vnLog.h"
#include "DC_File.h"
#include "../database/PassPortDatabaseConnection.h"
#include "vnVariableScript.h"
#include "vnVariableAccessor.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;

DataCenter * DC_File::create() {
    return vnnew DC_File();
}

void DC_File::destroy(DataCenter *p) {
    delete (DC_File *)p;
}



bool DC_File::init()
{
	  m_fs = FileManager::instance().findFileSystem(1001);
	    if (!m_fs) {
	    	//创建一个新的目录
	        m_fs = vnnew FileSystemOS("users/");
	    }
	    m_fs_log = FileManager::instance().findFileSystem(1002);
	    if (!m_fs_log) {
	        m_fs_log = vnnew FileSystemOS("logs/");
	    }
	    std::list<str8> files;
	    if (!m_fs->list(str8(), files)) {
	        return false;
	    }
	    VariableScript vs;
	    for (auto &i : files) {
	        if (i.length() < 4) {
	            continue;
	        }
	        if (strcmp(i.c_str() + i.length() - 4, ".bin")) {
	            continue;
	        }
	        //打开文件(只读二进制rb)
	        FileStream *fs = m_fs->open(i);
	        if (!fs) {
	            continue;
	        }
	        UUID uuid(i.substr(0, i.length() - 4));
	        if (uuid) {
	            LoggedUserPtr user = vnnew LoggedUser(uuid);
	            size_t len = (size_t)fs->size();
	            auto buf = vnmalloc(c8, len);
	            len = fs->read(buf, len);
	            auto root = vs.parse(buf, len, nullptr);
	            vnfree(buf);
	            if (root) {
	                user->load(*root);
	                if (user->restart()) {
	                    _addUser(user);
	                }
	            }
	        }
	        fs->close();
	    }
	//读取当前目录下的server.var
	 m_fs = FileManager::instance().findFileSystem(1001);
	 if (!m_fs)
	 {
	        m_fs = vnnew FileSystemOS();
	  }
	auto fs = m_fs->open("server.var");
		if (fs) {
			size_t size = (size_t)fs->size();
			if (size) {
				c8 *buf = vnmalloc(c8, size);
				size = fs->read(buf, size);
				if (size) {
					VariableScript vs;
					auto obj = vs.parse(buf, size, nullptr);
					if (obj) {
						m_serverData.load(*obj);
					}
				}
				vnfree(buf);
			}
			fs->close();
		}
		_init_redeem_packets();
		return true;
}

void DC_File::_init_redeem_packets() {
    auto fs = m_fs->open("redeem_packets.var");
    if (fs) {
        size_t size = (size_t)fs->size();
        if (size) {
            c8 *buf = vnmalloc(c8, size);
            size = fs->read(buf, size);
            if (size) {
                VariableScript vs;
                auto obj = vs.parse(buf, size, nullptr);
                if (obj) {
                    VariableAccessor<Variable_object> va(obj->queryAttributeObject("packets"));
                    while (va.fetch()) {
                        auto i = va.at<0>();
                        u32 codeId = i->queryAttributeInt32("id");
                        if (codeId) {
                            auto p = vnnew RedeemPacket();
                            p->codeId = i->queryAttributeInt32("id");
                            p->category = i->queryAttributeInt32("category");
                            p->money = i->queryAttributeInt32("money");
                            p->diamonds = i->queryAttributeInt32("diamonds");
                            m_redeemPackets[p->codeId] = p;
                        }
                    }

                    VariableAccessor<Variable_int32, Variable_string> vb(obj->queryAttributeObject("categories"));
                    while (vb.fetch()) {
                        m_redeemPacketCategories[vb.at<0>()->getInt32()] = vb.at<1>()->value();
                    }
                }
            }
            vnfree(buf);
        }
        fs->close();
    }
}

void DC_File::_flush_redeem_packets() {
    auto fs = m_fs->create("redeem_packets.var");
    if (fs) {
        Variable_object obj;
        RefCountedPtr<Variable_object> packets = vnnew Variable_object();
        m_redeemPacketPacketsLock.lock();
        for (auto &i : m_redeemPackets) {
            RefCountedPtr<Variable_object> p = vnnew Variable_object();
            p->setAttribute("id", vnnew Variable_int32(i.second->codeId));
            p->setAttribute("category", vnnew Variable_int32(i.second->category));
            p->setAttribute("money", vnnew Variable_int32(i.second->money));
            p->setAttribute("diamonds", vnnew Variable_int32(i.second->diamonds));
            packets->add(std::move(p));
        }
        m_redeemPacketPacketsLock.unlock();
        obj.setAttribute("packets", std::move(packets));

        RefCountedPtr<Variable_object> categories = vnnew Variable_object();
        m_redeemPacketCategoriesLock.lock();
        for (auto &i : m_redeemPacketCategories) {
            categories->add(vnnew Variable_int32(i.first));
            categories->add(vnnew Variable_string(i.second));
        }
        m_redeemPacketCategoriesLock.unlock();
        obj.setAttribute("categories", std::move(categories));
        auto txt = VariableScript::generate(&obj);
        fs->write(txt.c_str(), txt.length());
        fs->close();
    }
}




LoggedUserPtr DC_File::createUser() {
    UUID uuid;
    uuid.generate();
    LoggedUserPtr user = vnnew LoggedUser(uuid);
    user->_markDirty();
    _addUser(user);
    return std::move(user);
}

LoggedUserPtr DC_File::loadUser(const UUID &uuid) {
    str8 fileName = "users/"+uuid.to_string() + ".bin";
    FileStream *fs = m_fs->open(fileName);
    if (!fs) {
        return LoggedUserPtr();
    }
    LoggedUserPtr user = vnnew LoggedUser(uuid);
    VariableScript vs;
    size_t len = (size_t)fs->size();
    auto buf = vnmalloc(c8, len);
    len = fs->read(buf, len);
    auto root = vs.parse(buf, len, nullptr);
    vnfree(buf);
    if (root) {
        user->load(*root);
    } else {
        user.reset();
    }
    fs->close();
    return std::move(user);
}

//
void DC_File::saveUser(LoggedUserPtr user) {
    str8 fileName = "users/"+user->GetUUId() + ".bin";
    //以读属性打开文件（wb 覆盖原文件）
    FileStream *fs = m_fs->create(fileName);
    if (!fs) {
        VN_LOG_ERROR("DC_File: create file \"" << fileName << "\" failed!");
        return ;
    }
    //user->save(*fs);
    Variable_object obj;
    user->save(obj);
    auto txt = VariableScript::generate(&obj);
    fs->write(txt.c_str(), txt.length());
    fs->close();
}

vn::str8 DC_File::generateOrder(const UUID &uuid, vn::u32 productId, vn::s32 price) {
    UUID order;
    m_ordersLock.lock_write();
    std::lock_guard<ReadWriteLock> guard(m_ordersLock, std::adopt_lock);
    order.generate();
    m_dirty = true;
    return m_orders.insert({order.to_string(), {uuid, productId, price, kOrderStatus_Paying}}).first->first;
}

void DC_File::log_user_beginBattle(const UUID &uuid, u32 levelId, bool intrusion) {
    auto p = vnnew DC_File_EventLog_User_BeginBattle();
    p->levelId = levelId;
    p->intrusion = intrusion;
    _log(p, uuid);
}

void DC_File::log_user_endBattle(const UUID &uuid, u32 levelId, bool intrusion, u32 stars, bool watch, bool shovel) {
    auto p = vnnew DC_File_EventLog_User_EndBattle();
    p->levelId = levelId;
    p->intrusion = intrusion;
    p->stars = (u8)stars;
    p->watch = watch;
    p->shovel = shovel;
    _log(p, uuid);
}




DataCenter::OrderStatus DC_File::queryOrder(const vn::str8 &orderId, UUID &userId, vn::u32 &productId) {
    OrderInfo *info = 0;
    m_ordersLock.lock();
    auto it = m_orders.find(orderId);
    if (it != m_orders.end()) {
        info = &it->second;
    }
    m_ordersLock.unlock();

    if (!info) {
        return kOrderStatus_Invalid;
    }

    userId = info->userId;
    productId = info->productId;
    return info->status;
}

bool DC_File::finishOrder(const vn::str8 &orderId, const UUID &userId, bool confirmed) {
    OrderInfo *info = 0;
    m_ordersLock.lock();
    auto it = m_orders.find(orderId);
    if (it != m_orders.end()) {
        info = &it->second;
    }
    m_ordersLock.unlock();

    if (!info || info->userId != userId) {
        return false;
    }
    if (info->status != kOrderStatus_Paying) {
        return false;
    }
    info->status = (confirmed ? kOrderStatus_Confirmed : kOrderStatus_Invalid);
    m_dirty = true;
    return true;
}












void DC_File::fini()
{
	_flush();
	_flush_log();
	_flush_redeem_packets();
}

//accounts orders m_serverdata写入users文件
void DC_File::_flush() {
    if (!m_fs) {
        return ;
    }
    m_accountsLock.lock();
    auto fs = m_fs->create("accounts");
    if (fs) {
        try {
            *fs << DataStream::ccu(m_accounts.size());
            for (auto &i : m_accounts) {
                *fs << i.first << i.second.low() << i.second.high();
            }
        } catch (DataStream::Exception &) {

        }
        fs->close();
    }
    m_accountsLock.unlock();

    m_ordersLock.lock();
    fs = m_fs->create("orders");
    if (fs) {
        try {
            *fs << DataStream::ccu(m_orders.size());
            for (auto &i : m_orders) {
                *fs << i.first << i.second.userId.low() << i.second.userId.high() << i.second.productId << i.second.price << (int)i.second.status;
            }
        } catch (DataStream::Exception &) {

        }
        fs->close();
    }
    m_ordersLock.unlock();

//    fs = m_fs->create("server");
//    if (fs) {
//        try {
//            m_serverData.save(*fs);
//        } catch(DataStream::Exception &) {
//
//        }
//        fs->close();
//    }
    fs = m_fs->create("server.var");
    if (fs) {
        Variable_object obj;
        m_serverData.save(obj);
        auto txt = VariableScript::generate(&obj);
        fs->write(txt.c_str(), txt.length());
        fs->close();
    }
}
void DC_File::update(f32 deltaTime)
{
	//写入accounts ,orders,server.var 文件中
	 m_timer += deltaTime;
	    if (m_timer >= 120.f) {
	        m_timer = 0;
	        if (m_dirty) {
	            m_dirty = false;
	            _flush();
	        }
	    }

	    static f32 s_log_timer = 0;
	    s_log_timer += deltaTime;
	    if (s_log_timer >= 2.f) {
	        s_log_timer = 0;
	        _flush_log();
	    }
}

void DC_File::saveServerData(const ServerData &data)
{
	 m_serverData = data;
	  m_dirty = true;
}



bool DC_File::loadServerData(ServerData &data)
{
	 data = m_serverData;
	  return true;
}


///////////////////////////////////////////////////////////日志
void DC_File::_log(DC_File_EventLog *el) {
    el->date = DateTime::now();
    std::lock_guard<decltype(m_logs_lock)> guard(m_logs_lock);
    m_logs.push_back(std::unique_ptr<DC_File_EventLog>(el));
}

void DC_File::_log(DC_File_EventLog_User *el, const UUID &uuid) {
    el->userId.first = uuid.low();
    el->userId.second = uuid.high();
    _log(el);
}

void DC_File::_flush_log() {
    decltype(m_logs) logs;
    m_logs_lock.lock();
    m_logs.swap(logs);
    m_logs_lock.unlock();


    if (logs.empty()) {
        return ;
    }

    c8 buf[64];
    Date date = Date::today();
    size_t len = sprintf(buf, "%04u_%02u_%02u.log", date.year, date.month + 1, date.day);

    str8 name {buf, len};
    auto fs = m_fs_log->open(name, false);
    if (!fs) {
        fs = m_fs_log->create(name);
        if (!fs) {
            return ;
        }
    } else {
        fs->seek(0, DataStream::kEnd);
    }

    try {
        for (auto &i : logs) {
            auto p = i.get();
            *fs << DataStream::ccu(p->type());
            p->save(*fs);
        }
    } catch (DataStream::Exception &) {

    }
    fs->close();
}
////////////////////////////////////////////////////////////////用户登录登出日志
void DC_File::log_user_signin(const UUID &uuid) {
    _log(vnnew DC_File_EventLog_User_Signin(), uuid);
}

void DC_File::log_user_login(const UUID &uuid) {
    _log(vnnew DC_File_EventLog_User_Login(), uuid);
}

void DC_File::log_user_logout(const UUID &uuid) {
    _log(vnnew DC_File_EventLog_User_Logout(), uuid);
}


///////////////////////////////////////////////////////////////RedeemPacket
bool DC_File::addRedeemPacket(RedeemPacketPtr redeemPacket) {
    m_redeemPacketPacketsLock.lock_write();
    auto codeId = redeemPacket->codeId;
    auto ret = m_redeemPackets.insert({codeId, std::move(redeemPacket)});
    m_redeemPacketPacketsLock.unlock();
    return ret.second;
}

RedeemPacketPtr DC_File::queryRedeemPacket(u32 codeId) {
    std::lock_guard<ReadWriteLock> guard(m_redeemPacketPacketsLock);
    (void)guard;
    auto it = m_redeemPackets.find(codeId);
    if (it == m_redeemPackets.end()) {
        return nullptr;
    }
    return it->second;
}

void DC_File::removeRedeemPacket(u32 codeId, const UUID &uuid) {
    m_redeemPacketPacketsLock.lock_write();
    auto it = m_redeemPackets.find(codeId);
    if (it != m_redeemPackets.end()) {
        m_redeemPackets.erase(it);
    }
    m_redeemPacketPacketsLock.unlock();
}

str8 DC_File::queryRedeemPacketCategoryName(u32 category) {
    std::lock_guard<ReadWriteLock> guard(m_redeemPacketCategoriesLock);
    (void)guard;
    auto it = m_redeemPacketCategories.find(category);
    if (it == m_redeemPacketCategories.end()) {
        return {};
    }
    return it->second;
}

void DC_File::setRedeemPacketCategory(u32 category, const str8 &name) {
    m_redeemPacketCategoriesLock.lock_write();
    m_redeemPacketCategories[category] = name;
    m_redeemPacketCategoriesLock.unlock();
}


#ifndef __optd_srv__DC_File__
#define __optd_srv__DC_File__

#include "vnFileManager.h"
#include "DataCenter.h"
#include "../Log/DC_File_EventLog.h"
#include "vnReadWriteLock.h"
#include "vnSpinLock.h"
#include <map>

class DC_File : public DataCenter {
public:
    static DataCenter * create();
    static void destroy(DataCenter *p);
    
    virtual bool init()override;
     virtual void fini()override;
     virtual void update(f32 deltaTime)override;
     virtual void saveServerData(const ServerData &data)override;
      virtual bool loadServerData(ServerData &data)override;
		virtual LoggedUserPtr createUser()override ;
		 virtual LoggedUserPtr loadUser(const UUID &uuid)override ;
		 virtual void saveUser(LoggedUserPtr user)override;
		 //订单
		virtual vn::str8 generateOrder(const UUID &uuid, vn::u32 productId, vn::s32 price)override;
		virtual OrderStatus queryOrder(const vn::str8 &orderId, UUID &userId, vn::u32 &productId)override;
		virtual bool finishOrder(const vn::str8 &orderId, const UUID &userId, bool confirmed)override;

		 //记录日志
		void _log(DC_File_EventLog *el);
		void _log(DC_File_EventLog_User *el, const UUID &uuid);
		void _flush_log();


		 virtual void log_user_signin(const UUID &uuid)override;
		virtual void log_user_login(const UUID &uuid)override;
		virtual void log_user_logout(const UUID &uuid)override;
		virtual void log_user_beginBattle(const UUID &uuid, u32 levelId, bool intrusion);
		virtual void log_user_endBattle(const UUID &uuid, u32 levelId, bool intrusion, u32 stars, bool watch, bool shovel);



	virtual bool addRedeemPacket(RedeemPacketPtr redeemPacket)override;
	virtual RedeemPacketPtr queryRedeemPacket(u32 codeId)override;
	virtual void removeRedeemPacket(u32 codeId, const UUID &uuid)override;
	virtual str8 queryRedeemPacketCategoryName(u32 category)override;
	virtual void setRedeemPacketCategory(u32 category, const str8 &name)override;

	virtual ~DC_File(){};

protected:
		    vn::f32 m_timer = 0;
		    struct OrderInfo {
		        UUID userId;
		        u32 productId;
		        s32 price;
		        OrderStatus status;
		    };


      bool m_dirty = false;
      vn::FileSystemPtr m_fs, m_fs_log;
      std::map<str8, UUID> m_accounts;
      vn::ReadWriteLock m_accountsLock;
      std::map<str8, OrderInfo> m_orders;
      vn::ReadWriteLock m_ordersLock;

      ServerData m_serverData;
      void _flush();
      //日志
      std::deque<std::unique_ptr<DC_File_EventLog>> m_logs;
      SpinLock m_logs_lock;

      std::map<u32, str8> m_redeemPacketCategories;
         std::map<u32, RedeemPacketPtr> m_redeemPackets;
         ReadWriteLock m_redeemPacketCategoriesLock, m_redeemPacketPacketsLock;
      void _init_redeem_packets();
       void _flush_redeem_packets();
};

#endif /* defined(__optd_srv__DC_File__) */

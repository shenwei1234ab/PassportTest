//
//  vnCommandList.h
//  vnbase
//
//  Created by Wayne on 14-1-1.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnbase_vnCommandList_h
#define vnbase_vnCommandList_h

#include "vnAllocatedObject.h"

_vn_begin

class CommandList;
class Command {
	friend class CommandList;
public:
	Command();
	
	virtual void _free() = 0;
	virtual void _do() = 0;
	virtual void _undo() = 0;
	virtual void _redo() = 0;
	
private:
	Command *m_next;
	Command *m_prev;
};

class CommandList : public AllocatedObject {
public:
	CommandList(size_t capacity = 0x7FFF);
	~CommandList();
	
	void do_(Command *cmd);
	
	bool canUndo() const;
	bool canRedo() const;
	
	void undo();
	void redo();
	
	void clear();
	
private:
	Command *m_head;
	Command *m_tail;
	Command *m_current;
	size_t m_capacity;
	size_t m_size;
};

_vn_end

#endif

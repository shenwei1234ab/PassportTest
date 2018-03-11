//
//  vnCommandList.cpp
//  vnbase
//
//  Created by Wayne on 14-1-1.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnCommandList.h"
#include "vnAssert.h"

_vn_begin

Command::Command()
: m_next(0), m_prev(0) {
	
}

CommandList::CommandList(size_t capacity)
: m_head(0), m_tail(0)
, m_current(0)
, m_capacity(capacity)
, m_size(0) {
	if (m_capacity == 0) {
		m_capacity = 1;
	}
}

CommandList::~CommandList() {
	clear();
}

void CommandList::do_(Command *cmd) {
	vnassert(!cmd->m_next && !cmd->m_prev);
	if (m_current != m_tail) {
		if (!m_current) {
			clear();
		} else {
			do {
				Command *cmd = m_tail;
				m_tail = m_tail->m_prev;
				--m_size;
				cmd->_free();
			} while (m_current != m_tail);
			m_current->m_next = 0;
		}
	}
	m_current = cmd;
	cmd->m_prev = m_tail;
	if (m_tail) {
		m_tail->m_next = cmd;
	}
	m_tail = cmd;
	if (!m_head) {
		m_head = cmd;
	}
	if (++m_size > m_capacity) {
		Command *cmd = m_head;
		m_head = m_head->m_next;
		m_head->m_prev = 0;
		--m_size;
		cmd->_free();
	}
	cmd->_do();
}

bool CommandList::canUndo() const {
	return m_current != 0;
}

bool CommandList::canRedo() const {
	return m_current != m_tail;
}

void CommandList::undo() {
	if (!m_current) {
		return ;
	}
	Command *cmd = m_current;
	m_current = m_current->m_prev;
	cmd->_undo();
}

void CommandList::redo() {
	if (m_current == m_tail) {
		return ;
	}
	if (m_current) {
		m_current = m_current->m_next;
	} else {
		m_current = m_head;
	}
	m_current->_redo();
}

void CommandList::clear() {
	while ( m_head ) {
		Command *cmd = m_head;
		m_head = m_head->m_next;
		cmd->_free();
	}
	m_tail = m_current = 0;
	m_size = 0;
}

_vn_end

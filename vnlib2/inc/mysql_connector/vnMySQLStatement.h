//
//  vnMySQLStatement.h
//  vnlib
//
//  Created by Wayne on 7/7/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnMySQLStatement_h
#define vnlib_vnMySQLStatement_h

#include "vnMySQLConnection.h"

_vn_begin

class MySQL_Connection;

class MySQL_Statement : public AllocatedObject, public NonCopyable {
    friend class MySQL_Connection;
public:
    ~MySQL_Statement();
    
    size_t param_count() const { return m_inputs.size(); }
    MySQL_InputData * param(size_t index) const { return m_inputs[index].get(); }

    size_t field_count() const { return m_outputs.size(); }
    MySQL_OutputData * field(size_t index) const { return m_outputs[index].get(); }
    
    bool execute(bool errlog = true);
    
    bool fetch();
    
    void free_result();

    u64 affected_rows();
    u64 insert_id();
private:
    MySQL_Statement() = default;
    
    void *m_stmt = 0;
    void *m_paramBinds;
    
    std::vector<MySQL_InputDataPtr> m_inputs;
    std::vector<MySQL_OutputDataPtr> m_outputs;
};

typedef std::unique_ptr<MySQL_Statement> MySQL_StatementPtr;

_vn_end

#endif

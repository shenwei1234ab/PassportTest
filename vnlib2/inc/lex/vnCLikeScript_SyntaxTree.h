//
//  vnCLikeScript_SyntaxTree.h
//  vnlib
//
//  Created by Wayne on 7/1/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnCLikeScript_SyntaxTree_h
#define vnlib_vnCLikeScript_SyntaxTree_h

#include "vnLexLR_Node.h"

#include <vector>

_vn_begin

namespace clks {
    struct ST_Element : RefCounted {
        lex::Token::FilePtr file;
        int file_row = 0, file_col = 0;
        
        void setFileInfo(const lex::Token &token);
    };
    
    struct ST_TypeName : ST_Element {
        enum Type {
            kBasic = 1,
            kContainer,
            kContainerMember,
            kFunction,
            kUser,
            kPointer
        };
        virtual Type type() const = 0;
    };
    
    typedef RefCountedPtr<ST_TypeName> ST_TypeNamePtr;
    
    struct ST_TypeName_Basic : public ST_TypeName {
        virtual Type type() const { return kBasic; }
        enum BasicType {
            kNil = 0,
            kBoolean,
            kInteger,
            kLongInteger,
            kFloat,
            kDouble,
            kString
        } name = kNil;
    };
    
    struct ST_TypeName_Container : public ST_TypeName {
        virtual Type type() const { return kContainer; }
        enum ContainerType {
            kVector = 1,
            kList,
            kArray,
            kMap
        };
        virtual ContainerType containerType() const = 0;
    };
    
    struct ST_TypeName_Vector : ST_TypeName_Container {
        virtual ContainerType containerType() const { return kVector; }
        ST_TypeNamePtr elementType;
    };
    
    struct ST_TypeName_List : ST_TypeName_Vector {
        virtual ContainerType containerType() const { return kList; }
    };
    
    struct ST_TypeName_Map : ST_TypeName_Container {
        virtual ContainerType containerType() const { return kMap; }
        ST_TypeNamePtr keyType;
        ST_TypeNamePtr valueType;
    };
    
    struct ST_TypeName_ContainerMember : ST_TypeName {
        virtual Type type() const { return kContainerMember; }
        RefCountedPtr<ST_TypeName_Container> containerType;
        str8 memberName;
    };
    
    struct ST_ReturnType : ST_Element {
        ST_TypeNamePtr typeName;
        bool reference;
    };
    
    struct ST_ParameterType : ST_ReturnType {
        str8 parameterName;
    };
    
    struct ST_TypeName_Function : ST_TypeName {
        virtual Type type() const { return kFunction; }
        std::vector<RefCountedPtr<ST_ParameterType>> parametersType;
        RefCountedPtr<ST_ReturnType> returnType;
    };
    
    struct ST_Expression : ST_Element {
        enum Type {
            kConstant = 1,
            kIdentifier,
            kUnaryOperation,
            kBinaryOperation,
            kCondition
        };
        virtual Type type() const = 0;
    };
    
    typedef RefCountedPtr<ST_Expression> ST_ExpressionPtr;
    
    struct ST_TypeName_Array : public ST_TypeName_Vector {
        virtual ContainerType containerType() const { return kArray; }
        ST_ExpressionPtr countExpression;
    };
    
    struct ST_Constant : ST_Expression {
        virtual Type type() const { return kConstant; }
        typedef ST_TypeName_Basic::BasicType ConstantType;
        virtual ConstantType constantType() const = 0;
    };
    
    struct ST_Nil : ST_Constant {
        virtual ConstantType constantType() const { return ST_TypeName_Basic::kNil; }
    };
    
    struct ST_Boolean : ST_Constant {
        virtual ConstantType constantType() const { return ST_TypeName_Basic::kBoolean; }
        bool value = false;
    };
    
    struct ST_Integer : ST_Constant {
        virtual ConstantType constantType() const { return ST_TypeName_Basic::kInteger; }
        u32 value = 0;
    };
    
    struct ST_LongInteger : ST_Constant {
        virtual ConstantType constantType() const { return ST_TypeName_Basic::kLongInteger; }
        u64 value = 0;
    };
    
    struct ST_Float : ST_Constant {
        virtual ConstantType constantType() const { return ST_TypeName_Basic::kFloat; }
        f32 value = 0;
    };
    
    struct ST_Double : ST_Constant {
        virtual ConstantType constantType() const { return ST_TypeName_Basic::kDouble; }
        f64 value = 0;
    };
    
    struct ST_String : ST_Constant {
        virtual ConstantType constantType() const { return ST_TypeName_Basic::kString; }
        str8 value = 0;
    };
    
    struct ST_Expression_Identifier : ST_Expression {
        virtual Type type() const { return kIdentifier; }
        enum IdentifierType {
            kId = 0,
            kSelf,
            kSuper
        };
        virtual IdentifierType identifierType() const = 0;
    };
    
    struct ST_Expression_Identifier_Id : ST_Expression_Identifier {
        virtual IdentifierType identifierType() const { return kId; }
        str8 identifier;
    };
    
    struct ST_Expression_Identifier_Self : ST_Expression_Identifier {
        virtual IdentifierType identifierType() const { return kSelf; }
    };
    
    struct ST_Expression_Identifier_Super : ST_Expression_Identifier {
        virtual IdentifierType identifierType() const { return kSuper; }
    };
    
    struct ST_Expression_UnaryOperation : ST_Expression {
        virtual Type type() const { return kUnaryOperation; }
        enum Operation {
            kCastType = 1,
            kFunctionCall,
            kNewFunction,
            kObjectMember,
            kDomainMember,
            kMinus,
            kIncrement,
            kDecrement,
            kPostIncrement,
            kPostDecrement,
            kLogicalNot,
        };
        virtual Operation operation() const = 0;
        
        ST_ExpressionPtr expression;
    };
    
    struct ST_Expression_CastType : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kCastType; }
        ST_ExpressionPtr expression;
        ST_TypeNamePtr typeName;
    };
    
    struct ST_Expression_NewFunction : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kNewFunction; }
    };
    
    struct ST_Expression_ObjectMember : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kObjectMember; }
        str8 memberName;
    };
    
    struct ST_Expression_DomainMember : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kDomainMember; }
        str8 memberName;
    };
    
    struct ST_Expression_Minus : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kMinus; }
    };
    
    struct ST_Expression_Increment : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kIncrement; }
    };
    
    struct ST_Expression_Decrement : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kDecrement; }
    };
    
    struct ST_Expression_PostIncrement : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kPostIncrement; }
    };
    
    struct ST_Expression_PostDecrement : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kPostDecrement; }
    };
    
    struct ST_Expression_LogicalNot : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kLogicalNot; }
    };
    
    struct ST_Expression_FunctionCall : ST_Expression_UnaryOperation {
        virtual Operation operation() const { return kFunctionCall; }
        std::vector<ST_ExpressionPtr> arguments;
    };
    
    struct ST_Expression_BinaryOperation : ST_Expression {
        virtual Type type() const { return kUnaryOperation; }
        enum Operation {
            kUnknown = 0,
            kSubscript,
            kMultiply,
            kDivide,
            kModular,
            kAdd,
            kSubstract,
            kGreaterThan,
            kLessThan,
            kEqual,
            kNotEqual,
            kGreaterThanEqual,
            kLessThanEqual,
            kLogicalAnd,
            kLogicalOr,
            kAssign,
            kAssign_Add,
            kAssign_Sub,
            kAssign_Mul,
            kAssign_Div,
            kAssign_Mod
        } operation = kUnknown;
        
        ST_ExpressionPtr leftExpression, rightExpression;
        
        ST_Expression_BinaryOperation(ST_ExpressionPtr lhs, ST_ExpressionPtr rhs, Operation op)
        : operation(op), leftExpression(std::move(lhs)), rightExpression(std::move(rhs)) {}
    };
    
    struct ST_Expression_Condition : ST_Expression {
        virtual Type type() const { return kCondition; }
        ST_ExpressionPtr condition, truePart, falsePart;
    };
    
}

_vn_end

#endif

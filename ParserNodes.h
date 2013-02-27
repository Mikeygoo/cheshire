/* 
 * File:   nodes.h
 * Author: Michael Goulet
 * Implementation: Nodes.c
 */

#ifndef NODES_H
#define	NODES_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include "ParserEnums.h"

#define PANIC_OR_RETURN_NULL { printf("Couldn't allocate anything. NULL!\n"); exit(0); return NULL; }

struct tagParserTopNode;
struct tagParameterList;
struct tagExpressionNode;
struct tagExpressionList;
struct tagStatementNode;
struct tagBlockList;

typedef int TypeKey;

typedef struct tagCheshireType {
    TypeKey typeKey;
} CheshireType;

typedef struct tagParserTopNode {
    ParserReturnType type;
    union {
        struct {
            char* functionName;
            struct tagCheshireType type;
            struct tagParameterList* params;
            struct tagBlockList* body;
        } method;
        
        //todo: also classes!
    };
} ParserTopNode;

typedef struct tagParameterList {
    struct tagCheshireType type;
    char* name;
    struct tagParameterList* next;
} ParameterList;

typedef struct tagExpressionNode {
    OperationType type;
    union {
        double numberValue;
        struct {
            struct tagExpressionNode* left;
            struct tagExpressionNode* right;
        } binary;
        struct tagExpressionNode* unaryChild;
        char* string;
        struct tagCheshireType typeNode;
        ReservedLiteral reserved;
        /* simple types above */
        struct {
            struct tagExpressionNode* expression;
            char* variable;
        } access;
        
        struct {
            struct tagExpressionNode* expression;
            struct tagCheshireType type;
        } instanceof;
        
        struct {
            struct tagExpressionNode* child;
            struct tagCheshireType type;
        } cast;
        
        struct {
            char* type;
            struct tagExpressionList* params;
        } instantiate;
        
        struct {
            char* fn_name;
            struct tagExpressionList* params;
        } methodcall;
        
        struct {
            struct tagExpressionNode* object;
            char* fn_name;
            struct tagExpressionList* params;
        } objectcall;
        
        struct {
            struct tagExpressionNode* callback;
            struct tagExpressionList* params;
        } callbackcall;
    };
} ExpressionNode;

typedef struct tagExpressionList {
    struct tagExpressionNode* parameter;
    struct tagExpressionList* next;
} ExpressionList;

typedef struct tagStatementNode {
    StatementType type;
    union {
        struct tagExpressionNode* expression;
        struct tagBlockList* block;
        struct {
            struct tagExpressionNode* condition;
            struct tagStatementNode* block;
            struct tagStatementNode* elseBlock;
        } conditional;
        struct {
            struct tagCheshireType type;
            char* variable;
            struct tagExpressionNode* value;
        } varDefinition;
    };
} StatementNode;

typedef struct tagBlockList {
    struct tagStatementNode* statement;
    struct tagBlockList* next;
} BlockList;

//defined in ExpessionNode.c
ExpressionNode* createSelfNode(void);
ExpressionNode* createUnaryOperation(OperationType, ExpressionNode*);
ExpressionNode* createBinOperation(OperationType, ExpressionNode* left, ExpressionNode* right);
ExpressionNode* createInstanceOfNode(ExpressionNode* expression, CheshireType type);
ExpressionNode* createVariableAccess(char* variable);
ExpressionNode* createStringNode(char* str);
ExpressionNode* createNumberNode(double);
ExpressionNode* createCastOperation(ExpressionNode*, CheshireType type);
ExpressionNode* createInstantiationOperation(InstantiationType, char* type, ExpressionList*);
ExpressionNode* createMethodCall(char* functionName, ExpressionList*);
ExpressionNode* createObjectCall(ExpressionNode* object, char* functionName, ExpressionList*);
ExpressionNode* createCallbackCall(ExpressionNode* callback, ExpressionList*);
ExpressionNode* createIncrementOperation(IncrementPrePost, ExpressionNode*, OperationType);
ExpressionNode* createSizeOfExpression(ExpressionNode*);
ExpressionNode* createSizeOfTypeExpression(CheshireType);
ExpressionNode* createReservedLiteralNode(ReservedLiteral);
ExpressionNode* createAccessNode(ExpressionNode*, char* variable);
void deleteExpressionNode(ExpressionNode*);

//defined in ExpressionList.c
ExpressionList* linkExpressionList(ExpressionNode* val, ExpressionList* next);
void deleteExpressionList(ExpressionList*);

//defined in TypeNode.cpp
TypeKey getReservedTypeKey(ReservedType);
TypeKey getTypeKey(char*);
TypeKey getLambdaTypeKey(CheshireType returnType, ParameterList* parameters);
CheshireType getType(TypeKey base, Boolean isUnsafe);


//Defined in StatementNode.c
StatementNode* createExpressionStatement(ExpressionNode*);
StatementNode* createBlockStatement(BlockList*);
StatementNode* createIfStatement(ExpressionNode* condition, StatementNode* ifBlock);
StatementNode* createIfElseStatement(ExpressionNode* condition, StatementNode* ifBlock, StatementNode* elseBlock);
StatementNode* createWhileStatement(ExpressionNode* condition, StatementNode* block);
StatementNode* createVariableDefinition(CheshireType, char* variable, ExpressionNode* value);
void deleteStatementNode(StatementNode*);

//defined in BlockList.c
BlockList* linkBlockList(StatementNode*, BlockList*);
void deleteBlockList(BlockList*);

//defined in ParserTopNode.c
ParserTopNode* createMethodDeclaration(CheshireType, char* functionName, ParameterList* params);
ParserTopNode* createMethodDefinition(CheshireType, char* functionName, ParameterList* params, BlockList* body);
void deleteParserTopNode(ParserTopNode*);

//defined in ParameterList.c
ParameterList* linkParameterList(CheshireType type, char* name, ParameterList* next);
void deleteParameterList(ParameterList*);


#ifdef	__cplusplus
}
#endif

#endif	/* NODES_H */


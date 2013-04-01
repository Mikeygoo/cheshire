/* File: ClassList.c
 * Author: Michael Goulet
 * Implements: ParserNodes.h
 */

#include "ParserNodes.h"

static ClassList* allocClassList(void) {
    ClassList* node = (ClassList*) malloc(sizeof(ClassList));
    
    if (node == NULL)
        PANIC_OR_RETURN_NULL;
    
    return node;
}

ClassList* linkClassList(CheshireType type, char* name, ExpressionNode* defaultValue, ClassList* next) {
    ClassList* node = allocClassList();
    
    if (node == NULL)
        return NULL;
    
    node->name = name;
    node->type = type;
    node->defaultValue = defaultValue;
    node->next = next;
    return node;
}

void deleteClassList(ClassList* node) {
    if (node == NULL)
        return;
    
    free(node->name);
    deleteClassList(node->next);
    deleteExpressionNode(node->defaultValue);
    free(node);
}
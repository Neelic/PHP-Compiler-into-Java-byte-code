#include "ClassStmtType.cpp"

class ClassStmtNode
{
public:
    ClassExprNode* class_expr;
    ClassAccessModList* access_mod;
    FunctionStmtDeclNode* function_stmt_decl;
    IdListNode* id_list;
    ClassStmtDeclNode* class_stmt_decl;
    ClassStmtType type;

    static ClassStmtNode* CreateFromClassExpr(ClassExprNode* class_expr);
    static ClassStmtNode* CreateFromFunctionStmtDecl(ClassAccessModList* access_mod, FunctionStmtDeclNode* function_stmt_decl);
    static ClassStmtNode* CreateFromIdList(IdListNode* id_list);
    static ClassStmtNode* CreateFromClassStmtDecl(ClassStmtDeclNode* class_stmt_decl);
};
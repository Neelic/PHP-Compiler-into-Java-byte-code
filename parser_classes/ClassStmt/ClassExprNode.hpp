#include "ClassExprType.cpp"
#include <string>

class ClassAccessModList;
class GetValueNode;
class ExprNode;
class ConstDeclList;

class ClassExprNode
{
public:
    ClassAccessModList* access_mod_list;
    GetValueNode* get_value;
    std::string* id;
    ExprNode* expr;
    ConstDeclList* const_decl_list;
    ClassExprType type;

    static ClassExprNode* CreateFromGetValueAssign(ClassAccessModList* access_mod_list, GetValueNode* get_value, std::string* id, ExprNode* expr);
    static ClassExprNode* CreateFromGetValue(ClassAccessModList* access_mod_list, GetValueNode* get_value, std::string* id);
    static ClassExprNode* CreateFromConstant(ClassAccessModList* access_mod_list, ConstDeclList* const_decl_list);
};
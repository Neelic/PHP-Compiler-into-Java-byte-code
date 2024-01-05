#include "semantic.h"

vector<string *> sem_errors;
string *file_name;

//Global scope vars
vector<ExprNode *> variables;
//Global scope consts
vector<ConstDeclNode *> consts;
//Global scope functions
vector<FunctionStmtDeclNode *> functions;
//Global scope classes
vector<ClassStmtDeclNode *> classes; //Здесь и в функциях заменил ClassStmtNode на ClassStmtDeclNode, тут же он нужен?
//Global scope interfaces
vector<InterfaceStmtDeclNode *> interfaces;
//Global scope traits
vector<TraitStmtDeclNode *> traits;

void inspectExpr(ExprNode *node, vector<ExprNode *> &variablesScope, const vector<ConstDeclNode *> &constsScope,
                 vector<FunctionStmtDeclNode *> &functionsScope, bool isInClass = false);

void inspectFunction(FunctionStmtDeclNode *node);

void inspectFunctionDef(FunctionDefNode *node);

void inspectExprFunc(ExprFuncNode *node);

void inspectGetValueFunc(GetValueFuncNode *node);

void inspectClass(ClassStmtDeclNode *node);

void inspectClassDef(ClassDefNode *node);

void inspectClassStmt(ClassStmtNode *node);

void inspectClassExpr(ClassExprNode *node);

void inspectClassAccessModList(const vector<ClassAccessModList *> &list);

void inspectStmt(StmtNode *node, vector<ExprNode *> &variablesScope, vector<ConstDeclNode *> &constsScope,
                 vector<FunctionStmtDeclNode *> &functionsScope, bool isInClass = false);

bool isDeclaredVariable(string *id, const vector<ExprNode *> &list) {
    if (id == nullptr) return false;

    return any_of(list.cbegin(), list.cend(),
                  [&id](auto &var) { return *var->id == *id; });
}

bool isDeclaredConst(string *id, const vector<ConstDeclNode *> &list) {
    if (id == nullptr) return false;

    return any_of(list.cbegin(), list.cend(),
                  [&id](auto &var) { return *var->id == *id; });
}

bool isDeclaredFunction(string *id, const vector<FunctionStmtDeclNode *> &list) {
    if (id == nullptr) return false;

    return any_of(list.cbegin(), list.cend(),
                  [&id](auto &var) {
                      return *var->function_def->func_id == *id;
                  });
}

bool isDeclaredClass(string *id, const vector<ClassStmtDeclNode *> &list) {
    if (id == nullptr) return false;

    return any_of(list.cbegin(), list.cend(),
                  [&id](auto &var) {
                      return *var->class_def->class_id == *id;
                  });
}

bool isDeclaredInterface(string *id, const vector<InterfaceStmtDeclNode *> &list) {
    if (id == nullptr) return false;

    return any_of(list.cbegin(), list.cend(),
                  [&id](auto &var) {
                      return *var->expr_definition->id == *id;
                  });
}

bool isDeclaredTrait(string *id) {
    if (id == nullptr) return false;

    return any_of(traits.cbegin(), traits.cend(),
                  [&id](auto &var) {
                      return *var->id == *id;
                  });
}

// Определяет, является ли id стандартным типом
bool isStandartType(string *id) {
    if (id == nullptr) return false;

    // Тут просто сравниваю со всеми стандартными типами, дополнения приветствуются
    return id == new string("int") || id == new string("float") ||
           id == new string("string") || id == new string("bool") ||
           id == new string("array") || id == new string("object");
}

// Определяет, является ли id системной константой
bool isPredeclaredConst(string *id) {
    if (id == nullptr) return false;

    return id == new string("PHP_VERSION") || id == new string("PHP_MAJOR_VERSION") ||
           id == new string("PHP_MINOR_VERSION") || id == new string("PHP_RELEASE_VERSION") ||
           id == new string("PHP_VERSION_ID") || id == new string("PHP_EXTRA_VERSION") ||
           id == new string("PHP_ZTS") || id == new string("PHP_DEBUG") ||
           id == new string("PHP_MAXPATHLEN") || id == new string("PHP_OS") ||
           id == new string("PHP_OS_FAMILY") || id == new string("PHP_SAPI") ||
           id == new string("PHP_EOL") || id == new string("PHP_INT_MAX") ||
           id == new string("PHP_INT_MIN") || id == new string("PHP_INT_SIZE") ||
           id == new string("PHP_FLOAT_DIG") || id == new string("PHP_FLOAT_EPSILON") ||
           id == new string("PHP_FLOAT_MIN") || id == new string("PHP_FLOAT_MAX") ||
           id == new string("DEFAULT_INCLUDE_PATH") || id == new string("PEAR_INSTALL_DIR") ||
           id == new string("PEAR_EXTENSION_DIR") || id == new string("PHP_EXTENSION_DIR") ||
           id == new string("PHP_PREFIX") || id == new string("PHP_BINDIR") ||
           id == new string("PHP_MINOR_VERSION") || id == new string("PHP_RELEASE_VERSION") ||
           id == new string("PHP_BINARY") || id == new string("PHP_MANDIR") ||
           id == new string("PHP_LIBDIR") || id == new string("PHP_DATADIR") ||
           id == new string("PHP_SYSCONFDIR") || id == new string("PHP_LOCALSTATEDIR") ||
           id == new string("PHP_CONFIG_FILE_PATH") || id == new string("PHP_CONFIG_FILE_SCAN_DIR") ||
           id == new string("PHP_SHLIB_SUFFIX") || id == new string("PHP_FD_SETSIZE") ||
           id == new string("E_ERROR") || id == new string("E_WARNING") ||
           id == new string("E_PARSE") || id == new string("E_NOTICE") ||
           id == new string("E_CORE_ERROR") || id == new string("E_CORE_WARNING") ||
           id == new string("E_COMPILE_ERROR") || id == new string("E_COMPILE_WARNING") ||
           id == new string("E_USER_ERROR") || id == new string("E_USER_WARNING") ||
           id == new string("E_USER_NOTICE") || id == new string("E_RECOVERABLE_ERROR") ||
           id == new string("E_DEPRECATED") || id == new string("E_USER_DEPRECATED") ||
           id == new string("E_ALL") || id == new string("E_STRICT") ||
           id == new string("__COMPILER_HALT_OFFSET__") || id == new string("true") ||
           id == new string("false") || id == new string("null") ||
           id == new string("PHP_WINDOWS_EVENT_CTRL_C") || id == new string("PHP_WINDOWS_EVENT_CTRL_BREAK") ||
           id == new string("__LINE__") || id == new string("__FILE__") ||
           id == new string("__DIR__") || id == new string("__FUNCTION__") ||
           id == new string("__CLASS__") || id == new string("__TRAIT__") ||
           id == new string("__METHOD__") || id == new string("__NAMESPACE__") ||
           id == new string("ClassName::class");
}

// Определяет, является ли id системной переменной
bool isPredeclaredVariable(string *id) {
    if (id == nullptr) return false;

    return id == new string("GLOBALS") || id == new string("_SERVER") ||
           id == new string("_GET") || id == new string("_POST") ||
           id == new string("_FILES") || id == new string("_REQUEST") ||
           id == new string("_SESSION") || id == new string("_ENV") ||
           id == new string("_COOKIE") || id == new string("php_errormsg") ||
           id == new string("http_response_header") || id == new string("argc") ||
           id == new string("argv");
}

void inspectGlobalScope(StartNode *node) {
    if (node == nullptr || node->top_stmt_list == nullptr) return;

    for (auto *tmp: node->top_stmt_list->vector) {
        switch (tmp->type) {
            case TopStmtType::stmt_top_type:
                inspectStmt(tmp->stmt, variables, consts, functions);
                break;
            case TopStmtType::class_top_type:
                break;
            case TopStmtType::function_top_type:
                inspectFunction(tmp->func_stmt_decl);
                break;
            case TopStmtType::interface_top_type:
                break;
            case TopStmtType::trait_top_type:
                break;
        }
    }
}


//Functions
void inspectFunction(FunctionStmtDeclNode *node) {

    if (node == nullptr) return;

    if (isDeclaredFunction(node->function_def->func_id, functions))
        throw runtime_error(
                string("Fatal error: Cannot redeclare " + *node->function_def->func_id + " (previously declared in " +
                       *file_name + ") in " + *file_name));

    functions.push_back(node);

    inspectFunctionDef(node->function_def);

    for (auto & i : node->stmt_list->vector) {
        auto tmp1 = vector<ExprNode *>();
        auto tmp2 = vector<ConstDeclNode *>();
        inspectStmt(i, tmp1, tmp2, functions);
    }
}

void inspectFunctionDef(FunctionDefNode *node) {
    if (node == nullptr) return;

    switch (node->type) {
        case FunctionDefType::no_type:
            break;
        case FunctionDefType::with_type:
            if (!isDeclaredClass(node->type_id, classes) &&
                !isDeclaredInterface(node->type_id, interfaces) &&
                !isStandartType(node->type_id))
                throw runtime_error(
                        string("Fatal error: Uncaught Error: Class \"" + *node->type_id + "\" not found in " +
                               *file_name));
            break;
    }

    for (auto & i : node->expr_func_list->vector) {
        inspectExprFunc(i);
    }
}

void inspectExprFunc(ExprFuncNode *node) {
    if (node == nullptr) return;

    inspectGetValueFunc(node->get_value_func);

    switch (node->type) {
        case ExprFuncType::get_value_expr_type:
            break;
        case ExprFuncType::get_value_assign_expr_type:
            auto tmp1 = vector<ExprNode *>();
            auto tmp2 = vector<ConstDeclNode *>();
            inspectExpr(node->expr, tmp1, tmp2, functions);
            break;
    }
}

void inspectGetValueFunc(GetValueFuncNode *node) {
    if (node == nullptr) return;

    if (isPredeclaredVariable(node->id_value))
        throw runtime_error(
                string("Fatal error: Cannot re-assign auto-global variable " + *node->id_value + " in " + *file_name));

    switch (node->type) {
        case GetValueFuncType::no_ref_no_type:
            break;
        case GetValueFuncType::no_ref_with_type:
            if (!isDeclaredClass(node->id_type, classes) &&
                !isDeclaredInterface(node->id_type, interfaces) &&
                !isStandartType(node->id_type))
                throw runtime_error(
                        string("Fatal error: Uncaught Error: Class \"" + *node->id_type + "\" not found in " +
                               *file_name));
            break;
        case GetValueFuncType::with_ref_no_type:
            break;
        case GetValueFuncType::with_ref_with_type:
            if (!isDeclaredClass(node->id_type, classes) &&
                !isDeclaredInterface(node->id_type, interfaces) &&
                !isStandartType(node->id_type))
                throw runtime_error(
                        string("Fatal error: Uncaught Error: Class \"" + *node->id_type + "\" not found in " +
                               *file_name));
            break;
    }
}


//Classes
void inspectClass(ClassStmtDeclNode *node) {
    if (node == nullptr) return;

    if (isDeclaredClass(node->class_def->class_id, classes) ||
        isDeclaredInterface(node->class_def->class_id, interfaces) ||
        isDeclaredTrait(node->class_def->class_id))
        throw runtime_error(string("Fatal error: Cannot declare class " + *node->class_def->class_id +
                                   ", because the name is already in use in " + *file_name));

    inspectClassDef(node->class_def);

    for (auto & i : node->class_stmt_list->vector) {
        inspectClassStmt(i);
    }
}

void inspectClassDef(ClassDefNode *node) {
    if (node == nullptr) return;

    switch (node->type) {
        case ClassDefType::class_def_type:
            break;
        case ClassDefType::extends_type:
            //Проверка, является ли наследуемый id объявленным классом
            if (!isDeclaredClass(node->extend_id, classes)) {
                // Если не является классом, является ли интерфейсом
                if (isDeclaredInterface(node->extend_id, interfaces))
                    throw runtime_error(string("Fatal error: Class " + *node->class_id + " cannot extend interface " +
                                               *node->extend_id + " in " + *file_name));
                    // Или является ли признаком
                else if (isDeclaredTrait(node->extend_id))
                    throw runtime_error(string("Fatal error: Class " + *node->class_id + " cannot extend trait " +
                                               *node->extend_id + " in " + *file_name));
                    // Иначе является неопознанным индентификатором
                else throw runtime_error(
                            string("Fatal error: Uncaught Error: Class \"" + *node->extend_id + "\" not found in " +
                                   *file_name));
            }
            break;
        case ClassDefType::implements_type:
            // Для каждого id в списке 
            for (int i = 0; i < node->impl_id_list->vector.size(); i++) {
                // Является ли объявленным интерфейсом
                if (!isDeclaredInterface(node->impl_id_list->vector[i], interfaces)) {
                    // Если не интерфейс, является ли классом или трейтом
                    if (isDeclaredClass(node->impl_id_list->vector[i], classes) ||
                        isDeclaredTrait(node->impl_id_list->vector[i]))
                        throw runtime_error(string("Fatal error: " + *node->class_id + " cannot implement " +
                                                   *node->impl_id_list->vector[i] + " - it is not an interface in " +
                                                   *file_name));
                        // Иначе неопознанный индентификатор
                    else throw runtime_error(
                                string("Fatal error: Uncaught Error: Interface \"" + *node->impl_id_list->vector[i] +
                                       "\" not found in " + *file_name));
                }
            }
            break;
        case ClassDefType::extends_implements_type:
            //Проверка extends
            if (!isDeclaredClass(node->extend_id, classes)) {
                if (isDeclaredInterface(node->extend_id, interfaces))
                    throw runtime_error(string("Fatal error: Class " + *node->class_id + " cannot extend interface " +
                                               *node->extend_id + " in " + *file_name));
                else if (isDeclaredTrait(node->extend_id))
                    throw runtime_error(string("Fatal error: Class " + *node->class_id + " cannot extend trait " +
                                               *node->extend_id + " in " + *file_name));
                else throw runtime_error(
                            string("Fatal error: Uncaught Error: Class \"" + *node->extend_id + "\" not found in " +
                                   *file_name));
            }
            // Проверка includes
            for (int i = 0; i < node->impl_id_list->vector.size(); i++) {
                if (!isDeclaredInterface(node->impl_id_list->vector[i], interfaces)) {
                    if (isDeclaredClass(node->impl_id_list->vector[i], classes) ||
                        isDeclaredTrait(node->impl_id_list->vector[i]))
                        throw runtime_error(string("Fatal error: " + *node->class_id + " cannot implement " +
                                                   *node->impl_id_list->vector[i] + " - it is not an interface in " +
                                                   *file_name));
                    else throw runtime_error(
                                string("Fatal error: Uncaught Error: Interface \"" + *node->impl_id_list->vector[i] +
                                       "\" not found in " + *file_name));
                }
            }
            break;
    }
}


//Statements
void inspectStmt(StmtNode *node, vector<ExprNode *> &variablesScope, vector<ConstDeclNode *> &constsScope,
                 vector<FunctionStmtDeclNode *> &functionsScope, bool isInClass) {
    if (node == nullptr) return;

    switch (node->type) {
        case StmtType::expr:
            inspectExpr(node->expr_left, variablesScope, constsScope, functionsScope, isInClass);
            break;
        case StmtType::for_stmt:
            break;
        case StmtType::foreach_stmt:
            break;
        case StmtType::do_while_stmt:
            break;
        case StmtType::while_stmt:
            break;
    }
}

void inspectExpr(ExprNode *node, vector<ExprNode *> &variablesScope, const vector<ConstDeclNode *> &constsScope,
                 vector<FunctionStmtDeclNode *> &functionsScope, bool isInClass) {
    if (node == nullptr) return;

    switch (node->exprType) {
        case ExprType::assign_op:
            //if re-assign $this
            if (*node->left->id == "this" && node->get_value != nullptr && node->get_value->count == 1)
                //Throw fatal error
                throw runtime_error(string("Fatal error: Cannot re-assign $this in " + *file_name));
            //if use right-side variable is not declared
            if (node->right->exprType == ExprType::variable && !isDeclaredVariable(node->right->id, variables)) {
                sem_errors.push_back(
                        new string("Warning: Undefined variable $" + *node->right->id + " in " + *file_name));
                node->right = ExprNode::CreateFromAssignOp(node->right, ExprNode::CreateFromNull());
                variables.push_back(node->right->left);
            }

            inspectExpr(node->left, variablesScope, constsScope, functionsScope, isInClass);
            inspectExpr(node->right, variablesScope, constsScope, functionsScope, isInClass);
            break;
        case ExprType::assign_ref_op:
            break;
        case ExprType::class_field_by_ref_op:
            //TODO добавить еще подобных кейз случаев
            break;
        case ExprType::this_keyword:
            if (!isInClass)
                sem_errors.push_back(new string(
                        "Fatal error: Uncaught error: Using $this when in not in object context in " + *file_name));
            break;
    }
}

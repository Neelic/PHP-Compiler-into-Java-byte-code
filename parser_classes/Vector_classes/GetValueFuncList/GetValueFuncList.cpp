#include "GetValueFuncList.hpp"

int GetValueFuncList::GLOBAL_ID = 0;

GetValueFuncList *GetValueFuncList::CreateNode(GetValueFuncNode *node) {
    auto *tmp = new GetValueFuncList();
    if (node != nullptr)
        tmp->vector.push_back(node);
    tmp->cur_id = GetValueFuncList::GLOBAL_ID++;
    return tmp;
}

std::string *GetValueFuncList::idTag() const {
    auto* tmp = new std::string(std::string("GetValueFuncList") + std::to_string(this->cur_id));
    return tmp;
}


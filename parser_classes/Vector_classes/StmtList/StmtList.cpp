#include "StmtList.hpp"

int StmtList::GLOBAL_ID = 0;

StmtList *StmtList::CreateNode(StmtNode *node) {
    auto *tmp = new StmtList();
    if (node != nullptr)
        tmp->vector.push_back(node);
    tmp->cur_id = StmtList::GLOBAL_ID++;
    return tmp;
}

std::string *StmtList::idTag() const {
    auto* tmp = new std::string(std::string("StmtList") + std::to_string(this->cur_id));
    return tmp;
}

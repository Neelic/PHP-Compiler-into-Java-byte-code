#include <vector>

class CaseDefaultStmtNode;

class CaseDefaultStmtList {
public:
    static int GLOBAL_ID;
    int cur_id;
    std::vector<CaseDefaultStmtNode *> vector;

    static CaseDefaultStmtList *CreateNode(CaseDefaultStmtNode *node);
};
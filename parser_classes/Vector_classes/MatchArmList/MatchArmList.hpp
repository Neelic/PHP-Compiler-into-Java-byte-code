#include <vector>

class MatchArmNode;

class MatchArmList
{
public:
  static int GLOBAL_ID;
  int cur_id;
  std::vector<MatchArmNode*>* vector;

  static MatchArmList* CreateNode(std::vector<MatchArmNode*>* vector);
  void Insert(MatchArmNode* element);
};
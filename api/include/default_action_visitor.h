#include "iaction_visitor.h"

namespace api {

class DefaultActionVisitor : public IActionVisitor {
 public:
  void Visit(ClickElementAction* action) override;
  void Visit(EmulateMouseMoveAction* action) override;
  void Visit(ExtractFieldFromUrlAction* action) override;
  void Visit(GetElementByCssSelectorAction* action) override;
  void Visit(GetElementByXpathAction* action) override;
  void Visit(GetImagesByXpathAction* action) override;
  void Visit(GoToUrlAction* action) override;
  void Visit(KeyPressAction* action) override;
  void Visit(RandomPauseAction* action) override;
  void Visit(RunScriptAction* action) override;
  void Visit(SetCookiesAction* action) override;
  void Visit(TypeTextOnElementByXpathAction* action) override;
  void Visit(WaitForElementByXpathAction* action) override;
};

}
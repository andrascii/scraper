#pragma once

namespace api {

class ClickElementAction;
class EmulateMouseMoveAction;
class ExtractFieldFromUrlAction;
class GetElementByCssSelectorAction;
class GetElementByXpathAction;
class GetImagesByXpathAction;
class GoToUrlAction;
class KeyPressAction;
class RandomPauseAction;
class RunScriptAction;
class SetCookiesAction;
class TypeTextOnElementByXpathAction;
class WaitForElementByXpathAction;

class IActionVisitor {
 public:
  virtual ~IActionVisitor() = default;

  virtual void Visit(ClickElementAction* action) = 0;
  virtual void Visit(EmulateMouseMoveAction* action) = 0;
  virtual void Visit(ExtractFieldFromUrlAction* action) = 0;
  virtual void Visit(GetElementByCssSelectorAction* action) = 0;
  virtual void Visit(GetElementByXpathAction* action) = 0;
  virtual void Visit(GetImagesByXpathAction* action) = 0;
  virtual void Visit(GoToUrlAction* action) = 0;
  virtual void Visit(KeyPressAction* action) = 0;
  virtual void Visit(RandomPauseAction* action) = 0;
  virtual void Visit(RunScriptAction* action) = 0;
  virtual void Visit(SetCookiesAction* action) = 0;
  virtual void Visit(TypeTextOnElementByXpathAction* action) = 0;
  virtual void Visit(WaitForElementByXpathAction* action) = 0;
};

}
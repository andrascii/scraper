#include "default_action_visitor.h"

namespace api {

void DefaultActionVisitor::Visit(ClickElementAction*) {}
void DefaultActionVisitor::Visit(EmulateMouseMoveAction*) {}
void DefaultActionVisitor::Visit(ExtractFieldFromUrlAction*) {}
void DefaultActionVisitor::Visit(GetElementByCssSelectorAction*) {}
void DefaultActionVisitor::Visit(GetElementByXpathAction*) {}
void DefaultActionVisitor::Visit(GetImagesByXpathAction*) {}
void DefaultActionVisitor::Visit(GoToUrlAction*) {}
void DefaultActionVisitor::Visit(KeyPressAction*) {}
void DefaultActionVisitor::Visit(RandomPauseAction*) {}
void DefaultActionVisitor::Visit(RunScriptAction*) {}
void DefaultActionVisitor::Visit(SetCookiesAction*) {}
void DefaultActionVisitor::Visit(TypeTextOnElementByXpathAction*) {}
void DefaultActionVisitor::Visit(WaitForElementByXpathAction*) {}

}
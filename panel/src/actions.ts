export interface ActionBase {
  optional: boolean;
}

export interface ClickElementAction extends ActionBase {
  type: 'click-element';
  xPath: string;
}

export interface EmulateMouseMoveAction extends ActionBase {
  type: 'emulate-mouse-move';
}

export interface ExtractFieldFromUrlAction extends ActionBase {
  type: 'extract-field-from-url';
  regExp: string;
  outField: string;
}

export interface GetElementByCssSelectorAction extends ActionBase {
  type: 'get-element-by-css-selector';
  selector: string;
  outField: string;
}

export interface GetElementByXpathAction extends ActionBase {
  type: 'get-element-by-xpath';
  xPath: string;
  outField: string;
}

export interface GetImagesByXpathAction extends ActionBase {
  type: 'get-images-by-xpath';
  xPath: string;
  outField: string;
}

export interface GoToUrlAction extends ActionBase {
  type: 'go-to-url';
  url: string;
}

export interface KeyPressAction extends ActionBase {
  type: 'key-press';
  keyCode: number;
}

export interface RandomPauseAction extends ActionBase {
  type: 'random-pause';
  fromMs: number;
  toMs: number;
}

export interface RunScriptAction extends ActionBase {
  type: 'run-script';
  script: string;
}

export interface SetCookiesAction extends ActionBase {
  type: 'set-cookies';
  name: string;
  value: string;
}

export interface TypeTextOnElementByXpathAction extends ActionBase {
  type: 'type-text-on-element-by-xpath';
  text: string;
  xPath: string;
}

export interface WaitForElementByXpathAction extends ActionBase {
  type: 'wait-for-element-by-xpath';
  maxTimeoutMsecs: number;
  xPath: string;
}

export type BrowserAction =
  ClickElementAction |
  EmulateMouseMoveAction |
  ExtractFieldFromUrlAction |
  GetElementByCssSelectorAction |
  GetElementByXpathAction |
  GetImagesByXpathAction |
  GoToUrlAction |
  KeyPressAction |
  RandomPauseAction |
  RunScriptAction |
  SetCookiesAction |
  TypeTextOnElementByXpathAction |
  WaitForElementByXpathAction;

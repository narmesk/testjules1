program PriceTuningTool;

uses
  Vcl.Forms,
  MainForm in 'MainForm.pas' {FormMain},
  VirtualKeyboardForm in 'VirtualKeyboardForm.pas' {VKForm},
  SecurityLogic in 'SecurityLogic.pas';

{$R *.res}

begin
  if not CheckTimeLimit then
    Exit;

  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TFormMain, FormMain);
  Application.CreateForm(TVKForm, VKForm);
  Application.Run;
end.

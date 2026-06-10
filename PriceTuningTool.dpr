program PriceTuningTool;

uses
  Vcl.Forms,
  MainForm in 'MainForm.pas' {FormMain},
  VirtualKeyboardForm in 'VirtualKeyboardForm.pas' {VKForm};

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TFormMain, FormMain);
  Application.CreateForm(TVKForm, VKForm);
  Application.Run;
end.

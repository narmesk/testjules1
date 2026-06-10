unit VirtualKeyboardForm;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls;

type
  TVKForm = class(TForm)
    EditValue: TEdit;
    PanelKeys: TPanel;
    Btn1: TPanel;
    Btn2: TPanel;
    Btn3: TPanel;
    Btn4: TPanel;
    Btn5: TPanel;
    Btn6: TPanel;
    Btn7: TPanel;
    Btn8: TPanel;
    Btn9: TPanel;
    Btn0: TPanel;
    BtnComma: TPanel;
    BtnClear: TPanel;
    BtnEnter: TPanel;
    BtnCancel: TPanel;
    procedure PanelBtnClick(Sender: TObject);
    procedure BtnClearClick(Sender: TObject);
    procedure BtnEnterClick(Sender: TObject);
    procedure BtnCancelClick(Sender: TObject);
    procedure BtnCommaClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    function Execute(var AValue: string): Boolean;
  end;

var
  VKForm: TVKForm;

implementation

{$R *.dfm}

function TVKForm.Execute(var AValue: string): Boolean;
begin
  EditValue.Text := AValue;
  Result := (ShowModal = mrOk);
  if Result then
    AValue := EditValue.Text;
end;

procedure TVKForm.PanelBtnClick(Sender: TObject);
var
  Caption: string;
begin
  Caption := (Sender as TPanel).Caption;
  if EditValue.Text = '0,00' then
    EditValue.Text := Caption
  else
    EditValue.Text := EditValue.Text + Caption;
end;

procedure TVKForm.BtnCommaClick(Sender: TObject);
begin
  if Pos(',', EditValue.Text) = 0 then
    EditValue.Text := EditValue.Text + ',';
end;

procedure TVKForm.BtnClearClick(Sender: TObject);
begin
  EditValue.Text := '0,00';
end;

procedure TVKForm.BtnEnterClick(Sender: TObject);
begin
  ModalResult := mrOk;
end;

procedure TVKForm.BtnCancelClick(Sender: TObject);
begin
  ModalResult := mrCancel;
end;

end.

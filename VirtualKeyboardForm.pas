unit VirtualKeyboardForm;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls;

type
  TVKForm = class(TForm)
    EditValue: TEdit;
    GridPanel1: TGridPanel;
    Btn1: TButton;
    Btn2: TButton;
    Btn3: TButton;
    Btn4: TButton;
    Btn5: TButton;
    Btn6: TButton;
    Btn7: TButton;
    Btn8: TButton;
    Btn9: TButton;
    Btn0: TButton;
    BtnComma: TButton;
    BtnClear: TButton;
    BtnEnter: TButton;
    BtnCancel: TButton;
    procedure BtnNumberClick(Sender: TObject);
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

procedure TVKForm.BtnNumberClick(Sender: TObject);
begin
  if EditValue.Text = '0,00' then
    EditValue.Text := TButton(Sender).Caption
  else
    EditValue.Text := EditValue.Text + TButton(Sender).Caption;
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

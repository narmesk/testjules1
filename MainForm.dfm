object FormMain: TFormMain
  Left = 0
  Top = 0
  Caption = 'Price Tuning Tool'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  OnCreate = FormCreate
  TextHeight = 15
  object MemoJSON: TMemo
    Left = 0
    Top = 0
    Width = 624
    Height = 384
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Consolas'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
    ExplicitWidth = 620
    ExplicitHeight = 383
  end
  object PanelBottom: TPanel
    Left = 0
    Top = 384
    Width = 624
    Height = 57
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 383
    ExplicitWidth = 620
    object LabelIncrement: TLabel
      Left = 184
      Top = 20
      Width = 83
      Height = 15
      Caption = 'Artis Miktari:'
    end
    object BtnLoad: TButton
      Left = 8
      Top = 15
      Width = 75
      Height = 25
      Caption = 'JSON Yukle'
      TabOrder = 0
      OnClick = BtnLoadClick
    end
    object BtnProcess: TButton
      Left = 400
      Top = 15
      Width = 100
      Height = 25
      Caption = 'Hesapla && Duzenle'
      TabOrder = 1
      OnClick = BtnProcessClick
    end
    object BtnSave: TButton
      Left = 89
      Top = 15
      Width = 75
      Height = 25
      Caption = 'Kaydet'
      TabOrder = 2
      OnClick = BtnSaveClick
    end
    object EditIncrement: TEdit
      Left = 273
      Top = 17
      Width = 80
      Height = 23
      TabOrder = 3
      Text = '0.5'
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'json'
    Filter = 'JSON Dosyaları (*.json)|*.json|Tüm Dosyalar (*.*)|*.*'
    Left = 528
    Top = 32
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'json'
    Filter = 'JSON Dosyaları (*.json)|*.json|Tüm Dosyalar (*.*)|*.*'
    Left = 528
    Top = 88
  end
end

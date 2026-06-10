object FormMain: TFormMain
  Left = 0
  Top = 0
  Caption = 'Fiyat Ayarlama Aracı'
  ClientHeight = 500
  ClientWidth = 650
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  OnCreate = FormCreate
  TextHeight = 15
  object StringGridPrices: TStringGrid
    Left = 0
    Top = 0
    Width = 650
    Height = 443
    Align = alClient
    ColCount = 4
    FixedCols = 0
    RowCount = 17
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs]
    TabOrder = 0
    ColWidths = (
      150
      100
      100
      100)
  end
  object PanelBottom: TPanel
    Left = 0
    Top = 443
    Width = 650
    Height = 57
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object LabelIncrement: TLabel
      Left = 216
      Top = 20
      Width = 71
      Height = 15
      Caption = 'Artış Miktarı:'
    end
    object BtnLoad: TButton
      Left = 8
      Top = 15
      Width = 90
      Height = 25
      Caption = 'JSON Yükle'
      TabOrder = 0
      OnClick = BtnLoadClick
    end
    object BtnProcess: TButton
      Left = 410
      Top = 15
      Width = 110
      Height = 25
      Caption = 'Hesapla'
      TabOrder = 1
      OnClick = BtnProcessClick
    end
    object BtnSave: TButton
      Left = 104
      Top = 15
      Width = 90
      Height = 25
      Caption = 'Kaydet'
      TabOrder = 2
      OnClick = BtnSaveClick
    end
    object EditIncrement: TEdit
      Left = 300
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

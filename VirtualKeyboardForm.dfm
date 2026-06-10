object VKForm: TVKForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Virtual Keyboard'
  ClientHeight = 400
  ClientWidth = 300
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poMainFormCenter
  TextHeight = 21
  object EditValue: TEdit
    Left = 10
    Top = 10
    Width = 280
    Height = 33
    Alignment = taRightJustify
    ReadOnly = True
    TabOrder = 0
    Text = '0,00'
  end
  object GridPanel1: TGridPanel
    Left = 10
    Top = 50
    Width = 280
    Height = 340
    BevelOuter = bvNone
    ColumnCollection = <
      item
        Value = 33.333333333333330000
      end
      item
        Value = 33.333333333333330000
      end
      item
        Value = 33.333333333333340000
      end>
    ControlCollection = <
      item
        Column = 0
        Control = Btn7
        Row = 0
      end
      item
        Column = 1
        Control = Btn8
        Row = 0
      end
      item
        Column = 2
        Control = Btn9
        Row = 0
      end
      item
        Column = 0
        Control = Btn4
        Row = 1
      end
      item
        Column = 1
        Control = Btn5
        Row = 1
      end
      item
        Column = 2
        Control = Btn6
        Row = 1
      end
      item
        Column = 0
        Control = Btn1
        Row = 2
      end
      item
        Column = 1
        Control = Btn2
        Row = 2
      end
      item
        Column = 2
        Control = Btn3
        Row = 2
      end
      item
        Column = 0
        Control = Btn0
        Row = 3
      end
      item
        Column = 1
        Control = BtnComma
        Row = 3
      end
      item
        Column = 2
        Control = BtnClear
        Row = 3
      end
      item
        Column = 0
        Control = BtnCancel
        Row = 4
      end
      item
        Column = 1
        ColumnSpan = 2
        Control = BtnEnter
        Row = 4
      end>
    RowCollection = <
      item
        Value = 20.000000000000000000
      end
      item
        Value = 20.000000000000000000
      end
      item
        Value = 20.000000000000000000
      end
      item
        Value = 20.000000000000000000
      end
      item
        Value = 20.000000000000000000
      end>
    TabOrder = 1
    object Btn7: TButton
      Left = 0
      Top = 0
      Width = 93
      Height = 68
      Align = alClient
      Caption = '7'
      TabOrder = 0
      OnClick = BtnNumberClick
    end
    object Btn8: TButton
      Left = 93
      Top = 0
      Width = 93
      Height = 68
      Align = alClient
      Caption = '8'
      TabOrder = 1
      OnClick = BtnNumberClick
    end
    object Btn9: TButton
      Left = 186
      Top = 0
      Width = 94
      Height = 68
      Align = alClient
      Caption = '9'
      TabOrder = 2
      OnClick = BtnNumberClick
    end
    object Btn4: TButton
      Left = 0
      Top = 68
      Width = 93
      Height = 68
      Align = alClient
      Caption = '4'
      TabOrder = 3
      OnClick = BtnNumberClick
    end
    object Btn5: TButton
      Left = 93
      Top = 68
      Width = 93
      Height = 68
      Align = alClient
      Caption = '5'
      TabOrder = 4
      OnClick = BtnNumberClick
    end
    object Btn6: TButton
      Left = 186
      Top = 68
      Width = 94
      Height = 68
      Align = alClient
      Caption = '6'
      TabOrder = 5
      OnClick = BtnNumberClick
    end
    object Btn1: TButton
      Left = 0
      Top = 136
      Width = 93
      Height = 68
      Align = alClient
      Caption = '1'
      TabOrder = 6
      OnClick = BtnNumberClick
    end
    object Btn2: TButton
      Left = 93
      Top = 136
      Width = 93
      Height = 68
      Align = alClient
      Caption = '2'
      TabOrder = 7
      OnClick = BtnNumberClick
    end
    object Btn3: TButton
      Left = 186
      Top = 136
      Width = 94
      Height = 68
      Align = alClient
      Caption = '3'
      TabOrder = 8
      OnClick = BtnNumberClick
    end
    object Btn0: TButton
      Left = 0
      Top = 204
      Width = 93
      Height = 68
      Align = alClient
      Caption = '0'
      TabOrder = 9
      OnClick = BtnNumberClick
    end
    object BtnComma: TButton
      Left = 93
      Top = 204
      Width = 93
      Height = 68
      Align = alClient
      Caption = ','
      TabOrder = 10
      OnClick = BtnCommaClick
    end
    object BtnClear: TButton
      Left = 186
      Top = 204
      Width = 94
      Height = 68
      Align = alClient
      Caption = 'CLR'
      TabOrder = 11
      OnClick = BtnClearClick
    end
    object BtnCancel: TButton
      Left = 0
      Top = 272
      Width = 93
      Height = 68
      Align = alClient
      Caption = 'X'
      TabOrder = 12
      OnClick = BtnCancelClick
    end
    object BtnEnter: TButton
      Left = 93
      Top = 272
      Width = 187
      Height = 68
      Align = alClient
      Caption = 'ENTER'
      TabOrder = 13
      OnClick = BtnEnterClick
    end
  end
end

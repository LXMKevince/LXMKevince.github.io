Blockly.Blocks['relay_highlow'] = {
   init: function() {
    this.setColour(200);
    this.appendDummyInput("")
        .appendField(new Blockly.FieldDropdown([["开", "LOW"], ["合", "HIGH"]]), 'BOOL')
    this.setOutput(true, Boolean);
    this.setTooltip("返回继电器开合状态s");
  }
};
Blockly.Blocks.Relay_write2 = {
  init: function() {
    this.setColour(200);
    this.appendValueInput("PIN", Number)
        .appendField("继电器 管脚#")
        .setCheck(Number);
    this.appendValueInput("STAT")
        .appendField("设为")
        .setCheck([Number,Boolean]);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setInputsInline(true);
    this.setTooltip("设定指定管脚继电器开合状态");
  }
};


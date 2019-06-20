//RFID
Blockly.Blocks.RFID_init={
	init: function() {
    this.setColour(Blockly.Blocks.blockgroup.HUE1);
	this.appendDummyInput("")
	    .appendTitle("初始化rfid为")
		.appendTitle("SDA")
		.appendTitle(new Blockly.FieldDropdown([["10", "10"]]), "PORT1")
	    .appendTitle("SCK")
		.appendTitle(new Blockly.FieldDropdown([["13", "13"]]), "PORT2")
		.appendTitle("MOSI")
		.appendTitle(new Blockly.FieldDropdown([["11", "11"]]), "PORT3")
		.appendTitle("MISO")
	    .appendTitle(new Blockly.FieldDropdown([["12", "12"]]), "PORT4");
	this.setPreviousStatement(true);
    this.setNextStatement(true);
	//this.setOutput(true, Number);
	//var thisBlock = this;
   
  }
};

Blockly.Blocks.RFID_on = {
  init: function () {
    this.appendDummyInput("")
      .appendTitle("RFID")
      .appendTitle("侦测到信号");
    this.appendStatementInput("do_");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setColour(Blockly.Blocks.blockgroup.HUE1);
    this.setTooltip('');
  }
};


//读卡号
Blockly.Blocks.RFID_readcardnum={
	init: function() {
    this.setColour(Blockly.Blocks.blockgroup.HUE1);
	this.appendDummyInput("")
	    .appendTitle("rfid所读卡的卡号")
	this.setOutput(true, String);
	var thisBlock = this;
   
  }
};

//串口打印卡号
/* Blockly.Blocks.RFID_serialprintcardnum = {
   init: function() {
    this.setColour(Blockly.Blocks.blockgroup.HUE1);
    this.appendDummyInput("")
		.appendTitle(new Blockly.FieldDropdown(profile.default.serial_select), "serial_select")
        .appendTitle('打印RFID卡号');
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
  }
}; */


Blockly.Blocks.RFID_in = {
  init: function () {
    this.appendValueInput("uid_")
      .appendTitle("如果")
      .appendTitle('RFID')
      .appendTitle("侦测到的卡号为");
    this.appendStatementInput("do_")
      .appendTitle("执行");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
    this.setColour(Blockly.Blocks.blockgroup.HUE1);
  }
};

//写数据块
Blockly.Blocks.RFID_writecarddata={
	init: function() {
    this.setColour(Blockly.Blocks.blockgroup.HUE1);
	this.appendValueInput("address1")
      .appendTitle("RFID写卡")
      .appendTitle('数据块')
	this.appendDummyInput("")
      .appendTitle("写入数据为")
	  .appendTitle(new Blockly.FieldTextInput('mylist'), 'data1')
	this.setPreviousStatement(true);
    this.setNextStatement(true);
	this.setTooltip('这是RFID的读卡');
  }
};


//读数据块的内容
Blockly.Blocks.RFID_readcarddata={
	init: function() {
    this.setColour(Blockly.Blocks.blockgroup.HUE1);
	this.appendValueInput("address")
	    .appendTitle("RFID读取")
		.appendTitle("数据块")
	this.appendDummyInput("")
		.appendTitle("的数据")
	this.setOutput(true, Number);
	var thisBlock = this;
   
  }
};

/* //串口打印数据内容
Blockly.Blocks.RFID_serialprintcarddata = {
   init: function() {
    this.setColour(Blockly.Blocks.blockgroup.HUE1);
    this.appendValueInput("address")
		.appendTitle(new Blockly.FieldDropdown(profile.default.serial_select), "serial_select")
        .appendTitle("打印RFID数据块");
	this.appendDummyInput("")
	    .appendTitle("内容")
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
  }
}; */

//关闭RFID
Blockly.Blocks.RFID_off={
	init: function() {
    this.setColour(Blockly.Blocks.blockgroup.HUE1);
	this.appendDummyInput("")
	    .appendTitle("rfid停止侦测")
	//this.setOutput(true, Number);
	this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
	var thisBlock = this;
   
  }
};
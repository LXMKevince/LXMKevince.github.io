Blockly.Arduino.RFID_init = function() {
	
	Blockly.Arduino.definitions_['define_SPI'] = '#include <SPI.h>';
	Blockly.Arduino.definitions_['define_RFID'] = '#include <RFID.h>';		
	Blockly.Arduino.definitions_['define_origin'] = 'RFID rfid(10,5);';
	Blockly.Arduino.definitions_['define_i and tmp'] = 'unsigned char i,tmp;';
	Blockly.Arduino.definitions_['define_status'] = 'unsigned char status;';		
	Blockly.Arduino.definitions_['define_strmax'] = 'unsigned char str[MAX_LEN];';
	Blockly.Arduino.definitions_['define_RC_size'] = 'unsigned char RC_size;';
	Blockly.Arduino.definitions_['define_blockAddr'] = 'unsigned char blockAddr;        //选择操作的块地址0～63';		

	Blockly.Arduino.definitions_['define_1'] = '//4字节卡序列号，第5字节为校验字节';
	Blockly.Arduino.definitions_['define_2'] = 'unsigned char serNum[5];';		
	Blockly.Arduino.definitions_['define_3'] = '//写卡数据';	
	Blockly.Arduino.definitions_['define_5'] = '//原扇区A密码，16个扇区，每个扇区密码6Byte';
	Blockly.Arduino.definitions_['define_6'] = 'unsigned char sectorKeyA[16][16] = {';		
	Blockly.Arduino.definitions_['define_7'] = ' {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},';
	Blockly.Arduino.definitions_['define_8'] = ' {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},';
	Blockly.Arduino.definitions_['define_9'] = ' {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};';
	
	Blockly.Arduino.definitions_['define_10'] = '//新扇区A密码，16个扇区，每个扇区密码6Byte';
	Blockly.Arduino.definitions_['define_11'] = 'unsigned char sectorNewKeyA[16][16] = {';
	Blockly.Arduino.definitions_['define_12'] = ' {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},';
	Blockly.Arduino.definitions_['define_13'] = ' {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff,0x07,0x80,0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},';
	Blockly.Arduino.definitions_['define_14'] = ' {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff,0x07,0x80,0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};';
	
	Blockly.Arduino.setups_['setup_output_1'] = 'Serial.begin(9600);';
	Blockly.Arduino.setups_['setup_output_2'] = 'SPI.begin();';
	Blockly.Arduino.setups_['setup_output_3'] = 'rfid.init();';
	return "";
};

Blockly.Arduino.RFID_on = function() {
  // Do while/until loop.
  var argument0 = Blockly.Arduino.valueToCode(this, 'uid_',
      Blockly.Arduino.ORDER_NONE) || 'false';
  var branch = Blockly.Arduino.statementToCode(this, 'do_');
  if (Blockly.Arduino.INFINITE_LOOP_TRAP) {
    branch = Blockly.Arduino.INFINITE_LOOP_TRAP.replace(/%1/g,
        '\'' + this.id + '\'') + branch;
  }

  return 'if ( rfid.isCard()  ) {\n' + branch + '}\n';
};

Blockly.Arduino.RFID_readcardnum = function() {
  var funcName='RFID_readcardnum';
	var code='String'+ ' ' + funcName + '() {\n'
		+"\n"+' rfid.readCardSerial();  '
		+"\n"+'  String stringserNum=String(rfid.serNum[0], HEX)+String(rfid.serNum[1], HEX)+String(rfid.serNum[2], HEX)+String(rfid.serNum[3], HEX)+String(rfid.serNum[4], HEX);'
		+"\n"+'      //选卡，返回卡容量（锁定卡片，防止多次读写）'		
		+"\n"+'      rfid.selectTag(rfid.serNum);'
		+"\n"+'  return stringserNum; '//直接返回string
		+ '\n}\n';
	Blockly.Arduino.definitions_[funcName] = code;	
	return  [funcName+'()', Blockly.Arduino.ORDER_ATOMIC];
};

/* Blockly.Arduino.RFID_serialprintcardnum = function() {
  var funcName='RFID_serialprintcardnum';
	var code='void'+ ' ' + funcName + '() {\n'
		+"\n"+' //找卡  '
		+"\n"+' //读取卡序列号 '
		+"\n"+' if (rfid.readCardSerial())  '
		+"\n"+'   {'
		+"\n"+'   Serial.print("The card\'s number is  : "); '
		+"\n"+'  Serial.print(rfid.serNum[0],HEX);  '
		+"\n"+'  Serial.print(rfid.serNum[1],HEX); '
		+"\n"+'   Serial.print(rfid.serNum[2],HEX); '
		+"\n"+'    Serial.print(rfid.serNum[3],HEX);  '
		+"\n"+'  Serial.print(rfid.serNum[4],HEX); '
		+"\n"+'   Serial.println(" "); '
		+"\n"+'    }'	
		+"\n"+'      //选卡，返回卡容量（锁定卡片，防止多次读写）'		
		+"\n"+'      rfid.selectTag(rfid.serNum);'			
		+ '\n}\n';
	Blockly.Arduino.definitions_[funcName] = code;	
	return  funcName+'();\n';
}; */




Blockly.Arduino.RFID_writecarddata = function() {
	var address2 = Blockly.Arduino.valueToCode(this, 'address1', Blockly.Arduino.ORDER_ATOMIC);
	var data2 = this.getTitleValue('data1');
	var funcName='RFID_writecarddata';
	var code='void'+ ' ' + funcName + '(int ad2) {\n' 
	+"\n"+'rfid.readCardSerial();'
	
	+"\n"+'      //选卡，返回卡容量（锁定卡片，防止多次读写）'		
	+"\n"+'      rfid.selectTag(rfid.serNum);'
	+"\n"+'//写数据卡 '
	+"\n"+'   blockAddr = ad2;     '
	+"\n"+'   if (rfid.auth(PICC_AUTHENT1A, blockAddr, sectorKeyA[blockAddr/4], rfid.serNum) == MI_OK)'
	+"\n"+'	{'
	+"\n"+'  //写数据'
	+"\n"+'   status = rfid.write(blockAddr, sectorKeyA[blockAddr/4]);'
	+"\n"+'   Serial.print("set the new card password, and can modify the data of the Sector: "); '
	+"\n"+'   Serial.println(blockAddr/4,DEC);'
	+"\n"+'   blockAddr=blockAddr-3; '
	+"\n"+'   status=rfid.write(blockAddr,(unsigned char*)'+data2+');'
	+"\n"+'   if(status == MI_OK) '
	+"\n"+'   { '
	+"\n"+'   Serial.println("Write card OK!");'
	+"\n"+' } '
	+"\n"+'   }'
	+ '\n}\n';
    Blockly.Arduino.definitions_[funcName] = code;
	return funcName+'('+address2+');\n';
}

Blockly.Arduino.RFID_readcarddata = function() {
	var address3 = Blockly.Arduino.valueToCode(this, 'address', Blockly.Arduino.ORDER_ATOMIC);
	var funcName='RFID_readcarddata'
	var code='String'+ ' ' + funcName + '(int ad3) {\n' 
	+"\n"+'//读卡 '
	+"\n"+'  blockAddr =ad3; '
	+"\n"+'  status = rfid.auth(PICC_AUTHENT1A, blockAddr, sectorNewKeyA[blockAddr/4], rfid.serNum);'
	+"\n"+'	 if (status == MI_OK)  //认证'
	+"\n"+'  {'
	+"\n"+'  //读数据'
	+"\n"+'  if( rfid.read(blockAddr, str) == MI_OK)'
	+"\n"+'  {'
	+"\n"+'  Serial.print("Read from the card ,the data is : ");'
	+"\n"+'  Serial.println((char *)str);'
	+"\n"+'  } '
	+"\n"+'   } '
	+"\n"+'  rfid.halt();'
	+"\n"+'  String stringstr((char*)str);'//str是一个char数组，必须先转换成char*，才能继续转换成string
	+"\n"+'  return stringstr;'
	+ '\n}\n';
    Blockly.Arduino.definitions_[funcName] = code;
	return [funcName+'('+address3+')\n', Blockly.Arduino.ORDER_ATOMIC];
};


/* Blockly.Arduino.RFID_serialprintcarddata = function() {
	var address3 = Blockly.Arduino.valueToCode(this, 'address', Blockly.Arduino.ORDER_ATOMIC);
  var funcName='RFID_serialprintcarddata';
	var code='void'+ ' ' + funcName + '(int ad3) {\n'
	+"\n"+'//读卡 '
	+"\n"+'  blockAddr =ad3; '
	+"\n"+'  status = rfid.auth(PICC_AUTHENT1A, blockAddr, sectorNewKeyA[blockAddr/4], rfid.serNum);'
	+"\n"+'	 if (status == MI_OK)  //认证'
	+"\n"+'  {'
	+"\n"+'  //读数据'
	+"\n"+'  if( rfid.read(blockAddr, str) == MI_OK)'
	+"\n"+'  {'
	+"\n"+'  Serial.print("Read from the card ,the data is : ");'
	+"\n"+'  Serial.println((char *)str);'
	+"\n"+'  } '
	+"\n"+'   } '
	+"\n"+'  rfid.halt();'
	+ '\n}\n';	
	Blockly.Arduino.definitions_[funcName] = code;	
	return  funcName+'('+address3+');\n';
}; */


Blockly.Arduino.RFID_off = function() {
	var funcName='RFID_off';
	var code='void'+ ' ' + funcName + '() {\n'
		+"\n"+' rfid.halt();   '
		+ '\n}\n';
	Blockly.Arduino.definitions_[funcName] = code;	
	return  funcName+'();\n';
};


Blockly.Arduino.RFID_in = function() {
  // Do while/until loop.

  var argument0 = Blockly.Arduino.valueToCode(this, 'uid_',
      Blockly.Arduino.ORDER_NONE) || 'false';
  var branch = Blockly.Arduino.statementToCode(this, 'do_');
  if (Blockly.Arduino.INFINITE_LOOP_TRAP) {
    branch = Blockly.Arduino.INFINITE_LOOP_TRAP.replace(/%1/g,
        '\'' + this.id + '\'') + branch;
  }

  return 'if (' + 'strcmp(RFID_read(),'+argument0+'.c_str())==0' + ') {\n' + branch + '}\n';
};

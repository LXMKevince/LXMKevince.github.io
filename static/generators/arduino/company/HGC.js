Blockly.Arduino.relay_highlow = function() {
  var code = (this.getFieldValue('BOOL') == 'HIGH') ? 'HIGH' : 'LOW';
  return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino.Relay_write2 = function() {
  var dropdown_pin = Blockly.Arduino.valueToCode(this, 'PIN',Blockly.Arduino.ORDER_ATOMIC);
  var dropdown_stat = Blockly.Arduino.valueToCode(this, 'STAT', Blockly.Arduino.ORDER_ATOMIC);
  var code = "";
  if(window.isNaN(dropdown_pin)){
      code = code+'pinMode('+dropdown_pin+', OUTPUT);\n';
  } else {
      if (Blockly.Arduino.setups_['setup_input_' + dropdown_pin]) 
          delete Blockly.Arduino.setups_['setup_input_' + dropdown_pin];
       Blockly.Arduino.setups_['setup_output_' + dropdown_pin] = 'pinMode(' + dropdown_pin + ', OUTPUT);';
   
  }
  code += 'digitalWrite('+dropdown_pin+','+dropdown_stat+');\n'
  return code;
};
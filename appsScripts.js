var sheet_id = "1OFLMSrX9s72X3WD_etHiYP3TH6diri7A5BuYlY-EbMY";
var sheet_name = "ATM";
function doGet(e)
{
  var ss = SpreadsheetApp.openById(sheet_id);
  var sheet = ss.getSheetByName(sheet_name);
  var e_username = e.parameter.username;
  var e_password = e.parameter.password;
  var debited_amount = Number(e.parameter.amountd); 
  var credited_amount = Number(e.parameter.amountc);
  var flag1 = e.parameter.flag1;
  var data = sheet.getDataRange().getValues();
  if (flag1 == '0')
  {
    for(var i=1;i<data.length;i++)
    {
      if(data[i][0]==e_username)
      {
        if(data[i][1]==e_password)
        {
          return ContentService.createTextOutput('true');
        }
      }
    }
  }
  if (debited_amount != 0 && debited_amount < 15000 && flag1 == '1')
  {
    for (var i=1;i<data.length;i++)
    {
      if (data[i][0]==e_username)
      {
        if (data[i][1]==e_password)
        {
          sheet.getRange(i+1,4).setValue(debited_amount);
          var debit = sheet.getRange(i+1,4);
          var avlbal1 = sheet.getRange(i+1,3);
          var net1 = avlbal1.getValue() - debit.getValue();
          sheet.getRange(i+1,3).setValue(net1);
          return ContentService.createTextOutput(net1);
        }
      }
    }      
  }
  else if (debited_amount > 15000 && flag1 == '1')
  {
    return ContentService.createTextOutput('Insufficient Balance');
  }
  else if (credited_amount != 0 && flag1 == '2')
  {
    for (var i=1;i<data.length;i++)
    {
      if (data[i][0]==e_username)
      {
        if (data[i][1]==e_password)
        {
          sheet.getRange(i+1,5).setValue(credited_amount);
          var credit = sheet.getRange(i+1,5);
          var avlbal2 = sheet.getRange(i+1,3);
          var net2 = avlbal2.getValue() + credit.getValue();
          sheet.getRange(i+1,3).setValue(net2);
          return ContentService.createTextOutput(net2);
        }
      }
    }      
  }
  if (flag1 == '3')
  {
    for (var i=1;i<data.length;i++)
    {
      if (data[i][0]==e_username)
      {
        if (data[i][1]==e_password)
        {
          var avlbal3 = sheet.getRange(i+1,3).getValue();
          return ContentService.createTextOutput(avlbal3);
        }
      }
    }
  }  
  return ContentService.createTextOutput('false');
}
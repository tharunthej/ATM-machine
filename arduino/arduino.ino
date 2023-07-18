#include "WiFi.h"
#include <HTTPClient.h>
#include <UniversalTelegramBot.h>
//WiFi Credentials
const char* ssid = "Mobile";
const char* wifi_password = "gaugefactor";

#define BOT_TOKEN "5656532955:AAE6dYdVd1nhoC04avcQRcABCJwUHkHVJUY"
String username = "";
String password = "";
String amountd = "0";
String amountc = "0";
String avlbal = "0";
String text = "";
String chat_id = "";
String flag1 = "0";



const unsigned long BOT_MTBS = 1000; // mean time between scan messages

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;          // last time messages' scan has been done
bool Start = false;
//Google Script ID
String GOOGLE_SCRIPT_ID = "AKfycbw0aRvK8WMbdQKZefQisNiDzj-gg2N8plEjOMlIN7azdlHMHd8jk4ZSaetenL-xlcf6zA";
void handleNewMessages(int numNewMessages)
{
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++)
  {
    chat_id = bot.messages[i].chat_id;
    text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (text == "/start")
    {
      String welcome = "Welcome to ATM Telegram bot, " + from_name + ".\n";
      welcome += "Please Enter your Username.\n\n";
      bot.sendMessage(chat_id, welcome);
      while(bot.getUpdates(bot.last_message_received +1)==0){};
      username = bot.messages[i].text;
      String pass = "Please Enter your Password";
      bot.sendMessage(chat_id, pass);
      while(bot.getUpdates(bot.last_message_received +1)==0){};
      password = bot.messages[i].text;
    }
    String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + "username=" + username + "&password=" + password + "&amountd=" + amountd + "&amountc=" + amountc + "&flag1=" + flag1;
    HTTPClient http;
    Serial.println(url);
    http.begin(url.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    String flag = http.getString();
    Serial.println(flag);
    if (flag == "true")
    {
      String success = "Login Successful";
      success += "\nAvailable Options:";
      success += "\n /debit - To make a Withdrawl.";
      success += "\n /credit - To make a Deposit.";
      success += "\n /balancecheck - To check Available Balance.";
      bot.sendMessage(chat_id, success);
      flag1 = "true";
      flag = "";
    }
    if (flag == "false" && flag1 != "true")
    {
      String failure = "Login failed";
      failure += "Wrong Username or Password.";
      bot.sendMessage(chat_id, failure);
    }
    if (flag1 == "true")
    {
      if (text == "/debit")
      {
        flag1 = "1";
        String debit = "Enter required amount";
        bot.sendMessage(chat_id, debit);
        while(bot.getUpdates(bot.last_message_received +1)==0){};
        amountd = bot.messages[i].text;
        String url1 = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + "username=" + username + "&password=" + password + "&amountd=" + amountd + "&amountc=" + amountc + "&flag1=" + flag1;
        HTTPClient http;
        Serial.println(url1);
        http.begin(url1.c_str());
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        int httpCode1 = http.GET(); 
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode1);
        String ret1 = http.getString();
        ret1 += " remaining";
        bot.sendMessage(chat_id, ret1);
      }
      if (text == "/credit")
      {
        flag1 = "2";
        String credit = "Enter the amount";
        bot.sendMessage(chat_id, credit);
        while(bot.getUpdates(bot.last_message_received +1)==0){};
        amountc = bot.messages[i].text;
        String url2 = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + "username=" + username + "&password=" + password + "&amountd=" + amountd + "&amountc=" + amountc + "&flag1=" + flag1;
        HTTPClient http;
        http.begin(url2.c_str());
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        int httpCode2 = http.GET(); 
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode2);
        String ret2 = http.getString();
        ret2 += " remaining";
        bot.sendMessage(chat_id, ret2);
      }
      if (text == "/balancecheck")
      {
        flag1 = "3";
        String url3 = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + "username=" + username + "&password=" + password + "&amountd=" + amountd + "&amountc=" + amountc + "&flag1=" + flag1;
        HTTPClient http;
        http.begin(url3.c_str());
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        int httpCode3 = http.GET(); 
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode3);
        String ret3 = http.getString();
        String Avlbal = "Available Balance: " + ret3;
        bot.sendMessage(chat_id, Avlbal);
      }
    }
  }  
}


void setup()
{
  Serial.begin(115200);
  Serial.println();

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(ssid);
  WiFi.begin(ssid, wifi_password);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    if (WiFi.status() == WL_CONNECTED)
    {
      while (numNewMessages)
      {
        Serial.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
    }

    bot_lasttime = millis();
  }
} 

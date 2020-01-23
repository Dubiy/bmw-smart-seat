
#include "webpages.h"

String htmlPage(String title, String body) {
  return "<!DOCTYPE html><html>"
         "<head>"
           "<meta charset=\"UTF-8\">"
           "<title>" + title + "</title>"
         "</head>"
         "<body>" +
           "<div class='container'>"
             "<header>"
               "<a href='/'>Home</a> <a href='/config'>Config</a>"
             "</header>"

             "<h2>" + title + "</h2>" +
             body +
             "<footer>"
               "&copy; MadLab, 2018.<br />Created by: Ihor Dubii <a href='http://gary.pp.ua' target='_blank'>http://gary.pp.ua</a>"
             "</footer>"
           "</div>"
           "<style>"
             "body{font-family:sans-serif;margin:0}header{border:1px solid gray;padding:10px;text-align:left;box-shadow:#eee 2px 2px 5px;border-radius:0 0 3px 3px}h2{text-align:center}label{display:block;margin-bottom:15px}form{display:block;border:1px solid gray;padding:20px;margin-bottom:20px;border-radius:3px}.container{text-align:center;width: 800px;max-width:100%;margin:0 auto 20px;display:block;box-shadow:#eee 2px 2px 5px}textarea,input{width:100%;display:block;border:1px solid gray;border-radius:4px;outline:0;padding:10px 0;margin:10px 0}span{color:gray;font-family:'Courier New',Serif;display:block}.btn{-webkit-appearance:button;display:inline-block;padding:6px 12px;margin-bottom:0;font-size:14px;font-weight:400;line-height:1.42857143;text-align:center;white-space:nowrap;vertical-align:middle;-ms-touch-action:manipulation;touch-action:manipulation;cursor:pointer;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;background-image:none;border:1px solid transparent;border-radius:4px}.btn.save,.btn.unlock{color: #fff;background-color:#5cb85c;border-color:#4cae4c}footer {border:1px solid gray;text-align:left;padding:20px;box-shadow:#eee 2px 2px 5px;border-radius:3px 3px 0 0}ul{padding:0;list-style:none;margin-bottom:0}"
           "</style>"
           "</body>"
"</html>";
}


String indexHtml() {
  String body = "<form action='/' method='POST'>"
                  "<input class='btn unlock' type='submit' name='unlock' value='Blink!'>"
                "</form>";
  return htmlPage("TempCube: hello", body);
}

//ESP8266WebServer webServer, Settings settings, bool isPost
String configHtml(WebServer &webServer, Settings &settings, bool isPost) {
  String body = "";

  //if no or wrong password show auth form. else show and handle hormal form
  if (!settings.secret.equals(webServer.arg("auth"))) {
      String body = "<form action='/config'>"
                      "<label>Device secret code:"
                        "<input type='password' name='auth' placeholder='you shall not pass!'>"
                        "<input class='btn save' type='submit' value='Login'>"
                      "</label>";
    return htmlPage("TempCube: config auth", body);
  }

  if (isPost) {
    //Outch, there is no argument inside if zero.
    if (webServer.args() == 0) {
      return htmlPage("TempCube: config", "BAD ARGS");
    }

    for (int x=0; x <= webServer.args(); x++) {
      //print all arguments received in POST payload
      Serial.print(webServer.argName(x));
      Serial.print(": ");
      Serial.println(webServer.arg(x));

      if (webServer.argName(x).equals("ssid")) {
        settings.ssid = webServer.arg(x);
      }
      if (webServer.argName(x).equals("pass")) {
        settings.pass = webServer.arg(x);
      }
      if (webServer.argName(x).equals("title")) {
        settings.title = webServer.arg(x);
      }
      if (webServer.argName(x).equals("secret")) {
        settings.secret = webServer.arg(x);
      }
      // if (webServer.argName(x).equals("brightness")) {
      //   settings.brightness = webServer.arg(x);
      // }
    }


    setSettings(settings);

    body = body + "<h4>Settings saved</h4>";

    //Print out the amount of arguments received in POST payload
    Serial.println(webServer.args());
  }

  body = body + "<form action='' method='POST'>"
                  "<label for='ssid'>WiFi SSID:</label>"
                  "<input type='text' name='ssid' id='ssid' value='" + settings.ssid + "'>"

                  "<label for='pass'>WiFi pass:</label>"
                  "<input type='text' name='pass' id='pass' minlength='8' value='" + settings.pass + "'>"

                  "<label for='title'>Device name:</label>"
                  "<input type='text' name='title' id='title' value='" + settings.title + "'>"

                  "<label for='secret'>Device secret</label>"
                  "<input type='password' name='secret' id='secret' value='" + settings.secret + "'>"

                  // "<label for='brightness'>Brightness</label>"
                  // "<input type='number' name='brightness' id='brightness' min='0' max='255' value='" + settings.brightness + "'>"

                  "<input class='btn unlock' type='submit' name='save' value='Save'>"
                "</form>";
  return htmlPage("TempCube: config", body);

}

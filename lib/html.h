#ifndef html_h
#define html_h

const char* main_page = R"mainhtml(
<html>
    <head>
      <link href="/css/style.css" rel="stylesheet" type="text/css">
      <title>Aquarium Controller</title>
      <meta charset="utf-8">
      <script src=//cdn.rawgit.com/Mikhus/canvas-gauges/gh-pages/download/2.1.7/all/gauge.min.js></script>
      <script src="//ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
      <script src="/js/index.js"></script>
    </head>
    <body>
        <h2>Aquarium Controller</h2>
    <table>
      <tr>
        <td align="center"><strong>Temperature [C]</strong></td>
        <td align="center"><strong>FAN Speed [%]</strong></td>
        <td align="center"><strong>Water Level [cm]</strong></td>
      <tr>
        <td><canvas id="temp_gauge"></canvas></td>
        <td><canvas id="pwm_gauge"></canvas></td>
        <td><canvas id="lvl_gauge"></canvas></td>
      </tr>
      <tr>
        <td align="left"><a id="settingsbutton" href="/settings"> settings </a></td>
        <td align="left"></td>
      <tr>
    </table>
    <script src="/js/gauges.js"></script>
  </body>
</html>
)mainhtml";

const char* index_script = R"indexscript(
$(document).ready(function () {
  setInterval(updateGauges, 1000);  
})
function updateGauges(){ 
  $.ajax({ 
    type: 'GET', 
    contentType: 'application/json; charset=utf-8',
    url: '/fishtank.json', 
    data: {}, 
    dataType: 'json',
    success: function (data) { 
        tempGauge.value=data.fishtankdata.temperature;
        pwmGauge.value=data.fishtankdata.fan_pwm/250*100;
        lvlGauge.value=data.fishtankdata.level;
    }
  });
}
)indexscript";

const char* gauges_script = R"gaugesscript(
var pwmGauge = new RadialGauge({
  renderTo: 'pwm_gauge',
  height: 300,
  units: '%',
  minValue: 0,
  maxValue: 100,
  majorTicks: [
    '0',
    '10',
    '20',
    '30',
    '40',
    '50',
    '60',
    '70',
    '80',
    '90',
    '100'
  ],
  colorPlate: "#202020",
  minorTicks: 2,
  valueBox: true,
  animationDuration: 1500,
  animationRule: "dequint",
  animatedValue: true
});
pwmGauge.draw();
pwmGauge.value = 0;

var tempGauge = new RadialGauge({
  renderTo: 'temp_gauge',
  height: 300,
  units: '°C',
  minValue: 20,
  maxValue: 35,
  majorTicks: [
    '20',
    '21',
    '22',
    '23',
    '24',
    '25',
    '26',
    '27',
    '28',
    '29',
    '30',
    '31',
    '32',
    '33',
    '34',
    '35'
  ],
  minorTicks: 5,
  highlights : [
    {from: 24, to: 26, color: 'rgba(50, 200, 50, .75)'},
    {from: 22, to: 24, color: 'rgba(255, 140, 50, .75)'},
    {from: 20, to: 22, color: 'rgba(255, 50, 50, .75)'},
    {from: 26, to: 28, color: 'rgba(255, 140, 50, .75)'},
    {from: 28, to: 35, color: 'rgba(255, 50, 50, .75)'}
  ],
  colorPlate: "#202020",
  valueBox: true,
  animationDuration: 1500,
  animationRule: "dequint",
  animatedValue: true
});
tempGauge.draw();
tempGauge.value = 0;


var lvlGauge = new LinearGauge({
  renderTo: 'lvl_gauge',
  width: 120,
  height: 300,
  units: 'mm',
  minValue: 0,
  maxValue: 80,
  majorTicks: [
    '0',
    '10',
    '20',
    '30',
    '40',
    '50',
    '60',
    '70',
    '80'
  ],
  minorTicks: 5,
  strokeTicks: true,
  highlights  : [
    {from: 0, to: 35, color: 'rgba(200, 50, 50, .75)'},
    {from: 36, to: 69, color: 'rgba(255, 140, 50, .75)'},
    {from: 70, to: 80, color: 'rgba(50, 255, 50, .75)'}
  ],
  colorPlate: "#202020",
  borderShadowWidth: 0,
  borders: false,
  needleType: "arrow",
  needleWidth: 2,
  animationDuration: 1500,
  animationRule: "linear",
  tickSide: "left",
  numberSide: "left",
  needleSide: "left",
  barStrokeWidth: 7,
  barBeginCircle: false
});
lvlGauge.draw();
lvlGauge.value = 0;
)gaugesscript";

const char* settings_page = R"settingshtml(
<html>
<head>
  <title>Aquarium Controller</title>
  <meta charset="utf-8">
  <script src="//ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="/js/settings.js"></script>
  <link href="/css/style.css" rel="stylesheet" type="text/css">
</head>
<body>
  <h2>Aquarium Controller</h2>
  <form method="post" action="">
  <table>
    <tr>
      <td align="left"><u>Description</u></td>
      <td align="right"><u>Current Val</u></td>
      <td align="right"><u>Target Val</u></td>
      <td align="middle"><u>Setting</u></td>
    </tr><tr>
      <td align="left">Desired water temp:</td>
      <td align="right"><span id="current_temp_value">0</span>°C</td>
      <td align="right"><span id="desired_temp_value">0</span>°C</td>
      <td class="slidecontainer"><input type="range" min="20" max="30" value="" class="slider" id="desired_temp" name="desired_temp"></td>
    </tr><tr>
      <td align="left">Maximum okay offset:</td>
      <td></td>
      <td align="right"><span id="temp_offset_value">0.0</span>°C</td>
      <td class="slidecontainer"><input type="range" min="0" max="5" value="" step="0.1" class="slider" id="temp_offset" name="temp_offset"></td>
    </tr><tr>
      <td align="left">Max cold white intensity:</td>
      <td align="right"><span id="current_cold_value">0</span>%</td>
      <td align="right"><span id="desired_cold_value">0</span>%</td>
      <td class="slidecontainer"><input type="range" min="1" max="100" value="" class="slider" id="desired_cold" name="desired_cold"></td>
    </tr><tr>
      <td align="left">Max neutral white intensity:</td>
      <td align="right"><span id="current_neutral_value">0</span>%</td>
      <td align="right"><span id="desired_neutral_value">0</span>%</td>
      <td class="slidecontainer"><input type="range" min="1" max="100" value="" class="slider" id="desired_neutral" name="desired_neutral"></td>
    </tr><tr>
      <td align="left">Max warm white intensity:</td>
      <td align="right"><span id="current_warm_value">0</span>%</td>
      <td align="right"><span id="desired_warm_value">0</span>%</td>
      <td class="slidecontainer"><input type="range" min="1" max="100" value="" class="slider" id="desired_warm" name="desired_warm"></td>
    </tr><tr>
      <td align="left">Max blue light intensity:</td>
      <td align="right"><span id="current_blue_value">0</span>%</td>
      <td align="right"><span id="desired_blue_value">0</span>%</td>
      <td class="slidecontainer"><input type="range" min="1" max="100" value="" class="slider" id="desired_blue" name="desired_blue"></td>
    </tr><tr>
      <td align="left">Day/Night fade duration:</td>
      <td align="right"><span id="daynight_remeaning_duration_value">0</span>min</td>
      <td align="right"><span id="desired_daynight_duration_value">0</span>min</td>
      <td class="slidecontainer"><input type="range" min="1" max="100" value="" class="slider" id="daynight_duration" name="daynight_duration"></td>
    </tr><tr>
      <td align="left">Day/Night mode:</td>
      <td></td>
      <td></td>
      <td align="left">
        <div class='switch'><div class='mode'>
          <input id='dn0' name='daynight' type='radio' value='0'>
          <label for='dn0'>Auto</label>
        </div><div class='mode'>
          <input id='dn1' name='daynight' type='radio' value='1'>
          <label for='dn1'>Day</label>
        </div><div class='mode'>
          <input id='dn2' name='daynight' type='radio' value='2'>
          <label for='dn2'>Night</label>
        </div></div>
      </td>
    </tr><tr>
      <td align="left">Light switch mode:</td>
      <td></td>
      <td></td>
      <td align="left">
        <div class='switch'><div class='light_mode'>
          <input id='inst0' name='instant' type='radio' value='0'>
          <label for='inst0'>Fade</label>
        </div><div class='light_mode'>
          <input id='inst1' name='instant' type='radio' value='1'>
          <label for='inst1'>Switch</label>
        </div></div>
      </td>
    </tr><tr>
      <td align="left"><a id="backbutton" href="/">Back to Status</a></td>
      <td align="left"><input type="submit" name="submit" value="Save" /></td>
      <td></td>
      <td></td>
    <tr>
  </table>
</form>
)settingshtml";

const char* settings_script = R"settingsscript(
$(document).ready(function () {
  var desiredTemp = document.getElementById("desired_temp");
  var valueCurrentTemp = document.getElementById("current_temp_value");
  var valueDesiredTemp = document.getElementById("desired_temp_value");
  var tempOffset = document.getElementById("temp_offset");
  var valueTempOffset = document.getElementById("temp_offset_value");
  var coldLight = document.getElementById("desired_cold");
  var valueCurrentColdLight = document.getElementById("current_cold_value");
  var valueColdLight = document.getElementById("desired_cold_value");
  var neutralLight = document.getElementById("desired_neutral");
  var valueCurrentNeutralLight = document.getElementById("current_neutral_value");
  var valueNeutralLight = document.getElementById("desired_neutral_value");
  var warmLight = document.getElementById("desired_warm");
  var valueCurrentWarmLight = document.getElementById("current_warm_value");
  var valueWarmLight = document.getElementById("desired_warm_value");
  var blueLight = document.getElementById("desired_blue");
  var valueCurrentBlueLight = document.getElementById("current_blue_value");
  var valueBlueLight = document.getElementById("desired_blue_value");
  var daynightDuration = document.getElementById("daynight_duration");
  var valueRemeaningDaynightDuration = document.getElementById("daynight_remeaning_duration_value");
  var valueDaynightDuration = document.getElementById("desired_daynight_duration_value");
  setInterval(refreshLive, 1000); 
    $.ajax({ 
      type: 'GET', 
      contentType: 'application/json; charset=utf-8',
      url: '/fishtank.json', 
      data: {}, 
      dataType: 'json',
      success: function (data) {
        desiredTemp.value=data.fishtanksettings.desired_temp;
        valueCurrentTemp.innerHTML=data.fishtankdata.temperature;
        valueDesiredTemp.innerHTML=data.fishtanksettings.desired_temp;
        tempOffset.value=data.fishtanksettings.temp_offset;
        valueTempOffset.innerHTML=data.fishtanksettings.temp_offset;
        coldLight.value=data.fishtanksettings.desired_cold;
        valueCurrentColdLight.innerHTML = data.fishtanksettings.current_brightness_0;
        valueColdLight.innerHTML = data.fishtanksettings.desired_cold;
        neutralLight.value=data.fishtanksettings.desired_neutral;
        valueCurrentNeutralLight.innerHTML = data.fishtanksettings.current_brightness_2;
        valueNeutralLight.innerHTML=data.fishtanksettings.desired_neutral;
        warmLight.value=data.fishtanksettings.desired_warm;
        valueCurrentWarmLight.innerHTML = data.fishtanksettings.current_brightness_4;
        valueWarmLight.innerHTML=data.fishtanksettings.desired_warm;
        blueLight.value=data.fishtanksettings.desired_blue;
        valueCurrentBlueLight.innerHTML = data.fishtanksettings.current_brightness_6;
        valueBlueLight.innerHTML=data.fishtanksettings.desired_blue;
        daynightDuration.value=data.fishtanksettings.daynight_duration;
        valueRemeaningDaynightDuration.innerHTML = data.fishtanksettings.daynight_duration;
        valueDaynightDuration.innerHTML=data.fishtanksettings.daynight_duration;
        document.getElementById("dn"+data.fishtanksettings.daynight).checked = true;
        document.getElementById("inst"+data.fishtanksettings.instant).checked = true;
      }
    });
    function refreshLive(){ 
      $.ajax({ 
        type: 'GET', 
        contentType: 'application/json; charset=utf-8',
        url: '/fishtank.json', 
        data: {}, 
        dataType: 'json',
        success: function (data) {
          valueCurrentTemp.innerHTML=data.fishtankdata.temperature;
          valueCurrentColdLight.innerHTML = data.fishtanksettings.current_brightness_0;
          valueCurrentNeutralLight.innerHTML = data.fishtanksettings.current_brightness_2;
          valueCurrentWarmLight.innerHTML = data.fishtanksettings.current_brightness_4;
          valueCurrentBlueLight.innerHTML = data.fishtanksettings.current_brightness_6;
          valueRemeaningDaynightDuration.innerHTML = data.fishtanksettings.daynight_duration;
        }
      });
    }
  desiredTemp.oninput = function() {
    valueDesiredTemp.innerHTML = this.value;
  }
  tempOffset.oninput = function() {
    valueTempOffset.innerHTML = this.value;
  }
  coldLight.oninput = function() {
    valueColdLight.innerHTML = this.value;
  }
  neutralLight.oninput = function() {
    valueNeutralLight.innerHTML = this.value;
  }
  warmLight.oninput = function() {
    valueWarmLight.innerHTML = this.value;
  }
  blueLight.oninput = function() {
    valueBlueLight.innerHTML = this.value;
  }
  daynightDuration.oninput = function() {
    valueDaynightDuration.innerHTML = this.value;
  }
})
)settingsscript";

const char* stylesheet = R"stylesheet(
html { font-family: "Gudea", Helvetica, sans-serif; height: 100%; }
body { background-color: #202020; }
h2, td, a { color: #95A5A6; }
p { color: white; }
td { padding-bottom: 5px;font-weight:bold; }
span { font-weight:bold;color:#4CAF50; }
.slidecontainer { width: 50%; }
.slider { -webkit-appearance: none; appearance: none; width: 100%; height: 15px; border-radius: 5px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s; }
.slider:hover { opacity: 1; }
.slider::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 50%; background: #4CAF50;  cursor: pointer; }
.slider::-moz-range-thumb { width: 25px; height: 25px; border-radius: 50%; background: #4CAF50; cursor: pointer; }
.switch { position: relative; margin: 0px auto; width: 100%; height: 20px; border: 3px solid #d3d3d3; color: #95A5A6; font-size: 15px; border-radius: 5px; }
.mode { position: relative; display: inline-block; width: 33.3%; height: 100%; line-height: 20px; }
.light_mode { position: relative; display: inline-block; width: 50%; height: 100%; line-height: 20px; }
.light_mode:first-child label, .mode:first-child label { border-radius: 2px 0 0 2px; }
.light_mode:last-child label, .mode:last-child label { border-radius: 0 2px 2px 0; }
.light_mode label, .mode label { position: absolute; top: 0; left: 0; width: 100%; height: 100%; cursor: pointer; font-style: bold; text-align: center; transition: transform 0.4s, color 0.4s, background-color 0.4s; }
.light_mode input[type="radio"], .mode input[type="radio"] { appearance: none; width: 0; height: 0; opacity: 0; }
.light_mode input[type="radio"]:focus, .mode input[type="radio"]:focus { outline: 0; outline-offset: 0; }
.light_mode input[type="radio"]:checked ~ label, .mode input[type="radio"]:checked ~ label { background-color: #4CAF50; color: #111; }
.light_mode input[type="radio"]:active ~ label, .mode input[type="radio"]:active ~ label { transform: scale(1.05); }
a#backbutton, input[type=submit] { border: 3px solid #d3d3d3; color: #95A5A6; font-size: 15px; font-style: bold; border-radius: 5px;background: #202020; text-decoration:none; }
a#backbutton:hover, input[type=submit]:hover { border: 3px solid #d3d3d3; color: #111; font-size: 15px; font-style: bold; border-radius: 5px;background: #4CAF50; cursor: pointer;transition: transform 0.4s, color 0.4s, background-color 0.4s; text-decoration:none; }
a#settingsbutton { padding:0px 5px 0px 5px; border: 3px solid #d3d3d3; color: #95A5A6; font-size: 15px; font-style: bold; border-radius: 5px;background: #202020; text-decoration:none; }
a#settingsbutton:hover { padding:0px 5px 0px 5px; border: 3px solid #d3d3d3; color: #111; font-size: 15px; font-style: bold; border-radius: 5px;background: #4CAF50; cursor: pointer;transition: transform 0.4s, color 0.4s, background-color 0.4s; text-decoration:none; }
a#backbutton, a#backbutton:hover{ padding:0px 5px 0px 5px; }
)stylesheet";
#endif

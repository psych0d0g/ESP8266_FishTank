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
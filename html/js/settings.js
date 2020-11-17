$(document).ready(function () {
  var desiredTemp = document.getElementById("desired_temp");
  var valueDesiredTemp = document.getElementById("desired_temp_value");
  var tempOffset = document.getElementById("temp_offset");
  var valueTempOffset = document.getElementById("temp_offset_value");
  var coldLight = document.getElementById("desired_cold");
  var valueColdLight = document.getElementById("desired_cold_value");
  var neutralLight = document.getElementById("desired_neutral");
  var valueNeutralLight = document.getElementById("desired_neutral_value");
  var warmLight = document.getElementById("desired_warm");
  var valueWarmLight = document.getElementById("desired_warm_value");
  var blueLight = document.getElementById("desired_blue");
  var valueBlueLight = document.getElementById("desired_blue_value");
    $.ajax({ 
      type: 'GET', 
      contentType: 'application/json; charset=utf-8',
      url: '/fishtank.json', 
      data: {}, 
      dataType: 'json',
      success: function (data) {
        desiredTemp.value=data.fishtanksettings.desired_temp;
        valueDesiredTemp.innerHTML=data.fishtanksettings.desired_temp;
        tempOffset.value=data.fishtanksettings.temp_offset;
        valueTempOffset.innerHTML=data.fishtanksettings.temp_offset;
        coldLight.value=data.fishtanksettings.desired_cold;
        valueColdLight.innerHTML = data.fishtanksettings.desired_cold;
        neutralLight.value=data.fishtanksettings.desired_neutral;
        valueNeutralLight.innerHTML=data.fishtanksettings.desired_neutral;
        warmLight.value=data.fishtanksettings.desired_warm;
        valueWarmLight.innerHTML=data.fishtanksettings.desired_warm;
        blueLight.value=data.fishtanksettings.desired_blue;
        valueBlueLight.innerHTML=data.fishtanksettings.desired_blue;
        document.getElementById("dn"+data.fishtanksettings.daynight).checked = true;
      }
    });
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
})
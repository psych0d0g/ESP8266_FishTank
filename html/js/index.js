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
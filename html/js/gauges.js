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
var Clay = require('@rebble/clay');
var defaults = {Palette: 0, Background: 0, Foreground: 0xFFFFAA, Accent: 0xFF0000, TimeFormat: 0, DateFormat: 1, ShowSeconds: 1, DigitalTime: 0, ShowBattery: 1};

function integer(value, fallback, maximum) {
  if (value === undefined || value === null || value === '' || typeof value === 'boolean') return fallback;
  var number = Number(value);
  return isFinite(number) && Math.floor(number) === number && number >= 0 && number <= maximum ? number : fallback;
}
function toggle(value, fallback) {return typeof value === 'boolean' ? Number(value) : integer(value, fallback, 1);}
function normalize(raw) {
  raw = raw || {};
  return {
    Palette: integer(raw.Palette, defaults.Palette, 6),
    Background: integer(raw.Background, defaults.Background, 0xFFFFFF),
    Foreground: integer(raw.Foreground, defaults.Foreground, 0xFFFFFF),
    Accent: integer(raw.Accent, defaults.Accent, 0xFFFFFF),
    TimeFormat: integer(raw.TimeFormat, defaults.TimeFormat, 2),
    DateFormat: integer(raw.DateFormat, defaults.DateFormat, 4),
    ShowSeconds: toggle(raw.ShowSeconds, defaults.ShowSeconds),
    DigitalTime: toggle(raw.DigitalTime, defaults.DigitalTime),
    ShowBattery: toggle(raw.ShowBattery, defaults.ShowBattery)
  };
}
var clay = new Clay([
  {type: 'heading', defaultValue: 'Triangle'},
  {type: 'text', defaultValue: 'A sculpted triangular dial with slim hands and an optional digital footer.'},
  {type: 'select', messageKey: 'TimeFormat', label: 'Optional digital time format', defaultValue: '0', options: [
    {label: 'Watch default', value: '0'}, {label: '12-hour', value: '1'}, {label: '24-hour', value: '2'}
  ]},
  {type: 'select', messageKey: 'DateFormat', label: 'Date', defaultValue: '1', options: [
    {label: 'Off', value: '0'}, {label: 'Weekday · month · day', value: '1'},
    {label: 'YYYY-MM-DD', value: '2'}, {label: 'MM/DD/YYYY', value: '3'}, {label: 'DD/MM/YYYY', value: '4'}
  ]},
  {type: 'toggle', messageKey: 'ShowSeconds', label: 'Seconds hand', defaultValue: true},
  {type: 'toggle', messageKey: 'ShowBattery', label: 'Battery percentage', defaultValue: true},
  {type: 'toggle', messageKey: 'DigitalTime', label: 'Digital time footer', defaultValue: false},
  {type: 'heading', defaultValue: 'Colors'},
  {type: 'select', messageKey: 'Palette', label: 'Palette', defaultValue: '0', options: [
    {label: 'Graphite', value: '0'}, {label: 'Steel', value: '1'},
    {label: 'Olive', value: '2'}, {label: 'Ivory', value: '3'}, {label: 'Navy', value: '4'},
    {label: 'Gold', value: '5'}, {label: 'Custom colors', value: '6'}
  ]},
  {type: 'text', defaultValue: 'Select Custom colors to use the choices below. The accent colors the seconds hand. The analog dial is always twelve-hour; the time-format setting controls the optional footer. Choose contrasting text and background colors.'},
  {type: 'color', messageKey: 'Background', label: 'Background', defaultValue: '000000', sunlight: false, allowGray: true},
  {type: 'color', messageKey: 'Foreground', label: 'Hands & labels', defaultValue: 'FFFFAA', sunlight: false, allowGray: true},
  {type: 'color', messageKey: 'Accent', label: 'Seconds hand', defaultValue: 'FF0000', sunlight: false, allowGray: true},
  {type: 'submit', defaultValue: 'Save to watch'}
], null, {autoHandleEvents: false});

function sendSaved() {
  var raw = localStorage.getItem('clay-settings');
  if (!raw) return;
  try { Pebble.sendAppMessage(normalize(JSON.parse(raw))); } catch (error) { console.log('Triangle settings could not be loaded'); }
}
Pebble.addEventListener('ready', sendSaved);
Pebble.addEventListener('showConfiguration', function() { Pebble.openURL(clay.generateUrl()); });
Pebble.addEventListener('webviewclosed', function(event) {
  if (!event || !event.response || event.response === 'CANCELLED') return;
  try {
    clay.getSettings(event.response, false);
    sendSaved();
  } catch (error) { console.log('Triangle settings were not saved'); }
});

module.exports = {normalize: normalize, defaults: defaults};

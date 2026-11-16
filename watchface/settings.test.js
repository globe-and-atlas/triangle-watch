var assert=require('assert'),Module=require('module'),path=require('path');
var original=Module._load;
var keys={Palette:10000,Background:10001,Foreground:10002,Accent:10003,TimeFormat:10004,DateFormat:10005,ShowSeconds:10006,DigitalTime:10007,ShowBattery:10008};
var bundle=path.join(__dirname,'node_modules/@rebble/clay/src/js/index.js');
var config;
Module._load=function(name) {
  if(name==='message_keys') return keys;
  if(name==='@rebble/clay') {
    var ActualClay=original.call(this,bundle);
    return function(items,custom,options) {config=items;return new ActualClay(items,custom,options);};
  }
  return original.apply(this,arguments);
};
var events={},storage={},sent=[];
global.localStorage={getItem:function(k) {return storage[k]||null;},setItem:function(k,v) {storage[k]=v;}};
global.Pebble={
  addEventListener:function(k,fn) {(events[k]||(events[k]=[])).push(fn);},
  getAccountToken:function() {return 'test';},getWatchToken:function() {return 'test';},
  getActiveWatchInfo:function() {return {platform:'emery',model:'emery',language:'en_US',firmware:{major:4,minor:33,patch:1}};},
  sendAppMessage:function(value) {sent.push(value);},openURL:function(url) {assert(url.indexOf('data:text/html')===0);}
};
var settings=require('./src/pkjs/index');
function fire(name,event) {(events[name]||[]).forEach(function(fn) {fn(event);});}
assert.deepStrictEqual(settings.normalize({}),settings.defaults);
for(var p=0;p<=6;p++)assert.strictEqual(settings.normalize({Palette:String(p)}).Palette,p);
assert.strictEqual(settings.normalize({Palette:7}).Palette,0);
for(var t=0;t<=2;t++) assert.strictEqual(settings.normalize({TimeFormat:String(t)}).TimeFormat,t);
for(var d=0;d<=4;d++) assert.strictEqual(settings.normalize({DateFormat:String(d)}).DateFormat,d);
fire('ready');assert.strictEqual(sent.length,0);fire('showConfiguration');
fire('webviewclosed',{response:''});fire('webviewclosed',{response:'CANCELLED'});fire('webviewclosed',{response:'invalid'});
assert.strictEqual(sent.length,0);
var raw={TimeFormat:{value:'2'},DateFormat:{value:'4'},Palette:{value:'6'},Background:{value:0},Foreground:{value:0xAAFFAA},Accent:{value:0xAA0000}};
fire('webviewclosed',{response:encodeURIComponent(JSON.stringify(raw))});
assert.deepStrictEqual(sent[0],{TimeFormat:2,DateFormat:4,Palette:6,Background:0,Foreground:0xAAFFAA,Accent:0xAA0000,ShowSeconds:1,DigitalTime:0,ShowBattery:1});
fire('ready');assert.deepStrictEqual(sent[0],sent[1]);
var saved=storage['clay-settings'];fire('webviewclosed',{response:'CANCELLED'});assert.strictEqual(storage['clay-settings'],saved);
fire('webviewclosed',{response:encodeURIComponent(JSON.stringify({ShowSeconds:{value:false},DigitalTime:{value:true},ShowBattery:{value:false}}))});
assert.strictEqual(sent[sent.length-1].ShowSeconds,0);assert.strictEqual(sent[sent.length-1].DigitalTime,1);assert.strictEqual(sent[sent.length-1].ShowBattery,0);
fire('ready');assert.strictEqual(sent[sent.length-1].ShowSeconds,0);

['Palette','Background','Foreground','Accent','TimeFormat','DateFormat'].forEach(function(key){assert(config.some(function(i){return i.messageKey===key;}));});
assert.strictEqual(settings.normalize({ShowSeconds:false}).ShowSeconds,0);
assert.strictEqual(settings.normalize({DigitalTime:true}).DigitalTime,1);
assert.strictEqual(settings.normalize({ShowBattery:false}).ShowBattery,0);
console.log('PASS: Triangle real Clay Save, Cancel, reload, time/date and palettes');

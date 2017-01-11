// LEGOdancingRobotExtension.js
// Kreg Hanning, January 2017
//
// Extension for LEGO dancing robot activity

(function(ext) {

  var PING = 0x64,
    PONG = 0x65;

  var RED_BEAT = 1,
    BLUE_BEAT = 2;

  var device = null;
  var connected = false;
  var poller = null;
  var watchdog = null;

  ext.redBeat = function() {
    device.send(new Uint8Array([RED_BEAT]).buffer);
  };

  ext.blueBeat = function() {
    device.send(new Uint8Array([BLUE_BEAT]).buffer);
  };

  var potentialDevices = [];
  ext._deviceConnected = function(dev) {
    potentialDevices.push(dev);
    if (!device) tryNextDevice();
  };

  function tryNextDevice() {
    device = potentialDevices.shift();
    if (!device) return;
    device.open({stopBits: 0, bitRate: 9600, ctsFlowControl: 0}, function() {
      poller = setInterval(function() {
        device.send(new Uint8Array([PING]).buffer);
      }, 500);

      device.set_receive_handler(function(rawData) {
        var data = new Uint8Array(rawData);
        console.log(data);
        for (var i=0; i<data.length; i++) {
          if (data[i] == PONG) {
            connected = true;
            clearInterval(poller);
            poller = null;
            clearTimeout(watchdog);
            watchdog = null;
            device.send(new Uint8Array([0x65]).buffer);
          }
        }
      });

      watchdog = setTimeout(function() {
        device.set_receive_handler(null);
        device.close();
        device = null;
        tryNextDevice();
      }, 10000);
    });
  }

  ext._shutdown = function() {
    shutdown = true;
    device = null;
  };

  ext._getStatus = function() {
    if(!connected) return {status: 1, msg: 'Disconnected'};
    return {status: 2, msg: 'Connected'};
  }

  var descriptor = {
    blocks: [
      [' ', 'beam red beat', 'redBeat'],
      [' ', 'beam blue beat', 'blueBeat']
    ],
    url: 'https://github.com/LEGOIdeaStudio/LEGODancingRobots'
  };

  ScratchExtensions.register('LEGO Dancing Robot', descriptor, ext, {type: 'serial'});

})({});

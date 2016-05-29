

(function(ext) {    

    var pollTimeout = null;
    var device = null;
    // Cleanup function when the extension is unloaded
    ext._shutdown = function() {
	if (pollTimeout != null) {
	    clearTimeout(pollTimeout);
	    pollTimeout = null;
	}
	if (device) {
	    device.close();
	    device = null;
	}
    };

    var received = [];
    ext._deviceConnected = function(dev) {
	if(device) return;
	
	device = dev;
	device.open();
	console.log("Device connected");
	
	pollTimeout = setInterval(function() {
            device.read(function(data) {
		received = data;
	    });
        }, 100);
	pollTimeout = setInterval(function() {
		console.log(received);
        }, 1000);
    };
    
    ext._deviceRemoved = function(dev) {
	console.log("Device removed");
        if(device != dev) return;
        device = null;
        stopPolling();
    };
    
    function stopPolling() {
        if(pollTimeout) clearInterval(pollTimeout);
        pollTimeout = null;
    }

    

    ext._getStatus = function() {
        if(!device) return {status: 1, msg: 'Controller disconnected'};
        return {status: 2, msg: 'Controller connected'};
    }
    
    ext.set_state = function(state)
    {
	console.log("Sending data");
	var d = UInt8Array([]);
	device.write(d);
	console.log("Sent data");
    }
    
    
    // Block and block menu descriptions
    var descriptor = {
        blocks: [
	    [' ', 'set state %n', 'set_state',0],
        ]
    };
    // Register the extension
    ScratchExtensions.register('HID test', descriptor, ext, {type: "hid", vendor: 0x03f0, product: 0x0024});
})({});

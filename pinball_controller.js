

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
    
    ext._deviceConnected = function(dev) {
	if(device) return;
	
	device = dev;
	device.open();
	console.log("Device connected");
	
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
    
    ext.set_LED_RGB = function(index, red, green , blue)
    {
	console.log("Sending data");
	var d = UInt8Array([index, 0, index + 1, 0, red, green, blue]);
	device.write(d);
	console.log("Sent data");
    }
    
    
    // Block and block menu descriptions
    var descriptor = {
        blocks: [
	    [' ', 'set LED %n to %n,%n,%n', 'set_LED_RGB',0,0.5,0.5,0.4],
        ]
    };
    // Register the extension
    ScratchExtensions.register('Pinball controller', descriptor, ext, {type: "hid", vendor: 0xffff, product: 0xffff});
})({});

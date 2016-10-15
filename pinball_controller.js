

(function(ext) {    

    var reconnectTimer = null;
    var device = null;

    // Extension ID for the HID driver
    var helperID = "jjmgliejomegdebmoaaidfbeimfclnaj";
    var port = null;
    
    function connect() {
	reconnectTimer = null;
	port = chrome.runtime.connect(helperID, {"name":"ScratchX"});
	port.onDisconnect.addListener(function() {
	    if (chrome.runtime.lastError) {
		console.log("Connection failed: ", chrome.runtime.lastError.message);
		port = null;
		// Try to reconnect after 5s
		reconnectTimer = setTimeout(connect, 5000);
	    }
	    
	});
	port.onMessage.addListener(function(msg, port) {
	    console.log("Got reply: ", msg);
	});
    }
    // Cleanup function when the extension is unloaded
    ext._shutdown = function() {
	if (reconnectTimer != null) {
	    clearTimeout(reconnectTimer);
	    reconnectTimer = null;
	}
	if (port) {
	    port.disconnect();
	    port = null;
	}
    };
    

    ext._getStatus = function() {
        if(!port) return {status: 1, msg: 'Controller disconnected'};
        return {status: 2, msg: 'Controller connected'};
    }
    
    function clamp(num, min, max) {
	return num <= min ? min : num >= max ? max : num;
    }
    
    ext.set_LED_RGB = function(index, red, green , blue)
    {
	red = clamp(red, 0, 1);
	green = clamp(green, 0, 1);
	blue = clamp(blue, 0, 1);
	port.postMessage({request:"set_color", start: index, end: index + 1, 
			  red: red*255, green: green*255, blue: blue*255});
    }
    
    
    // Block and block menu descriptions
    var descriptor = {
        blocks: [
	    [' ', 'set LED %n to %n,%n,%n', 'set_LED_RGB',0,0.5,0.5,0.4],
        ]
    };
    // Register the extension
    ScratchExtensions.register('Pinball controller', descriptor, ext, {type: "hid", vendor: 0xffff, product: 0xffff});

    // Start connection
    connect();
    
})({});


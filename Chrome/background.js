function HIDHandler(vid,pid)
{
    var deviceId = undefined;
    var connectionId = undefined;

    start();
    chrome.hid.onDeviceAdded.addListener(deviceAdded);
    chrome.hid.onDeviceRemoved.addListener(deviceRemoved);

    function deviceAdded(device)
    {
	if (!deviceId) {
	    console.log("Connecting");
	    console.log(device.vendorId, device.productId);
	    console.log(device.productName, device.serialNumber);
	    deviceId = device.deviceId;
	    chrome.hid.connect(deviceId, deviceConnected);
	}
    }

    
    function deviceRemoved(id) {
	console.log("Device removed "+id);
	if (deviceId == id) {
	    console.log("Disconnected");
	    deviceId = undefined;
	    connectionId = undefined;
	    //chrome.hid.disconnect(id);
	}
    }
    
    
    function deviceConnected(connection)
    {
	console.log("Connected "+deviceId);
	if (chrome.runtime.lastError) {
	    console.log("Connection to HID device failed: "+
			chrome.runtime.lastError.message);
	    deviceId = undefined;
	    return;
	}
	connectionId = connection.connectionId;
	chrome.hid.receive(connectionId,
			   function receivedReport(reportId, data)
			   {
			       if (chrome.runtime.lastError) {
				   console.log("Reception from HID device failed: "+
					       chrome.runtime.lastError.message);				   chrome.hid.receive(connectionId, 
																      receivedReport);
				   return;
			       }
			       console.log("Received report: "+ reportId);
			       var view = new Uint8Array(data);
			       var str = '';
			       view.forEach(function(v)
					{
					    str += v.toString(16)+",";
					});
			       console.log("Data: "+str);
			       chrome.hid.receive(connectionId, 
						  receivedReport);
			   });
    }
    
    function gotDevices(devices)
    {
	console.log("gotDevices " + devices.length);
	for (var d = 0; d < devices.length; d++) {
	    var device = devices[d];
	    deviceAdded(device);
	}
    }

    function start()
    {
	console.log("Window opened");
	chrome.hid.getDevices({filters:[{}]}, gotDevices);
    }

    function send_done() {
	if (chrome.runtime.lastError) {
	    console.log("Failed to send report: "+ chrome.runtime.lastError.message);
	}
    }

    this.set_color = function(start, end, r,g,b)
    {
	if (connectionId) {
	    var data = new Uint8Array(4+(end-start)*3);
	    data.set([start,0,end,1]);
	    var p = 4;
	    for (var x = start; x < end; x++) {
		data.set([r, g,b], p);
		p += 3;
	    }
	    console.log("Sending ", data);
	    chrome.hid.send(connectionId, 2, data.buffer,
			    send_done);
	    console.log("Sent "+data.length+" bytes");
	}
    }
}

var handler;
chrome.app.runtime.onLaunched.addListener(function() {
    /*chrome.app.window.create('window.html', {
	'id': 'PinballControllerMainWindowID',
	'outerBounds': {
	    'width': 400,
	    'height': 500
	}
    });*/
    handler = new HIDHandler(0xffff,0xffff);
    chrome.runtime.onConnectExternal.addListener(function(port) {
	console.log("Connection from ",port.name);
	port.onMessage.addListener(function(msg, port) {
	    console.log("Got request: ", msg);
	    handle_request(msg);
	});
	port.onDisconnect.addListener(function() {
	    console.log("Disconnected");
	});

	function handle_request(msg)
	{
	    var f = {"set_color": set_color}[msg.request];
	    if (f) {
		var error = f(msg);
		if (!error) {
		    port.postMessage({status: "OK"});
		} else {
		    port.postMessage({status: "ERROR", message:error});
		}
	    } else {
		port.postMessage({status: "ERROR", message:"Unknown request"});
	    }
	}
	function set_color(msg) 
	{
	    handler.set_color(msg.start, msg.end,msg.red,msg.green, msg.blue);
	}
    });
    
    console.log("Launched");
});

chrome.runtime.onSuspend.addListener(function() {
    console.log("Suspended");
    handler = undefined;
});

				     
					 

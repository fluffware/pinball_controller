
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
					       chrome.runtime.lastError.message);				   
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
    this.send_report = function ()
    {
	console.log("send_report");
	var color = document.querySelector("#color");
	var first = document.querySelector("#first_lamp");
	var last = document.querySelector("#last_lamp");
	if (!color || !first || !last) return;
	console.log(color.value);
	var r = parseInt(color.value.substr(1,2),16);
	var g = parseInt(color.value.substr(3,2),16);
	var b = parseInt(color.value.substr(5,2),16);
	var first = parseInt(first.value) - 1;
	var last = parseInt(last.value);
	if (connectionId) {
	    var data = new Uint8Array(4+(last-first)*3);
	    data.set([first,0,last,1]);
	    var p = 4;
	    for (var x = first; x < last; x++) {
		data.set([r, g,b], p);
		p += 3;
	    }
	    chrome.hid.send(connectionId, 2, data.buffer,
			    send_done);
	    console.log("Sent "+data.length+" bytes");
	}
    }
}

var handler;
document.addEventListener('DOMContentLoaded', function () {
    handler = new HIDHandler(0xffff,0xffff);

    var send_report = document.querySelector("#send_report");
    if (send_report) {
	send_report.addEventListener('click', function() {handler.send_report()});
    }
    var first = document.querySelector("#first_lamp");
    var last = document.querySelector("#last_lamp");
    if (first) {
	first.addEventListener('input', function() {
	    if (parseInt(first.value) > parseInt(last.value)) {
		last.value = first.value;
	    }
	});
    }
    if (last) {
	last.addEventListener('input', function() {
	    if (parseInt(first.value) > parseInt(last.value)) {
		last.value = first.value;
	    }
	});
    }
    
});



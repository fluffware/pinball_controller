function PinnballConnection() {
    var device;
    
    this.connect = function() {
	if (device) return;
	console.log("Connect");
	chrome.getDevices([], gotDeviceList);
	
	
    }
    
    function getDeviceFailed(error)
    {
	console.log("Failed to get USB device: "+error);
    }

    function gotDeviceList(list)
    {
	console.log("Got "+list.length+" devices");
    }
    
    
    function gotDevice(d)
    {
	device = d;
	console.log("Manufacturer: "+device.manufacturerName);
	console.log("Product: "+device.productName);
	device.open()
	    .then(()=>device.selectConfiguration(1))
	    .then(()=>device.claimInterface(0))
	    .then(pollDevice)
	    .catch(error => {console.log("Setting up device failed: "+error)});
    }
    function pollDevice() {
	console.log("Polling ");
	device.transferIn(1, 10)
	    .then(getReport,reportFailed);
    }

    function byte2bin(v) {
	var str = v.toString(2);
	return (("00000000"+str).slice(-8));
    }
    
    function getReport(res) {
	if (!device) return;
	if (res.status == "ok") {
	    if (res.data.getUint8(0) == 3) {
		console.log(byte2bin(res.data.getUint8(1)) + " "
			    + byte2bin(res.data.getUint8(2)));
	    }
	}
	device.transferIn(1, 10)
	    .then(getReport, reportFailed);
    }

    function reportFailed(error) {
	if (!device) return;
	console.log("Failed to read report");
    }
    
    this.disconnect = function() {
	if (!device) return;
	var d = device;
	device = undefined;
	d.releaseInterface(0)
	    .then(() => d.close());
    }

    this.set_color = function(start, end, r,g,b)
    {
	if (!device) return
	var data = new Uint8Array(5+(end-start)*3);
	data.set([2,start,0,end,1]);
	var p = 5;
	for (var x = start; x < end; x++) {
	    data.set([r, g,b], p);
	    p += 3;
	}
	device.controlTransferOut({
	    requestType: 'class',
	    recipient: 'interface',
	    request: 0x09, // SET_REPORT
	    value: 0x0202,
	    index: 0x00},
				  data)
	    .catch(() => {console.log("Failed to send report");});
    }
}

window.onload = function() {
    var conn = new PinnballConnection();
    var button = document.getElementById("connect");
    button.onclick = function() {
	conn.connect();
    };
    var button = document.getElementById("disconnect");
    button.onclick = function() {
	conn.disconnect();
    };
    var button = document.getElementById("set_color");
    button.onclick = set_color;

    var color = document.querySelector("#color");
    var first_lamp = document.querySelector("#first_lamp");
    var last_lamp = document.querySelector("#last_lamp");
    console.log(color, first_lamp, last_lamp);
    if (first_lamp) {
	first_lamp.addEventListener('input', function() {
	    if (parseInt(first_lamp.value) > parseInt(last_lamp.value)) {
		last_lamp.value = first_lamp.value;
	    }
	});
    }
    if (last_lamp) {
	last_lamp.addEventListener('input', function() {
	    if (parseInt(first_lamp.value) > parseInt(last_lamp.value)) {
		last_lamp.value = first_lamp.value;
	    }
	});
    }

    function set_color()
    {
	console.log("set_color");
	if (!color || !first_lamp || !last_lamp) return;
	console.log(color.value);
	var r = parseInt(color.value.substr(1,2),16);
	var g = parseInt(color.value.substr(3,2),16);
	var b = parseInt(color.value.substr(5,2),16);
	var first = parseInt(first_lamp.value) - 1;
	var last = parseInt(last_lamp.value);
	conn.set_color(first,last, r,g,b);
    }
};


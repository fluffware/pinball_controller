document.addEventListener('DOMContentLoaded', function () {
    var helper_id = "jjmgliejomegdebmoaaidfbeimfclnaj";
    var port = chrome.runtime.connect(helper_id, {"name":"Test client"});
    console.log(helper_id);
    port.onDisconnect.addListener(function() {
	console.log("Disconnected");
	if (chrome.runtime.lastError) {
	    console.log("Connection failed: ", chrome.runtime.lastError.message);
	}
    });
			      
    port.onMessage.addListener(function(msg, port) {
	console.log("Got reply: ", msg);
    });
    console.log(port);
    
    var first = document.querySelector("#first_lamp");
    var last = document.querySelector("#last_lamp");
    var color_sel = document.querySelector("#color");
    if (color_sel) {
	color_sel.addEventListener('change', function() {
	    console.log(color_sel);
	    var r = parseInt(color_sel.value.substr(1,2),16);
	    var g = parseInt(color_sel.value.substr(3,2),16);
	    var b = parseInt(color_sel.value.substr(5,2),16);
	    var start = parseInt(first.value) - 1;
	    var end = parseInt(last.value);
	    set_color(start,end,r, g, b);
	});
    }
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
    var color_buttons = document.querySelectorAll(".color_table td");
    var colorre = /rgb\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\)/
    for(var i = 0; i < color_buttons.length; i++) {
	var color = window.getComputedStyle(color_buttons[i]).getPropertyValue('background');
	var rgb = color.match(colorre)
	if (rgb) {
	    var r = parseInt(rgb[1]);
	    var g = parseInt(rgb[2]);
	    var b = parseInt(rgb[3]);
	    console.log(r,g,b);
	    add_color_listener(r,g,b);
	}

	
    }
    
    function add_color_listener(r,g,b) {
	color_buttons[i].addEventListener('click', function() {
	    console.log(r,g,b);
	    var start = parseInt(first.value) - 1;
	    var end = parseInt(last.value);
	    set_color(start,end,r, g, b);
	});
    }
    
    function set_color(start,end,r,g,b)
    {
	port.postMessage({request: "set_color", start,end, red:r, green:g, blue:b});	
    }
});


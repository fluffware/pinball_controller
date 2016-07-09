chrome.app.runtime.onLaunched.addListener(function() {
    chrome.app.window.create('window.html', {
	'id': 'PinballControllerMainWindowID',
	'outerBounds': {
	    'width': 400,
	    'height': 500
	}
    });
    console.log("Launched");
});

chrome.runtime.onSuspend.addListener(function() {
    console.log("Suspended");
});

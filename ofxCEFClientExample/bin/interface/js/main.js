
$(document).ready(function() {

	$('#big_orange_button').on('click', function(event) {
		var msg = "You Pressed The Big Orange Button!"; 
		app.sendMessage('big_orange_button', [msg]);
	}); 

	app.setMessageCallback('random_ofx', function(name, args) {
		$('#big_orange_button').text(args); 
	});


});
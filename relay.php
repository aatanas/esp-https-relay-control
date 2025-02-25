//Aleksandar Atanasovski
<?php
// GET request handling
if ($_SERVER["REQUEST_METHOD"] === "GET") {
	$state = file_get_contents("relay.dat");
    if(!$state){
		http_response_code(204); // No Content
	}
	else{
		http_response_code(205); // Reset Content
		$state = false;
		file_put_contents("relay.dat", $state);
	}
}

// POST request handling
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    // Access POST data using $_POST superglobal
    $data = file_get_contents("php://input");
	if($data === "passphrase"){
		$state = true;
		file_put_contents("relay.dat", $state);
	}
}
?>

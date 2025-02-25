// Aleksandar Atanasovski
<?php
// GET request handling
if ($_SERVER["REQUEST_METHOD"] === "GET") {
	$state = file_get_contents("sched.dat");
    if(!$state){
		http_response_code(204); // No Content - Schedule OFF
	}
	else{
		http_response_code(205); // Reset Content - Schedule ON
	}
}

// POST request handling
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    // Access POST data
    $data = file_get_contents("php://input");
	if($data === "passphrase"){
		$state = file_get_contents("sched.dat");
		$state ^= 1;
		file_put_contents("sched.dat", $state);
	}
}
?>

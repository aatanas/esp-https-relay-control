/*
  Aleksandar Atanasovski
  Based on the example created by Ivan Grokhotkov, 2015 (File > Examples > ESP8266WiFi > HTTPSRequests)
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

#define RELAY_PIN 0

//Replace with your web server
#define API_BASE_URL https://yourserver.com/api/

// Replace with your servers certificate
const char my_cert [] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)CERT";

// Replace with your network credentials
const char ssid[] = "";
const char password[] = "";

// Create a list of certificates with the server certificate
X509List cert(my_cert);

WiFiClientSecure client;
uint8_t relay_st;
HTTPClient https;

void setup() {

	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, LOW);
	wifi_set_sleep_type(LIGHT_SLEEP_T);

	//Connect to Wi-Fi
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
	}

	// Set time via NTP, as required for x.509 validation
	configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

	time_t now = time(nullptr);
	while (now < 8 * 3600 * 2) {
		delay(500);
		now = time(nullptr);
	}
	client.setTrustAnchors(&cert);
}

void loop() {

	// wait for WiFi connection
	if ((WiFi.status() == WL_CONNECTED)) {

		time_t now = time(nullptr);
		struct tm timeinfo;
		gmtime_r(&now, &timeinfo);

		if ((timeinfo.tm_hour == 0 || timeinfo.tm_hour == 5) && timeinfo.tm_min == 0) {
			if (https.begin(client, "API_BASE_URL/sched.php")) {  // HTTPS
				int httpCode = https.GET();
				https.end();

				if (httpCode == HTTP_CODE_RESET_CONTENT) {
					if (timeinfo.tm_hour == 0) {
						digitalWrite(RELAY_PIN, HIGH);
					} else {
						digitalWrite(RELAY_PIN, LOW);
					}
				}
				delay(10000); //making sure we move to next minute
			}
		} else if (https.begin(client, "API_BASE_URL/relay.php")) { // HTTPS

			int httpCode = https.GET();
			https.end();

			if (httpCode == HTTP_CODE_RESET_CONTENT) {
				relay_st ^= 1;
				digitalWrite(RELAY_PIN, relay_st);
			}
		}

	}
	delay(50000); //polling interval
}

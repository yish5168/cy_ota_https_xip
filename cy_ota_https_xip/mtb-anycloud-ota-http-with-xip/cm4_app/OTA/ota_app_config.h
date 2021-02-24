/******************************************************************************
* File Name: ota_app_config.h
*
* Description: Contains all the configurations required for the OTA App.
*
*******************************************************************************
* (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#ifndef SOURCE_OTA_APP_CONFIG_H_
#define SOURCE_OTA_APP_CONFIG_H_

/***********************************************
 * Connection configuration
 **********************************************/


/* Security type of the Wi-Fi access point. See 'cy_wcm_security_t' structure
 * in "cy_wcm.h" for more details.
 */
#if 1
        #define WIFI_SSID                         "ruigao"
        #define WIFI_PASSWORD                     "21671516"
#endif

#define WIFI_SECURITY       (CY_WCM_SECURITY_WPA2_AES_PSK)

/* HTTP Server */
#define HTTP_SERVER         "192.168.4.47"//"192.168.0.110"
//#define HTTP_SERVER         "192.168.199.225"

/* Macro to enable/disable TLS */
#define ENABLE_TLS          (0)

#if (ENABLE_TLS == true)
/* HTTP Server Port */
#define HTTP_SERVER_PORT    (443)
#else
/* HTTP Server Port */
#define HTTP_SERVER_PORT    (8080)
#endif

/* Name of the JSON job file for HTTP  */
#define OTA_HTTP_JOB_FILE    "/ota_update.json"
#define OTA_HTTP_JOB_FILE1   "/ota_update1.json"

/**********************************************
 * Certificates and Keys - TLS Mode only
 *********************************************/
/* Root CA Certificate -
   Must include the PEM header and footer:

        "-----BEGIN CERTIFICATE-----\n" \
        ".........base64 data.......\n" \
        "-----END CERTIFICATE-------\n"
*/
#define ROOT_CA_CERTIFICATE \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDmTCCAoGgAwIBAgIJAI/8h7TAHRz4MA0GCSqGSIb3DQEBCwUAMGMxCzAJBgNV\n"\
"BAYTAkNOMRAwDgYDVQQIDAdTaUNodWFuMRAwDgYDVQQHDAdDaGVuZ0R1MQswCQYD\n"\
"VQQKDAJDWTEUMBIGA1UECwwLRW5naW5lZXJpbmcxDTALBgNVBAMMBG15Q0EwHhcN\n"\
"MjAxMjE1MDgzNjU5WhcNMzAxMjEzMDgzNjU5WjBjMQswCQYDVQQGEwJDTjEQMA4G\n"\
"A1UECAwHU2lDaHVhbjEQMA4GA1UEBwwHQ2hlbmdEdTELMAkGA1UECgwCQ1kxFDAS\n"\
"BgNVBAsMC0VuZ2luZWVyaW5nMQ0wCwYDVQQDDARteUNBMIIBIjANBgkqhkiG9w0B\n"\
"AQEFAAOCAQ8AMIIBCgKCAQEA1SvOs8d0ZyM2uw9hDeevNxYghVLQgsvOjtSfUme9\n"\
"s111Uoz3SA0xgierq2MYVfZ1PFRwPFQVUG65FNsBPeDKJ4e3wIw0CDWPmyVexuUS\n"\
"ZNztIntLvMx8l1wzIkRe9PwQS5A39M471E7dKf19GzYbEXY6z1MUxtFqBixITXOM\n"\
"0g32JMISwY+l/pC6XAiQ589rYPlbbZOVGgxJXam4O+WiASRWbvp2lpICump1Kpjl\n"\
"bsdD9dCQNhHWaOG5aXehs8/maXYo2yMO1ZNtzrZcToGh4qT0D82gMuZwYXhuG25j\n"\
"4NSuK/sAsIB/wztYfhOIvf9dixjs3mvyvDMYn7udUkaxpwIDAQABo1AwTjAdBgNV\n"\
"HQ4EFgQU8sT8SXKXHEJF00tldCMjxZ9HeOMwHwYDVR0jBBgwFoAU8sT8SXKXHEJF\n"\
"00tldCMjxZ9HeOMwDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAZF+P\n"\
"/owAd0giBWFQU9Jeuq7tWUJPyMdmavIaSMDCG21mZ8PHOpXDYPiBOhIzONTR12Zm\n"\
"oApiotHRjDtAPIhSQucMvY74C5RSVujyBq281ecQZvo8ZJDxWU0mmT0PgtI7cXLX\n"\
"NV+A7Cxu+z5EZDWN5lH5fs58yBTt3cgQuRn7YAgmT+8yu7q5gGai8bzAbQuhd1U9\n"\
"OUYfmvi+dDLuLDclSAB6nz/kVpu0OhwKYm1qzwCl0KED3BNOOxChBrQzs74xmsY0\n"\
"ed6hqRrDLOtbmSOAdyPdSWl7GQHfXMdX2JKKZiIj3U7p+1LXOr1QjGfJ2ISeYRTE\n"\
"ciJI0zynf2zMCB7eOA==\n"\
"-----END CERTIFICATE-----\n"


/* Client Certificate
   Must include the PEM header and footer:

        "-----BEGIN CERTIFICATE-----\n" \
        ".........base64 data.......\n" \
        "-----END CERTIFICATE-------\n"
*/
#define CLIENT_CERTIFICATE \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDRDCCAiwCCQDnLm/k6R089TANBgkqhkiG9w0BAQsFADBjMQswCQYDVQQGEwJD\n"\
"TjEQMA4GA1UECAwHU2lDaHVhbjEQMA4GA1UEBwwHQ2hlbmdEdTELMAkGA1UECgwC\n"\
"Q1kxFDASBgNVBAsMC0VuZ2luZWVyaW5nMQ0wCwYDVQQDDARteUNBMB4XDTIwMTIx\n"\
"NTA4MzcwMFoXDTMwMTIxMzA4MzcwMFowZTELMAkGA1UEBhMCQ04xEDAOBgNVBAgM\n"\
"B1NpQ2h1YW4xEDAOBgNVBAcMB0NoZW5nRHUxCzAJBgNVBAoMAkNZMRQwEgYDVQQL\n"\
"DAtFbmdpbmVlcmluZzEPMA0GA1UEAwwGQ2xpZW50MIIBIjANBgkqhkiG9w0BAQEF\n"\
"AAOCAQ8AMIIBCgKCAQEArfCo+45YmwXWW+flRl0wpgFRrxdqdET4Dhw9Xn5QtgsD\n"\
"pMHvIrumIP4/u63BZmNCApX6nG/TQKhu5+wEJL914M65UM8bVHn933H0KBr/LeWF\n"\
"zsjfTvwFaTd8JPadAHEDmGlCEFvsY4s1YOXmIi8DK01YaezgKruge1m+ND2ZeUDD\n"\
"cdU9u7cUzGxfseiihVNGEfGkQ7AbxiaDNFaSsEHN1cG+QZUcGREFP0j4bIrnF15L\n"\
"Z95yj2U86RfRtm1yKCeqxVDi0vZfCncRajb7EtVhe+3bbr/xD4d+6CxJ5kn91/0a\n"\
"psUd+LNGJg4XQLool1+S2a+aeLBDyTIvX57y8I9iWwIDAQABMA0GCSqGSIb3DQEB\n"\
"CwUAA4IBAQA7DED6KO2h+qk4tmtxylCu3nygEvOFdTpA5wV34oRq4Gcz9kg4eg39\n"\
"8NxVShG57Yg8WrqUDoAmO489vaZJ9I7kA76MsRhwtyBpsOdhnMHaaYMI0gMIiNIO\n"\
"Rlloiqsn/YL4+HM7MA1Pssj3r58BUuTS9CZhlVdqz1Fy4mk1qTHEPi5rceCG2Sds\n"\
"RSckPy2WFkoc0Mz7060kP/wcmiffWhq+lnop387n6RO9EBx5VzrQ/6Q1R54NykVr\n"\
"lwHm/qgYMmyjdjchS2tKqHw4qmRHnGn+B05vb0lzVAxxfRTMk6hBM2TUIcWXl6Ab\n"\
"4aMkGWehGFegY8yq6xOfSHrIbvJtg2Cw\n"\
"-----END CERTIFICATE-----\n"

/* Private Key
   Must include the PEM header and footer:

        "-----BEGIN RSA PRIVATE KEY-----\n" \
        "...........base64 data.........\n" \
        "-----END RSA PRIVATE KEY-------\n"
*/
#define CLIENT_KEY  \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEowIBAAKCAQEArfCo+45YmwXWW+flRl0wpgFRrxdqdET4Dhw9Xn5QtgsDpMHv\n"\
"IrumIP4/u63BZmNCApX6nG/TQKhu5+wEJL914M65UM8bVHn933H0KBr/LeWFzsjf\n"\
"TvwFaTd8JPadAHEDmGlCEFvsY4s1YOXmIi8DK01YaezgKruge1m+ND2ZeUDDcdU9\n"\
"u7cUzGxfseiihVNGEfGkQ7AbxiaDNFaSsEHN1cG+QZUcGREFP0j4bIrnF15LZ95y\n"\
"j2U86RfRtm1yKCeqxVDi0vZfCncRajb7EtVhe+3bbr/xD4d+6CxJ5kn91/0apsUd\n"\
"+LNGJg4XQLool1+S2a+aeLBDyTIvX57y8I9iWwIDAQABAoIBADm7Ptr/i/koHtWy\n"\
"Si7oj1UJ6wqqQvMEUTCdvuC2HCPP1A3DMJPepG0e9hhcu2w4VZh2Hw/9LaiwPt9L\n"\
"PrTTt1Q+SNPBadjIBLM9J0SIcVnSbG8NzNByZXleqR27nLCg0gisJvk3CWagtb7L\n"\
"j26ZG/wrV8UQkfac/87xImhEacQMZjO45M4WcNsJGKpi/MK1wqWWZUKJPxts5pph\n"\
"+MLO1fz1Tx0L+CJ02mW2XDaF9F2GuGedbGlIYqARXGK10UutWplGnROVypkl5puR\n"\
"RQFHjQgMBRlP1Xp9AQPIyoWeWBug/cO7ZL0AA6Hu9nQQWVUpcMB1PeYTcKdOP3Ab\n"\
"9CwVHgECgYEA5mjgWsTdXboxaD7UGLT8kpCGKcSg/WJsQRtBO4kZziEq7Aa8l3PO\n"\
"hancPpJxAc3j1xdkrryPPzJoMHj3QXhe2j+bzEk5TojPLD5tQKV8Zul7WTguWPi1\n"\
"ktf0DvslrQ8I1FXql38RzTPTcoB6YpzeqAFZi8P4Y9pc7rfY6y4ObKECgYEAwUI2\n"\
"ONfWDj0kC0ZQnhVqsUz2+ohNtetyrbjyd5RIkPAN+FV0X5Ft2G7oU0M+ZIo4AdhE\n"\
"zdGmTdT0x4MbwcH4wiIUe9HraKU9UVWv7Hxz1QKR/TUZMAYIHNMwDw/x+IqEsBZf\n"\
"Fg187xWZi2Nt2C3gyfatQwP6jKuG7sMhx0PakXsCgYBDoqmpO3zX1J5IQZk1H3u8\n"\
"8lvPyv6PIuMwQXXmqho/HhFhc6DMvJv1NKWIYKh3pR6qtx1BBezu2jcnc3rxZwpA\n"\
"+scbMqt12Ecob7LWdT2NYCBrUkiYqXlzf50PAL5QjMdEvWu9yPlKyVnalMhYRkA+\n"\
"BRdqJb62YRVCpgkxEDX/wQKBgFZjsrlWQmm7CxG/LOp9yGPZc84qgBKCXZEqy7iI\n"\
"oKepp5ULHipaaDAH4/LUbF+lKcQpWRhoT18bgxBsm0vR2dFuSpnPDNyKgQ6Kr0mX\n"\
"mG/v5jK6Obgt+hJolWMmTZQ813OfjIHfJan6jdixabjpSUL+8hSzAp0/T+ILeF4L\n"\
"BvlhAoGBANEgAFYA9pOtbqnkGVUvov5mDBAr8hyRHkQoZbQQRNIy41G3ib7EchJy\n"\
"+Nr//E7EO6nbyv+8QLQjeK1pa0NDpHepHWTk1SoGwl8ByBh4WquQO/E3wRipJJRb\n"\
"qu8yOaRVJhj7RxvvnVZP5Ft+1ZMuej5eyCaOs16pdSha/SJv21rN\n"\
"-----END RSA PRIVATE KEY-----\n"

#endif /* SOURCE_OTA_APP_CONFIG_H_ */

# ESP8266_LCD_CRYPTO_TICKER
Make a crypto ticker with the ESP8266. Scroll down for an explanation, on how to make one yourself!

## Finished Product:
![Picture of the Ticker](ticker.jpg)

## Fritzing Schematic:
![Schematic of the Ticker](FRITZING_TICKER.png)

## Prerequisites:
First of all, you're going to need an ESP8266 board. They are available off of AliExpress for a decent price. You're also going to need a 16x2 LCD with preferably a header attached to the back, it saves you time from soldering the pins. Jumper cables should be bought as well, and a breadboard. These are pretty self-explanatory. Adding a buzzer is optional. You'll also need to install some libraries, be sure to check the includes.

## How do I change the cryptocurrency displayed?
The cryptocurrency that's being tracked is hardcoded, you'll have to change it yourself if you don't want to use NANO (cryptocurrency). The program has been tested to support other cryptocurrencies, and it works just fine. Search the cryptocurrency you would like to use here: https://api.coingecko.com/api/v3/coins/list. Copy the id of the currency and replace that with nano in the code. Open an issue if you would like more help setting it up, I'll get back to you as soon as I can.

## Powered by CoinGecko API:
This project couldn't have been finished without CoinGecko's API. [Check them out!](https://coingecko.com)

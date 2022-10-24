# iotdevice

Android App for Bouy IoT

## General

The phone works as a gateway between cloud and WeMOS. Additionally, the phone provides data like GPS.
The application needs to be started manually by pressing the "Start" button.

## Phone WeMOS

Connention works via plain TCP Socket. Receiving raw values from the WeMOS and putting them into a data data structure which is later converted to a JSON string.

## Phone Cloud

Connection works via Mqtt. Data is always send in bulk (10 data packets at a time). In case of losing the connection the phone tries to reconnect to the cloud.


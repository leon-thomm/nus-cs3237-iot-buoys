## Arduino IDE

**install IDE**

```bash
sudo snap install arduino
```
(Snap, Ubuntu)

**install ESP8266 WeMOS D1 board**

Open *File->Preferences* and add to *Additional Boards Manager URLs*:

```bash
https://arduino.esp8266.com/stable/package_esp8266com_index.json
```

Open *Tools->Board->Boards Manager* and install *esp8266*.

Then select *Tools->Board->ESP8266 Boards->LOLIN (WEMOS) D1 R2 & mini*.

**install MPU9250 library**

Open *Sketch->Include Library->Manage Libraries* and install *MPU9250*.

## Arduino CLI

**install arduino-cli**

```bash
mkdir ~/.arduino-cli/
cd ~/.arduino-cli/
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
export PATH="~/.arduino-cli/bin:$PATH"
```

**install MPU9250 library**

```bash
arduino-cli lib install --git-url https://github.com/hideakitai/MPU9250
```

**ESP8266 board support**

```bash
arduino-cli config init
vim ~/.arduino15/arduino-cli.yaml
```
edit `~/.arduino15/arduino-cli.yaml` to include

```yaml
board_manager:
  additional_urls: 
    [https://arduino.esp8266.com/stable/package_esp8266com_index.json]
```

update index 

```bash
arduino-cli core update-index
```

**compile**

```bash
arduino-cli compile --fqbn esp8266:esp8266:d1 test1
```

**upload**

(compile first)

```bash
arduino-cli upload --fqbn esp8266:esp8266:d1 -p /dev/ttyUSB0 test1
```

**monitor serial output**

```bash
arduino-cli monitor -p /dev/ttyUSB0 --fqbn esp8266:esp8266:d1
```

## Resources

- https://www.youtube.com/watch?v=wazPfdGBeZA
- https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5
- https://github.com/esp8266/Arduino
- https://arduino.github.io/arduino-cli/
- https://github.com/hideakitai/MPU9250
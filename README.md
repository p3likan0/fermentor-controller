# How it works:

One microcontroler, in this case ESP8266, take temperature from fermentor with LM65, and take control of temperature switching on/off a pump.
This pump is connected to a cold bench, to transfer cold to fermentor.

When ESP8266 take a temperature of fermentor, push this temperature and pump status to InfluxDB.

# Config InfluxDB:

Install it with: `sudo apt install influxdb`, or run it Docker(`https://hub.docker.com/_/influxdb/`). Everyway add this is the configuration to standard config:

/etc/influxdb/influxdb.conf

```
[[udp]]
  enabled = true
  bind-address = ":8089"
  database = "fermentador_1"
```

This allows esp8266 to push data to InfluxDB over UPD port.

# Config Grafana:

Using docker: `https://hub.docker.com/r/grafana/grafana/`

Configrue grafana to use Influxdb as: http://docs.grafana.org/features/datasources/influxdb/.

# Config mos to push to InfluxDB:

vim src/main.c:

```
static const char* influx_db_server = "udp://192.168.1.234:8089";
```

Replace 192.168.1.234 with your influxdb server.

# Install mos:

```
curl -fsSL https://mongoose-os.com/downloads/mos/install.sh | /bin/bash
~/.mos/bin/mos --help      
~/.mos/bin/mos
```

# Clone Fermentator Controler:

```
git clone https://github.com/p3likan0/fermentor-controller.git
```


# Build:

```
mos build --arch esp8266
```

# Flash:

```
mos flash
```

# Console:

```
mos console
```

# Build && Flash && Console:

```
mos build --arch esp8266 && mos flash && mos console  
```

# Configuration:

### Get configurations 

```
mos call Config.Get '{"key": "temperature.up_limit"}'
```

```
mos call Config.Get '{"key": "temperature.down_limit"}'
```

```
mos call Config.Get '{"key": "temperature.termic_inertia"}'
```

```
mos call Config.Get '{"key": "temperature.samples_to_average"}'
```

### Get configurations 

```
mos config-set temperature.up_limit=20
```

```
mos config-set temperature.down_limit=20
```

```
mos config-set temperature.termic_inertia=0.7
```

```
mos config-set temperature.samples_to_average=6
```



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



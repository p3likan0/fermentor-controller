#include "mgos.h"
#include "mgos_wifi.h"
#include "mgos_adc.h"

#define PUMP_GPIO (16)
#define PRENDIDA 0
#define APAGADA 1
#define AVERAGE 6

static const char* influx_db_server = "udp://192.168.1.234:8089";
bool pump_is_on = PRENDIDA;
float last_temperatures[3];

float get_adc_average_value(){
    int analogValue = 0;
    for(int i=0; i<(int)mgos_sys_config_get_temperature_samples_to_average(); i++){
        analogValue += mgos_adc_read(0);
    }
    return analogValue/(int)mgos_sys_config_get_temperature_samples_to_average();
}

float read_temp(){
    float adc_value = get_adc_average_value();
    float millivolts = (adc_value/1024.0) * 3000;
    float temp_in_celsius = millivolts/10;
    printf("Temp: %f Pump: %d\n", temp_in_celsius, !pump_is_on);
    return temp_in_celsius;
}

void init_gpios(){
    mgos_gpio_set_mode(PUMP_GPIO, MGOS_GPIO_MODE_OUTPUT);
    mgos_gpio_set_pull(PUMP_GPIO, MGOS_GPIO_PULL_NONE);
    mgos_gpio_write(PUMP_GPIO, PRENDIDA);
}

void pump_manager(float temp_in_celsius){
    if(temp_in_celsius <= mgos_sys_config_get_temperature_down_limit()+mgos_sys_config_get_temperature_termic_inertia()){
        mgos_gpio_write(PUMP_GPIO, APAGADA);
        pump_is_on = APAGADA;
    }
    if(temp_in_celsius >= mgos_sys_config_get_temperature_up_limit()){
        mgos_gpio_write(PUMP_GPIO, PRENDIDA);
        pump_is_on = PRENDIDA;
    }
}

void send_temperature_to_server(float temperature){
    struct mg_connection* udp = mg_connect(mgos_get_mgr(), influx_db_server, NULL, NULL);
    mg_printf(udp, "inside temp=%f", temperature);
    mg_send(udp, "", 0);
    udp->flags |= MG_F_SEND_AND_CLOSE;
}

void send_pump_status_to_server(bool pump_status){
    struct mg_connection* udp = mg_connect(mgos_get_mgr(), influx_db_server, NULL, NULL);
    mg_printf(udp, "pump_status status=%d", !pump_status);
    mg_send(udp, "", 0);
    udp->flags |= MG_F_SEND_AND_CLOSE;
}

static void timer(void* arg) {
    (void)arg;
    float temperature = read_temp();
    pump_manager(temperature);
    send_temperature_to_server(temperature);
    send_pump_status_to_server(pump_is_on);
}

enum mgos_app_init_result mgos_app_init(void) {
  mgos_set_timer(1000*10, true, timer, NULL);
  init_gpios();
  return MGOS_APP_INIT_SUCCESS;
}

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_server.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include <string.h>
#include <stdlib.h>
#include "define.h"

static httpd_handle_t server = NULL;

esp_err_t root_get_handler(httpd_req_t *req)
{
    const char* html =
    "<!DOCTYPE html><html><body>"
    "<h2>Drone PID Config</h2>"
    "<form action=\"/set\" method=\"get\">"

    "<h3>ROLL ANGLE (P only)</h3>"
    "P:<input name=\"rap\"><br><br>"

    "<h3>PITCH ANGLE (P only)</h3>"
    "P:<input name=\"pap\"><br><br>"

    "<h3>ROLL RATE</h3>"
    "Kp:<input name=\"rrkp\"><br>"
    "Ki:<input name=\"rrki\"><br>"
    "Kd:<input name=\"rrkd\"><br><br>"

    "<h3>PITCH RATE</h3>"
    "Kp:<input name=\"prkp\"><br>"
    "Ki:<input name=\"prki\"><br>"
    "Kd:<input name=\"prkd\"><br><br>"

    "<h3>YAW RATE</h3>"
    "Kp:<input name=\"yrkp\"><br>"
    "Ki:<input name=\"yrki\"><br>"
    "Kd:<input name=\"yrkd\"><br><br>"

    "<input type=\"submit\" value=\"Guardar\">"
    "</form></body></html>";

    // 🔥 ESTA LINEA ES LA IMPORTANTE
    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

esp_err_t set_get_handler(httpd_req_t *req)
{
    char buf[200] = {0};
    char param[32] = {0};

    if (httpd_req_get_url_query_str(req, buf, sizeof(buf)) == ESP_OK)
    {
        // -------- ROLL ANGLE (solo P) --------
        if (httpd_query_key_value(buf, "rap", param, sizeof(param)) == ESP_OK)
            roll_gains[0] = atof(param);

        // -------- PITCH ANGLE (solo P) --------
        if (httpd_query_key_value(buf, "pap", param, sizeof(param)) == ESP_OK)
            pitch_gains[0] = atof(param);

        // -------- ROLL RATE --------
        if (httpd_query_key_value(buf, "rrkp", param, sizeof(param)) == ESP_OK)
            roll_rate_gains[0] = atof(param);

        if (httpd_query_key_value(buf, "rrki", param, sizeof(param)) == ESP_OK)
            roll_rate_gains[1] = atof(param);

        if (httpd_query_key_value(buf, "rrkd", param, sizeof(param)) == ESP_OK)
            roll_rate_gains[2] = atof(param);

        // -------- PITCH RATE --------
        if (httpd_query_key_value(buf, "prkp", param, sizeof(param)) == ESP_OK)
            pitch_rate_gains[0] = atof(param);

        if (httpd_query_key_value(buf, "prki", param, sizeof(param)) == ESP_OK)
            pitch_rate_gains[1] = atof(param);

        if (httpd_query_key_value(buf, "prkd", param, sizeof(param)) == ESP_OK)
            pitch_rate_gains[2] = atof(param);

        // -------- YAW RATE --------
        if (httpd_query_key_value(buf, "yrkp", param, sizeof(param)) == ESP_OK)
            yaw_rate_gains[0] = atof(param);

        if (httpd_query_key_value(buf, "yrki", param, sizeof(param)) == ESP_OK)
            yaw_rate_gains[1] = atof(param);

        if (httpd_query_key_value(buf, "yrkd", param, sizeof(param)) == ESP_OK)
            yaw_rate_gains[2] = atof(param);
    }

    // 🔥 Actualizar PIDs
    roll_pid.update_gains(roll_gains);
    pitch_pid.update_gains(pitch_gains);

    roll_rate_pid.update_gains(roll_rate_gains);
    pitch_rate_pid.update_gains(pitch_rate_gains);
    yaw_rate_pid.update_gains(yaw_rate_gains);

    httpd_resp_sendstr(req, "PID Gains Updated Successfully");
    return ESP_OK;
}

void start_webserver()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_start(&server, &config);

    httpd_uri_t root = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_get_handler
    };

    httpd_uri_t set = {
        .uri = "/set",
        .method = HTTP_GET,
        .handler = set_get_handler
    };

    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &set);
}

void wifi_init_softap()
{
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {};
    strcpy((char*)wifi_config.ap.ssid, "DRONE_CONFIG");
    wifi_config.ap.ssid_len = strlen("DRONE_CONFIG");
    wifi_config.ap.channel = 1;
    wifi_config.ap.max_connection = 4;
    wifi_config.ap.authmode = WIFI_AUTH_OPEN;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}
#include "GrooveWeb.h"

static const char* TAG = "WebService";

/* Answer a get request of Client */
static esp_err_t GetHandler(httpd_req_t *req)
{
    char*  buf;
    size_t buf_len;

    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = (char *) malloc(buf_len);
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Host: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-2") + 1;
    if (buf_len > 1) {
        buf = (char *) malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Test-Header-2", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Test-Header-2: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-1") + 1;
    if (buf_len > 1) {
        buf = (char *)malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Test-Header-1", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Test-Header-1: %s", buf);
        }
        free(buf);
    }

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = (char *)malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[32];
            /* Get value of expected key from query string */
            if (httpd_query_key_value(buf, "query1", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => query1=%s", param);
            }
            if (httpd_query_key_value(buf, "query3", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => query3=%s", param);
            }
            if (httpd_query_key_value(buf, "query2", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => query2=%s", param);
            }
        }
        free(buf);
    }

    /* Set some custom headers */
    httpd_resp_set_hdr(req, "Custom-Header-1", "Custom-Value-1");
    httpd_resp_set_hdr(req, "Custom-Header-2", "Custom-Value-2");

    /* Send response with custom headers and body set as the
     * string passed in user context*/
    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, strlen(resp_str));

    /* After sending the HTTP response the old HTTP request
     * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}

/* Answer a post request of Client */
static esp_err_t PostHandler(httpd_req_t *req)
{
    static MessageService& msgService = MessageService::getInstance();
    char buf[100];

    if(req->content_len >= 100)
    {
        sprintf(buf,"request too long!");
        httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
        return ESP_FAIL;
    }

    httpd_req_recv(req, buf, sizeof(buf));

    /* Log data received */
    ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
    ESP_LOGI(TAG, "%s", buf);
    ESP_LOGI(TAG, "====================================");

    std::string msg(buf);
    std::string returnMsg = msgService.handleMessage(msg);

    // End response
    httpd_resp_send(req, returnMsg.c_str(), returnMsg.length());
    return ESP_OK;
}

static const httpd_uri_t GetUri = {
    .uri       = "/api",
    .method    = HTTP_GET,
    .handler   = GetHandler,
    .user_ctx  = (void *)"Hello World!"
};
static const httpd_uri_t PostUri = {
    .uri       = "/api",
    .method    = HTTP_POST,
    .handler   = PostHandler,
    .user_ctx  = NULL
};

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "Starting HTTP server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &GetUri);
        httpd_register_uri_handler(server, &PostUri);
        return server;
    }
    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server); // Stop the httpd server
}


static void disconnect_handler(void* arg, esp_event_base_t event_base, 
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base, 
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}

GrooveWeb& GrooveWeb::getInstance()
{
	static GrooveWeb _instance;
	return _instance;
}
GrooveWeb::~GrooveWeb(){}

GrooveWeb::GrooveWeb()
{
    uint8_t mac[6];
    char   *hostname;
    WifiService::init();
    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(mdns_init());    //initialize mDNS service
    esp_efuse_mac_get_default(mac);
    if (-1 == asprintf(&hostname, "%s-%02X%02X", "Groovegrid", mac[4], mac[5])) {
        abort();
    }
    ESP_ERROR_CHECK(mdns_hostname_set(hostname));  //set hostname
    ESP_ERROR_CHECK(mdns_instance_name_set(hostname)); //set default instance
    ESP_LOGI(TAG,"MDNS initialized. hostname: %s",hostname);

    //structure with TXT records
    mdns_txt_item_t serviceTxtData[1] = {
        {"ApiPath","/api"}
    };

    //initialize service
    ESP_ERROR_CHECK( mdns_service_add("Groovegrid", "_http", "_tcp", 80, serviceTxtData, 1) );

    free(hostname);

    /* Register event handlers to stop the server when Wi-Fi or Ethernet is disconnected,
     * and re-start it upon connection.
     */
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

    /* Start the server for the first time */
    server = start_webserver();
}
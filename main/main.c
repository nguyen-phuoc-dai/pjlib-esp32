/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_vfs_fat.h"

#include "esp_pthread.h"
#include <pthread.h>

//#include "../components/FreeRTOS-POSIX/include/FreeRTOS_POSIX/semaphore.h"
//#include "../components/FreeRTOS-POSIX/include/FreeRTOS_POSIX/time.h"
//#include "activesock.h"

#include "test.h"

static const char* TAG = "main";

static pthread_t new_thread = (pthread_t)NULL;
static pthread_attr_t attr;
static wl_handle_t wl_handle = WL_INVALID_HANDLE;

static void *pjlib_thread(void *arg);

static void initialize_nvs()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void app_main()
{
	// Initialize NVS memory
	initialize_nvs();

	// Initialize the tcp stack
	tcpip_adapter_init();	// It need to run sock_test()

	int res = pthread_attr_init(&attr);
	if(res == 0) {
		attr.stacksize = 8192 * 2;
		printf("stack size = %d\n", attr.stacksize);
	} else {
		printf("Failed to initialize attribute");
		esp_restart();
	}

	res = pthread_create(&new_thread, &attr, pjlib_thread, NULL);
	if(res == 0) {
		printf("Created new POSIX thread");
	} else {
		printf("Failed to create new POSIX thread");
		esp_restart();
	}

	/*
    printf("Hello world!\n");

	struct tm* local;
	time_t t = time(NULL);
    // Get the localtime
    local = localtime(&t);
    printf("Local time and date: %s\n", asctime(local));

    // Print chip information
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Try to use pjlib
    pj_activesock_cfg var;
    var.whole_data = 0;
    if (var.whole_data == 0) {
    	printf("Can use pjlib variable\n");
    }

    printf("Restarting now.\n");
    fflush(stdout);

    esp_restart();
    */
}

static void *pjlib_thread(void *arg)
{
	const esp_vfs_fat_mount_config_t mount_config = {
			.max_files = 4,
			.format_if_mount_failed = true
	};

	esp_err_t ret = esp_vfs_fat_spiflash_mount("", "storage", &mount_config, &wl_handle);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Mounting failed: %s", esp_err_to_name(ret));
		esp_restart();
	}

	test_main();

	ret = esp_vfs_fat_spiflash_unmount("", wl_handle);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Unmounting failed: %s", esp_err_to_name(ret));
		return NULL;
	}

	return NULL;
}

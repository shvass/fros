#include "esp_system.h"
#include "esp_log.h"
// #include "esp_console.h"
// #include "esp_vfs_dev.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include <esp_spiffs.h>
#include <dirent.h>
#include <fcntl.h>

// #include "payload.h"
#include "../payload/payload-array.h"

#include <urosHandler.hpp>


static const char* TAG = "main";

esp_vfs_spiffs_conf_t conf = {
    .base_path = "/overlays",
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = true
};

std::string overlay_dir = "/overlays";

void init_fs(){
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
        esp_spiffs_format(conf.partition_label);
        return;
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    // Check consistency of reported partiton size info.
    if (used > total) {
        ESP_LOGW(TAG, "Number of used bytes cannot be larger than total. Performing SPIFFS_check().");
        esp_spiffs_format(conf.partition_label);
        // Could be also used to mend broken files, to clean unreferenced pages, etc.
        // More info at https://github.com/pellepl/spiffs/wiki/FAQ#powerlosses-contd-when-should-i-run-spiffs_check
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
            return;
        } else {
            ESP_LOGI(TAG, "SPIFFS_check() successful");
        }
    }
};


void save_overlay_to_file(char* id, char* buffer, size_t size){

    std::string filePath = overlay_dir + std::string("/") + id;
    FILE* fd = fopen(filePath.c_str(), "w");
    fwrite(buffer, 1, size, fd);
    fclose(fd);
    
    ESP_LOGD(TAG, "saved %s size %d", filePath.c_str(), size);
};


void elf_passover(char* bin, size_t size, bool save = false);



urosHandler* rosHandler = nullptr;
extern "C" void app_main(void) {

    // initalize filesystem
    init_fs();


    urosHandler::config cfg = {
        // .node_name = "fros",
        .mode = urosHandler::config::TRANSPORT_USB,
    };

    
    rosHandler = new urosHandler(cfg);

    rosHandler->addThreadExecutor({
        // new urosLoader(),
    }, APP_CPU_NUM
    );



    DIR *dp;
    struct dirent *ep;

    
    ESP_LOGI(TAG, "listing available overlays");
    dp = opendir(overlay_dir.c_str());
    FILE* fd;
    struct stat ovStat;
    char* buffer;

    if (dp != NULL){
        while ((ep = readdir (dp))) {
            std::string filePath = overlay_dir + std::string("/") + ep->d_name;
            stat(filePath.c_str(), &ovStat);

            fd = fopen(filePath.c_str(), "r");
            buffer = new char[ovStat.st_size];

            fread(buffer, 1, ovStat.st_size, fd);
            fclose(fd);


            
            ESP_LOGI(TAG, "found overlay %s size %ld", (ep->d_name), ovStat.st_size);
            elf_passover(buffer, ovStat.st_size);
        };
        
        closedir(dp);
    }
    else
        ESP_LOGI(TAG, "Couldn't open the directory");

}

#include "esp_system.h"
#include "esp_log.h"
// #include "esp_console.h"
// #include "esp_vfs_dev.h"

// #include "payload.h"
#include "../payload/payload-array.h"

#include <urosHandler.hpp>
#include <urosLoader.hpp>


static const char* TAG = "main";




urosHandler* rosHandler = nullptr;
extern "C" void app_main(void) {

    urosHandler::config cfg = {
        // .node_name = "fros",
        .mode = urosHandler::config::TRANSPORT_USB,
    };

    
    rosHandler = new urosHandler(cfg);

    rosHandler->addThreadExecutor({
        // new urosLoader(),
    }, APP_CPU_NUM
    );



    while(1){

        vTaskDelay(pdMS_TO_TICKS(100));
    };
}

#include "utils/Logger.h"
#include <iostream>
#include <fstream>
#include <cassert>

int main() {
    std::cout << "Running Logger Tests..." << std::endl;
    
    try {
        auto& logger = Logger::getInstance();
        
        // Test basic configuration
        logger.setLogFile("test.log");
        logger.setLogLevel(Logger::Level::DEBUG);
        logger.setConsoleOutput(false);
        logger.setFileOutput(true);
        
        // Test logging
        logger.debug("Debug test");
        logger.info("Info test");
        logger.warn("Warning test");
        logger.err("Error test");
        logger.fatal("Fatal test");
        
        // Test macros
        LOG_DEBUG("Macro debug test");
        LOG_INFO("Macro info test");
        LOG_WARN("Macro warning test");
        LOG_ERR("Macro error test");
        LOG_FATAL("Macro fatal test");
        
        // Test conditional debug
        LOG_DEBUG_IF(true, "Conditional debug - should appear");
        LOG_DEBUG_IF(false, "Conditional debug - should NOT appear");
        
        // Verify file was created
        std::ifstream file("test.log");
        if (file.good()) {
            std::cout << "All Logger tests passed! ✅" << std::endl;
            file.close();
            std::remove("test.log");
            return 0;
        } else {
            std::cout << "Test failed: log file not created ❌" << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}

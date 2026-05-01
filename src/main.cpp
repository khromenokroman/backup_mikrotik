#include <fmt/format.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ssh_client.hpp>

#include "backup.hpp"

int main() {
    try {
        Backup backup("192.168.88.1", 22, "admin", "Test");
        backup.backup_text_config();
        return EXIT_SUCCESS;

    } catch (std::exception const &ex) {
        ::fmt::print(stderr, "Ошибка во время работы программы: {}\n", ex.what());
        return EXIT_FAILURE;
    }
}

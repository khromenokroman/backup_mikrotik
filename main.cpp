#include <algorithm>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <ssh_client.hpp>

std::string get_name_device(std::string_view replay) {
    std::string a(replay);
    std::ranges::remove(a, '\r');
    std::string res;
    auto p1 = a.find(':');
    if (std::string::npos != p1) {
        auto p2 = a.find('\n');
        if (p2 != std::string::npos) {
            res = a.substr(p1 + 1, p2 - p1 - 1);
        } else {
            throw std::runtime_error("Не найден символ конца строки '\\n' в ответе от устройства");
        }
    } else {
        throw std::runtime_error("Не найден разделитель ':' в ответе от устройства");
    }
    return res;
}

int main() {
    SshClient client("192.168.88.1", 22, "admin", "Test");
    auto a1 = client.execute("system/identity/print");
    auto device = get_name_device(a1);
    std::cout << device << std::endl;

    std::filesystem::path base{"backup"};
    std::filesystem::create_directories(base / device);

    auto back = client.execute("export show-sensitive");
    std::ranges::remove(back, '\r');
    auto file_bk = base / device / "backup";
    std::ofstream file(file_bk);
    if (!file.is_open()) {
        auto err = errno;
        throw std::runtime_error(
                ::fmt::format("Не удалось открыть файл '{}' для записи: {}", file_bk.string(), strerror(err)));
    }
    file << back;
    file.close();

    return 0;
}

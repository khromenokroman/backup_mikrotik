#include <algorithm>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <ssh_client.hpp>

class Backup {
public:
    explicit Backup(std::string_view host, int port, std::string_view user, std::string_view pass) :
        m_client{host, port, user, pass}, m_device_name{get_name_device()} {}

    void backup_text_config() {
        std::filesystem::path base{"backup"};
        std::filesystem::create_directories(base / m_device_name);

        auto back = m_client.execute("export show-sensitive");
        std::ranges::remove(back, '\r');
        auto file_bk = base / m_device_name / "backup";
        std::ofstream file(file_bk);
        if (!file.is_open()) {
            auto err = errno;
            throw std::runtime_error(
                    ::fmt::format("Не удалось открыть файл '{}' для записи: {}", file_bk.string(), strerror(err)));
        }
        file << back;
    }

private:
    std::string get_name_device() {
        auto a = m_client.execute("system/identity/print");
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

    SshClient m_client; // 128
    std::string m_device_name; // 32
};

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

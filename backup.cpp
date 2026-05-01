#include "backup.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
Backup::Backup(std::string_view host, int port, std::string_view user, std::string_view pass)
    : m_client{host, port, user, pass}, m_device_name{get_name_device()} {}
void Backup::backup_text_config() {
    std::filesystem::path base{"backup"};
    std::filesystem::create_directories(base / m_device_name);

    auto back = m_client.execute("export show-sensitive");
    std::ranges::remove(back, '\r');
    auto file_bk = base / m_device_name / ::fmt::format("backup_{}", get_current_time_for_filename());
    std::ofstream file(file_bk);
    if (!file.is_open()) {
        auto err = errno;
        throw std::runtime_error(::fmt::format("Не удалось открыть файл '{}' для записи: {}", file_bk.string(), strerror(err)));
    }
    file << back;
}
std::string Backup::get_current_time_for_filename() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
    localtime_r(&time, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");

    return oss.str();
}
std::string Backup::get_name_device() {
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
#pragma once
#include <string_view>

#include "ssh_client.hpp"

class Backup {
   public:
    explicit Backup(std::string_view host, int port, std::string_view user, std::string_view pass);
    ~Backup() = default;

    Backup(Backup const&) = delete;
    Backup(Backup&&) = delete;
    Backup& operator=(Backup const&) = delete;
    Backup& operator=(Backup&&) = delete;

    void backup_text_config();

   private:
    std::string get_current_time_for_filename();
    std::string get_name_device();

    SshClient m_client;        // 128
    std::string m_device_name; // 32
};

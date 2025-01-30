#include <iostream>
#include "nlohmann/json.hpp"
#include <dpp/dpp.h>
#include <nlohmann/json.hpp>

using namespace std;
using namespace dpp;
json parseEnvFile() {
    json envData;
    std::ifstream file(".env");
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);

                // Key ve value'leri json yapýsýna ekle
                envData[key] = value;
            }
        }
        file.close();
    }
    return envData;
}
json envData = parseEnvFile();
int main() {
    setlocale(LC_ALL, "Turkish");
    dpp::cluster bot(envData["BOT_TOKEN"], dpp::i_default_intents | dpp::i_message_content);
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (event.shard_id == 0) {
            std::cout << "Bot baþarýyla baðlandý!" << std::endl;
        }
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand ping("ping", "pong!", bot.me.id);
            bot.global_bulk_command_create({ ping });
        }

        bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_game, "Github: Keremlolgg"));
        });
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("pong!");
            std::cout << "Pong!" << endl;
        }
    });

    bot.start(dpp::st_wait);
    return 0;
}

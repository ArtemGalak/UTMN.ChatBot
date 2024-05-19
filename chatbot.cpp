#include "crow.h"
#include <iostream>
#include <string>

std::string generate_response(const std::string& user_message) {
    std::string message_lower = user_message;
    std::transform(message_lower.begin(), message_lower.end(), message_lower.begin(), ::tolower);

    if (message_lower.find("привет") != std::string::npos) {
        return "Здравствуйте! Как я могу помочь вам сегодня?";
    }
    else if (message_lower.find("приемная комиссия") != std::string::npos) {
        return "Вы можете связаться с приемной комиссией по телефону +7 (495) 123-45-67 или по электронной почте admission@university.ru.";
    }
    else if (message_lower.find("курсы") != std::string::npos) {
        return "Наш университет предлагает широкий спектр курсов. Подробности вы можете найти на нашем сайте в разделе 'Курсы'.";
    }
    else if (message_lower.find("стипендия") != std::string::npos) {
        return "Информация о стипендиях доступна в разделе 'Стипендии' на нашем сайте.";
    }
    else {
        return "Извините, я не понимаю ваш запрос. Пожалуйста, задайте вопрос по-другому.";
    }
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/chatbot").methods(crow::HTTPMethod::Post)([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x) {
            return crow::response(400);
        }

        std::string user_message = x["message"].s();
        std::string response_message = generate_response(user_message);

        crow::json::wvalue response;
        response["response"] = response_message;

        return crow::response(response);
        });

    app.port(18080).multithreaded().run();
}
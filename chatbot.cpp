#include <iostream>
#include <string>
#include <algorithm>

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
    setlocale(LC_ALL, "Russian");
    std::string user_message;
    std::cout << "Добро пожаловать в чат-бот университета. Введите ваше сообщение:" << std::endl;

    while (true) {
        std::getline(std::cin, user_message);
        if (user_message == "выход" || user_message == "exit") {
            std::cout << "До свидания!" << std::endl;
            break;
        }
        std::string response_message = generate_response(user_message);
        std::cout << response_message << std::endl;
    }

    return 0;
}

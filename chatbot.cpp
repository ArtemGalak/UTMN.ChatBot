#include <iostream>
#include <string>
#include <algorithm>
#include <stack>

using namespace std;

string generate_response(const string& user_message) {
    string message_lower = user_message;
    transform(message_lower.begin(), message_lower.end(), message_lower.begin(), ::tolower);

    if (message_lower.find("hi") != string::npos || message_lower.find("hello") != string::npos) {
        return "Здравствуйте! Как я могу помочь вам сегодня?";
    }
    else if (message_lower.find("uni") != string::npos) {
        return "Вы можете связаться с приемной комиссией по телефону +7 (495) 123-45-67 или по электронной почте admission@university.ru.";
    }
    else if (message_lower.find("course") != string::npos) {
        return "Наш университет предлагает широкий спектр курсов. Подробности вы можете найти на нашем сайте в разделе 'Курсы'.";
    }
    else if (message_lower.find("paid") != string::npos || message_lower.find("stepend") != string::npos) {
        return "Информация о стипендиях доступна в разделе 'Стипендии' на нашем сайте.";
    }
    else if (message_lower.find("help") != string::npos) {
        return "Да, конечно, я помогу вам.";
    }
    else if (message_lower.find("bot") != string::npos) {
        return "Бот работает!))";
    }
    else {
        return "Извините, я не понимаю ваш запрос. Пожалуйста, задайте вопрос по-другому.";
    }
}

class FuncBot {
    stack<string> lfunc;
    ///     это стек для спец функций бота
    ///     туда данные пока заганяются вручную
    ///     потом этот кастыль заменю на считывание данных из файл txt
public:
    FuncBot() {
        lfunc.push("help");
        lfunc.push("call");
        lfunc.push("mail"); // специально написано неполное слово, чтобы было больше совпадений
    }

    void botfunc() {
        while (!lfunc.empty()) {
            cout << "Специальная функция бота: " << lfunc.top() << endl;
            lfunc.pop();
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    FuncBot objFuncBot;

    cout << "Добро пожаловать в чат-бот университета. Как я могу вам помочь?" << endl;
    cout << "Вы можете воспользоваться моими специальными функциями:" << endl;
    objFuncBot.botfunc(); // Использование специальной функции бота

    string user_message;

    while (true) {
        getline(cin, user_message);
        if (user_message == "выход" || user_message == "exit" || user_message == "пока") {
            cout << "До свидания!" << endl;
            break;
        }
        string response_message = generate_response(user_message);
        cout << response_message << endl;
    }

    return 0;
}

/// Пожалуйста, используйте этот счётчик при каждом нахождении бага: 0
/// А это будет счётчик всех неудачных попыток запуска: 16

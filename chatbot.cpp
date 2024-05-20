#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <fstream>
#include <unordered_map>

using namespace std;

string generate_response(const string& user_message) {
    string message_lower = user_message;
    transform(message_lower.begin(), message_lower.end(), message_lower.begin(), ::tolower);

    cout << "UTMN_Bot: ";

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
        return "UTMN_Bot: Вы всегда можете воспользоваться моей специальной командой 'help'.";
    }
}

class BotT9 {
private:
    unordered_map<string, vector<string>> fileWordMap;

public:
    BotT9() {
        // это файлы-группы для Т9
        
        loadFiles("study.txt");
        //loadFiles("uni.txt"); - Этот файл ещё не готов
        loadFiles("stepend.txt");
    }

    void loadFiles(const string& filename) {
        ifstream file(filename);

        if (file.is_open()) {
            string word;
            while (file >> word) { // Считываем каждое слово отдельно
                for (auto& c : word) {
                    c = tolower(c); // Приводим к нижнему регистру
                }
                fileWordMap[filename].push_back(word);
            }
            // чтение производиться столбцом
            file.close();
        }
        else {
            cerr << "Ошибка открытия файла " << filename << '\n';
        }
    }

    void findSimilarWords(const string& user_message) {
        bool found = false;

        for (const auto& pair : fileWordMap) {
            const auto& wordsList = pair.second;

            for (const auto& word : wordsList) {
                if (isSimilar(user_message, word)) {
                    found = true;

                    // Выводим все слова из списка, кроме найденного (а зачем нам найденное? =) )
                    cout << "UTMN_Bot: Вам может подойти другие варианты, которые представленны ниже" << endl;
                    for (const auto& w : wordsList) {
                        if (w != word) {
                            cout << w << endl;
                        }
                    }
                    break;
                }
            }
        }

        if (!found) {
            cerr << "Извините, я не понимаю ваш запрос. Пожалуйста, задайте вопрос по - другому.\n"; // Надо ещё доработать штатные фразы
        }
    }

    bool isSimilar(string s1, string s2) {
        int n = s1.length();
        int m = s2.length();

        if (abs(n - m) > 1)
            return false;

        int count = 0;

        for (int i = 0, j = 0; i < n && j < m;) {
            if (s1[i] != s2[j]) { // надо ещё доделать эту проверку т.к пользователь может пропустить букву в слове
                count++;
                if (count > 1) { // Пользователь допустил ошибку в слове (при чём различие может быть лишь в 1 символ)
                    return false;
                }
                if (n == m) {
                    i++;
                    j++;
                }
                else if (n > m) { // Учитываем различие в длине строк
                    i++;
                }
                else {
                    j++;
                }
            }
            else {
                i++;
                j++;
            }
        }

        count += abs(n - m); // Учитываем разницу длин

        return count <= 1; // Разрешаем до одной ошибки
    }

};

class FuncBot {
    stack<string> lfunc;

public:
    FuncBot() {
        // Считываем специальные функции бота из файла
        ifstream file("FuncNames.txt");
        if (file.is_open()) {
            string funcName;
            while (getline(file, funcName)) {
                lfunc.push(funcName);
            }
            file.close();
        }
        else {
            cerr << "Ошибка открытия файла" << endl;
        }
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
    BotT9 bot;


    cout << "UTMN_Bot: Добро пожаловать в чат-бот университета. Как я могу вам помочь?" << endl;
    cout << "UTMN_Bot: Вы можете воспользоваться моими специальными функциями:" << endl;
    objFuncBot.botfunc(); // Использование специальной функции бота

    string user_message;

    while (true) {
        getline(cin, user_message);
        if (user_message == "выход" || user_message == "exit" || user_message == "пока") {
            cout << "UTMN_Bot: До свидания!" << endl;
            break;
        }
        bot.findSimilarWords(user_message);
        string response_message = generate_response(user_message);
        cout << response_message << endl;
    }

    return 0;
}

/// Пожалуйста, используйте этот счётчик при каждом нахождении бага: 4
/// А это будет счётчик всех неудачных попыток запуска: 23

#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <fstream>
#include <unordered_map>
#include <locale>

using namespace std;

unordered_map<string, string> loadResponses(const string& filename) {
    unordered_map<string, string> responses;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t delimiter_pos = line.find(" - ");
            if (delimiter_pos != string::npos) {
                string keyword = line.substr(0, delimiter_pos);
                string response = line.substr(delimiter_pos + 3);
                transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
                responses[keyword] = response;
            }
        }
        file.close();
    }
    else {
        cerr << "Ошибка открытия файла " << filename << '\n';
    }
    return responses;
}

string generate_response(const string& user_message, const unordered_map<string, string>& responses) {
    string message_lower = user_message;
    transform(message_lower.begin(), message_lower.end(), message_lower.begin(), ::tolower);

    for (const auto& pair : responses) {
        if (message_lower.find(pair.first) != string::npos) {
            return pair.second;
        }
    }

    return "UTMN_Bot: Вы всегда можете воспользоваться моей специальной командой 'help'.";
}
//  ^
//  |
// нужно сделать вывод ответа бота по скрипту из файла
// пока он это делает вручную и криво

class BotT9 {
private:
    unordered_map<string, vector<string>> fileWordMap;

public:
    BotT9() {
        // это файлы-группы для Т9
        
        loadFiles("BotFiles/FilesT9/uni.txt");// - Этот файл ещё не готов
        loadFiles("BotFiles/FilesT9/study.txt");
        loadFiles("BotFiles/FilesT9/stepend.txt");
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

        //if (!found) {
            //cerr << "Извините, я не понимаю ваш запрос. Пожалуйста, задайте вопрос по - другому.\n";  Надо ещё доработать штатные фразы
        //}
    }

    bool isSimilar(string s1, string s2) {
        int n = s1.length();
        int m = s2.length();

        //cout << s1 << endl;// - это проверка, после правки бага с русским языком эти строчки можно убрать
        //cout << s2 << endl;

        if (abs(n - m) > 1)
            return false;

        int count = 0;

        for (int i = 0, j = 0; i < n && j < m;) {
            if (s1[i] != s2[j]) {
                count++;
                if (count > 1) { // Пользователь допустил ошибку в слове (при чём различие может быть лишь в 1 символ)
                    return false;
                }
                if (n == m) {
                    i++;
                    j++;
                }
                else if (n > (m + 1)) { // Учитываем различие в длине строк
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
        ifstream file("BotFiles/FuncNames.txt");
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
    setlocale(LC_CTYPE, "");

    FuncBot objFuncBot;
    BotT9 bot;

    string responses_file = "asw_scripts.txt";
    unordered_map<string, string> responses = loadResponses(responses_file);

    cout << "UTMN_Bot: Добро пожаловать в чат-бот университета. Как я могу вам помочь?" << endl;
    cout << "Вы можете воспользоваться моими специальными функциями:" << endl;
    objFuncBot.botfunc(); // Использование специальной функции бота

    string user_message;

    while (true) {
        getline(cin, user_message);
        if (user_message == "выход" || user_message == "exit" || user_message == "пока") {
            cout << "UTMN_Bot: До свидания!" << endl;
            break;
        }
        bot.findSimilarWords(user_message);
        string response_message = generate_response(user_message, responses);
        cout << response_message << endl;
    }

    return 0;
}


/// Это будет счётчик всех неудачных попыток запуска: 27

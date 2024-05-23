#define CROW_MAIN
#include "crow_all.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <fstream>
#include <unordered_map>
#include <locale>
#include <sstream>
#include <vector>

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

void handleRequest(const crow::request& req, crow::response& res) {
    unordered_map<string, string> responses = loadResponses("asw_scripts.txt");

    if (req.url == "/get_response") {
        auto it = responses.find(req.url_params.get("keyword"));
        if (it != responses.end()) {
            res.write(it->second);
        }
        else {
            res.code = 404;
            res.write("Response not found");
        }
    }
    else {
        res.code = 404;
        res.write("Not Found");
    }
}

string generate_response(const string& user_message, const unordered_map<string, string>& responses) {
    string message_lower = user_message;
    transform(message_lower.begin(), message_lower.end(), message_lower.begin(), ::tolower);

    for (const auto& pair : responses) {
        if (message_lower.find(pair.first) != string::npos) {
            return pair.second;
        }
    }

    return "UTMN_Bot: You always can use my special function 'help'.";
}

class BotT9 {
private:
    unordered_map<string, vector<string>> fileWordMap;

public:
    BotT9() {
        loadFiles("BotFiles/FilesT9/uni.txt");
        loadFiles("BotFiles/FilesT9/study.txt");
        loadFiles("BotFiles/FilesT9/stepend.txt");
    }

    void loadFiles(const string& filename) {
        ifstream file(filename);

        if (file.is_open()) {
            string word;
            while (file >> word) {
                for (auto& c : word) {
                    c = tolower(c);
                }
                fileWordMap[filename].push_back(word);
            }
            file.close();
        }
        else {
            cerr << "Ошибка открытия файла " << filename << '\n';
        }
    }

    string findSimilarWords(const string& user_message) {
        string result = "Sorry, now I can't understand your message.\n";
        bool found = false;

        for (const auto& pair : fileWordMap) {
            const auto& wordsList = pair.second;

            for (const auto& word : wordsList) {
                if (isSimilar(user_message, word)) {
                    found = true;
                    result = "Maybe these themes will be interesting for you:\n";
                    for (const auto& w : wordsList) {
                        result += w + "\n";
                    }
                    break;
                }
            }

            if (found) break;
        }

        return result;
    }

    bool isSimilar(string s1, string s2) {
        int n = s1.length();
        int m = s2.length();

        if (abs(n - m) > 1)
            return false;

        int count = 0;

        for (int i = 0, j = 0; i < n && j < m;) {
            if (s1[i] != s2[j]) {
                count++;
                if (count > 1) {
                    return false;
                }
                if (n == m) {
                    i++;
                    j++;
                }
                else if (n > m) {
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

        count += abs(n - m);

        return count <= 1;
    }
};

class FuncBot {
    stack<string> lfunc;

public:
    FuncBot() {
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

    string botfunc() {
        string result = "You always can use my special functions:\n";
        while (!lfunc.empty()) {
            result += lfunc.top() + "\n";
            lfunc.pop();
        }
        return result;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    setlocale(LC_CTYPE, "");

    FuncBot objFuncBot;
    BotT9 bot;

    string responses_file = "asw_scripts.txt";
    unordered_map<string, string> responses = loadResponses(responses_file);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/get_response").methods("GET"_method)(handleRequest);
    CROW_ROUTE(app, "/")([&responses, &bot, &objFuncBot] {
        ostringstream os;
        os << "<!DOCTYPE html>\n"
            << "<html>\n"
            << "<head>\n"
            << "    <meta charset=\"UTF-8\">\n"
            << "    <title>UTMN_Bot Chat</title>\n"
            << "    <style>\n"
            << "        body { font-family: Arial, sans-serif; margin: 20px; }\n"
            << "        #chat { border: 1px solid #ccc; padding: 10px; width: 300px; height: 400px; overflow-y: scroll; }\n"
            << "        #user_input { width: 240px; }\n"
            << "    </style>\n"
            << "</head>\n"
            << "<body>\n"
            << "    <h1>UTMN_Bot Chat</h1>\n"
            << "    <div id=\"chat\">\n"
            << "        <div>UTMN_Bot: Welcome, I'm UTMN_Bot. How can I help you?</div>\n"
            //<< "        <div>UTMN_Bot: " << objFuncBot.botfunc() << "</div>\n"
            << "        <div>UTMN_Bot: If you need my help, please, text me 'help'.</div>\n"
            << "    </div>\n"
            << "    <input type=\"text\" id=\"user_input\" placeholder=\"Text message here\" />\n"
            << "    <button onclick=\"sendMessage()\">Send message</button>\n"
            << "    <script>\n"
            << "        function sendMessage() {\n"
            << "            var userMessage = document.getElementById('user_input').value;\n"
            << "            var chat = document.getElementById('chat');\n"
            << "            var userMessageDiv = document.createElement('div');\n"
            << "            userMessageDiv.textContent = 'You: ' + userMessage;\n"
            << "            chat.appendChild(userMessageDiv);\n"
            << "            fetch('/chat', {\n"
            << "                method: 'POST',\n"
            << "                headers: {\n"
            << "                    'Content-Type': 'application/json'\n"
            << "                },\n"
            << "                body: JSON.stringify({ message: userMessage })\n"
            << "            }).then(response => response.json())\n"
            << "              .then(data => {\n"
            << "                var botMessageDiv = document.createElement('div');\n"
            << "                botMessageDiv.textContent = 'UTMN_Bot: ' + data.response;\n"
            << "                chat.appendChild(botMessageDiv);\n"
            << "                document.getElementById('user_input').value = '';\n"
            << "            });\n"
            << "        }\n"
            << "    </script>\n"
            << "</body>\n"
            << "</html>";
        return crow::response(os.str());
        });

    CROW_ROUTE(app, "/chat").methods("POST"_method)([&responses, &bot, &objFuncBot](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400);
        string user_message = x["message"].s();

        //objFuncBot.botfunc();

        string response_message = generate_response(user_message, responses);
        string similar_words_message = bot.findSimilarWords(user_message);
        if (response_message == "UTMN_Bot: You always can use my special function 'help'.") {
            response_message += "\n" + similar_words_message;
        }
        crow::json::wvalue response;
        response["response"] = response_message;
        return crow::response(response);
        });

    app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}

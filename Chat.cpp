#include "Chat.h"
#include <iostream>
#include <stdexcept>

void Chat::registerUser(const std::string& login, const std::string& password, const std::string& name) {
    if (user_logins.find(login) != user_logins.end()) {
        throw std::runtime_error("Логин уже существует.");
    }
    users.push_back(User(login, password, name));
    user_logins.insert(login);
}

bool Chat::loginUser(const std::string& login, const std::string& password) {
    for (const auto& user : users) {
        if (user.getLogin() == login && user.getPassword() == password) {
            return true;
        }
    }
    return false;
}

void Chat::sendMessage(const std::string& from, const std::string& to, const std::string& message) {
    if (loginUser(from, "")) {
        throw std::runtime_error("Отправитель не вошел в чат.");
    }
    if (loginUser(to, "")) {
        throw std::runtime_error("Получатель не вошел в чат.");
    }

    messages.insert({ from + " -> " + to, message });
    trie.insert(message); // Добавляем сообщение в Trie
}

std::string Chat::readMessages(const std::string& user) {
    std::string result = "Сообщения для " + user + ":\n";
    for (const auto& message : messages) {
        if (message.first.find(user) != std::string::npos) {
            result += message.first + ": " + message.second + "\n";
        }
    }
    return result;
}

std::vector<std::string> Chat::autocomplete(const std::string& prefix) {
    return trie.autocomplete(prefix);
}

// Оставшаяся часть файла chat.cpp остается без изменений


int main() {
    setlocale(LC_ALL, "RUSSIAN");
    Chat chat;
    std::string current_user;

    while (true) {
        std::cout << "Выберите действие:\n1. Регистрация\n2. Вход\n3. Выход\n4. Отправить сообщение\n5. Прочитать сообщения\n6. Автодополнение\n7. Выход из программы\n";
        int choice;
        std::cin >> choice;

        try {
            if (choice == 1) {
                std::string login, password, name;
                std::cout << "Введите логин: ";
                std::cin >> login;
                std::cout << "Введите пароль: ";
                std::cin >> password;
                std::cout << "Введите имя: ";
                std::cin >> name;
                chat.registerUser(login, password, name);
            }
            else if (choice == 2) {
                if (current_user.empty()) {
                    std::string login, password;
                    std::cout << "Введите логин: ";
                    std::cin >> login;
                    std::cout << "Введите пароль: ";
                    std::cin >> password;
                    if (chat.loginUser(login, password)) {
                        current_user = login;
                        std::cout << "Вы вошли как: " << current_user << "\n";
                    }
                    else {
                        std::cout << "Ошибка входа. Проверьте логин и пароль.\n";
                    }
                }
                else {
                    std::cout << "Вы уже вошли как: " << current_user << "\n";
                }
            }
            else if (choice == 3) {
                current_user = "";
                std::cout << "Выход выполнен.\n";
            }
            else if (choice == 4) {
                if (current_user.empty()) {
                    std::cout << "Вы не вошли в чат. Войдите или зарегистрируйтесь.\n";
                }
                else {
                    std::string to, message;
                    std::cout << "Введите получателя: ";
                    std::cin >> to;
                    std::cin.ignore();
                    std::cout << "Введите сообщение: ";
                    std::getline(std::cin, message);
                    chat.sendMessage(current_user, to, message);
                    std::cout << "Сообщение отправлено.\n";
                }
            }
            else if (choice == 5) {
                if (current_user.empty()) {
                    std::cout << "Вы не вошли в чат. Войдите или зарегистрируйтесь.\n";
                }
                else {
                    std::cout << chat.readMessages(current_user) << std::endl;
                }
            }
            else if (choice == 6) {
                if (current_user.empty()) {
                    std::cout << "Вы не вошли в чат. Войдите или зарегистрируйтесь.\n";
                }
                else {
                    std::string prefix;
                    std::cout << "Введите префикс: ";
                    std::cin >> prefix;
                    std::vector<std::string> words = chat.autocomplete(prefix);
                    for (const auto& word : words) {
                        std::cout << word << std::endl;
                    }
                }
            }
            else if (choice == 7) {
                break;
            }
            else {
                std::cout << "Неверный выбор. Попробуйте еще раз.\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}
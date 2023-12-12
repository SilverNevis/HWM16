#include "Chat.h"
#include <iostream>
#include <stdexcept>

void Chat::registerUser(const std::string& login, const std::string& password, const std::string& name) {
    if (user_logins.find(login) != user_logins.end()) {
        throw std::runtime_error("����� ��� ����������.");
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
        throw std::runtime_error("����������� �� ����� � ���.");
    }
    if (loginUser(to, "")) {
        throw std::runtime_error("���������� �� ����� � ���.");
    }

    messages.insert({ from + " -> " + to, message });
    trie.insert(message); // ��������� ��������� � Trie
}

std::string Chat::readMessages(const std::string& user) {
    std::string result = "��������� ��� " + user + ":\n";
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

// ���������� ����� ����� chat.cpp �������� ��� ���������


int main() {
    setlocale(LC_ALL, "RUSSIAN");
    Chat chat;
    std::string current_user;

    while (true) {
        std::cout << "�������� ��������:\n1. �����������\n2. ����\n3. �����\n4. ��������� ���������\n5. ��������� ���������\n6. ��������������\n7. ����� �� ���������\n";
        int choice;
        std::cin >> choice;

        try {
            if (choice == 1) {
                std::string login, password, name;
                std::cout << "������� �����: ";
                std::cin >> login;
                std::cout << "������� ������: ";
                std::cin >> password;
                std::cout << "������� ���: ";
                std::cin >> name;
                chat.registerUser(login, password, name);
            }
            else if (choice == 2) {
                if (current_user.empty()) {
                    std::string login, password;
                    std::cout << "������� �����: ";
                    std::cin >> login;
                    std::cout << "������� ������: ";
                    std::cin >> password;
                    if (chat.loginUser(login, password)) {
                        current_user = login;
                        std::cout << "�� ����� ���: " << current_user << "\n";
                    }
                    else {
                        std::cout << "������ �����. ��������� ����� � ������.\n";
                    }
                }
                else {
                    std::cout << "�� ��� ����� ���: " << current_user << "\n";
                }
            }
            else if (choice == 3) {
                current_user = "";
                std::cout << "����� ��������.\n";
            }
            else if (choice == 4) {
                if (current_user.empty()) {
                    std::cout << "�� �� ����� � ���. ������� ��� �����������������.\n";
                }
                else {
                    std::string to, message;
                    std::cout << "������� ����������: ";
                    std::cin >> to;
                    std::cin.ignore();
                    std::cout << "������� ���������: ";
                    std::getline(std::cin, message);
                    chat.sendMessage(current_user, to, message);
                    std::cout << "��������� ����������.\n";
                }
            }
            else if (choice == 5) {
                if (current_user.empty()) {
                    std::cout << "�� �� ����� � ���. ������� ��� �����������������.\n";
                }
                else {
                    std::cout << chat.readMessages(current_user) << std::endl;
                }
            }
            else if (choice == 6) {
                if (current_user.empty()) {
                    std::cout << "�� �� ����� � ���. ������� ��� �����������������.\n";
                }
                else {
                    std::string prefix;
                    std::cout << "������� �������: ";
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
                std::cout << "�������� �����. ���������� ��� ���.\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "������: " << e.what() << std::endl;
        }
    }

    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

struct Task {
    char letter;
    string title;
    int difficulty;
    int memory;
    string input_type;
    string output_type;
    bool has_solution;
    long long file_size;
};

// Функция для получения размера файла
long long get_file_size(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    if (!file.is_open()) {
        return -1;
    }
    return file.tellg();
}

// Функция для копирования в буфер обмена
void copy_to_clipboard(const string& str) {
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hglb = GlobalAlloc(GMEM_MOVEABLE, (str.size() + 1) * sizeof(char));
        if (hglb) {
            char* buffer = (char*)GlobalLock(hglb);
            strcpy(buffer, str.c_str());
            GlobalUnlock(hglb);
            SetClipboardData(CF_TEXT, hglb);
        }
        CloseClipboard();
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector <Task> tasks;
    string line;

    cout << "Введите (завершите ввод пустой строкой):\n";

    // Чтение ввода до строки "END"
    while (getline(cin, line)) {
        if (line == "") {
            break;
        }

        // Разбор строки
        stringstream ss(line);
        Task task;

        ss >> task.letter;
        // Чтение названия задачи (может содержать пробелы)
        string title_part;
        getline(ss, title_part, '\t'); // пропускаем первый таб
        getline(ss, task.title, '\t');

        ss >> task.difficulty;
        ss >> task.memory;
        ss >> task.input_type;
        ss >> task.output_type;

        // Проверяем наличие файла с решением
        string filename = "solutions/" + task.title + ".txt";
        task.file_size = get_file_size(filename);
        task.has_solution = (task.file_size != -1);

        tasks.push_back(task);
    }

    // Очищаем экран
    system("cls");

    // Фильтруем задачи с решениями
    vector <Task> tasks_with_solutions;
    for (const auto& task : tasks) {
        if (task.has_solution) {
            tasks_with_solutions.push_back(task);
        }
    }

    // Сортируем задачи с решениями по размеру файла
    sort(tasks_with_solutions.begin(), tasks_with_solutions.end(),
        [](const Task& a, const Task& b) {
            return a.file_size < b.file_size;
        });

    // Выводим доступные задачи в одну строку
    if (tasks_with_solutions.empty()) {
        cout << "Решений не найдено!\n";
        system("PAUSE");
        return 0;
    }

    cout << "Доступно: ";
    for (size_t i = 0; i < tasks_with_solutions.size(); i++) {
        cout << tasks_with_solutions[i].letter;
        if (i < tasks_with_solutions.size() - 1) {
            cout << " ";
        }
    }
    cout << "\n\n";

    // Основной цикл получения решений
    while (true) {
        cout << "Введите букву (Ctrl+C для выхода): ";

        string input;
        if (!getline(cin, input)) {
            // Если произошла ошибка ввода (например, Ctrl+C), выходим
            break;
        }

        if (input.empty())
            continue;

        char choice = toupper(input[0]);

        // Ищем задачу с такой буквой
        Task* selected_task = nullptr;
        for (auto& task : tasks_with_solutions)
            if (task.letter == choice) {
                selected_task = &task;
                break;
            }

        if (selected_task) {
            string filename = "solutions/" + selected_task->title + ".txt";
            ifstream solution_file(filename);

            if (solution_file.is_open()) {
                stringstream solution_content;
                solution_content << solution_file.rdbuf();
                solution_file.close();

                copy_to_clipboard(solution_content.str());
                cout << "Решение задачи " << selected_task->letter << " успешно.\n";
            }
            else {
                cout << "Ошибка при чтении файла решения.\n";
            }
        }
        else {
            cout << "Задача с буквой '" << choice << "' не найдена или нет решения.\n";
            cout << "Доступные задачи: ";
            for (size_t i = 0; i < tasks_with_solutions.size(); i++) {
                cout << tasks_with_solutions[i].letter;
                if (i < tasks_with_solutions.size() - 1) {
                    cout << " ";
                }
            }
            cout << "\n";
        }

        cout << "\n";
    }

    return 0;
}

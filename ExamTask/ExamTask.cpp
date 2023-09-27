#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

class Task {
public:
    Task(const std::string& title, int priority, const std::string& tag, const std::string& dueDate)
        : title(title), priority(priority), tag(tag), dueDate(dueDate) {}

    std::string getTitle() const {
        return title;
    }

    int getPriority() const {
        return priority;
    }

    std::string getTag() const {
        return tag;
    }

    std::string getDueDate() const {
        return dueDate;
    }

private:
    std::string title;
    int priority;
    std::string tag;
    std::string dueDate;
};

class TaskList {
public:
    void addTask(const Task& task) {
        tasks.push_back(task);
    }

    void removeTask(const std::string& title) {
        tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
            [title](const Task& task) { return task.getTitle() == title; }), tasks.end());
    }

    void updateTask(const std::string& title, const Task& newTask) {
        for (auto& task : tasks) {
            if (task.getTitle() == title) {
                task = newTask;
                break;
            }
        }
    }

    std::vector<Task> getTasks() const {
        return tasks;
    }

private:
    std::vector<Task> tasks;
};

class TaskSearch {
public:
    std::vector<Task> searchByDate(const std::string& date, const TaskList& taskList) const {
        std::vector<Task> foundTasks;
        for (const auto& task : taskList.getTasks()) {
            if (task.getDueDate() == date) {
                foundTasks.push_back(task);
            }
        }
        return foundTasks;
    }

    std::vector<Task> searchByTag(const std::string& tag, const TaskList& taskList) const {
        std::vector<Task> foundTasks;
        for (const auto& task : taskList.getTasks()) {
            if (task.getTag() == tag) {
                foundTasks.push_back(task);
            }
        }
        return foundTasks;
    }

    std::vector<Task> searchByPriority(int priority, const TaskList& taskList) const {
        std::vector<Task> foundTasks;
        for (const auto& task : taskList.getTasks()) {
            if (task.getPriority() == priority) {
                foundTasks.push_back(task);
            }
        }
        return foundTasks;
    }
};

class TaskFileHandler {
public:
    void saveToFile(const TaskList& taskList, const std::string& filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& task : taskList.getTasks()) {
                file << task.getTitle() << "," << task.getPriority() << ","
                    << task.getTag() << "," << task.getDueDate() << "\n";
            }
            file.close();
        }
    }

    TaskList loadFromFile(const std::string& filename) const {
        TaskList taskList;
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string title, tag, dueDate;
                int priority;

                if (iss >> title >> priority >> tag >> dueDate) {
                    Task task(title, priority, tag, dueDate);
                    taskList.addTask(task);
                }
            }
            file.close();
        }
        return taskList;
    }
};

void displayTasks(const std::vector<Task>& tasks) {
    for (const auto& task : tasks) {
        std::cout << "Назва: " << task.getTitle() << std::endl;
        std::cout << "Приоритет: " << task.getPriority() << std::endl;
        std::cout << "Тег: " << task.getTag() << std::endl;
        std::cout << "Дата виконання: " << task.getDueDate() << std::endl;
        std::cout << "---------------------------" << std::endl;
    }
}

int main() {
    TaskList taskList;
    TaskSearch taskSearch;
    TaskFileHandler fileHandler;

    while (true) {
        std::cout << "Меню:" << std::endl;
        std::cout << "1. Додати задачу" << std::endl;
        std::cout << "2. Видалити задачу" << std::endl;
        std::cout << "3. Оновити задачу" << std::endl;
        std::cout << "4. Знайти задачу за датою" << std::endl;
        std::cout << "5. Знайти задачу за тегом" << std::endl;
        std::cout << "6. Знайти задачу за приоритетом" << std::endl;
        std::cout << "7. Зберегти список задач у файл" << std::endl;
        std::cout << "8. Завантажити список задач з файлу" << std::endl;
        std::cout << "9. Вийти" << std::endl;

        int choice;
        std::cin >> choice;

        if (choice == 9) {
            break; // Вихід з програми
        }

        switch (choice) {
        case 1: {
            std::string title, tag, dueDate;
            int priority;

            std::cout << "Назва задачі: ";
            std::cin.ignore(); // Для обходу буфера вводу
            std::getline(std::cin, title);

            std::cout << "Тег: ";
            std::getline(std::cin, tag);

            std::cout << "Приоритет (1 - найнижчий, 10 - найвищий): ";
            std::cin >> priority;

            std::cout << "Дата виконання (формат YYYY-MM-DD): ";
            std::cin >> dueDate;

            Task task(title, priority, tag, dueDate);
            taskList.addTask(task);
            break;
        }
        case 2: {
            std::string title;
            std::cout << "Введіть назву задачі для видалення: ";
            std::cin.ignore(); // Для обходу буфера вводу
            std::getline(std::cin, title);
            taskList.removeTask(title);
            break;
        }
        case 3: {
            std::string title;
            std::cout << "Введіть назву задачі для оновлення: ";
            std::cin.ignore(); // Для обходу буфера вводу
            std::getline(std::cin, title);

            Task oldTask;
            for (const auto& task : taskList.getTasks()) {
                if (task.getTitle() == title) {
                    oldTask = task;
                    break;
                }
            }

            if (!oldTask.getTitle().empty()) {
                std::string newTitle, newTag, newDueDate;
                int newPriority;

                std::cout << "Нова назва задачі: ";
                std::getline(std::cin, newTitle);

                std::cout << "Новий тег: ";
                std::getline(std::cin, newTag);

                std::cout << "Новий приоритет (1 - найнижчий, 10 - найвищий): ";
                std::cin >> newPriority;

                std::cout << "Нова дата виконання (формат YYYY-MM-DD): ";
                std::cin >> newDueDate;

                Task newTask(newTitle, newPriority, newTag, newDueDate);
                taskList.updateTask(title, newTask);
            }
            else {
                std::cout << "Задачу не знайдено." << std::endl;
            }
            break;
        }
        case 4: {
            std::string date;
            std::cout << "Введіть дату для пошуку (формат YYYY-MM-DD): ";
            std::cin >> date;
            std::vector<Task> tasks = taskSearch.searchByDate(date, taskList);
            displayTasks(tasks);
            break;
        }
        case 5: {
            std::string tag;
            std::cout << "Введіть тег для пошуку: ";
            std::cin.ignore(); // Для обходу буфера вводу
            std::getline(std::cin, tag);
            std::vector<Task> tasks = taskSearch.searchByTag(tag, taskList);
            displayTasks(tasks);
            break;
        }
        case 6: {
            int priority;
            std::cout << "Введіть приоритет для пошуку: ";
            std::cin >> priority;
            std::vector<Task> tasks = taskSearch.searchByPriority(priority, taskList);
            displayTasks(tasks);
            break;
        }
        case 7: {
            std::string filename;
            std::cout << "Введіть ім'я файлу для збереження: ";
            std::cin.ignore(); // Для обходу буфера вводу
            std::getline(std::cin, filename);
            fileHandler.saveToFile(taskList, filename);
            break;
        }
        case 8: {
            std::string filename;
            std::cout << "Введіть ім'я файлу для завантаження: ";
            std::cin.ignore(); // Для обходу буфера вводу
            std::getline(std::cin, filename);
            taskList = fileHandler.loadFromFile(filename);
            break;
        }
        default:
            std::cout << "Неправильний вибір. Спробуйте ще раз." << std::endl;
        }
    }

    return 0;
}

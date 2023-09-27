#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <Windows.h>

// Клас для представлення задачі
class Task {
public:
    Task() : priority(0) {} // Конструктор за замовчуванням

    Task(const std::string& title, int priority, const std::string& tag, const std::string& dueDate)
        : title(title), priority(priority), tag(tag), dueDate(dueDate) {}

    // Геттери та сеттери для полів
    std::string getTitle() const 
    {
        return title;
    }

    int getPriority() const 
    {
        return priority;
    }

    std::string getTag() const 
    {
        return tag;
    }

    std::string getDueDate() const 
    {
        return dueDate;
    }

private:
    std::string title;
    int priority;
    std::string tag;
    std::string dueDate;
};

// Клас для списку задач
class TaskList {
public:
    void addTask(const Task& task) 
    {
        tasks.push_back(task);
    }

    void removeTask(const std::string& title) 
    {
        tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
            [title](const Task& task) { return task.getTitle() == title; }), tasks.end());
    }

    void updateTask(const std::string& title, const Task& newTask)
    {
        for (auto& task : tasks) {
            if (task.getTitle() == title) {
                // Оновити задачу
                task = newTask;
                break;
            }
        }
    }

    // Інші методи для роботи зі списком задач
    std::vector<Task> getTasks() const {
        return tasks;
    }

private:
    std::vector<Task> tasks;
};

// Клас для пошуку задач
class TaskSearch 
{
public:
    std::vector<Task> searchByDate(const std::string& date) const {
        std::vector<Task> result;
        // Пошук за датою
        for (const auto& task : tasks) {
            if (task.getDueDate() == date) {
                result.push_back(task);
            }
        }
        return result;
    }

    std::vector<Task> searchByTag(const std::string& tag) const {
        std::vector<Task> result;
        // Пошук за тегом
        for (const auto& task : tasks) {
            if (task.getTag() == tag) {
                result.push_back(task);
            }
        }
        return result;
    }

    std::vector<Task> searchByPriority(int priority) const {
        std::vector<Task> result;
        // Пошук за приоритетом
        for (const auto& task : tasks) {
            if (task.getPriority() == priority) {
                result.push_back(task);
            }
        }
        return result;
    }
private:
    std::vector<Task> tasks;
};

// Клас для збереження та завантаження списку задач з файлу
class TaskFileHandler {
public:
    void saveToFile(const TaskList& taskList, const std::string& filename) const {
        // Відкриття файлу для запису
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Помилка відкриття файлу для запису: " << filename << std::endl;
            return;
        }

        // Отримання списку задач
        std::vector<Task> tasks = taskList.getTasks();

        // Запис кожної задачі у файл
        for (const Task& task : tasks) {
            file << task.getTitle() << std::endl;
            file << task.getPriority() << std::endl;
            file << task.getTag() << std::endl;
            file << task.getDueDate() << std::endl;
        }

        // Закриття файлу
        file.close();
    }

    TaskList loadFromFile(const std::string& filename) const {
        TaskList taskList;

        // Відкриття файлу для читання
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Помилка відкриття файлу для читання: " << filename << std::endl;
            return taskList;
        }

        std::string title, tag, dueDate;
        int priority;

        // Читання задач з файлу та додавання їх до списку
        while (getline(file, title)) {
            file >> priority;
            file.ignore(); // Ігноруємо перехід на новий рядок
            getline(file, tag);
            getline(file, dueDate);

            Task task(title, priority, tag, dueDate);
            taskList.addTask(task);
        }

        // Закриття файлу
        file.close();

        return taskList;
    }
};

// Функція для відображення списку задач
void displayTasks(const std::vector<Task>& tasks) {
    for (const auto& task : tasks) {
        std::cout << "Назва: " << task.getTitle() << std::endl;
        // Виведення інших полів задачі
        std::cout << "Приоритет: " << task.getPriority() << std::endl;
        std::cout << "Тег: " << task.getTag() << std::endl;
        std::cout << "Дата виконання: " << task.getDueDate() << std::endl;
        std::cout << "---------------------------" << std::endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    TaskList taskList;
    TaskSearch taskSearch;
    TaskFileHandler fileHandler;

    while (true) {
        std::cout << "Меню:" << std::endl;
        std::cout << "1. Додати нову задачу" << std::endl;
        std::cout << "2. Видалити задачу" << std::endl;
        std::cout << "3. Оновити задачу" << std::endl;
        std::cout << "4. Показати всі задачі" << std::endl;
        std::cout << "5. Пошук задачі за датою" << std::endl;
        std::cout << "6. Пошук задачі за тегом" << std::endl;
        std::cout << "7. Пошук задачі за приоритетом" << std::endl;
        std::cout << "8. Зберегти список задач у файл" << std::endl;
        std::cout << "9. Завантажити список задач з файлу" << std::endl;
        std::cout << "0. Вийти" << std::endl;

        int choice;
        std::cin >> choice;

        if (choice == 0) {
            break; // Вихід з програми
        }

        switch (choice) {
        case 1: {
            // Додавання нової задачі
            std::string title, tag, dueDate;
            int priority;

            std::cout << "Назва задачі: ";
            std::cin.ignore();
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
            // Видалення задачі
            std::string title;
            std::cout << "Введіть назву задачі для видалення: ";
            std::cin.ignore();
            std::getline(std::cin, title);
            taskList.removeTask(title);
            break;
        }
        case 3: {
            // Оновлення задачі
            std::string title;
            std::cout << "Введіть назву задачі для оновлення: ";
            std::cin.ignore();
            std::getline(std::cin, title);

            // Знайдемо задачу за назвою
            Task oldTask;
            for (const auto& task : taskList.getTasks()) {
                if (task.getTitle() == title) {
                    oldTask = task;
                    break;
                }
            }

            if (!oldTask.getTitle().empty()) {
                // Введіть нові дані для задачі
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

                // Створюємо нову задачу та оновлюємо її дані
                Task newTask(newTitle, newPriority, newTag, newDueDate);
                taskList.updateTask(title, newTask);
            }
            else {
                std::cout << "Задачу не знайдено." << std::endl;
            }
            break;
        }
        case 4: {
            // Показати всі задачі
            std::vector<Task> tasks = taskList.getTasks();
            displayTasks(tasks);
            break;
        }
        case 5: {
            // Пошук задачі за датою
            std::string date;
            std::cout << "Введіть дату для пошуку (формат YYYY-MM-DD): ";
            std::cin >> date;
            std::vector<Task> tasks = taskSearch.searchByDate(date);
            displayTasks(tasks);
            break;
        }
        case 6: {
            // Пошук задачі за тегом
            std::string tag;
            std::cout << "Введіть тег для пошуку: ";
            std::cin.ignore();
            std::getline(std::cin, tag);
            std::vector<Task> tasks = taskSearch.searchByTag(tag);
            displayTasks(tasks);
            break;
        }
        case 7: {
            // Пошук задачі за приоритетом
            int priority;
            std::cout << "Введіть приоритет для пошуку: ";
            std::cin >> priority;
            std::vector<Task> tasks = taskSearch.searchByPriority(priority);
            displayTasks(tasks);
            break;
        }
        case 8: {
            // Збереження задач у файл
            std::string filename;
            std::cout << "Введіть ім'я файлу для збереження: ";
            std::cin >> filename;
            fileHandler.saveToFile(taskList, filename);
            break;
        }
        case 9: {
            // Завантаження задач з файлу
            std::string filename;
            std::cout << "Введіть ім'я файлу для завантаження: ";
            std::cin >> filename;
            taskList = fileHandler.loadFromFile(filename);
            break;
        }
        default:
            std::cout << "Неправильний вибір. Спробуйте ще раз." << std::endl;
        }
    }

    return 0;
}

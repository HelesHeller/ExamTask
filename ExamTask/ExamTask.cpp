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
        std::cout << "�����: " << task.getTitle() << std::endl;
        std::cout << "���������: " << task.getPriority() << std::endl;
        std::cout << "���: " << task.getTag() << std::endl;
        std::cout << "���� ���������: " << task.getDueDate() << std::endl;
        std::cout << "---------------------------" << std::endl;
    }
}

int main() {
    TaskList taskList;
    TaskSearch taskSearch;
    TaskFileHandler fileHandler;

    while (true) {
        std::cout << "����:" << std::endl;
        std::cout << "1. ������ ������" << std::endl;
        std::cout << "2. �������� ������" << std::endl;
        std::cout << "3. ������� ������" << std::endl;
        std::cout << "4. ������ ������ �� �����" << std::endl;
        std::cout << "5. ������ ������ �� �����" << std::endl;
        std::cout << "6. ������ ������ �� �����������" << std::endl;
        std::cout << "7. �������� ������ ����� � ����" << std::endl;
        std::cout << "8. ����������� ������ ����� � �����" << std::endl;
        std::cout << "9. �����" << std::endl;

        int choice;
        std::cin >> choice;

        if (choice == 9) {
            break; // ����� � ��������
        }

        switch (choice) {
        case 1: {
            std::string title, tag, dueDate;
            int priority;

            std::cout << "����� ������: ";
            std::cin.ignore(); // ��� ������ ������ �����
            std::getline(std::cin, title);

            std::cout << "���: ";
            std::getline(std::cin, tag);

            std::cout << "��������� (1 - ���������, 10 - ��������): ";
            std::cin >> priority;

            std::cout << "���� ��������� (������ YYYY-MM-DD): ";
            std::cin >> dueDate;

            Task task(title, priority, tag, dueDate);
            taskList.addTask(task);
            break;
        }
        case 2: {
            std::string title;
            std::cout << "������ ����� ������ ��� ���������: ";
            std::cin.ignore(); // ��� ������ ������ �����
            std::getline(std::cin, title);
            taskList.removeTask(title);
            break;
        }
        case 3: {
            std::string title;
            std::cout << "������ ����� ������ ��� ���������: ";
            std::cin.ignore(); // ��� ������ ������ �����
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

                std::cout << "���� ����� ������: ";
                std::getline(std::cin, newTitle);

                std::cout << "����� ���: ";
                std::getline(std::cin, newTag);

                std::cout << "����� ��������� (1 - ���������, 10 - ��������): ";
                std::cin >> newPriority;

                std::cout << "���� ���� ��������� (������ YYYY-MM-DD): ";
                std::cin >> newDueDate;

                Task newTask(newTitle, newPriority, newTag, newDueDate);
                taskList.updateTask(title, newTask);
            }
            else {
                std::cout << "������ �� ��������." << std::endl;
            }
            break;
        }
        case 4: {
            std::string date;
            std::cout << "������ ���� ��� ������ (������ YYYY-MM-DD): ";
            std::cin >> date;
            std::vector<Task> tasks = taskSearch.searchByDate(date, taskList);
            displayTasks(tasks);
            break;
        }
        case 5: {
            std::string tag;
            std::cout << "������ ��� ��� ������: ";
            std::cin.ignore(); // ��� ������ ������ �����
            std::getline(std::cin, tag);
            std::vector<Task> tasks = taskSearch.searchByTag(tag, taskList);
            displayTasks(tasks);
            break;
        }
        case 6: {
            int priority;
            std::cout << "������ ��������� ��� ������: ";
            std::cin >> priority;
            std::vector<Task> tasks = taskSearch.searchByPriority(priority, taskList);
            displayTasks(tasks);
            break;
        }
        case 7: {
            std::string filename;
            std::cout << "������ ��'� ����� ��� ����������: ";
            std::cin.ignore(); // ��� ������ ������ �����
            std::getline(std::cin, filename);
            fileHandler.saveToFile(taskList, filename);
            break;
        }
        case 8: {
            std::string filename;
            std::cout << "������ ��'� ����� ��� ������������: ";
            std::cin.ignore(); // ��� ������ ������ �����
            std::getline(std::cin, filename);
            taskList = fileHandler.loadFromFile(filename);
            break;
        }
        default:
            std::cout << "������������ ����. ��������� �� ���." << std::endl;
        }
    }

    return 0;
}

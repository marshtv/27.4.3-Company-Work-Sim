#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>

enum class Task {
    NONE,
    A,
    B,
    C
};

class Employee {
private:
    std::string name = "unknown";
public:
    Employee(std::string in_name) {
        this->name.resize(in_name.size());
        this->name = in_name;
    }
    void setName(std::string in_name) {
        this->name.resize(in_name.size());
        this->name = in_name;
    }
    std::string getName() {
        return this->name;
    }
};

class Worker : public Employee {
private:
    Task task = Task::NONE;
public:
    Worker(std::string in_name, Task in_task) : Employee(in_name) {
        this->task = in_task;
    }
    void setTask(Task in_task) {
        this->task = in_task;
    }
    Task getTask() {
        return this->task;
    }
};

class Manager : public Employee {
private:
    int managerID = 0;
public:
    Manager(std::string in_name, int in_managerID) : Employee(in_name) {
        assert(in_managerID >= 0);
        this->managerID = in_managerID;
    }
    void setManagerID(int in_managerID) {
        this->managerID = in_managerID;
    }
    int getManagerID() {
        return this->managerID;
    }
};

class Team {
private:
    int ceoID = 0;
    int teamID = 0;
    int tasksNum = 0;
    int workersNum = 0;
    Manager* manager = nullptr;
    Worker** workers = nullptr;
    Task* tasks = nullptr;
public:
    Team(int in_ceoID, int in_teamID, int in_workersNum, std::string in_managerName, int in_managerID) {
        this->ceoID = in_ceoID;
        assert(in_teamID >= 0);
        this->teamID = in_teamID;
        assert(in_workersNum > 0);
        this->workersNum = in_workersNum;
        this->manager = new Manager(in_managerName, in_managerID);
        workers = new Worker*[this->workersNum];
        for (int i = 0; i < this->workersNum; ++i) {
            Worker* worker = new Worker("Worker #" + std::to_string(i), Task::NONE);
            workers[i] = worker;
        }
    }
    int getTeamID() {
        return this->teamID;
    }
    int getTasksNum() {
        return this->tasksNum;
    }
    int getWorkersNum() {
        return this->workersNum;
    }
    Manager* getTeamManager() {
        return this->manager;
    }
    Worker* getWorkerByCount(int in_count) {
        return this->workers[in_count];
    }
    void makeTasks(int in_Ceo_ID) {
        if (this->manager != nullptr) {
            std::srand(in_Ceo_ID + this->manager->getManagerID());
            this->tasksNum = std::rand() % (this->workersNum + 1);
            while (this->tasksNum == 0) this->tasksNum = std::rand() % (this->workersNum + 1);
            for (int i = 0; i < this->tasksNum; ++i) {
                int in_rand = std::rand() % 4;
                while (in_rand == 0)
                    in_rand = std::rand() % 4;
                if (in_rand == 1)
                    this->workers[i]->setTask(Task::A);
                if (in_rand == 2)
                    this->workers[i]->setTask(Task::B);
                if (in_rand == 3)
                    this->workers[i]->setTask(Task::C);
            }
        } else {
            std::cout << "No manager exist!" << std::endl;
            return;
        }
    }
};

class CEO: public Employee {
private:
    int ceoID = 0;
    int teamsNum = 0;
    Team** teams = nullptr;
public:
    CEO(std::string in_name, int in_teamNum, int in_workersNum) : Employee(in_name) {
        this->teamsNum = in_teamNum;
        this->teams = new Team*[this->teamsNum];
        for (int i = 0; i < this->teamsNum; ++i) {
            Team* team = new Team(this->ceoID, i, in_workersNum,
                                  "Manager #" + std::to_string(i), i);
            this->teams[i] = team;
        }
    }
    int getCeoID() {
        return this->ceoID;
    }
    int getTeamsNum() {
        return this->teamsNum;
    }
    Team* getTeamByCount(int in_count) {
        return this->teams[in_count];
    }
    void showTasksByWorker(int in_New_Ceo_ID) {
        this->ceoID = in_New_Ceo_ID;
        for (int i = 0; i < teamsNum; ++i) {
            this->teams[i]->makeTasks(in_New_Ceo_ID);
            std::cout << "--------------------------------------" << std::endl;
            std::cout << "Team ID#" << this->teams[i]->getTeamID() << std::endl;
            std::cout << "--------------------------------------" << std::endl;
            for (int j = 0; j < this->teams[i]->getWorkersNum(); ++j) {
                if (this->teams[i]->getWorkerByCount(j)->getTask() != Task::NONE) {
                    std::cout << this->teams[i]->getWorkerByCount(j)->getName() << " do the task ";
                    switch (this->teams[i]->getWorkerByCount(j)->getTask()) {
                        default:
                        case Task::A:
                            std::cout << "=A=";
                            break;
                        case Task::B:
                            std::cout << "=B=";
                            break;
                        case Task::C:
                            std::cout << "=C=";
                            break;
                    }
                    std::cout << std::endl;
                }
            }
        }
    }
};

int main() {
    std::cout << "Input CEO's name:";
    std::string in_CeoName;
    std::cin >> in_CeoName;
    std::cout << "Input number of teams:";
    int in_teamNum;
    std::cin >> in_teamNum;
    std::cout << "Input number of workers in each team:";
    int in_workersNum;
    std::cin >> in_workersNum;
    auto* company = new CEO(in_CeoName, in_teamNum, in_workersNum);

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "Input an integer number for generate tasks (-1 for exit):";
    int in_CeoID;
    std::cin >> in_CeoID;
    std::cout << "------------------------------------------------------------" << std::endl;
    while (in_CeoID != -1) {
        company->showTasksByWorker(in_CeoID);

        std::cout << "------------------------------------------------------------" << std::endl;
        std::cout << "Input an integer number for generate tasks (-1 for exit):";
        in_CeoID;
        std::cin >> in_CeoID;
        std::cout << "------------------------------------------------------------" << std::endl;
    }
    std::cout << "Exit program. Good by." << std::endl;

    delete company;
    company = nullptr;

    return 0;
}

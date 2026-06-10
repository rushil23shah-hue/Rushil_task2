#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <fstream>
#include <queue>
#include <sstream>
using namespace std;

class Task
{
public:
    string id;

    int burstTime;
    int remainingTime;

    vector<string> memoryBlocks;

    int memoryIndex;

    int arrivalTime;

    Task(
        string id,
        int burstTime,
        vector<string> memoryBlocks,
        int arrivalTime
    )
    {
        this->id = id;
        this->burstTime = burstTime;
        this->remainingTime = burstTime;
        this->memoryBlocks = memoryBlocks;
        this->memoryIndex = 0;
        this->arrivalTime = arrivalTime;
    }
};





class Core
{
public:
    int id;
int busyCycles;
    Task* currentTask;

    int quantumUsed;

    Core(int id)
    {
        this->busyCycles = 0;
        this->id = id;
        this->currentTask = nullptr;
        this->quantumUsed = 0;
    }
};




class CacheLevel
{
public:
    string name;
    int capacity;
    int latency;
    deque<string> blocks;

    CacheLevel(string name, int capacity, int latency)
    {
        this->name = name;
        this->capacity = capacity;
        this->latency = latency;
    }

    bool contains(string block)
    {
        return find(blocks.begin(), blocks.end(), block) != blocks.end();
    }

    void insert(string block)
    {
        if (contains(block))
        {
            return;
        }

        if (blocks.size() >= capacity)
        {
            blocks.pop_front();   // FIFO eviction
        }

        blocks.push_back(block);
    }

    void print()
    {
        cout << name << ": [ ";

        for (string block : blocks)
        {
            cout << block << " ";
        }

        cout << "]" << endl;
    }
    void insertOptimal(string block, vector<string> &future, int currentIndex)
{
    if (contains(block))
    {
        return;
    }

    if (blocks.size() < capacity)
    {
        blocks.push_back(block);
        return;
    }

    int indexToRemove = -1;
    int farthestUse = -1;

    for (int i = 0; i < blocks.size(); i++)
    {
        string currentBlock = blocks[i];
        int nextUse = -1;

        for (int j = currentIndex + 1; j < future.size(); j++)
        {
            if (future[j] == currentBlock)
            {
                nextUse = j;
                break;
            }
        }

        if (nextUse == -1)
        {
            indexToRemove = i;
            break;
        }

        if (nextUse > farthestUse)
        {
            farthestUse = nextUse;
            indexToRemove = i;
        }
    }

    blocks.erase(blocks.begin() + indexToRemove);
    blocks.push_back(block);
}
};




class MemoryHierarchy
{
public:
    CacheLevel L1;
    CacheLevel L2;
    CacheLevel L3;

    int l1Hits;
    int l2Hits;
    int l3Hits;
    int ramAccesses;

    int totalMemoryCycles;
    int promotionCycles;

    MemoryHierarchy()
        : L1("L1", 3, 4),
          L2("L2", 128, 12),
          L3("L3", 512, 40)
    {
        l1Hits = 0;
        l2Hits = 0;
        l3Hits = 0;
        ramAccesses = 0;
        totalMemoryCycles = 0;
        promotionCycles = 0;
    }

    string accessBlock(string block)
    {
        if (L1.contains(block))
        {
            l1Hits++;
            totalMemoryCycles += 4;
            return "L1 HIT (4 cycles)";
        }

        if (L2.contains(block))
        {
            l2Hits++;

            totalMemoryCycles += 12;

            // Copy promotion: L2 -> L1
            L1.insert(block);
            promotionCycles += 4;
            totalMemoryCycles += 4;

            return "L2 HIT + promoted to L1 (12 + 4 cycles)";
        }

        if (L3.contains(block))
        {
            l3Hits++;

            totalMemoryCycles += 40;

            // Copy promotion: L3 -> L2
            L2.insert(block);
            promotionCycles += 12;
            totalMemoryCycles += 12;

            return "L3 HIT + promoted to L2 (40 + 12 cycles)";
        }

        
        ramAccesses++;

        totalMemoryCycles += 200;
        L3.insert(block);

        return "RAM ACCESS -> loaded into L3 (200 cycles)";
    }

    void printCaches()
    {
        L1.print();
        L2.print();
        L3.print();
    }

    void printStats()
    {
        cout << "\nCache Statistics " << endl;
        cout << "L1 Hits: " << l1Hits << endl;
        cout << "L2 Hits: " << l2Hits << endl;
        cout << "L3 Hits: " << l3Hits << endl;
        cout << "RAM Accesses: " << ramAccesses << endl;
        cout << "Promotion Cycles: " << promotionCycles << endl;
        cout << "Total Memory Cycles: " << totalMemoryCycles << endl;
    }
};





class OptimalMemoryHierarchy
{
public:
    CacheLevel L1;
    CacheLevel L2;
    CacheLevel L3;

    int l1Hits;
    int l2Hits;
    int l3Hits;
    int ramAccesses;

    int totalMemoryCycles;
    int promotionCycles;

    OptimalMemoryHierarchy()
        : L1("L1", 3, 4),
          L2("L2", 128, 12),
          L3("L3", 512, 40)
    {
        l1Hits = 0;
        l2Hits = 0;
        l3Hits = 0;
        ramAccesses = 0;
        totalMemoryCycles = 0;
        promotionCycles = 0;
    }

    string accessBlock(
        string block,
        vector<string> &future,
        int currentIndex
    )
    {
        if (L1.contains(block))
        {
            l1Hits++;
            totalMemoryCycles += 4;
            return "L1 HIT";
        }

        if (L2.contains(block))
        {
            l2Hits++;
            totalMemoryCycles += 12;

            L1.insertOptimal(block, future, currentIndex);
            promotionCycles += 4;
            totalMemoryCycles += 4;

            return "L2 HIT";
        }

        if (L3.contains(block))
        {
            l3Hits++;
            totalMemoryCycles += 40;

            L2.insertOptimal(block, future, currentIndex);
            promotionCycles += 12;
            totalMemoryCycles += 12;

            return "L3 HIT";
        }

        ramAccesses++;
        totalMemoryCycles += 200;

        L3.insertOptimal(block, future, currentIndex);

        return "RAM ACCESS";
    }
};




class Scheduler
{
public:
    queue<Task*> readyQueue;

    void addTask(Task* task)
    {
        readyQueue.push(task);
    }

    bool hasTask()
    {
        return !readyQueue.empty();
    }

    Task* getNextTask()
    {
        if (readyQueue.empty())
        {
            return nullptr;
        }

        Task* task = readyQueue.front();
        readyQueue.pop();
        return task;
    }

    void printReadyQueue()
    {
        queue<Task*> temp = readyQueue;

        cout << "Ready Queue: [ ";

        while (!temp.empty())
        {
            cout << temp.front()->id << " ";
            temp.pop();
        }

        cout << "]" << endl;
    }
};




vector<Task> readTasksFromFile(string filename)
{
    vector<Task> tasks;

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Could not open input file!" << endl;
        return tasks;
    }

    string line;
    int arrivalTime = 0;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        stringstream ss(line);

        string word;
        string taskId;
        int burstTime;
        vector<string> memoryBlocks;

        ss >> word;      // TASK
        ss >> taskId;    // T1
        ss >> word;      // BURST
        ss >> burstTime; // 7
        ss >> word;      // MEM

        string block;

        while (ss >> block)
        {
            memoryBlocks.push_back(block);
        }

        Task task(
            taskId,
            burstTime,
            memoryBlocks,
            arrivalTime
        );

        tasks.push_back(task);

        arrivalTime++;
    }

    file.close();

    return tasks;
}
string getNextMemoryBlock(Task* task)
{
    if (task->memoryBlocks.empty())
    {
        return "NO_MEMORY";
    }

    string block = task->memoryBlocks[task->memoryIndex];

    task->memoryIndex++;

    if (task->memoryIndex >= task->memoryBlocks.size())
    {
        task->memoryIndex = 0;
    }

    return block;
}



void executeOneCycle(
    Core &core,
    MemoryHierarchy &memory
)
{
    if (core.currentTask == nullptr)
    {
        return;
    }
core.busyCycles++;
    string block =
        getNextMemoryBlock(
            core.currentTask
        );

    string result =
        memory.accessBlock(block);

    core.currentTask->remainingTime--;

    core.quantumUsed++;

    cout << "Core "
         << core.id
         << " running "
         << core.currentTask->id
         << endl;

    cout << "Memory Request: "
         << block
         << endl;

    cout << "Cache Result: "
         << result
         << endl;

    cout << "Remaining Time: "
         << core.currentTask->remainingTime
         << endl;

    cout << endl;
}



struct SimulationResult
{
    int totalCycles;
    int totalMemoryCycles;
    double averageUtilization;
};



SimulationResult runSimulation(int coreCount, bool verbose)
{
    vector<Task> tasks = readTasksFromFile("input.txt");

    Scheduler scheduler;
    MemoryHierarchy memory;

    vector<Core> cores;

    for (int i = 0; i < coreCount; i++)
    {
        cores.push_back(Core(i));
    }

    int currentCycle = 0;
    int completedTasks = 0;
    int quantum = 3;

    while (completedTasks < tasks.size())
    {
        if (verbose)
        {
            cout << "\n\n";
            cout << "Cycle " << currentCycle << endl;
            cout << "\n";
        }

        for (Task &task : tasks)
        {
            if (task.arrivalTime == currentCycle)
            {
                scheduler.addTask(&task);

                if (verbose)
                {
                    cout << "[ARRIVAL] " << task.id << " arrived\n";
                }
            }
        }

        for (Core &core : cores)
        {
            if (core.currentTask == nullptr && scheduler.hasTask())
            {
                core.currentTask = scheduler.getNextTask();
                core.quantumUsed = 0;

                if (verbose)
                {
                    cout << "[SCHEDULE] Core "
                         << core.id
                         << " got "
                         << core.currentTask->id
                         << endl;
                }
            }
        }

        for (Core &core : cores)
        {
            if (core.currentTask != nullptr)
            {
                if (verbose)
                {
                    executeOneCycle(core, memory);
                }
                else
                {
                    string block = getNextMemoryBlock(core.currentTask);
                    memory.accessBlock(block);
                    core.currentTask->remainingTime--;
                    core.quantumUsed++;
                    core.busyCycles++;
                }
            }
            else
            {
                if (verbose)
                {
                    cout << "Core " << core.id << " is idle\n\n";
                }
            }
        }

        for (Core &core : cores)
        {
            if (core.currentTask == nullptr)
            {
                continue;
            }

            if (core.currentTask->remainingTime == 0)
            {
                if (verbose)
                {
                    cout << "[COMPLETE] "
                         << core.currentTask->id
                         << " finished on Core "
                         << core.id
                         << endl;
                }

                completedTasks++;
                core.currentTask = nullptr;
                core.quantumUsed = 0;
            }
            else if (core.quantumUsed == quantum)
            {
                if (verbose)
                {
                    cout << "[QUANTUM EXPIRED] "
                         << core.currentTask->id
                         << " moved back to ready queue"
                         << endl;
                }

                scheduler.addTask(core.currentTask);
                core.currentTask = nullptr;
                core.quantumUsed = 0;
            }
        }

        if (verbose)
        {
            scheduler.printReadyQueue();

            cout << "\nCache State:\n";
            memory.printCaches();
        }

        currentCycle++;
    }

    double totalUtilization = 0.0;

    for (Core &core : cores)
    {
        double utilization = 0.0;

        if (currentCycle > 0)
        {
            utilization = (core.busyCycles * 100.0) / currentCycle;
        }

        totalUtilization += utilization;
    }

    double averageUtilization = totalUtilization / coreCount;

    if (verbose)
    {
        cout << "\n\n";
        cout << "SIMULATION COMPLETE\n";
        cout << "\n";

        cout << "Total Cycles: " << currentCycle << endl;
        cout << "Tasks Completed: " << completedTasks << endl;

        cout << "\n CPU Utilization " << endl;

        for (Core &core : cores)
        {
            double utilization =
                (core.busyCycles * 100.0) / currentCycle;

            cout << "Core " << core.id
                 << " Busy Cycles: " << core.busyCycles
                 << " / " << currentCycle
                 << " (" << utilization << "%)"
                 << endl;
        }

        memory.printStats();
    }

    SimulationResult result;
    result.totalCycles = currentCycle;
    result.totalMemoryCycles = memory.totalMemoryCycles;
    result.averageUtilization = averageUtilization;

    return result;
}



vector<string> buildMemorySequence(vector<Task> tasks)
{
    vector<string> sequence;

    for (Task task : tasks)
    {
        int index = 0;

        for (int i = 0; i < task.burstTime; i++)
        {
            if (!task.memoryBlocks.empty())
            {
                sequence.push_back(task.memoryBlocks[index]);

                index++;

                if (index >= task.memoryBlocks.size())
                {
                    index = 0;
                }
            }
        }
    }

    return sequence;
}



void compareFIFOAndOptimal()
{
    vector<Task> tasks = readTasksFromFile("input.txt");

    vector<string> sequence = buildMemorySequence(tasks);

    MemoryHierarchy fifoMemory;
    OptimalMemoryHierarchy optimalMemory;

    for (int i = 0; i < sequence.size(); i++)
    {
        fifoMemory.accessBlock(sequence[i]);
        optimalMemory.accessBlock(sequence[i], sequence, i);
    }

    cout << "\nCACHE POLICY COMPARISON " << endl;

    cout << "\nFIFO:" << endl;
    cout << "Total Memory Cycles: "
         << fifoMemory.totalMemoryCycles << endl;

    cout << "\nOptimal:" << endl;
    cout << "Total Memory Cycles: "
         << optimalMemory.totalMemoryCycles << endl;

    double reduction =
        ((fifoMemory.totalMemoryCycles - optimalMemory.totalMemoryCycles)
         * 100.0)
        / fifoMemory.totalMemoryCycles;

    cout << "\nMemory Cycle Reduction using Optimal: "
         << reduction
         << "%"
         << endl;
}



int main()
{
    SimulationResult twoCoreDetailed =
        runSimulation(2, true);

    cout << "\n\n CORE COMPARISON \n";

    SimulationResult oneCore =
        runSimulation(1, false);

    SimulationResult twoCore =
        runSimulation(2, false);

    cout << "\n1 Core:" << endl;
    cout << "Total CPU Cycles: "
         << oneCore.totalCycles << endl;
    cout << "Total Memory Cycles: "
         << oneCore.totalMemoryCycles << endl;
    cout << "Average CPU Utilization: "
         << oneCore.averageUtilization << "%" << endl;

    cout << "\n2 Cores:" << endl;
    cout << "Total CPU Cycles: "
         << twoCore.totalCycles << endl;
    cout << "Total Memory Cycles: "
         << twoCore.totalMemoryCycles << endl;
    cout << "Average CPU Utilization: "
         << twoCore.averageUtilization << "%" << endl;

    double speedup =
        (double)oneCore.totalCycles / twoCore.totalCycles;

    cout << "\nSpeedup from 1 Core to 2 Cores: "
         << speedup
         << "x"
         << endl;
compareFIFOAndOptimal();
    return 0;
}
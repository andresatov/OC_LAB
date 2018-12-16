//
// Created by andrey on 06.11.18.
//

#include <iostream>
#include <iomanip>
#include <omp.h>
#include "Action.h"

__declspec(thread) double pi;
int order = 10000000;
struct infoThread {
    HANDLE hThread;
    int num_block;
    double Pi;
    volatile bool calculating;
};

Action::Action() {
    actionList = {{1, std::bind(&Action::winAPI_calculation_PI, this)},
                  {2, std::bind(&Action::openMP_calculation_PI, this)},
    };
}

std::function<void()> Action::getAction(int num) {
    return actionList[num];

}

void Action::winAPI_calculation_PI() {
    block_size = 0;

    int number_thread;
    long begin;
    long end;
    std::cout << "your student ticket" << std::endl;
    std::cin >> block_size;
    block_size *= 10;
    number_blocks = order / block_size;
    std::cout << "Еnter the number of threads" << std::endl;
    std::cin >> number_thread;

    for (int i = 0; i < number_thread; ++i) {
        piThreads.push_back(CreateThread(NULL, 0, this->calculation(), (LPVOID) i, CREATE_SUSPENDED, NULL), 0, false);
    }
    std::vector<HANDLE> handles;
    for (auto x:piThreads) {
        handles.push_back(x.hThread);
    }
    begin = GetTickCount();
    for (auto x:piThreads) {
        ResumeThread(x.hThread);
    }
    while (first_free_block < number_blocks) {
        //rand() % numOfThreads
        for (int i = 0; first_free_block > number_blocks; i = (++i) % number_thread) {
            SwitchToThread();
            if (!piThreads[i].calculating)
                ResumeThread(piThreads[i].hThread);
            break;
        }

    }
    for (auto x:piThreads) {
        ResumeThread(x.hThread);
    }
    WaitForMultipleObjects(handles.size(), handles.rbegin(), true, INFINITE);
    end = GetTickCount();
    for (auto x:piThreads) {
        pi += x.Pi;
    }
    std::cout << std::setprecision(100) << "pi = " << pi << std::endl;
    for (auto x:piThreads) {
        CloseHandle(x.hThread);
    }
}


int Action::calculation(LPVOID lpParameter) {
    DWORD ind = static_cast<DWORD>(lpParameter);
    piThreads[ind].calculating = true;
    int beginInd;
    int endInd;
    while (first_free_block < number_blocks) {
        pi = 0;
        beginInd = piThreads[ind].num_block * block_size;
        endInd = (piThreads[ind].num_block + 1) * block_size;
        endInd = (endInd > order) ? order : endInd;

        for (int i = beginInd; i < endInd; i++) {
            double xi = (i + 0.5) / order;
            pi += 4 / (1 + xi * xi);
        }

        piThreads[ind].threadPi += pi;

        piThreads[ind].calculating = false;

        SuspendThread(piThreads[ind].hThread);
        piThreads[ind].num_block = InterlockedExchangeAdd(&first_free_block, 1);//nextBlock++;
        //++iterationPerThread[dwTlsThreadIndex];
        piThreads[ind].calculating = true;

    }
    return 0;
}

void Action::openMP_calculation_PI() {
    int number_thread;
    std::cout << "your student ticket" << std::endl;
    std::cin >> block_size;
    block_size *= 10;
    std::cout << "Еnter the number of threads" << std::endl;
    std::cin >> number_thread;
    omp_set_dynamic(0);
    omp_set_num_threads(number_thread);
    long begin = GetTickCount();

    double answer = 0;
#pragma omp parallel for schedule(dynamic, block_size) reduction(+:answer)
    for (int i = 0; i <order; i++) {
        double xi = (i + 0.5) / order;
        answer += 4 / (1 + xi * xi);
    }

    pi /= order;
    std::cout << std::setprecision(100) << "pi = " << answer << std::endl;
    double end = GetTickCount();
    std::cout << "time: " << (end - begin) << " mc" << std::endl;

}
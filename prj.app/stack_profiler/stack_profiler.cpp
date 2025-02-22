#include <chrono>
#include <fstream>
#include <iostream>

#include "stackarr/stackarr.hpp"
#include "stacklst/stacklst.hpp"

#define STACK_ARR_TIME "../prj.app/stack_profiler/data/stack_arr_time.txt"
#define STACK_LST_TIME "../prj.app/stack_profiler/data/stack_lst_time.txt"
#define STACK_ARR_PUSH "../prj.app/stack_profiler/data/stack_arr_push.txt"
#define STACK_LST_PUSH "../prj.app/stack_profiler/data/stack_lst_push.txt"


void CreateN(int N) {
    StackArray<int> s_arr;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        s_arr.Push(i);
    }
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    std::ofstream file_arr(STACK_ARR_TIME, std::ios_base::app);
    file_arr << N << ',' << duration.count() << '\n';

    Stack<int> s_lst;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        s_lst.Push(i);
    }
    stop = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    std::ofstream file_lst(STACK_LST_TIME, std::ios_base::app);
    file_lst << N << ',' << duration.count() << '\n';
}

void PopAndPush(int N) {
    StackArray<int> s_arr;
    Stack<int> s_lst;

    std::ofstream file_arr(STACK_ARR_PUSH, std::ios_base::app);
    std::ofstream file_lst(STACK_LST_PUSH, std::ios_base::app);
    for (int i = 0; i < N; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        s_arr.Push(i);
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        file_arr << i + 1 << ',' << duration.count() << '\n';

        start = std::chrono::high_resolution_clock::now();
        s_lst.Push(i);
        stop = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        file_lst << i + 1 << ',' << duration.count() << '\n';
    }
}

int main() {
    std::ofstream file_arr(STACK_ARR_TIME);
    file_arr << "n,time\n";
    std::ofstream file_lst(STACK_LST_TIME);
    file_lst << "n,time\n";
    for (int i = 0; i < 1e5; ++i) {
        CreateN(i);
        if (i % 1000 == 0) {
            std::cout << "Iteration: " << i << std::endl;
        }
    }
    PopAndPush(1e5);
}
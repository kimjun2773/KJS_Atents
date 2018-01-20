#include "WorkQueue.h"

void test1() {
    WorkQueue work;

    // 소비자 쓰레드 시작
    std::vector<std::thread> ths;
    for (int i = 0; i < 4; i++) {
        ths.push_back(std::thread([&work] { work.run(); }));
    }

    std::vector<int> res;
    res.resize(10000);

    // 작업대기열에 추가
    for (int i = 0; i < static_cast<int>(res.size()); i++) {
        work.push([i, &res] { res[i]++; });
    }

    // 소비자를 멈추고 쓰레드가 끝나기를 기다린다.
    work.stop();
    for (auto&& t : ths) t.join();

    // 모든 작업 항목이 실행되었는지 테스트합니다.
    for (int i = 0; i < static_cast<int>(res.size()); i++) {
        if (res[i] != 1)
            printf("ERROR: Index %d set to %d\n", i, res[i]);
    }
}
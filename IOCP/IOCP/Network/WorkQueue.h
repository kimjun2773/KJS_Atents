
// 멀티 프로세스에서의 다중 producer / consumer 작업 대기열
#pragma once
#include <mutex>
#include <queue>

#include "Callstack.h"

class WorkQueue {
  public:
    template <typename F>
    void push(F w) {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_q.push(std::move(w));
        m_cond.notify_all();
    }

    void run() {
        Callstack<WorkQueue>::Context ctx(this);
        while (true) {
            std::function<void()> w;
            {
                std::unique_lock<std::mutex> lock(m_mtx);
                m_cond.wait(lock, [this] { return !m_q.empty(); });
                w = std::move(m_q.front());
                m_q.pop();
            }

            if (w) {
                w();
            } else {
                push(nullptr);
                return;
            }
        };
    }

    void stop() {
        push(nullptr);
    }

    bool canDispatch() {
        return Callstack<WorkQueue>::contains(this) != nullptr;
    }

  private:
    std::condition_variable m_cond;
    std::mutex m_mtx;
    std::queue<std::function<void()>> m_q;
};

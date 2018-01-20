
//핸들러 실행을 직렬화 합니다.
//핸들러가 동시에 실행되지 않습니다.
//처리기는 지정된 프로세서에서만 실행됩니다.
//핸들러 실행순서가 보장되지 않습니다.

//지정된 프로세서는 다음 인터페이스를 구현해야 합니다.
//template <typename F> void Processor::push(F w);
//프로세서에 새 작업 항목을 추가합니다. F는 호출 할수 있는 컨버터블입니다.

// std :: function <void ()>
// bool Processor :: canDispatch ();
// 현재 스레드의 프로세서의 디스 패칭 함수에 있으면 true를 반환해야합니다.

#pragma once
#include "Callstack.h"
#include "Monitor.h"
#include <assert.h>
#include <queue>
#include <functional>

template <typename Processor>
class Strand {
  public:
    Strand(Processor& proc) : m_proc(proc) {}

    Strand(const Strand&) = delete;
    Strand& operator=(const Strand&) = delete;


    //모든 strand 보장이 충족되면 즉시 처리기를 실행하거나 이호출 내부에서 보장이 충족되지 않으면 나중에 실행하기 위해 처리기를 게시합니다.
    template <typename F>
    void dispatch(F handler) {

        //현재 프로세서 디스패치 함수 (이 스레드에서)에 없다면 여기에서 핸들러를 실행할 수 없으므로 큐에 쌓아둡니다.
        if (!m_proc.canDispatch()) {
            post(std::move(handler));
            return;
        }

        // 참고 : 이 시점에서 우리는 작업자 스레드에 있음을 알 수 있습니다 (위의 검사 때문에)

        //이 thread로 strand 를 실행하고있는 경우,
        //설계 상, 다른 thread는 strand 를 실행할 수 없기 때문에
        //다른 체크를 실시하지 않고 즉시 핸들러를 실행할 수 있습니다.
        if (runningInThisThread()) {
            handler();
            return;
        }

        auto trigger = m_data([&](Data& data) {
            if (data.running) {
                data.q.push(std::move(handler));
                return false;
            } else {
                data.running = true;
                return true;
            }
        });

        if (trigger) {
            Callstack<Strand>::Context ctx(this);
            handler();
            run();
        }
    }

    template <typename F>
    void post(F handler) {
        bool trigger = m_data([&](Data& data) {
            data.q.push(std::move(handler));
            if (data.running) {
                return false;
            } else {
                data.running = true;
                return true;
            }
        });

        if (trigger) {
            m_proc.push([this] { run(); });
        }
    }

    bool runningInThisThread() {
        return Callstack<Strand>::contains(this) != nullptr;
    }

  private:
    void run() {
        Callstack<Strand>::Context ctx(this);
        while (true) {
            std::function<void()> handler;

            m_data([&](Data& data) {
                assert(data.running);
                if (data.q.size()) {
                    handler = std::move(data.q.front());
                    data.q.pop();
                } else {
                    data.running = false;
                }
            });

            if (handler)
                handler();
            else
                return;
        }
    }

    struct Data {
        bool running = false;
        std::queue<std::function<void()>> q;
    };
    Monitor<Data> m_data;
    Processor& m_proc;
};
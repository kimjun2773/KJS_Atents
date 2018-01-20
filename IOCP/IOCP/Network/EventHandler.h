////http://vallista.tistory.com/entry/C-c%EC%97%90-%EC%9E%88%EB%8A%94-eventdelegate-system-%EA%B5%AC%ED%98%84
//
//#pragma once
//
//#include <iostream>
//#include <list>
//#include <memory>
//#include <functional>
//#include <algorithm>
//
//template<class T>
//class EventHandler {
//  public:
//    void addHandler(std::function<void()> &handler) {
//        handlers.push_back(&handler);
//    }
//
//    void removeHandler(std::function<void()>& handler) {
//        for (auto iter = handlers.begin(); iter != handlers.end();) {
//            if ((*iter) == &handler) {
//                handlers.erase(iter);
//                break;
//            }
//            iter++;
//        }
//    }
//
//    void operator()() {
//
//        for (auto iter = handlers.begin(); iter != handlers.end();) {
//            if( (*iter) != nullptr && *(*iter) != nullptr ) {
//                (*(*iter))();
//                iter++;
//            } else {
//                handlers.erase(iter++);
//            }
//        }
//    }
//
//    EventHandler &operator+=(std::function<void()>& handler) {
//        addHandler(handler);
//        return *this;
//    }
//
//    EventHandler &operator-=(std::function<void()>& handler) {
//        removeHandler(handler);
//        return *this;
//    }
//
//  private:
//    std::list<std::function<void()>> handlers;
//};

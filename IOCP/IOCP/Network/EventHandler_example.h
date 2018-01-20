//#include <iostream>
//#include "EventHandler.h"
//
//class MyClass {
//  public:
//
//    std::function<void()> DoSomething3;
//
//    MyClass() {
//        DoSomething3 = std::bind(&MyClass::Test, this);
//    }
//    ~MyClass() {
//    }
//
//    void Test() {
//        std::cout << "test notified!" << std::endl;
//    }
//};
//
//void test(void) {
//    EventHandler<void()> SomeBasicEvent{};
//    SomeBasicEvent();
//
//    std::function<void()> DoSomething1 = [] {
//        std::cout << "DoSomething1 notified!" << std::endl;
//    };
//
//    SomeBasicEvent.addHandler(DoSomething1);
//
//    std::function<void()> DoSomething2 = []() {
//        std::cout << "DoSomething2 notified!" << std::endl;
//    };
//    SomeBasicEvent += DoSomething2;
//
//    SomeBasicEvent();
//    SomeBasicEvent -= DoSomething1;
//    SomeBasicEvent();
//
//    MyClass *test = new MyClass();
//    std::function<void()> DoSomething3 = std::bind(&MyClass::Test, test);
//
//    SomeBasicEvent.addHandler(test->DoSomething3);
//    SomeBasicEvent();
//
//    delete test;
//
//    SomeBasicEvent();
//}
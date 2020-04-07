## 编译：
    g++ main.cpp -std=c++11 -lpthread

## 文件间的依赖关系
Messaging.h <--依赖--sender.h <--依赖-- withdraw.h
TemplateDispatcher.h <--依赖--Dispatcher.h

## 
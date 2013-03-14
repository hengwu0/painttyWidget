#ifndef ROUTER_H
#define ROUTER_H

#include <functional>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHash>

// name space for default handler type
namespace RouterZone{
    typedef std::function<void (const QJsonObject&)> DefaultHandler;
}


template<typename RouterFunc_ = RouterZone::DefaultHandler>
class Router
{
public:
    void addRule(const QString &rule)
    {
        records_.insert(rule, FuncTable());
        qDebug()<<"Rule"<<rule<<"added to Router";
    }

    void removeRule(const QString &rule)
    {
        records_.remove(rule);
        qDebug()<<"Rule"<<rule<<"removed from Router";
    }

    void regHandler(const QString &rule,
                 const QString &request,
                 RouterFunc_ func)
    {
        records_[rule].insert(request, func);
        qDebug()<<"A function handler of"<<request
               <<"registered to rule"<<rule;
    }

    void unregHandler(const QString &rule,
                   const QString &request)
    {
        if(records_.contains(rule)){
            records_[rule].remove(request);
            qDebug()<<"Function handler for"<<request
                   <<"unregistered from rule"<<rule;
        }
    }

    void onData(const QByteArray &bytes)
    {
        auto doc = QJsonDocument::fromJson(bytes);
        auto obj = doc.object();
        auto rules = records_.keys();
        for(auto item: rules){
            if(obj.contains(item)){
                auto request = obj.value(item).toString();
                if(records_[item].contains(request)){
                    auto func = records_[item][request];
                    func(obj);
                }
            }
        }
    }
private:
    typedef QHash<QString, RouterFunc_> FuncTable;
    QHash<QString, FuncTable > records_;
};

#endif // ROUTER_H

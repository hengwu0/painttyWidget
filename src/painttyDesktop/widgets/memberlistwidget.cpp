#include "memberlistwidget.h"
#include <QMenu>
#include <QContextMenuEvent>

MemberListWidget::MemberListWidget(QWidget *parent)
    : QListWidget(parent)
{
}

void MemberListWidget::setMemberList(const MemberList &memberList)
{
    clearMemberList();
    MemberList::const_iterator iterator = memberList.constBegin();
    while (iterator != memberList.constEnd())
    {
        addMember(iterator.key(), iterator.value().at(0).toString());
        iterator++;
    }
}

void MemberListWidget::clearMemberList()
{
    memberItemList.clear();
    clear();
}

void MemberListWidget::addMember(const QString &id, const QString &nickName)
{
    QListWidgetItem *item = new QListWidgetItem(nickName);
    addItem(item);
    memberItemList.insert(id, item);;
}

void MemberListWidget::removeMember(const QString &id)
{
    QListWidgetItem *item = memberItemList.take(id);
    if (item)
        delete item;
}

QString MemberListWidget::nickName(const QString &id) const
{
    QListWidgetItem *item = memberItemList.value(id, 0);
    if (item)
        return item->text();
    return QString();
}

QString MemberListWidget::currentMemberId() const
{
    QListWidgetItem *item = currentItem();
    if (item)
        return memberItemList.key(currentItem(), QString());
    return QString();
}

QString MemberListWidget::currentMemberNickName() const
{
    QListWidgetItem *item = currentItem();
    if (item)
        return currentItem()->text();
    return QString();
}

void MemberListWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    QMenu* popMenu = new QMenu(this);
    auto kick_action = new QAction(tr("Kick"), this);
    itemAt(event->pos())->setSelected(true);
    kick_action->setData(currentMemberId());
    connect(kick_action, &QAction::triggered,
            [this](){
        emit memberGetKicked(currentMemberId());
    });
    popMenu->addAction(kick_action);

    popMenu->exec(event->globalPos());
    event->accept();
}

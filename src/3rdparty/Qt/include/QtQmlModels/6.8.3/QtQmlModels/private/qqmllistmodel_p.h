// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQMLLISTMODEL_H
#define QQMLLISTMODEL_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qtqmlmodelsglobal_p.h>
#include <private/qqmlcustomparser_p.h>

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/qabstractitemmodel.h>

#include <private/qv4engine_p.h>
#include <private/qpodvector_p.h>

QT_REQUIRE_CONFIG(qml_list_model);

QT_BEGIN_NAMESPACE


class QQmlListModelWorkerAgent;
class ListModel;
class ListLayout;

namespace QV4 {
struct ModelObject;
}

class Q_QMLMODELS_EXPORT QQmlListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool dynamicRoles READ dynamicRoles WRITE setDynamicRoles)
    Q_PROPERTY(QObject *agent READ agent CONSTANT REVISION(2, 14))
    QML_NAMED_ELEMENT(ListModel)
    QML_ADDED_IN_VERSION(2, 0)
    QML_CUSTOMPARSER

public:
    QQmlListModel(QObject *parent=nullptr);
    ~QQmlListModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int,QByteArray> roleNames() const override;

    QVariant data(int index, int role) const;
    int count() const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE void remove(QQmlV4FunctionPtr args);
    Q_INVOKABLE void append(QQmlV4FunctionPtr args);
    Q_INVOKABLE void insert(QQmlV4FunctionPtr args);
    Q_INVOKABLE QJSValue get(int index) const;
    Q_INVOKABLE void set(int index, const QJSValue &value);
    Q_INVOKABLE void setProperty(int index, const QString& property, const QVariant& value);
    Q_INVOKABLE void move(int from, int to, int count);
    Q_INVOKABLE void sync();

    QQmlListModelWorkerAgent *agent();

    bool dynamicRoles() const { return m_dynamicRoles; }
    void setDynamicRoles(bool enableDynamicRoles);

    ListModel *listModel() const { return m_listModel; }

Q_SIGNALS:
    void countChanged();

private:
    friend class QQmlListModelParser;
    friend class QQmlListModelWorkerAgent;
    friend class ModelObject;
    friend struct QV4::ModelObject;
    friend class ModelNodeMetaObject;
    friend class ListModel;
    friend class ListElement;
    friend class DynamicRoleModelNode;
    friend class DynamicRoleModelNodeMetaObject;
    friend struct StringOrTranslation;

    // Constructs a flat list model for a worker agent
    QQmlListModel(QQmlListModel *orig, QQmlListModelWorkerAgent *agent);
    QQmlListModel(const QQmlListModel *owner, ListModel *data, QV4::ExecutionEngine *engine, QObject *parent=nullptr);

    QV4::ExecutionEngine *engine() const;

    inline bool canMove(int from, int to, int n) const { return !(from+n > count() || to+n > count() || from < 0 || to < 0 || n < 0); }

    mutable QQmlListModelWorkerAgent *m_agent;
    mutable QV4::ExecutionEngine *m_engine;
    QQmlRefPointer<QV4::ExecutableCompilationUnit> m_compilationUnit;
    bool m_mainThread;
    bool m_primary;

    bool m_dynamicRoles;

    ListLayout *m_layout;
    ListModel *m_listModel;
    std::unique_ptr<QPropertyNotifier> translationChangeHandler;

    QVector<class DynamicRoleModelNode *> m_modelObjects;
    QVector<QString> m_roles;

    struct ElementSync
    {
        DynamicRoleModelNode *src = nullptr;
        DynamicRoleModelNode *target = nullptr;
        int srcIndex = -1;
        int targetIndex = -1;
        QVector<int> changedRoles;
    };

    static bool sync(QQmlListModel *src, QQmlListModel *target);
    static QQmlListModel *createWithOwner(QQmlListModel *newOwner);

    void emitItemsChanged(int index, int count, const QVector<int> &roles);
    void emitItemsAboutToBeInserted(int index, int count);
    void emitItemsInserted();

    void removeElements(int index, int removeCount);

    void updateTranslations();
};

// ### FIXME
class QQmlListElement : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ListElement)
    QML_ADDED_IN_VERSION(2, 0)
};

class QQmlListModelParser : public QQmlCustomParser
{
public:
    enum PropertyType {
        Invalid,
        Boolean,
        Number,
        String,
        Script
    };


    QQmlListModelParser() : QQmlCustomParser(QQmlCustomParser::AcceptsSignalHandlers) {}

    void verifyBindings(
            const QQmlRefPointer<QV4::CompiledData::CompilationUnit> &compilationUnit,
            const QList<const QV4::CompiledData::Binding *> &bindings) override;
    void applyBindings(
            QObject *obj, const QQmlRefPointer<QV4::ExecutableCompilationUnit> &compilationUnit,
            const QList<const QV4::CompiledData::Binding *> &bindings) override;

private:
    bool verifyProperty(
            const QQmlRefPointer<QV4::CompiledData::CompilationUnit> &compilationUnit,
            const QV4::CompiledData::Binding *binding);
    // returns true if a role was set
    bool applyProperty(
            const QQmlRefPointer<QV4::ExecutableCompilationUnit> &compilationUnit,
            const QV4::CompiledData::Binding *binding, ListModel *model, int outterElementIndex);

    static bool definesEmptyList(const QString &);

    QString listElementTypeName;
};

template<>
inline QQmlCustomParser *qmlCreateCustomParser<QQmlListModel>()
{
    return new QQmlListModelParser;
}

QT_END_NAMESPACE

#endif // QQMLLISTMODEL_H

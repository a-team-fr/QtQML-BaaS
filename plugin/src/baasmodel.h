#ifndef BAASMODEL_H
#define BAASMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "baas.h"

class BaaSModelItem
{
public:

    BaaSModelItem() {}

    BaaSModelItem(QVariantMap newItems):items(newItems) {}

    void addValue(QString roleName, QVariant value){
        items[roleName] = value;
    }

    QVariant getRole(QString roleName) const
    {
        return items.value(roleName, QString());
    }


private:
    QMap<QString,QVariant> items;


};

class BaaSModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( BaaS* client READ getBackend  WRITE setBackend NOTIFY backendChanged)
    Q_PROPERTY( QString endPoint MEMBER endPoint WRITE setEndPoint )
    Q_PROPERTY( QString where MEMBER where WRITE setWhere )
    Q_PROPERTY( QString order MEMBER order WRITE setOrder )
    Q_PROPERTY( uint limit MEMBER limit WRITE setLimit )
    Q_PROPERTY( uint skip MEMBER skip WRITE setSkip )
    Q_PROPERTY( QString keys MEMBER keys WRITE setKeys )
    Q_PROPERTY( QString include MEMBER include WRITE setInclude )
    Q_PROPERTY( QStringList rolesList READ lstRoles NOTIFY queryDone)




public:
    BaaSModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const Q_DECL_OVERRIDE;
    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE ;
    Q_INVOKABLE void resetModel();

    QStringList lstRoles(){
        QStringList ret;
        for (QString role : roleNames())
            ret.append(role);
        return ret;
    }

    Q_INVOKABLE QString get(int index, QByteArray roleName){
        if  ( (index>=0) && (index < rows.size() )){
            BaaSModelItem item = rows[index];
            return item.getRole( roleName).toString();
        }
        return QString();
    }

    BaaS* getBackend() Q_REQUIRED_RESULT{
        return backend;
    }
    void setBackend(BaaS* be);

    void setEndPoint(QString _endPoint){
        endPoint = _endPoint;
        query();
    }
    void setWhere(QString _where){
        where = _where;
        query();
    }
    void setOrder(QString _order){
        order = _order;
        query();
    }
    void setLimit(uint _limit){
        limit = _limit;
        query();
    }
    void setSkip(uint _skip){
        skip = _skip;
        query();
    }
    void setKeys(QString _keys){
        keys = _keys;
        query();
    }
    void setInclude(QString _include){
        include = _include;
        query();
    }

private slots:
    void onQueryFailed(QJsonDocument json);
    void onQuerySucceeded(QHash<int, QByteArray> roles, QVector<QVariantMap> data);

signals:
    void backendChanged();
    void queryDone();

protected:
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:
    void query();
    BaaS* backend = nullptr;


    QVector<BaaSModelItem> rows;
    QHash<int, QByteArray> roles;

    QString endPoint = "";
    QString where = "";
    QString order = "";
    uint limit = 0;
    uint skip = 0;
    QString keys = "";
    QString include = "";


};

#endif // BAASMODEL_H

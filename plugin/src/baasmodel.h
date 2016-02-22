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



public:
    BaaSModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const Q_DECL_OVERRIDE;
    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE ;
    Q_INVOKABLE void resetModel();

    BaaS* getBackend() Q_REQUIRED_RESULT{
        return backend;
    }
    void setBackend(BaaS* be){
        if (be){
            //Disconnect
            if (backend)
            {
                disconnect(backend, &BaaS::queryFailed, this, &BaaSModel::onQueryFailed);
                disconnect(backend, &BaaS::querySucceeded, this, &BaaSModel::onQuerySucceeded);
            }
            //Update backend
            backend = be;
            //updace connections
            connect(backend, &BaaS::queryFailed, this, &BaaSModel::onQueryFailed);
            connect(backend, &BaaS::querySucceeded, this, &BaaSModel::onQuerySucceeded);
            //and finally query for refreshing model data
            query();
        }
    }

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

protected:
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:
    void query(){

        if ( !backend || !backend->isReady() || endPoint.isEmpty() )
            return;
        QUrlQuery extraParams;
        if ( !where.isEmpty())
            extraParams.addQueryItem("where", where);
        if ( !order.isEmpty())
            extraParams.addQueryItem("order", order);
        if ( limit!=0)
            extraParams.addQueryItem("limit", QString::number(limit));
        if ( skip!=0)
            extraParams.addQueryItem("skip", QString::number(skip));
        if ( !keys.isEmpty())
            extraParams.addQueryItem("keys", keys);
        if ( !include.isEmpty())
            extraParams.addQueryItem("include", include);


        backend->query( endPoint, extraParams);
    }

    BaaS* backend = nullptr;


    QVector<BaaSModelItem> rows = QVector<BaaSModelItem>();
    QHash<int, QByteArray> roles = QHash<int, QByteArray>();

    QString endPoint = "";
    QString where = "";
    QString order = "";
    uint limit = 0;
    uint skip = 0;
    QString keys = "";
    QString include = "";


};

#endif // BAASMODEL_H

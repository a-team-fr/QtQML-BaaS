#include "baasmodel.h"

BaaSModel::BaaSModel()
{

}

QHash<int, QByteArray> BaaSModel::roleNames() const
{
    return roles;
}

Qt::ItemFlags BaaSModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::NoItemFlags;//Qt::ItemIsEditable;

}

int BaaSModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return rows.size();
}

QVariant BaaSModel::data( const QModelIndex & index, int role ) const
{
    int row = index.row();
    //role -= (Qt::UserRole + 1);

    if ( (role < 0) || (role >= roles.size()) )
        return QVariant();

    if ( (row < 0) || (row >= rowCount()) )
        return QVariant();

    //debug
    /*QString roleNames = roles[role];
    BaaSModelItem item = rows[row];
    QVariant ret = item.getRole(roleNames);
    return ret;*/
    return rows[ row].getRole( roles[role] );

}

void BaaSModel::setBackend(BaaS* be){
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

void BaaSModel::onQueryFailed(QJsonDocument json)
{
    beginResetModel();
    rows.clear();
    roles.clear();
    endResetModel();
    emit queryDone();
    qDebug() << json.toJson();
}

void BaaSModel::onQuerySucceeded(QHash<int, QByteArray> _roles, QVector<QVariantMap> _data)
{
    beginResetModel();
    rows.clear();
    roles.clear();

    roles= _roles;
    for (auto elem : _data)
        rows.push_back( BaaSModelItem( elem) );

    endResetModel();
    emit queryDone();
}


void BaaSModel::resetModel()
{
    query();
}

void BaaSModel::query()
{

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

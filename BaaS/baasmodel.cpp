#include "baasmodel.h"

BaaSModel::BaaSModel()
{

}

QHash<int, QByteArray> BaaSModel::roleNames() const
{
    return m_roles;
}

Qt::ItemFlags BaaSModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::NoItemFlags;//Qt::ItemIsEditable;

}

int BaaSModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_rows.size();
}

QVariant BaaSModel::data( const QModelIndex & index, int role ) const
{
    int row = index.row();
    //role -= (Qt::UserRole + 1);

    if ( (role < 0) || (role >= m_roles.size()) )
        return QVariant();

    if ( (row < 0) || (row >= rowCount()) )
        return QVariant();

    //debug
    /*QString roleNames = roles[role];
    BaaSModelItem item = rows[row];
    QVariant ret = item.getRole(roleNames);
    return ret;*/
    return m_rows[ row].getRole( m_roles[role] );

}

void BaaSModel::setBackend(BaasBase* be){
    if (be){
        //Disconnect
        if (m_backend)
        {
            //disconnect(backend, &RestManager::finished, this, &BaaSModel::onFinished);
            disconnect(m_backend, &BaasBase::querySucceeded, this, &BaaSModel::onQuerySucceeded);
        }
        //Update backend
        m_backend = be;
        //updace connections
        //connect(backend, &RestManager::finished, this, &BaaSModel::onFinished);
        connect(m_backend, &BaasBase::querySucceeded, this, &BaaSModel::onQuerySucceeded);
        //and finally query for refreshing model data
        reload();
    }
}

//void BaaSModel::onQueryFailed(QString msg)
//{
//    beginResetModel();
//    rows.clear();
//    roles.clear();
//    endResetModel();
//    emit queryDone();
//    qDebug() << msg;
//}

void BaaSModel::onQuerySucceeded(const QHash<int, QByteArray>& _roles, const QVector<QVariantMap>& _data)
{
    beginResetModel();
    m_rows.clear();
    m_roles.clear();

    m_roles= _roles;
    for (auto elem : _data)
        m_rows.push_back( BaaSModelItem( elem) );

    endResetModel();
    emit queryDone();
}


void BaaSModel::resetModel()
{
    reload();
}

void BaaSModel::reload()
{

    if ( !m_backend || !m_backend->isReady() || m_endPoint.isEmpty() )
        return;
    QUrlQuery extraParams;
    if ( !m_where.isEmpty())
        extraParams.addQueryItem("where", m_where);
    if ( !m_order.isEmpty())
        extraParams.addQueryItem("order", m_order);
    if ( m_limit!=0)
        extraParams.addQueryItem("limit", QString::number(m_limit));
    if ( m_skip!=0)
        extraParams.addQueryItem("skip", QString::number(m_skip));
    if ( !m_keys.isEmpty())
        extraParams.addQueryItem("keys", m_keys);
    if ( !m_include.isEmpty())
        extraParams.addQueryItem("include", m_include);

    if (m_prefix.isEmpty())
        m_backend->query( m_endPoint, extraParams);
    else m_backend->query( m_endPoint, extraParams, m_prefix);
}


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

void BaaSModel::onQueryFailed(QJsonDocument json)
{
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
}


void BaaSModel::resetModel()
{
    query();
}

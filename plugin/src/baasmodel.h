#ifndef BAASMODEL_H
#define BAASMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "baasbase.h"

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

    Q_PROPERTY( BaasBase* source READ backend  WRITE setBackend NOTIFY backendChanged)
    Q_PROPERTY( QString endPoint MEMBER m_endPoint WRITE setEndPoint )
    Q_PROPERTY( QString prefix MEMBER m_prefix WRITE setPrefix )
    Q_PROPERTY( QString where MEMBER m_where WRITE setWhere )
    Q_PROPERTY( QString order MEMBER m_order WRITE setOrder )
    Q_PROPERTY( uint limit MEMBER m_limit WRITE setLimit )
    Q_PROPERTY( uint skip MEMBER m_skip WRITE setSkip )
    Q_PROPERTY( QString keys MEMBER m_keys WRITE setKeys )
    Q_PROPERTY( QString include MEMBER m_include WRITE setInclude )
    Q_PROPERTY( QStringList rolesList READ lstRoles NOTIFY queryDone)




public:
    BaaSModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const override;
    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &index) const override ;
    Q_INVOKABLE void resetModel();

    QStringList lstRoles(){
        QStringList ret;
        for (QString role : roleNames())
            ret.append(role);
        return ret;
    }

    Q_INVOKABLE QString get(int index, QByteArray roleName){
        if  ( (index>=0) && (index < m_rows.size() )){
            BaaSModelItem item = m_rows[index];
            return item.getRole( roleName).toString();
        }
        return QString();
    }

    Q_INVOKABLE void reload();

    BaasBase* backend() { return m_backend;}
    void setBackend(BaasBase*);

    void setEndPoint(QString _endPoint){
        m_endPoint = _endPoint;
        reload();
    }
    void setPrefix(QString _prefix){
        m_prefix = _prefix;
        reload();
    }
    void setWhere(QString _where){
        m_where = _where;
        reload();
    }
    void setOrder(QString _order){
        m_order = _order;
        reload();
    }
    void setLimit(uint _limit){
        m_limit = _limit;
        reload();
    }
    void setSkip(uint _skip){
        m_skip = _skip;
        reload();
    }
    void setKeys(QString _keys){
        m_keys = _keys;
        reload();
    }
    void setInclude(QString _include){
        m_include = _include;
        reload();
    }

private slots:
    void onQuerySucceeded(const QHash<int, QByteArray>&, const QVector<QVariantMap>&);

signals:
    void backendChanged();
    void queryDone();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:

    BaasBase* m_backend = nullptr;


    QVector<BaaSModelItem> m_rows;
    QHash<int, QByteArray> m_roles;

    QString m_endPoint = "";
    QString m_prefix = "";
    QString m_where = "";
    QString m_order = "";
    uint m_limit = 0;
    uint m_skip = 0;
    QString m_keys = "";
    QString m_include = "";


};

#endif // BAASMODEL_H

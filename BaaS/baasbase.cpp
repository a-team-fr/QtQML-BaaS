#include "baasbase.h"

BaasBase::BaasBase(): RestManager ()
{
    connect(this, &RestManager::beforeFinished, this, &BaasBase::resetStateProperties);   //why this is not working?
    connect(this, &BaasBase::operationSuccess, this, &BaasBase::processSuccess);          //why this is not working?
    //connect(this, SIGNAL( beforeFinished()), this, SLOT(resetStateProperties()) );
    //connect(this, SIGNAL( operationSuccess( BaasOperation, QVariant)), this, SLOT(processSuccess(BaasOperation, QVariant)) );

}

/// propertie setters
void BaasBase::SetMasterMode(bool _useMaster)
{
    if (m_masterKey.isEmpty()) return;
    m_useMaster = _useMaster;
    emit masterModeChanged();
}


QString BaasBase::deviceToken()
{

#ifdef ANDROID
    //FCM call :FirebaseInstanceId.getInstance().getInstanceId()
    return QString ();
#endif
    return QString();
}


void BaasBase::setApplicationId(const QString& res)
{
    m_applicationId = res;
    emit applicationIdChanged();
    emit readyChanged();
}
void BaasBase::setMasterKey(const QString& res)
{
    m_masterKey = res;
    emit masterKeyChanged();
}
void BaasBase::setRestKey(const QString& res)
{
    m_restKey = res;
    emit restKeyChanged();
}

void BaasBase::query(QString endPoint, QUrlQuery extraParams, QString prefix)
{
    if (!isReady()) return;

    if (extraParams.isEmpty())
        setEndPoint(endPoint);
    else setEndPoint( endPoint + "?" + extraParams.toString() );

    ensureEndPointHasPrefix(prefix);

    QMetaObject::Connection  conn ;
    conn = connect(this, &RestManager::finished, [&,conn](){

        disconnect(conn);
        if (!namError()){
                //structure to return from parse
                QHash<int, QByteArray> roles = QHash<int, QByteArray>();
                QVector<QVariantMap> data = QVector<QVariantMap>();

                Q_ASSERT(httpResponse().isObject());
                QJsonObject obj = httpResponse().object();
                QJsonValue tmp = obj.value("results");

                if (tmp.isArray()){
                    QJsonArray res = tmp.toArray();
                    for ( QJsonValue elem: res){
                        if (elem.isObject()){
                            obj = elem.toObject();

                            //Update roles
                            QStringList keys = obj.keys();
                            for (QString roleName : keys )
                            {
                                if ( roles.key( roleName.toUtf8(), -1) == -1)
                                    roles[roles.size()] = roleName.toUtf8();
                            }
                            //Add values
                            data.push_back( obj.toVariantMap());


                        }
                    }

                    emit querySucceeded(roles, data);
                }
                else {
                    QStringList keys = obj.keys();
                    for (QString roleName : keys )
                    {
                        if ( roles.key( roleName.toUtf8(), -1) == -1)
                            roles[roles.size()] = roleName.toUtf8();
                    }
                    //Add values
                    data.push_back( obj.toVariantMap());
                    emit querySucceeded(roles, data);
                }
        }
    } );

    initHeaders();
    request( QNetworkAccessManager::GetOperation);

}



void BaasBase::processError()
{
    //get baas specific error code and message by calling the specialized readErrorFromResponse function
    if ( readErrorFromResponse( m_errorCode, m_errorMessage) )
        emit error();
    else{
        qDebug("failed retrieving error info");
        //Q_ASSERT(false);
    }
}

void BaasBase::processSuccess(BaasOperation operation, QVariant extraContext)
{
    Q_UNUSED(extraContext);
    static QMap<BaasOperation,QString> msgMap;
    msgMap[ BaasOperation::Unknown ] = tr("unknown operation");
    msgMap[ BaasOperation::userSignIn ] = tr("welcome %1 !").arg( extraContext.toString() );
    msgMap[ BaasOperation::userSignOut ] = tr("goodbye %1..").arg( extraContext.toString() );
    msgMap[ BaasOperation::userDelete ] = tr("user %1 has been deleted").arg( extraContext.toString() );
    msgMap[ BaasOperation::userRegister ] = tr("user (%1) has been created").arg( extraContext.toString() );
    msgMap[ BaasOperation::userUpdate ] = tr("user (%1) has been modified");
    msgMap[ BaasOperation::userMe ] = tr("accessing my profile");
    msgMap[ BaasOperation::verifyEmail ] = tr("request for verifying %1 has been sent").arg( extraContext.toString() );;
    msgMap[ BaasOperation::passwordReset ] = tr("request for reseting password associated with %1 has been sent").arg( extraContext.toString() );;
    msgMap[ BaasOperation::objectCreate ] = tr("object created");
    msgMap[ BaasOperation::objectGet ] = tr("object read");
    msgMap[ BaasOperation::objectUpdate ] = tr("object modified");
    msgMap[ BaasOperation::objectDelete ] = tr("object deleted");
    msgMap[ BaasOperation::fileUpload ] = tr("file uploaded");
    msgMap[ BaasOperation::fileDelete ] = tr("file deleted");
    m_successCode = operation;
    m_successMessage = msgMap[ operation ];
    qDebug()<<"Operation success:"<<m_successMessage;
    emit success();

}


void BaasBase::resetStateProperties()
{
    m_errorCode=-1;
    m_errorMessage="";
    emit error();
    m_successCode = Unknown;
    m_successMessage ="";
    emit success();
}

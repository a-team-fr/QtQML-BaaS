#include "parse.h"

Parse::Parse(): BaaS()
{

    //Defines default headers
    setExtraHostURI("parse");
    setRawHeader("Accept","application/json");

}

QString Parse::getApplicationId() const
{
    return applicationId;
}

void Parse::setApplicationId(const QString& res)
{
    applicationId = res;
    emit applicationIdChanged();
    emit readyChanged();
    setRawHeader("X-Parse-Application-Id",applicationId.toUtf8());
}

QString Parse::getMasterKey() const
{
    return masterKey;
}

bool Parse::isReady() const
{
    return ( (!applicationId.isEmpty()) && (!getHostURI().isEmpty()) && (getPercComplete()==100) );
}


void Parse::setMasterKey(const QString& res)
{
    masterKey = res;
    emit masterKeyChanged();
}


void Parse::signup( QString username, QString password )
{
    if (!isReady()) return;

    setEndPoint("users");

    QJsonObject obj{
      {"username", username},
      {"password", password}
    };

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( getHttpCode() == 201 ){
            QJsonObject obj = json.object();
            sessionId = obj.value("sessionToken").toString();
            userId = obj.value("objectId").toString();
            qDebug() << "objectId" << obj.value("objectId").toString();
            qDebug() << "sessionToken" << sessionId;
        }

    } );

    request( BaaS::POST, QJsonDocument(obj));
}

void Parse::deleteUser( QString objectId)
{
    if (!isReady()) return;

    setEndPoint("users/" + objectId );

    setRawHeader("X-Parse-Session-Token", sessionId.toUtf8());

    request( BaaS::DELETE);
}

void Parse::login( QString username, QString password )
{
    if (!isReady()) return;

    QUrlQuery postData;
    postData.addQueryItem("username", username);
    postData.addQueryItem("password", password);

    setEndPoint( "login?" + postData.toString() ); //TODO improve me : not use endpoint to give url encoded params

    if (registerInstallationOnLogin)
        setRawHeader("X-Parse-Installation-Id", "1");

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( isLastRequestSuccessful() ) { //getHttpCode() == 201 ){
            QJsonObject obj = json.object();
            sessionId = obj.value("sessionToken").toString();
            userId = obj.value("objectId").toString();
            userName = obj.value("username").toString();
            qDebug() << "objectId" << obj.value("objectId").toString();
            qDebug() << "sessionToken" << sessionId;
            emit loginChanged();
        }

    } );

    request( BaaS::GET);

    removeRawHeader("X-Parse-Installation-Id");
}

void Parse::logout( )
{
    if (!isReady()) return;

    setEndPoint("logout");

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        Q_UNUSED(json);
        if ( isLastRequestSuccessful() ) {
            sessionId = "";
            userId = "";
            userName = "";
            removeRawHeader("X-Parse-Session-Token");
            emit loginChanged();
        }

    } );

    request( BaaS::POST);
}

void Parse::passwordReset( QString email)
{
    if (!isReady()) return;

    setEndPoint("requestPasswordReset");

    QJsonObject obj{
      {"email", email}
    };

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        Q_UNUSED(json);
        if ( isLastRequestSuccessful() ) {

        }

    } );

    request( BaaS::POST, QJsonDocument(obj));
}

void Parse::query(QString endPoint, QUrlQuery extraParams)
{
    if (!isReady()) return;

    if (extraParams.isEmpty())
        setEndPoint( endPoint);
    else setEndPoint( endPoint + "?" + extraParams.toString() ); //TODO improve me : not use endpoint to give url encoded params


    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if (isLastRequestSuccessful()){
                //structure to return from parse
                QHash<int, QByteArray> roles = QHash<int, QByteArray>();
                QVector<QVariantMap> data = QVector<QVariantMap>();

                Q_ASSERT(json.isObject());
                QJsonObject obj = json.object();
                QJsonValue tmp = obj.value("results");
                Q_ASSERT(tmp.isArray());
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
            else emit queryFailed(json);

    } );

    request( BaaS::GET);



}

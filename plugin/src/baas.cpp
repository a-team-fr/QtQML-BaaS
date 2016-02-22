#include "baas.h"


/*!
    \class BaaS


*/

/* -------------------- Constructor and destructor ----------------------------------*/
BaaS::BaaS(QObject *parent) :
    QObject(parent)
{
    _NAM = new QNetworkAccessManager(this);

}
    
BaaS::~BaaS()
{

}


/* -------------------- Property accessors ----------------------------------*/
void BaaS::setHostURI(const QString& res){
    hostURI = res;
    emit hostChanged();
    emit readyChanged();

}
QString BaaS::getHostURI() const{
    return hostURI;
}
void BaaS::setError(const QString& res){
    error = res;
    emit errorChanged();
}
QString BaaS::getError() const{
    return error;
}
void BaaS::setEndPoint(const QString& res){
    endPoint = res;
    emit endPointChanged();

}
QString BaaS::getEndPoint() const{
    return endPoint;
}
void BaaS::setPercComplete(qreal res){
    percComplete = res;
    emit percCompleteChanged();
}
qreal BaaS::getPercComplete() const{
    return percComplete;
}
void BaaS::setHttpResponse(const QString& res){
    httpResponse = res;
    emit httpResponseChanged();
}
QString BaaS::getHttpResponse() const{
    return httpResponse;
}
void BaaS::setHttpCode(uint res){
    httpCode = res;
    emit httpCodeChanged();
}
uint BaaS::getHttpCode() const{
    return httpCode;
}


/* -------------------- Slots ----------------------------------*/



void BaaS::readReply( QNetworkReply *reply )
{
    setPercComplete(100);
    setHttpCode( reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() );
    //QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    Q_ASSERT( reply);
    //httpResponse = QString(reply->readAll());
    QJsonDocument json;
    lastRequestSuccessful = false;
    if (reply)
    {
        QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if (!redirectUrl.isEmpty())
        {
            reply->deleteLater();
            //open(redirectUrl);
            return;
        }
        else if (reply->error() == QNetworkReply::NoError)
        {
            lastRequestSuccessful = true;
            json = QJsonDocument::fromJson( reply->readAll() );
        }
        reply->deleteLater();
    }
    qDebug() << "REPLYFINISHED" << json;
    emit replyFinished(json);
}


void BaaS::replyError(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    Q_ASSERT( reply);
    setHttpCode( reply->error() );
    setError(reply->errorString());
    qDebug() << "REPLY ERROR" << reply->errorString();

}

void BaaS::replyProgress(qint64 bytesSent, qint64 bytesTotal)
{
    setPercComplete( (bytesTotal!=0) ? bytesSent/bytesTotal*100 : 100);
}


QNetworkReply* BaaS::request( BaaS::Operation operation, QUrl url , QJsonDocument data)
{
    QNetworkRequest request;
    request.setUrl( url);

    //Define headers
    for (auto k : rawHeader.keys()){
        //qDebug() << k << ":" << rawHeader.value( k );
        request.setRawHeader(k, rawHeader.value( k ));
    }
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //Proceed with request
    QBuffer *buffer;
    QNetworkReply *reply;
    switch (operation)
    {
    case GET:
        reply = _NAM->get(request);
        break;
    case POST:
        reply = _NAM->post(request, data.toJson());
        break;
    case PUT:
        reply = _NAM->put(request, data.toJson());
        break;
    case DELETE:
        reply = _NAM->deleteResource(request);
        break;
    case PATCH:
        buffer=new QBuffer();
        buffer->open((QBuffer::ReadWrite));
        buffer->write(data.toJson());
        buffer->seek(0);
        reply = _NAM->sendCustomRequest(request,"PATCH", buffer);
        buffer->close();
        break;
    }
    return reply;

}

QNetworkReply* BaaS::request( BaaS::Operation operation, QJsonDocument data )
{
    setPercComplete(0);

    QString qsUrl = hostURI;
    qsUrl += extraHostURI.isEmpty() ? "" : "/" + extraHostURI;
    qsUrl += endPoint.isEmpty() ? "" : "/" + endPoint;

    qDebug() << "Start request - " << operation << "- url:" << qsUrl;
    QNetworkReply *reply = request(operation, QUrl( qsUrl), data );


    connect(reply, &QNetworkReply::finished, [=](){ this->readReply(reply);} );
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));
    //connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(replyProgress(qint64, qint64)));
    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(replyProgress(qint64, qint64)));

    return reply;
}

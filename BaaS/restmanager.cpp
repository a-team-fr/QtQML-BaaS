#include "restmanager.h"


/*!
    \class RestManager


*/


void RestManager::setPercComplete(qreal res){
    m_percComplete = res;
    emit percCompleteChanged();

}
void RestManager::setHostURI(const QString& res){
    if (m_hostURI!= res){
        m_hostURI = res;
        emit hostChanged();
        emit readyChanged();
    }
}


void RestManager::setExtraHostURI(const QString& res){
    if (m_extraHostURI!= res){
        m_extraHostURI = res;
        emit hostChanged();
        emit readyChanged();
    }
}

void RestManager::setEndPoint(const QString& res){
    if (m_endPoint!= res){
        m_endPoint = res;
        emit endPointChanged();
    }

}


bool RestManager::isReady() const
{
    return ( (!hostURI().isEmpty()) && (percComplete()==100.) );
}


/* -------------------- Slots ----------------------------------*/



void RestManager::replyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    setPercComplete(100.);
    Q_ASSERT( reply);


    if (reply)
    {
        //QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        emit beforeFinished();

        m_httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        m_NAMerrorText = reply->errorString();
        m_NAMerror = reply->error();
        m_httpResponse = QJsonDocument::fromJson( reply->readAll() );

        reply->deleteLater();
        emit finished();
    }

}



void RestManager::replyProgress(qint64 bytesSent, qint64 bytesTotal)
{
    setPercComplete( (bytesTotal!=0) ? bytesSent/bytesTotal*100 : 100);
}


void RestManager::applyAllHeaders( QNetworkRequest& _request) const
{
    //Define headers
    for (auto k : m_rawHeaders.keys()){
        //qDebug() << k << ":" << rawHeader.value( k );
        _request.setRawHeader(k, m_rawHeaders.value( k ));
    }
    for (auto k : m_headers.keys()){
        //qDebug() << k << ":" << header.value( k );
        _request.setHeader(k, m_headers.value( k ));
    }

}

uint RestManager::request( QNetworkAccessManager::Operation operation, const QByteArray& data)
{

    setPercComplete(0);

    QString qsUrl = m_hostURI;
    qsUrl += m_extraHostURI.isEmpty() ? "" : "/" + m_extraHostURI;
    qsUrl += m_endPoint.isEmpty() ? "" : "/" + m_endPoint;
    qDebug() << "Start request - " << operation << "- url:" << qsUrl;

    QNetworkRequest request;
    request.setUrl( qsUrl);

    applyAllHeaders(request);

    //Proceed with request
    QBuffer* buffer = nullptr;
    QNetworkReply* reply = nullptr;
    switch (operation)
    {
    case QNetworkAccessManager::GetOperation:
        reply = m_NAM.get(request);
        break;
    case QNetworkAccessManager::PostOperation:
        reply = m_NAM.post(request, data);
        break;
    case QNetworkAccessManager::PutOperation:
        reply = m_NAM.put(request, data);
        break;
    case QNetworkAccessManager::DeleteOperation:
        reply = m_NAM.deleteResource(request);
        break;
    case QNetworkAccessManager::CustomOperation: //Patch
        buffer=new QBuffer();
        buffer->open((QBuffer::ReadWrite));
        buffer->write(data);
        buffer->seek(0);
        reply = m_NAM.sendCustomRequest(request,"PATCH", buffer);
        buffer->setParent(reply);
        buffer->close();
        break;
    case QNetworkAccessManager::HeadOperation:
    case QNetworkAccessManager::UnknownOperation:
        reply = nullptr;
        break;
    }

    //Empty the header
    resetHeaders();
    resetRawHeaders();

    //Connect the created request
    connect(reply, &QNetworkReply::finished, this, &RestManager::replyFinished );
    //connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(reply, &QNetworkReply::downloadProgress, this, &RestManager::replyProgress);
    connect(reply, &QNetworkReply::uploadProgress, this, &RestManager::replyProgress);

    return ++m_requestNumber;
}

bool RestManager::ensureEndPointHasPrefix(QString prefix)
{
    QString endpt = endPoint();

    if (endpt.startsWith( prefix ))
        return true;
    else{
        setEndPoint( prefix + "/" + endpt);
    }
    return false;
}




#include "FileDownloader.h"

// https://wiki.qt.io/Download_Data_from_URL

FileDownloader::FileDownloader(QUrl url, QObject* parent, RequestHeaders* headers) : QObject(parent) {

  connect(&m_WebCtrl,
          SIGNAL (finished(QNetworkReply*)),
          this,
          SLOT (fileDownloaded(QNetworkReply*))
  );

  QNetworkRequest request(url);

  if (headers) {
    for (auto h = headers->begin(); h != headers->end(); h++) {
      //qDebug() << (*h).second;
      request.setRawHeader((*h).first, (*h).second);
    }
  }

  m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
 m_DownloadedData = pReply->readAll();
 pReply->deleteLater();
 emit downloaded();
}

QByteArray FileDownloader::downloadedData() const {
 return m_DownloadedData;
}

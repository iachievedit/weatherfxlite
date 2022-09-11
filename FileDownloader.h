#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

// https://wiki.qt.io/Download_Data_from_URL

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QPair>

typedef QList<QPair<QByteArray, QByteArray> > RequestHeaders;

class FileDownloader : public QObject {
 Q_OBJECT
 public:
  explicit FileDownloader(QUrl url, QObject* parent = nullptr, RequestHeaders* headers = nullptr);
  virtual ~FileDownloader();
  QByteArray downloadedData() const;

 signals:
  void downloaded();

 private slots:
  void fileDownloaded(QNetworkReply* pReply);
  private:
  QNetworkAccessManager m_WebCtrl;
  QByteArray m_DownloadedData;
};

#endif // FILEDOWNLOADER_H

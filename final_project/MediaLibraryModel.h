#pragma once

#include <QAbstractListModel>
#include <QUrl>
#include <QVector>
#include <qqmlintegration.h>

#include "MediaPlayerBackend.h"

struct MediaEntry {
    QUrl url;
    QString title;
    MediaPlayerBackend::SourceType sourceType = MediaPlayerBackend::SourceType::Local;
};

class MediaLibraryModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        TitleRole = Qt::UserRole + 1,
        UrlRole,
        SourceTypeRole,
        IsCurrentRole,
    };
    Q_ENUM(Roles)

    explicit MediaLibraryModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int currentIndex() const { return m_currentIndex; }
    int count() const { return m_entries.size(); }

public slots:
    void addLocalFiles(const QList<QUrl> &fileUrls);
    void addUsbFiles(const QList<QUrl> &fileUrls);
    void addRadioStation(const QUrl &streamUrl, const QString &name);
    void removeAt(int row);
    void clear();
    void clearBySourceType(MediaPlayerBackend::SourceType type);

    void playAt(int row);
    void playNext();
    void playPrevious();

    QUrl urlAt(int row) const;
    QString titleAt(int row) const;
    int sourceTypeAt(int row) const;

signals:
    void currentIndexChanged();
    void countChanged();

    void requestPlay(const QUrl &url, const QString &title, MediaPlayerBackend::SourceType type);

private:
    void setCurrentIndex(int idx);

    QVector<MediaEntry> m_entries;
    int m_currentIndex = -1;
};

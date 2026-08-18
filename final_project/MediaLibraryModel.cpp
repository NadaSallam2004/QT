#include "MediaLibraryModel.h"

#include <QFileInfo>

MediaLibraryModel::MediaLibraryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int MediaLibraryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_entries.size();
}

QVariant MediaLibraryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size())
        return {};

    const MediaEntry &e = m_entries.at(index.row());
    switch (role) {
    case TitleRole:
        return e.title;
    case UrlRole:
        return e.url;
    case SourceTypeRole:
        return static_cast<int>(e.sourceType);
    case IsCurrentRole:
        return index.row() == m_currentIndex;
    default:
        return {};
    }
}

QHash<int, QByteArray> MediaLibraryModel::roleNames() const
{
    return {
        { TitleRole, "title" },
        { UrlRole, "url" },
        { SourceTypeRole, "sourceType" },
        { IsCurrentRole, "isCurrent" },
    };
}

void MediaLibraryModel::addLocalFiles(const QList<QUrl> &fileUrls)
{
    if (fileUrls.isEmpty())
        return;
    const int first = m_entries.size();
    beginInsertRows(QModelIndex(), first, first + fileUrls.size() - 1);
    for (const QUrl &url : fileUrls) {
        const QString title = QFileInfo(url.toLocalFile()).completeBaseName();
        m_entries.append({ url, title, MediaPlayerBackend::SourceType::Local });
    }
    endInsertRows();
    emit countChanged();
}

void MediaLibraryModel::addUsbFiles(const QList<QUrl> &fileUrls)
{
    if (fileUrls.isEmpty())
        return;
    const int first = m_entries.size();
    beginInsertRows(QModelIndex(), first, first + fileUrls.size() - 1);
    for (const QUrl &url : fileUrls) {
        const QString title = QFileInfo(url.toLocalFile()).completeBaseName();
        m_entries.append({ url, title, MediaPlayerBackend::SourceType::Usb });
    }
    endInsertRows();
    emit countChanged();
}

void MediaLibraryModel::addRadioStation(const QUrl &streamUrl, const QString &name)
{
    const int row = m_entries.size();
    beginInsertRows(QModelIndex(), row, row);
    m_entries.append({ streamUrl, name, MediaPlayerBackend::SourceType::Radio });
    endInsertRows();
    emit countChanged();
}

void MediaLibraryModel::removeAt(int row)
{
    if (row < 0 || row >= m_entries.size())
        return;
    beginRemoveRows(QModelIndex(), row, row);
    m_entries.removeAt(row);
    endRemoveRows();
    emit countChanged();

    if (row == m_currentIndex)
        setCurrentIndex(-1);
    else if (row < m_currentIndex)
        setCurrentIndex(m_currentIndex - 1);
}

void MediaLibraryModel::clear()
{
    if (m_entries.isEmpty())
        return;
    beginResetModel();
    m_entries.clear();
    endResetModel();
    setCurrentIndex(-1);
    emit countChanged();
}

void MediaLibraryModel::clearBySourceType(MediaPlayerBackend::SourceType type)
{
    beginResetModel();
    const QUrl currentUrl = (m_currentIndex >= 0 && m_currentIndex < m_entries.size())
                                 ? m_entries.at(m_currentIndex).url : QUrl();
    QVector<MediaEntry> kept;
    kept.reserve(m_entries.size());
    for (const auto &e : std::as_const(m_entries)) {
        if (e.sourceType != type)
            kept.append(e);
    }
    m_entries = kept;
    endResetModel();
    emit countChanged();

    int newIndex = -1;
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries.at(i).url == currentUrl) {
            newIndex = i;
            break;
        }
    }
    setCurrentIndex(newIndex);
}

void MediaLibraryModel::playAt(int row)
{
    if (row < 0 || row >= m_entries.size())
        return;
    setCurrentIndex(row);
    const MediaEntry &e = m_entries.at(row);
    emit requestPlay(e.url, e.title, e.sourceType);
}

void MediaLibraryModel::playNext()
{
    if (m_entries.isEmpty())
        return;
    const int next = (m_currentIndex + 1) % m_entries.size();
    playAt(next);
}

void MediaLibraryModel::playPrevious()
{
    if (m_entries.isEmpty())
        return;
    const int prev = (m_currentIndex - 1 + m_entries.size()) % m_entries.size();
    playAt(prev);
}

QUrl MediaLibraryModel::urlAt(int row) const
{
    if (row < 0 || row >= m_entries.size())
        return {};
    return m_entries.at(row).url;
}

QString MediaLibraryModel::titleAt(int row) const
{
    if (row < 0 || row >= m_entries.size())
        return {};
    return m_entries.at(row).title;
}

int MediaLibraryModel::sourceTypeAt(int row) const
{
    if (row < 0 || row >= m_entries.size())
        return -1;
    return static_cast<int>(m_entries.at(row).sourceType);
}

void MediaLibraryModel::setCurrentIndex(int idx)
{
    if (idx == m_currentIndex)
        return;
    const int old = m_currentIndex;
    m_currentIndex = idx;
    if (old >= 0 && old < m_entries.size())
        emit dataChanged(index(old), index(old), { IsCurrentRole });
    if (idx >= 0 && idx < m_entries.size())
        emit dataChanged(index(idx), index(idx), { IsCurrentRole });
    emit currentIndexChanged();
}

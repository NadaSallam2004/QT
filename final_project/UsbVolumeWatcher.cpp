#include "UsbVolumeWatcher.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QStorageInfo>

UsbVolumeWatcher::UsbVolumeWatcher(QObject *parent)
    : QObject(parent)
{
    m_pollTimer.setInterval(1500);
    connect(&m_pollTimer, &QTimer::timeout, this, &UsbVolumeWatcher::pollNow);
    m_pollTimer.start();
    pollNow();
}

QStringList UsbVolumeWatcher::mountedVolumeNames() const
{
    return m_mountNames;
}

void UsbVolumeWatcher::pollNow()
{
    QStringList currentPaths;
    QStringList currentNames;

    for (const QStorageInfo &vol : QStorageInfo::mountedVolumes()) {

        if (!vol.isValid() || vol.isRoot())
            continue;
        if (vol.bytesTotal() <= 0)
            continue;

        currentPaths << vol.rootPath();
        QString name = vol.displayName();
        if (name.trimmed().isEmpty())
            name = vol.rootPath();
        currentNames << name;
    }

    bool changed = false;

    for (int i = 0; i < currentPaths.size(); ++i) {
        if (!m_mountPaths.contains(currentPaths.at(i))) {
            changed = true;
            emit volumeAttached(currentNames.at(i));
        }
    }
    for (int i = 0; i < m_mountPaths.size(); ++i) {
        if (!currentPaths.contains(m_mountPaths.at(i))) {
            changed = true;
            emit volumeDetached(m_mountNames.at(i));
        }
    }

    if (changed || currentPaths != m_mountPaths) {
        m_mountPaths = currentPaths;
        m_mountNames = currentNames;
        emit volumesChanged();
    }
}

QList<QUrl> UsbVolumeWatcher::scanForMediaFiles() const
{
    QList<QUrl> results;
    const QStringList &exts = mediaExtensions();

    for (const QString &root : m_mountPaths) {
        QDirIterator it(root, QDir::Files | QDir::Readable | QDir::NoSymLinks,
                         QDirIterator::Subdirectories);
        while (it.hasNext()) {
            const QString path = it.next();
            const QString suffix = QFileInfo(path).suffix().toLower();
            if (exts.contains(suffix))
                results << QUrl::fromLocalFile(path);
        }
    }
    return results;
}

const QStringList &UsbVolumeWatcher::mediaExtensions()
{
    static const QStringList exts = {
        QStringLiteral("mp3"), QStringLiteral("wav"), QStringLiteral("flac"),
        QStringLiteral("ogg"), QStringLiteral("m4a"), QStringLiteral("aac"),
        QStringLiteral("wma"), QStringLiteral("opus"),
        QStringLiteral("mp4"), QStringLiteral("mkv"), QStringLiteral("mov"),
        QStringLiteral("avi"),
    };
    return exts;
}

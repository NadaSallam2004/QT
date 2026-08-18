#pragma once

#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <qqmlintegration.h>

class UsbVolumeWatcher : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QStringList mountedVolumeNames READ mountedVolumeNames NOTIFY volumesChanged)
    Q_PROPERTY(bool hasVolumes READ hasVolumes NOTIFY volumesChanged)

public:
    explicit UsbVolumeWatcher(QObject *parent = nullptr);

    QStringList mountedVolumeNames() const;
    bool hasVolumes() const { return !m_mountPaths.isEmpty(); }

public slots:

    Q_INVOKABLE QList<QUrl> scanForMediaFiles() const;
    void pollNow();

signals:
    void volumesChanged();
    void volumeAttached(const QString &name);
    void volumeDetached(const QString &name);

private:
    QTimer m_pollTimer;
    QStringList m_mountPaths;
    QStringList m_mountNames;

    static const QStringList &mediaExtensions();
};

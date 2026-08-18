#pragma once

#include <QAbstractListModel>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QList>
#include <qqmlintegration.h>

class AudioOutputModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString currentDeviceName READ currentDeviceName NOTIFY currentIndexChanged)

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        IsDefaultRole,
        IsLikelyBluetoothRole,
    };
    Q_ENUM(Roles)

    explicit AudioOutputModel(QObject *parent = nullptr);

    Q_INVOKABLE void bind(QAudioOutput *output);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int currentIndex() const { return m_currentIndex; }
    void setCurrentIndex(int idx);
    QString currentDeviceName() const;

public slots:
    void refresh();

signals:
    void currentIndexChanged();

private:
    static bool isLikelyBluetooth(const QAudioDevice &device);

    QMediaDevices m_deviceMonitor;
    QList<QAudioDevice> m_devices;
    int m_currentIndex = 0;
    QAudioOutput *m_boundOutput = nullptr;
};

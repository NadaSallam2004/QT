#pragma once

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QStringList>
#include <qqmlintegration.h>

class BluetoothManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool scanning READ scanning NOTIFY scanningChanged)
    Q_PROPERTY(bool adapterAvailable READ adapterAvailable CONSTANT)
    Q_PROPERTY(QStringList discoveredDeviceNames READ discoveredDeviceNames NOTIFY devicesChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY errorOccurred)

public:
    explicit BluetoothManager(QObject *parent = nullptr);

    bool scanning() const { return m_scanning; }
    bool adapterAvailable() const;
    QStringList discoveredDeviceNames() const;
    QString lastError() const { return m_lastError; }

public slots:
    void startScan();
    void stopScan();

    Q_INVOKABLE void pairWithDeviceName(const QString &name);

signals:
    void scanningChanged();
    void devicesChanged();
    void errorOccurred(const QString &message);
    void pairingSucceeded(const QString &name);
    void pairingFailed(const QString &name, const QString &reason);

private:
    QBluetoothDeviceDiscoveryAgent *m_agent = nullptr;
    QBluetoothLocalDevice *m_localDevice = nullptr;
    QList<QBluetoothDeviceInfo> m_discovered;
    bool m_scanning = false;
    QString m_lastError;
};

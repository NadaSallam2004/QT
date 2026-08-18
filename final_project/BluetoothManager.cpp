#include "BluetoothManager.h"

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject(parent)
    , m_agent(new QBluetoothDeviceDiscoveryAgent(this))
    , m_localDevice(new QBluetoothLocalDevice(this))
{

    m_agent->setLowEnergyDiscoveryTimeout(15000);

    connect(m_agent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, [this](const QBluetoothDeviceInfo &info) {
                for (const auto &existing : std::as_const(m_discovered)) {
                    if (existing.address() == info.address())
                        return;
                }
                m_discovered.append(info);
                emit devicesChanged();
            });

    connect(m_agent, &QBluetoothDeviceDiscoveryAgent::finished, this, [this]() {
        m_scanning = false;
        emit scanningChanged();
    });

    connect(m_agent, &QBluetoothDeviceDiscoveryAgent::errorOccurred, this,
            [this](QBluetoothDeviceDiscoveryAgent::Error) {
                m_scanning = false;
                m_lastError = m_agent->errorString();
                emit scanningChanged();
                emit errorOccurred(m_lastError);
            });

    connect(m_localDevice, &QBluetoothLocalDevice::pairingFinished, this,
            [this](const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing) {
                QString name = address.toString();
                for (const auto &d : std::as_const(m_discovered)) {
                    if (d.address() == address) {
                        name = d.name();
                        break;
                    }
                }
                if (pairing == QBluetoothLocalDevice::Unpaired) {
                    emit pairingFailed(name, tr("Pairing was rejected or timed out"));
                } else {
                    emit pairingSucceeded(name);
                }
            });
}

bool BluetoothManager::adapterAvailable() const
{
    return m_localDevice->isValid();
}

QStringList BluetoothManager::discoveredDeviceNames() const
{
    QStringList names;
    names.reserve(m_discovered.size());
    for (const auto &d : m_discovered)
        names << d.name();
    return names;
}

void BluetoothManager::startScan()
{
    if (!adapterAvailable()) {
        m_lastError = tr("No usable Bluetooth adapter found on this system.");
        emit errorOccurred(m_lastError);
        return;
    }
    m_discovered.clear();
    emit devicesChanged();
    m_scanning = true;
    emit scanningChanged();
    m_agent->start();
}

void BluetoothManager::stopScan()
{
    m_agent->stop();
    m_scanning = false;
    emit scanningChanged();
}

void BluetoothManager::pairWithDeviceName(const QString &name)
{
    for (const auto &d : std::as_const(m_discovered)) {
        if (d.name() == name) {

            m_localDevice->requestPairing(d.address(), QBluetoothLocalDevice::Paired);
            return;
        }
    }
    emit pairingFailed(name, tr("Device not found in the last scan results"));
}

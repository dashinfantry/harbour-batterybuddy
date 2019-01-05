#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QString>
#include <QFile>

class Battery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int  charge    READ getCharge    NOTIFY chargeChanged  )
    Q_PROPERTY(bool charging  READ getCharging  NOTIFY chargingChanged)

public:
    Battery(QObject* parent = nullptr);
    ~Battery();

    int getCharge();
    bool getCharging();

private:
    void updateData();

    QFile* chargeFile;
    QFile* chargingFile;

    int currentCharge;
    bool isCharging;

    int nextCharge;
    bool nextCharging;

signals:
    int chargeChanged();
    bool chargingChanged();
};

#endif // BATTERY_H
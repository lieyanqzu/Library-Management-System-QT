#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

#include "setting.h"

int Setting::getBorrowNumber() {
    return borrow_number;
}

int Setting::getRenewTimes() {
    return renew_times;
}

double Setting::getFinePerDay() {
    return fine_per_day;
}

void Setting::setBorrowNumber(int borrowNumber) {
    borrow_number = borrowNumber;
}

void Setting::setRenewTimes(int renewTimes) {
    renew_times = renewTimes;
}

void Setting::setFinePerDay(double finePerDay) {
    fine_per_day = finePerDay;
}

void Setting::initSetting() {
    QFile data("setting.ini");
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        QString json;
        json = in.readAll();

        if (json.isEmpty()) {
            saveSetting();
        } else {
            QJsonParseError error;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8(), &error);
            if (error.error == QJsonParseError::NoError) {
                if (jsonDocument.isObject()) {
                    QVariantMap result = jsonDocument.toVariant().toMap();
                    borrow_number = result["borrow_number"].toInt();
                    renew_times = result["renew_times"].toInt();
                    fine_per_day = result["fine_per_day"].toDouble();
                }
            } else {
                QMessageBox::critical(0, QObject::tr("JsonParse Error"),
                                      error.errorString());
                saveSetting();
                exit(1);
            }
        }
    } else {
        saveSetting();
    }
}

void Setting::saveSetting() {
    QFile data("setting.ini");
    if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&data);

        QJsonObject setting;
        setting.insert("borrow_number", borrow_number);
        setting.insert("renew_times", renew_times);
        setting.insert("fine_per_day", fine_per_day);

        QJsonDocument jsonDocument;
        jsonDocument.setObject(setting);
        if (!jsonDocument.isNull()) {
            out << jsonDocument.toJson();
        }
    }
}

int Setting::borrow_number = 10;
int Setting::renew_times = 1;
double Setting::fine_per_day = 0.1;

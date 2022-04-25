#pragma once

#include <QObject>

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString introductionText READ introductionText WRITE setIntroductionText NOTIFY introductionTextChanged)
private:
    QString m_introductionText = "Kirigami64-TestText";
public:
    explicit Backend(QObject *parent = nullptr);
    QString introductionText() const;
    void setIntroductionText(const QString &introductionText);
    Q_SIGNAL void introductionTextChanged();

};

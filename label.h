#ifndef LABEL_H
#define LABEL_H

#include <QtWidgets/QLabel>

class Label : public QLabel
{
    Q_OBJECT
public:
    Label(QWidget *parent = nullptr);
    ~Label() override;

protected:
    void showEvent(QShowEvent *event) override;

private:
    class Private;
    QScopedPointer<Private> d;
};
#endif // LABEL_H

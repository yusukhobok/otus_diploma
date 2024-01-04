#ifndef GPR_READER_MODAL_PROGRESS_DIALOG_H
#define GPR_READER_MODAL_PROGRESS_DIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

class ModalProgressDialog : public QDialog {
    Q_OBJECT

public:
    ModalProgressDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

        QVBoxLayout *layout = new QVBoxLayout(this);
        label = new QLabel("Calculation...", this);
        layout->addWidget(label);
    }

private:
    QLabel *label;
};

#endif //GPR_READER_MODAL_PROGRESS_DIALOG_H

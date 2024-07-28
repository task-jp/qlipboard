#include "label.h"

#include <QtCore/QMimeData>
#include <QtCore/QRegularExpression>
#include <QtCore/QTimer>

#include <QtGui/QClipboard>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>

class Label::Private
{
public:
    Private() : clipboard(QGuiApplication::clipboard()) {}
    QClipboard *clipboard = nullptr;
    QTimer timer;
};

Label::Label(QWidget *parent)
    : QLabel(parent)
    , d(new Private)
{
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setWindowFlag(Qt::BypassWindowManagerHint);

    setFrameStyle(QFrame::Panel | QFrame::Raised);

    auto font = this->font();
    font.setPointSize(20);
    setFont(font);

    connect(&d->timer, &QTimer::timeout, this, &QWidget::hide);
    connect(d->clipboard, &QClipboard::dataChanged, this, [this](){
        const QMimeData *mimeData = d->clipboard->mimeData();

        if (mimeData->hasImage()) {
            setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
        } else if (mimeData->hasHtml()) {
            setText(mimeData->html());
            setTextFormat(Qt::RichText);
        } else if (mimeData->hasText()) {
            auto text = mimeData->text();
            // Remove leading and trailing newlines
            static QRegularExpression leadingCRLF("^[\r\n]+");
            static QRegularExpression trailingCRLF("[\r\n]+$");
            text.remove(leadingCRLF);
            text.remove(trailingCRLF);
            setText(text);
            setTextFormat(Qt::PlainText);
        } else {
            setText(tr("Cannot display data"));
        }
        show();
    });
}

Label::~Label() = default;

void Label::showEvent(QShowEvent *event)
{
    QLabel::showEvent(event);
    move(QGuiApplication::primaryScreen()->geometry().center() - rect().center());
    d->timer.start(500);
}

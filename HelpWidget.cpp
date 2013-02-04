#include "HelpWidget.h"

HelpWidget::HelpWidget(int iconExtent, const QString &text, QWidget *parent) :
	QWidget(parent)
{
	createLayout(iconExtent, text);
}

HelpWidget::HelpWidget(int iconExtent, QWidget *parent) :
	QWidget(parent)
{
	createLayout(iconExtent, QString());
}

void HelpWidget::createLayout(int iconExtent, const QString &text)
{
	textLabel = new QLabel(text, this);
	textLabel->setWordWrap(true);
	QLabel *infoIcon = new QLabel(this);
	infoIcon->setPixmap(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation)
						.pixmap(iconExtent));
	QHBoxLayout *infoLayout = new QHBoxLayout(this);
	infoLayout->addWidget(infoIcon);
	infoLayout->addWidget(textLabel, 1);
	infoLayout->setContentsMargins(QMargins());
}

QString HelpWidget::text() const
{
	return textLabel->text();
}

void HelpWidget::setText(const QString &text)
{
	textLabel->setText(text);
}

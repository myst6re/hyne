#include "MetadataDialog.h"
#include "UserDirectory.h"

MetadataDialog::MetadataDialog(QWidget *parent) :
	QDialog(parent, Qt::Dialog | Qt::WindowCloseButtonHint)
{
	build();
}

MetadataDialog::MetadataDialog(const QMap<FF8Installation::Type, FF8Installation> &ff8Installations, QWidget *parent) :
	QDialog(parent, Qt::Dialog | Qt::WindowCloseButtonHint)
{
	build();
	fill(ff8Installations);
}

void MetadataDialog::build()
{
	HelpWidget *help = new HelpWidget(32, tr("Pour être placées dans le Cloud, "
											 "les sauvegardes doivent être signées. "
											 "Ces signatures sont écrites dans un fichier "
											 "nommé metadata.xml, normalement situé "
											 "dans le même dossier que vos sauvegardes."));
	_autoCheckBox = new QCheckBox(tr("Auto"), this);

	_path = new QLineEdit(this);
	_pathButton = new QPushButton(tr("Parcourir..."), this);

	QHBoxLayout *pathLayout = new QHBoxLayout;
	pathLayout->addWidget(_path);
	pathLayout->addWidget(_pathButton);
	pathLayout->setContentsMargins(QMargins());

	_userID = new QLineEdit(this);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
													   Qt::Horizontal, this);

	connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));

	_formWidget = new QWidget(this);

	QFormLayout *formLayout = new QFormLayout(_formWidget);
	formLayout->addRow(tr("metadata.xml"), pathLayout);
	formLayout->addRow(tr("Numéro d'utilisateur"), _userID);
	formLayout->setContentsMargins(QMargins());

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(help);
	layout->addWidget(_autoCheckBox);
	layout->addWidget(_formWidget);
	layout->addStretch();
	layout->addWidget(buttonBox);

	connect(_autoCheckBox, SIGNAL(toggled(bool)), SLOT(formDisabled(bool)));
	connect(_pathButton, SIGNAL(clicked()), SLOT(setMetadataPath()));
}

void MetadataDialog::formDisabled(bool disabled)
{
	_formWidget->setDisabled(disabled);
}

void MetadataDialog::fill(const QMap<FF8Installation::Type, FF8Installation> &ff8Installations)
{
	if(ff8Installations.contains(FF8Installation::Steam)) {
		_autoCheckBox->setChecked(true);
		FF8Installation installation = ff8Installations.value(FF8Installation::Steam);
		if(installation.hasMetadata()) {
			QString savePath = installation.savePath(1);
			_path->setText(savePath + "/metadata.xml");
			UserDirectory userDir(savePath);
			_userID->setText(userDir.userID());
			if(!userDir.hasUserId()) {
				//TODO
				_autoCheckBox->setChecked(false);
				_autoCheckBox->hide();
			}
		} else {
			//TODO
			_autoCheckBox->setChecked(false);
			_autoCheckBox->hide();
		}
	} else {
		_autoCheckBox->setChecked(false);
		_autoCheckBox->hide();
	}
}

void MetadataDialog::setMetadataPath(const QString &path)
{
	QString metadataPath;

	if(path.isNull()) {
		QStringList filter = QStringList()
				<< tr("Fichiers XML (*.xml)")
				<< tr("Tous les fichiers (*)");
		metadataPath = QFileDialog::getOpenFileName(this, tr("Rechercher metadata.xml"), _path->text(), filter.join(";;"));
		if(metadataPath.isNull()) {
			return;
		}
	} else {
		metadataPath = path;
	}

	_path->setText(metadataPath);
}

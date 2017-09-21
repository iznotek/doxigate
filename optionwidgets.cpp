#include "optionwidgets.h"

#include "config.h"

const int FixedSize = 24;

OptionBase::OptionBase(const QString &title, ConfigOptionPtr option, QWidget *parent)
	: QWidget(parent),
	myOption(option)
{
	myLayout = new QHBoxLayout();
	myLayout->setMargin(0);
	myLayout->setSpacing(0);
	setLayout(myLayout);

	myHelp = new QLabel(this);
	myHelp->setFixedSize(20,20);
	myHelp->setPixmap(QPixmap(":/gui/icon16_help.png"));
	myLayout->addWidget(myHelp);
	myHelp->installEventFilter(this);

	myTitleLabel = new QLabel(title, this);
	myTitleLabel->setFixedWidth(140);
	myLayout->addWidget(myTitleLabel);
}

OptionBase::~OptionBase()
{

}

void OptionBase::readValue()
{
}

void OptionBase::storeValue()
{
}

void OptionBase::doEnterEvent()
{
	if (myOption)
		emit highlighted(this);
}

void OptionBase::doLeaveEvent()
{
    emit offlighted(this);
}

bool OptionBase::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == myHelp) {
        switch (event->type()) {
            case QEvent::Enter:
                doEnterEvent();
                return true;
            case QEvent::Leave:
                doLeaveEvent();
                return true;
            default:;
        }
    }

    return QObject::eventFilter(obj, event);
}

int OptionBase::titleTextWidth()
{
    return fontMetrics().width(myTitleLabel->text());
}

void OptionBase::setTitleTextWidth(int w)
{
    myTitleLabel->setFixedWidth(w);
}

/////////////////////////////////////////////

OptionCustom::OptionCustom(const QString &title, const QString &hint, QWidget *parent):
	OptionBase(title, 0, parent),
	myTitle(title),
	myHint(hint)
{
}

OptionCustom::~OptionCustom()
{
}

void OptionCustom::doEnterEvent()
{
    emit highlighted(myTitle, myHint);
}

/////////////////////////////////////////////

OptionString::OptionString(const QString &title, ConfigStringPtr option, QWidget *parent)
	: OptionBase(title, option, parent)
{
	myLineEdit = new QLineEdit(this);
	myLineEdit->setFixedHeight(FixedSize);

    myLayout->addWidget(myLineEdit);

	switch (option->widgetType())
	{
		case ConfigString::File:
		{
		    QToolButton *fileButton = new QToolButton(this);
		    fileButton->setFocusPolicy(Qt::NoFocus);
		    fileButton->setFixedSize(FixedSize,FixedSize);
			fileButton->setIcon(QIcon(":/gui/icon16_file.png"));
		    myLayout->addWidget(fileButton);

		    connect(fileButton, SIGNAL(clicked()), this, SLOT(pickFile()));
        }
		break;

        case ConfigString::Dir:
        {
            QToolButton *dirButton = new QToolButton(this);
            dirButton->setFocusPolicy(Qt::NoFocus);
            dirButton->setFixedSize(FixedSize,FixedSize);
			dirButton->setIcon(QIcon(":/gui/icon16_dir.png"));
            myLayout->addWidget(dirButton);

            connect(dirButton, SIGNAL(clicked()), this, SLOT(pickDir()));
        }
        break;

        default:;

	}

	readValue();
}

OptionString::~OptionString()
{

}

void OptionString::pickDir()
{
    QString dir = QFileDialog::getExistingDirectory(this,
		tr("Choose the directory for: %1").arg(title()),
		myLineEdit->text());

    if (!dir.isEmpty())
        myLineEdit->setText(dir);
}

void OptionString::pickFile()
{
    QString file = QFileDialog::getOpenFileName(this,
            tr("Choose the file for: %1").arg(title()),
			myLineEdit->text());

    if (!file.isEmpty())
        myLineEdit->setText(file);
}

void OptionString::readValue()
{
    myLineEdit->setText(myOption.staticCast<ConfigString>()->value());
}

void OptionString::storeValue()
{
    myOption.staticCast<ConfigString>()->setValue(myLineEdit->text());
}

/////////////////////////////////////////////

OptionEnum::OptionEnum(const QString &title, ConfigEnumPtr option, QWidget *parent)
	: OptionBase(title, option, parent)
{
    myCombo = new QComboBox(this);
	myCombo->setFixedHeight(FixedSize);

    myLayout->addWidget(myCombo);

    initCombo(myCombo, option);

    //readValue();
}

OptionEnum::~OptionEnum()
{

}

void OptionEnum::readValue()
{
    int i = myCombo->findText(myOption.staticCast<ConfigEnum>()->value());
    if (i >= 0)
        myCombo->setCurrentIndex(i);
}

void OptionEnum::storeValue()
{
    myOption.staticCast<ConfigEnum>()->setValue(myCombo->currentText());
}

void OptionEnum::initCombo(QComboBox *combo, ConfigEnumPtr option)
{
	combo->addItems(option->values());
	int i = combo->findText(option->defaultValue());
	if (i >= 0)
		combo->setCurrentIndex(i);
}

/////////////////////////////////////////////

OptionBool::OptionBool(const QString &title, ConfigBoolPtr option, QWidget *parent)
	: OptionBase(title, option, parent)
{
    myCheckBox = new QCheckBox(this);
    connect(myCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onChecked(int)));

    myLayout->addWidget(myCheckBox);

    readValue();
}

OptionBool::~OptionBool()
{

}

void OptionBool::onChecked(int state)
{
    emit checked(myOption.staticCast<ConfigBool>(), (Qt::CheckState)state);
}

void OptionBool::setCheckState(Qt::CheckState state)
{
    myCheckBox->setCheckState(state);
}

void OptionBool::readValue()
{
    myCheckBox->setChecked(*myOption.staticCast<ConfigBool>()->valueRef());
}

void OptionBool::storeValue()
{
    *myOption.staticCast<ConfigBool>()->valueRef() = myCheckBox->isChecked();
}

/////////////////////////////////////////////

OptionEncoding::OptionEncoding(const QString &title, ConfigStringPtr option, QWidget *parent)
    : OptionBase(title, option, parent)
{
    QStringList sl;
    sl << "ASCII" << "ISO-8859-1" << "ISO-8859-2" << "ISO-8859-3" << "ISO-8859-4" << "ISO-8859-5"
        << "ISO-8859-7" << "ISO-8859-9" << "ISO-8859-10" << "ISO-8859-13" << "ISO-8859-14" << "ISO-8859-15" << "ISO-8859-16"
        << "KOI8-R" << "KOI8-U" << "KOI8-RU"
        << "CP1250" << "CP1251" << "CP1252" << "CP1253" << "CP1254" << "CP1257"
        << "CP850" << "CP866"
        << "MacRoman" << "MacCentralEurope" << "MacIceland" << "MacCroatian" << "MacRomania"
        << "MacCyrillic" << "MacUkraine" << "MacGreek" << "MacTurkish" << "Macintosh"
        // Semitic languages
        << "ISO-8859-6" << "ISO-8859-8" << "CP1255" << "CP1256" << "CP862" << "MacHebrew" << "MacArabic"
        // Japanese
        << "EUC-JP" << "SHIFT_JIS" << "CP932" << "ISO-2022-JP" << "ISO-2022-JP-2" << "ISO-2022-JP-1"
        // Chinese
        << "EUC-CN" << "HZ" << "GBK" << "CP936" << "GB18030" << "EUC-TW" << "BIG5" << "CP950"
        << "BIG5-HKSCS" << "BIG5-HKSCS:2001" << "BIG5-HKSCS:1999" << "ISO-2022-CN" << "ISO-2022-CN-EXT"
        // Korean
        << "EUC-KR" << "CP949" << "ISO-2022-KR" << "JOHAB"
        // Armenian
        << "ARMSCII-8"
        //Georgian
        << "Georgian-Academy" << "Georgian-PS"
        //Tajik
        << "KOI8-T"
        //Kazakh
        << "PT154" << "RK1048"
        //Thai
        << "ISO-8859-11" << "TIS-620" << "CP874" << "MacThai"
        //Laotian
        << "MuleLao-1" << "CP1133"
        //Vietnamese
        << "VISCII" << "TCVN" << "CP1258"
        //Platform specifics
        << "HP-ROMAN8" << "NEXTSTEP"
        //Full Unicode
        << "UTF-8"
        << "UCS-2" << "UCS-2BE" << "UCS-2LE"
        << "UCS-4" << "UCS-4BE" << "UCS-4LE"
        << "UTF-16" << "UTF-16BE" << "UTF-16LE"
        << "UTF-32" << "UTF-32BE" << "UTF-32LE"
        << "UTF-7"
        << "C99" << "JAVA"
        //Full Unicode, in terms of uint16_t or uint32_t (with machine dependent endianness and alignment)
        << "UCS-2-INTERNAL" << "UCS-4-INTERNAL"
        ;

    myComboEnc = new QComboBox(this);
	myComboEnc->setFixedHeight(FixedSize);
    myComboEnc->addItems(sl);
    myComboEnc->setEditable(true);
    myLayout->addWidget(myComboEnc);

    readValue();
}

OptionEncoding::~OptionEncoding()
{
}

void OptionEncoding::readValue()
{
    myComboEnc->lineEdit()->setText(myOption.staticCast<ConfigString>()->value());
}

void OptionEncoding::storeValue()
{
    myOption.staticCast<ConfigString>()->setValue(myComboEnc->currentText());
}

/////////////////////////////////////////////

#include "iisStringListEdit.h"

OptionList::OptionList(const QString &title, ConfigListPtr option, QWidget *parent)
    : OptionBase(title, option, parent)
{
    myLineEdit = new QLineEdit(this);
	myLineEdit->setFixedHeight(FixedSize);

    myLayout->addWidget(myLineEdit);

    QToolButton *editButton = new QToolButton(this);
    editButton->setFocusPolicy(Qt::NoFocus);
    editButton->setFixedSize(FixedSize,FixedSize);
	editButton->setIcon(QIcon(":/gui/icon16_list.png"));
    myLayout->addWidget(editButton);
    connect(editButton, SIGNAL(clicked()), this, SLOT(edit()));

    readValue();
}

OptionList::~OptionList()
{

}

void OptionList::edit()
{
    int flags = SLF_Custom;
    switch (myOption.staticCast<ConfigList>()->widgetType()) {
        case ConfigList::File:
            flags |= SLF_Files;
            break;
        case ConfigList::Dir:
            flags |= SLF_Dirs;
            break;
        case ConfigList::FileAndDir:
            flags |= SLF_Dirs | SLF_Files;
            break;
        default:;
    }

    QStringList sl = myLineEdit->text().split(";");
    bool b = iisStringListEditDialog::getList(
            sl,
            title(),
            flags,
			this);
    if (b)
        myLineEdit->setText(sl.join(";"));
}

void OptionList::readValue()
{
    QStringList sl = myOption.staticCast<ConfigList>()->values();
    myLineEdit->setText(sl.join(";"));
}

void OptionList::storeValue()
{
    QStringList sl = myLineEdit->text().split(";");
    myOption.staticCast<ConfigList>()->setValue(sl);
}

/////////////////////////////////////////////

OptionInt::OptionInt(const QString &title, ConfigIntPtr option, QWidget *parent)
    : OptionBase(title, option, parent)
{
    mySpinBox = new QSpinBox(this);
	mySpinBox->setFixedHeight(FixedSize);

    mySpinBox->setRange(option->minVal(), option->maxVal());

    myLayout->addWidget(mySpinBox);

    readValue();
}

OptionInt::~OptionInt()
{

}

void OptionInt::readValue()
{
    mySpinBox->setValue(*myOption.staticCast<ConfigInt>()->valueRef());
}

void OptionInt::storeValue()
{
    *myOption.staticCast<ConfigInt>()->valueRef() = mySpinBox->value();
}

/////////////////////////////////////////////


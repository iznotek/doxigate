#include "setupbasepage.h"

#include "config.h"

#include "optionwidgets.h"

#define HINTTIMEOUT 500

////////////////////////////////////////////////////////////////////////////////

void setEnumValue(const char *name, const QString &value)
{
    ConfigEnumPtr ce = getEnum(name);
    if (!ce.isNull())
        ce->setValue(value);
}

void setStringValue(const char *name, const QString &value)
{
    ConfigStringPtr ce = getString(name);
    if (!ce.isNull())
        ce->setValue(value);
}

void setBoolValue(const char *name, bool value)
{
    ConfigBoolPtr ce = getBool(name);
    if (!ce.isNull())
        *ce->valueRef() = value;
}

void setIntValue(const char *name, int value)
{
    ConfigIntPtr ce = getInt(name);
    if (!ce.isNull())
        *ce->valueRef() = value;
}



ConfigEnumPtr getEnum(const char *name)
{
    return Config::instance()->get(name).staticCast<ConfigEnum>();
}

ConfigStringPtr getString(const char *name)
{
    return Config::instance()->get(name).staticCast<ConfigString>();
}

ConfigBoolPtr getBool(const char *name)
{
    return Config::instance()->get(name).staticCast<ConfigBool>();
}

ConfigIntPtr getInt(const char *name)
{
    return Config::instance()->get(name).staticCast<ConfigInt>();
}

////////////////////////////////////////////////////////////////////////////////

SetupBasePage::SetupBasePage(const QPixmap &icon, const QString &title, QWidget *parent)
	: QFrame(parent),
	myGroup(0),
	myLastAdded(0)
{
	init(icon, title, parent);
}

SetupBasePage::SetupBasePage(const QString &title, QWidget *parent)
	: QFrame(parent),
	myGroup(0),
	myLastAdded(0)
{
	init(QPixmap(), title, parent);
}

void SetupBasePage::init(const QPixmap &icon, const QString &title, QWidget *)
{
    setFrameStyle(QFrame::StyledPanel);

    QVBoxLayout *vbl = new QVBoxLayout();
    setLayout(vbl);

    QLabel *titleLabel = new QLabel(title, this);
    titleLabel->setFrameStyle(QFrame::StyledPanel);
    titleLabel->setProperty("kind", "header1");
    vbl->addWidget(titleLabel);

    QHBoxLayout *hbl = new QHBoxLayout();
    hbl->setMargin(0);
    hbl->setSpacing(0);
    titleLabel->setLayout(hbl);
    if (!icon.isNull()) {
        QLabel *iconLabel = new QLabel(titleLabel);
        iconLabel->setPixmap(icon);
        iconLabel->setFixedSize(QSize(icon.width(),icon.height()));
        hbl->addStretch();
        hbl->addWidget(iconLabel);
    }

    myMainLayout = new QHBoxLayout();
    vbl->addLayout(myMainLayout);

    myLayout = new QVBoxLayout();
    myMainLayout->addLayout(myLayout);
    myMainLayout->setStretchFactor(myLayout,1);

    myTip = new QLabel(this);
    myTip->hide();
    myTip->setFrameStyle(QFrame::StyledPanel);
    myTip->setAlignment(Qt::AlignTop);
    myTip->setWordWrap(true);
    myTip->setProperty("kind", "helplabel");

    myHintTimer = new QTimer(this);
    connect(myHintTimer, SIGNAL(timeout()), this, SLOT(hintTimeout()));
}


SetupBasePage::~SetupBasePage()
{
}

void SetupBasePage::switchLayout(int l)
{
    QList<QWidget*> list = l ? myCol2 : myCol1;

    QList<QWidget*> tempList;

    // remove old layout
    while (myMainLayout->count()) {
            QLayoutItem *li = myMainLayout->takeAt(0);
            myLayout = (QVBoxLayout*)li->layout();
            if (myLayout)
            for (int i = 0; i < myLayout->count(); i++) {
                    QWidget *w = myLayout->itemAt(i)->widget();
                    if (w)
                            tempList.append(w);
            }
            delete li;
    }

	// set new layout
    myLayout = new QVBoxLayout();
    myMainLayout->addLayout(myLayout);
    myMainLayout->setStretchFactor(myLayout,1);

    for (int i = 0; i < tempList.count(); i++) {
            QWidget *w = tempList.at(i);
            myLayout->addWidget(w);
            if (list.count())
                    if (w == list.first()) {
                            list.takeFirst();
                            myLayout->addStretch();
                            myLayout = new QVBoxLayout();
                            myMainLayout->addSpacing(16);
                            myMainLayout->addLayout(myLayout);
                            myMainLayout->setStretchFactor(myLayout,1);
                    }
    }

    myLayout->addStretch();
}

void SetupBasePage::addColumn()
{
    finishGroup();

    if (myLayout)
        myLayout->addStretch();

    myLayout = new QVBoxLayout();
    myMainLayout->addSpacing(16);
    myMainLayout->addLayout(myLayout);
    myMainLayout->setStretchFactor(myLayout,1);

        myCol1.append(myLastAdded);
}

void SetupBasePage::addColumn2()
{
	myCol2.append(myLastAdded);
}

void SetupBasePage::addGroup(const QString &title)
{
    finishGroup();

    myGroup = new QGroupBox(title, this);
    myLayout->addWidget(myGroup);
    myGroupLayout = new QVBoxLayout();
    //myGroupLayout->setSpacing(4);
    //myGroupLayout->setMargin(6);
    myGroup->setLayout(myGroupLayout);

	myLastAdded = myGroup;
}

void SetupBasePage::finishGroup()
{
    if (myGroup) {
        myGroup = 0;
    }
}

void SetupBasePage::finishPage()
{
    finishGroup();

    if (myLayout)
        myLayout->addStretch();

    ((QVBoxLayout*)layout())->addStretch();

    // fit all the option text
    int baseWidth = myWidgets.first()->titleTextWidth();
    int minWidth = 0;
    foreach (OptionBase *ob, myWidgets) {
        minWidth = qMax(minWidth, ob->titleTextWidth());
    }
    if (minWidth > baseWidth)
        foreach (OptionBase *ob, myWidgets) {
            ob->setTitleTextWidth(minWidth+10);
        }
}

void SetupBasePage::addSection(const QString &title)
{
    finishGroup();

	QWidget *w = new QWidget(this);
	myLayout->addWidget(w);
	myLastAdded = w;

    QHBoxLayout *hbl = new QHBoxLayout;
	w->setLayout(hbl);
	hbl->setMargin(0);
//    myLayout->addLayout(hbl);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    hbl->addWidget(line);

    QLabel *label = new QLabel(title, this);
    label->setProperty("kind", "header2");
    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    hbl->addWidget(label);
}

void SetupBasePage::addOption(const QString &title, const char *id)
{
    ConfigOptionPtr option = Config::instance()->get(id);
	if (!option)
		return;

	switch(option->kind()) {
		case ConfigOption::O_String:
            addOptionWidget(new OptionString(title, option.staticCast<ConfigString>(), this));
			break;

		case ConfigOption::O_Enum:
            addOptionWidget(new OptionEnum(title, option.staticCast<ConfigEnum>(), this));
			break;

		case ConfigOption::O_List:
            addOptionWidget(new OptionList(title, option.staticCast<ConfigList>(), this));
			break;

		case ConfigOption::O_Bool: {
            OptionBool *ob = new OptionBool(title, option.staticCast<ConfigBool>(), this);
            connect(ob, SIGNAL(checked(ConfigBoolPtr, Qt::CheckState)),
                    this, SIGNAL(optionChecked(ConfigBoolPtr, Qt::CheckState)));
		    addOptionWidget(ob);
		}
			break;

		case ConfigOption::O_Int:
            addOptionWidget(new OptionInt(title, option.staticCast<ConfigInt>(), this));
			break;

		case ConfigOption::O_Info:
		case ConfigOption::O_Obsolete:
		default:
			break;
	}
}

void SetupBasePage::addOptionWidget(OptionBase *ob)
{
    if (myGroup)
        myGroupLayout->addWidget(ob);
	else {
        myLayout->addWidget(ob);
		myLastAdded = ob;
	}


//	if (ob->option())
//		connect(ob, SIGNAL(highlighted(ConfigOptionPtr)),
//			this, SIGNAL(optionHighlighted(ConfigOptionPtr)));
//	else
//		connect(ob, SIGNAL(highlighted(const QString&, const QString&)),
//			this, SIGNAL(optionsHighlighted(const QString&, const QString&)));

    myWidgets.append(ob);

    if (ob->option()) {
        connect(ob, SIGNAL(highlighted(OptionBase*)),
            this, SLOT(onOptionHighlighted(OptionBase*)));

        myWidgetMap[ob->option()] = ob;
    }
    else {
        connect(ob, SIGNAL(highlighted(const QString&, const QString&)),
            this, SLOT(onOptionHighlighted(const QString&, const QString&)));
    }

    connect(ob, SIGNAL(offlighted(OptionBase*)),
        this, SLOT(onOptionOfflighted(OptionBase*)));
}

void SetupBasePage::storeValues()
{
    foreach(OptionBase *ob, myWidgets) {
        ob->storeValue();
    }
}

void SetupBasePage::reloadWidget(const char *id)
{
    ConfigOptionPtr co = Config::instance()->get(id);
    if (co) {
        if (myWidgetMap.contains(co)) {
            OptionBase *ob = myWidgetMap[co];
            ob->readValue();
        }
    }
}



void SetupBasePage::hintTimeout()
{
    myHintTimer->stop();

    QPoint pos = mapFromGlobal(QCursor::pos());

    int lm = 11;
    int x = pos.x() > width()/2 ? lm : width()/2;
//    QToolTip::showText(mapToGlobal(QPoint(x,0)), txt, this, rect());
//    QToolTip::showText(QCursor::pos(), txt);

    myTip->setGeometry(x,40,width()/2-lm,height()-40-8);
    myTip->setText(myHintText);
    myTip->raise();
    myTip->show();
}

void SetupBasePage::onOptionHighlighted(OptionBase *option)
{
	QString txt = QString("Corresponds to Tag: <b>%1</b><hr><p>%2")
		.arg(option->option()->nameString(), option->option()->docString());

    onOptionHighlighted(option->title(), txt);
}

void SetupBasePage::onOptionHighlighted(const QString &title, const QString &text)
{
    myHintText = QString("<p><b>%1</b><img src=':/gui/icon16_help.png' align=right><hr><p>%2").arg(title, text);
    myHintTimer->start(HINTTIMEOUT);
}

void SetupBasePage::onOptionOfflighted(OptionBase*)
{
    myTip->hide();
    myHintTimer->stop();
}

void SetupBasePage::onOptionChecked(ConfigBoolPtr option, Qt::CheckState check)
{
    OptionBool *ob = (OptionBool*) myWidgetMap[option];
    if (!ob)
        return;

    ob->setCheckState(check);
}




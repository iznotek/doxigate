#ifndef OPTIONWIDGETS_H
#define OPTIONWIDGETS_H

#include <QtWidgets>
#include <config.h>

//class ConfigOption;
//typedef QSharedPointer<ConfigOption> ConfigOptionPtr;
////class ConfigOptionPtr;
//class ConfigString;
//class ConfigEnum;
//class ConfigBool;
//class ConfigList;
//class ConfigInt;

/////////////////////////////////////////////

class OptionBase : public QWidget
{
	Q_OBJECT

public:
    OptionBase(const QString &title, ConfigOptionPtr option, QWidget *parent);
	virtual ~OptionBase();

	virtual void readValue();
	virtual void storeValue();

	inline ConfigOptionPtr option() { return myOption; }
	inline const QString title() { return myTitleLabel->text(); }

	int titleTextWidth();
	void setTitleTextWidth(int w);

signals:
	void highlighted(OptionBase*);
	void offlighted(OptionBase*);

protected:
	virtual void doEnterEvent();
	virtual void doLeaveEvent();

	virtual bool eventFilter(QObject *obj, QEvent *event);

    ConfigOptionPtr myOption;
    QHBoxLayout *myLayout;
	QLabel *myHelp;
	QLabel *myTitleLabel;
};

/////////////////////////////////////////////

class OptionCustom : public OptionBase
{
	Q_OBJECT

public:
	OptionCustom(const QString &title, const QString &hint, QWidget *parent);
	virtual ~OptionCustom();

signals:
	void highlighted(const QString&, const QString&);

protected:
	virtual void doEnterEvent();

	QString myTitle, myHint;
};

/////////////////////////////////////////////

class OptionString : public OptionBase
{
	Q_OBJECT

public:
    OptionString(const QString &title, ConfigStringPtr option, QWidget *parent);
	virtual ~OptionString();

	virtual void readValue();
	virtual void storeValue();

protected slots:
    void pickDir();
    void pickFile();

private:
	QLineEdit *myLineEdit;
};

/////////////////////////////////////////////

class OptionEnum : public OptionBase
{
	Q_OBJECT

public:
    OptionEnum(const QString &title, ConfigEnumPtr option, QWidget *parent);
	virtual ~OptionEnum();

    virtual void readValue();
    virtual void storeValue();

    static void initCombo(QComboBox *combo, ConfigEnumPtr option);

protected:
    QComboBox *myCombo;
};

/////////////////////////////////////////////

class OptionBool : public OptionBase
{
	Q_OBJECT

public:
    OptionBool(const QString &title, ConfigBoolPtr option, QWidget *parent);
	virtual ~OptionBool();

	virtual void readValue();
	virtual void storeValue();

	void setCheckState(Qt::CheckState state);

signals:
    void checked(ConfigBoolPtr, Qt::CheckState state);

private slots:
    void onChecked(int);

private:
	QCheckBox *myCheckBox;
};

/////////////////////////////////////////////

class OptionEncoding : public OptionBase
{
    Q_OBJECT

public:
    OptionEncoding(const QString &title, ConfigStringPtr option, QWidget *parent);
    virtual ~OptionEncoding();

    virtual void readValue();
    virtual void storeValue();

protected:
    QComboBox *myComboEnc;
};

/////////////////////////////////////////////

class OptionList : public OptionBase
{
    Q_OBJECT

public:
    OptionList(const QString &title, ConfigListPtr option, QWidget *parent);
    virtual ~OptionList();

    virtual void readValue();
    virtual void storeValue();

protected slots:
    void edit();

private:
    QLineEdit *myLineEdit;
};

/////////////////////////////////////////////


class OptionInt : public OptionBase
{
    Q_OBJECT

public:
    OptionInt(const QString &title, ConfigIntPtr option, QWidget *parent);
    virtual ~OptionInt();

    virtual void readValue();
    virtual void storeValue();

private:
    QSpinBox *mySpinBox;
};

/////////////////////////////////////////////

#endif // OPTIONWIDGETS_H

#ifndef SETUPPROJECTPAGE_H
#define SETUPPROJECTPAGE_H

#include "setupbasepage.h"

/////////////////////////////////////////////

class SetupProjectPage : public SetupBasePage
{
	Q_OBJECT

public:
	SetupProjectPage(QWidget *parent);
	virtual ~SetupProjectPage();

private:

};

/////////////////////////////////////////////

#include "optionwidgets.h"

class OptionLanguage : public OptionCustom
{
	Q_OBJECT

public:
	OptionLanguage(const QString &title, QWidget *parent);
	virtual ~OptionLanguage();

	virtual void readValue();
	virtual void storeValue();

protected:
	QComboBox *myCombo;
};

/////////////////////////////////////////////

//class OptionEncoding : public OptionCustom
//{
//	Q_OBJECT
//
//public:
//	OptionEncoding(const QString &title, QWidget *parent);
//	virtual ~OptionEncoding();
//
//	virtual void readValue();
//	virtual void storeValue();
//
//protected:
//	QComboBox *myComboLang, *myComboEnc;
//};

/////////////////////////////////////////////

#endif // SETUPPROJECTPAGE_H

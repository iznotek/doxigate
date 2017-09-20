#ifndef SETUPBASEPAGE_H
#define SETUPBASEPAGE_H

#include <QtWidgets>

class ConfigOption;
class ConfigOptionPtr;
class ConfigEnum;
class ConfigString;
class ConfigBool;
class ConfigInt;

class OptionBase;


extern void setEnumValue(const char *name, const QString &value);
extern void setStringValue(const char *name, const QString &value);
extern void setBoolValue(const char *name, bool value);
extern void setIntValue(const char *name, int value);

extern ConfigEnum* getEnum(const char *name);
extern ConfigString* getString(const char *name);
extern ConfigBool* getBool(const char *name);
extern ConfigInt* getInt(const char *name);


class SetupBasePage : public QFrame
{
	Q_OBJECT

public:
	SetupBasePage(const QString &title, QWidget *parent);
	SetupBasePage(const QPixmap &icon, const QString &title, QWidget *parent);
	~SetupBasePage();

	void addColumn();
	void addColumn2();
	void addSection(const QString &title);
	void addOption(const QString &title, const char *id);
	void addGroup(const QString &title = QString());
	void finishGroup();
	void finishPage();

	void switchLayout(int l);

	void storeValues();

	void reloadWidget(const char *id);

signals:
	void optionHighlighted(OptionBase*);
	void optionsHighlighted(const QString&, const QString&);

	void optionChecked(ConfigBool*, Qt::CheckState check);

protected slots:
    void onOptionHighlighted(OptionBase*);
    void onOptionHighlighted(const QString&, const QString&);
    void onOptionOfflighted(OptionBase*);

    void onOptionChecked(ConfigBool*, Qt::CheckState check);

    void hintTimeout();

protected:
	void init(const QPixmap &icon, const QString &title, QWidget *parent);
	void addOptionWidget(OptionBase *ob);

	QBoxLayout *myMainLayout, *myLayout, *myGroupLayout;
	QGroupBox *myGroup;

    QMap<ConfigOptionPtr, OptionBase*> myWidgetMap;
	QList<OptionBase*> myWidgets;

	QList<QWidget*> myCol1, myCol2;
	QWidget *myLastAdded;

	QLabel *myTip;
    QString myHintText;
    QTimer *myHintTimer;
};

#endif // SETUPBASEPAGE_H

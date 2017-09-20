/*
 * options.h
 *
 *  Created on: 13.08.2008
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <QtCore>

class Option
{
public:

    Option()
    {
        widget = 0;
    }

    inline void setup(const QString &_id, const QVariant &_def, int _numId = 0)
    {
        id = _id;
        val = def = _def;
        numId = _numId;
    }

    inline void connectWidget(QWidget *w)
    {
        widget = w;
    }

    void readFromWidget();
    void storeToWidget(bool _def = false);

    inline operator bool() { return val.toBool(); }
    inline operator int() { return val.toInt(); }
    inline operator QString() { return val.toString(); }

//    inline QVariant& operator[](int i) { return data[i]; }

    template <typename T> inline Option& operator=(const T& v) { val = v; return *this; }

    QString id;
    int numId;
    QVariant def;
    QVariant val;

    QWidget *widget;
    QMap<int, QVariant> data;
};


class Options
{
public:
    Options();
    virtual ~Options();

    void setup(Option &op, const QString &_id, const QVariant &_def, int _numId = 0);

    void store(QSettings &set);
    void restore(QSettings &set);

    void readFromWidgets();
    void storeToWidgets();
    void storeDefaultsToWidgets();

    inline QList<Option*> options() const { return myOptions; }
    QList<Option*> options(int id);

protected:
    QList<Option*> myOptions;
};

#endif /* OPTIONS_H_ */

#ifndef _CAFORM_H
#define _CAFORM_H

#include <QMetaType>
#include "../basicform.h"
#include "ca.h"

Q_DECLARE_METATYPE(const rule*);

class QDoubleSpinBox;

class CAForm : public BasicForm
{
	Q_OBJECT
protected slots:
	virtual void draw();
	virtual void cont();
	virtual void init();
protected:
	void addRule(const QString &s, const rule &r);
	QSpinBox *spinWidth;
	QSpinBox *spinHeight;
	QComboBox *comboRule;
	QSpinBox *spinLeft;
	QSpinBox *spinRight;
	QSpinBox *spinTop;
	QSpinBox *spinBottom;
	QSpinBox *spinTurns;
	QDoubleSpinBox *spinIntensity;
	QPushButton *buttonContinue;
	QScopedPointer<ca> ca_sim;
};

#endif

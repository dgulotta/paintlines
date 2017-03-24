#include "../inputwidgets.h"
#include "walk.h"
#include "walkwidget.h"
#include <QCheckBox>

WalkWidget::WalkWidget()
{
	auto ht = new SizeSpin(1);
	layout()->addRow("Height",ht);
	auto wd = new SizeSpin(1);
	layout()->addRow("Width",wd);
	auto ba = new QCheckBox("Balanced");
	ba->setChecked(true);
	layout()->addRow(ba);
	addGenerator("Draw",[=] () {
		return draw_walk(wd->value(),ht->value(),ba->isChecked());
	});
}

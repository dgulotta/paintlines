#include "../inputwidgets.h"
#include "walk.h"
#include "walkwidget.h"

WalkWidget::WalkWidget()
{
	auto ht = new SizeSpin(1);
	layout()->addRow("Height",ht);
	auto wd = new SizeSpin(1);
	layout()->addRow("Width",wd);
	addGenerator("Draw",[=] () {
		return draw_walk(wd->value(),ht->value());
	});
}

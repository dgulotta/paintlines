/***************************************************************************
 *   Copyright (C) 2014, 2016 by Daniel Gulotta                            *
 *   dgulotta@alum.mit.edu                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA         *
 *   02110-1301  USA                                                       *
 ***************************************************************************/

#include <QDoubleSpinBox>
#include <QLabel>
#include "../imagedata.h"
#include "../inputwidgets.h"
#include "cawidget.h"

Q_DECLARE_METATYPE(const rule*);

CAWidget::CAWidget()
{
	auto width = new QSpinBox;
	width->setRange(1,0x10000);
	width->setValue(1600);
	layout()->addRow("Width",width);
	auto height = new QSpinBox;
	height->setRange(1,0x10000);
	height->setValue(900);
	layout()->addRow("Height",height);
	DataComboAdapter<const rule *> comboRule({
		{"34 Life",&rule::threefour},
		{"Banners",&rule::banners},
		{"Beams",&rule::beams},
		{"Blob",&rule::blob},
		{"Bombers",&rule::bombers},
		{"Bubbles",&rule::bubbles3},
		{"Colony",&rule::colony},
		{"Coral",&rule::coral},
		{"Conway's Life",&rule::conway},
		{"Diamoeba",&rule::diamoeba},
		{"Dust Clouds",&rule::dustclouds},
		{"Ebb & Flow",&rule::ebbandflow},
		{"Fireworks",&rule::fireworks},
		{"Flakes",&rule::flakes},
		{"Gnats",&rule::gnats},
		{"Polluters",&rule::polluters},
		{"Prairie on Fire",&rule::prairieonfire},
		{"Pyramids",&rule::pyramids},
		{"Seeds",&rule::seeds},
		{"Soft Freeze",&rule::softfreeze},
		{"Smoke Plumes",&rule::plumes},
		{"Spill",&rule::spill},
		{"Sponge",&rule::sponge},
		{"Streets",&rule::streets},
		{"Streaks",&rule::streaks},
		{"Tendrils",&rule::tendrils},
		{"Tubes",&rule::tubes},
		{"Wanderers",&rule::wanderers}
	});
	layout()->addRow("Rule",comboRule.box());
	layout()->addRow(new QLabel("Starting area"));
	auto left = new QSpinBox;
	left->setRange(1,0x10000);
	left->setValue(797);
	layout()->addRow("Left",left);
	auto top = new QSpinBox;
	top->setRange(1,0x10000);
	top->setValue(447);
	layout()->addRow("Top",top);
	auto right = new QSpinBox;
	right->setRange(1,0x10000);
	right->setValue(803);
	layout()->addRow("Right",right);
	auto bottom = new QSpinBox;
	bottom->setRange(1,0x10000);
	bottom->setValue(453);
	layout()->addRow("Bottom",bottom);
	auto turns = new QSpinBox;
	turns->setRange(1,0x10000);
	turns->setValue(3000);
	layout()->addRow("Turns",turns);
	auto intensity = new QDoubleSpinBox;
	intensity->setMaximum(10.);
	intensity->setValue(1.);
	intensity->setSingleStep(.01);
	layout()->addRow("Intensity",intensity);
	auto cont = [=] () {
		ca_sim->run_for(turns->value());
		return ca_sim->get_diff_image(intensity->value());
	};
	auto buttonStart = addGenerator("Start",[=] () {
		ca_sim.reset(new ca(width->value(),height->value(),
			*(comboRule.value())));
		ca_sim->random_rectangle(left->value(),top->value(),right->value(),
			bottom->value());
		return cont();
	});
	auto buttonContinue = addGenerator("Continue",cont);
	buttonContinue->setEnabled(false);
	connect(buttonStart,&QPushButton::clicked,[=] () { buttonContinue->setEnabled(true); });
}

/***************************************************************************
 *   Copyright (C) 2014 by Daniel Gulotta                                  *
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

#include <QtWidgets>
#include "../imagedata.h"
#include "cawidget.h"

using std::get;

CAWidget::CAWidget()
{
	QFormLayout *layout = new QFormLayout;
	spinWidth = new QSpinBox;
	spinWidth->setMaximum(65536);
	spinWidth->setValue(1600);
	layout->addRow("Width",spinWidth);
	spinHeight = new QSpinBox;
	spinHeight->setMaximum(65536);
	spinHeight->setValue(900);
	layout->addRow("Height",spinHeight);
	comboRule = new QComboBox;
	addRule("34 Life",rule::threefour);
	addRule("Banners",rule::banners);
	addRule("Beams",rule::beams);
	addRule("Blob",rule::blob);
	addRule("Bombers",rule::bombers);
	addRule("Bubbles",rule::bubbles3);
	addRule("Colony",rule::colony);
	addRule("Coral",rule::coral);
	addRule("Conway's Life",rule::conway);
	addRule("Diamoeba",rule::diamoeba);
	addRule("Dust Clouds",rule::dustclouds);
	addRule("Ebb & Flow",rule::ebbandflow);
	addRule("Fireworks",rule::fireworks);
	addRule("Flakes",rule::flakes);
	addRule("Gnats",rule::gnats);
	addRule("Polluters",rule::polluters);
	addRule("Prairie on Fire",rule::prairieonfire);
	addRule("Pyramids",rule::pyramids);
	addRule("Seeds",rule::seeds);
	addRule("Soft Freeze",rule::softfreeze);
	addRule("Smoke Plumes",rule::plumes);
	addRule("Spill",rule::spill);
	addRule("Sponge",rule::sponge);
	addRule("Streets",rule::streets);
	addRule("Streaks",rule::streaks);
	addRule("Tendrils",rule::tendrils);
	addRule("Tubes",rule::tubes);
	addRule("Wanderers",rule::wanderers);
	layout->addRow("Rule",comboRule);
	layout->addRow(new QLabel("Starting area"));
	spinLeft = new QSpinBox;
	spinLeft->setMaximum(65536);
	spinLeft->setValue(797);
	layout->addRow("Left",spinLeft);
	spinTop = new QSpinBox;
	spinTop->setMaximum(65536);
	spinTop->setValue(447);
	layout->addRow("Top",spinTop);
	spinRight = new QSpinBox;
	spinRight->setMaximum(65536);
	spinRight->setValue(803);
	layout->addRow("Right",spinRight);
	spinBottom = new QSpinBox;
	spinBottom->setMaximum(65536);
	spinBottom->setValue(453);
	layout->addRow("Bottom",spinBottom);
	spinTurns = new QSpinBox;
	spinTurns->setMaximum(65536);
	spinTurns->setValue(3000);
	layout->addRow("Turns",spinTurns);
	spinIntensity = new QDoubleSpinBox;
	spinIntensity->setMaximum(10.);
	spinIntensity->setValue(1.);
	spinIntensity->setSingleStep(.01);
	layout->addRow("Intensity",spinIntensity);
	QPushButton *buttonDraw = new QPushButton("Start");
	layout->addRow(buttonDraw);
	buttonContinue = new QPushButton("Continue");
	buttonContinue->setEnabled(false);
	layout->addRow(buttonContinue);
	setLayout(layout);
	connect(buttonContinue,&QPushButton::clicked,this,&CAWidget::cont);
	connect(buttonDraw,&QPushButton::clicked,this,&CAWidget::draw);
}

void CAWidget::addRule(const QString &s, const rule &r)
{
	comboRule->addItem(s,QVariant::fromValue(&r));
}

void CAWidget::draw()
{
	buttonContinue->setEnabled(true);
	const rule *r = comboRule->itemData(comboRule->currentIndex()).value<const rule *>();
	ca_sim.reset(new ca(spinWidth->value(),spinHeight->value(),*r));
	ca_sim->random_rectangle(spinLeft->value(),spinTop->value(),
		spinRight->value(),spinBottom->value());
	cont();
}

void CAWidget::cont()
{
	ca_sim->run_for(spinTurns->value());
	ImageData data(ca_sim->get_diff_image(spinIntensity->value()));
	emit newImage(data);
}

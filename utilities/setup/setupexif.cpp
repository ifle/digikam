/* ============================================================
 * Author: Gilles Caulier <caulier dot gilles at free.fr>
 *         Ralf Holzer <ralf at well.com>
 * Date  : 2003-08-03
 * Description : setup Image Editor tab.
 * 
 * Copyright 2003-2004 by Ralf Holzer and Gilles Caulier
 * Copyright 2005-2006 by Gilles Caulier
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * ============================================================ */

// QT includes.

#include <qlayout.h>
#include <qvbuttongroup.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qwhatsthis.h>

// KDE includes.

#include <klocale.h>
#include <kdialog.h>

// // Local includes.

#include "albumsettings.h"
#include "setupexif.h"

namespace Digikam
{

class SetupExifPriv
{
public:

    SetupExifPriv()
    {
        iconSaveExifBox           = 0;
        iconExifRotateBox         = 0;
        iconExifSetOrientationBox = 0;
    }

    QCheckBox *iconSaveExifBox;
    QCheckBox *iconExifRotateBox;
    QCheckBox *iconExifSetOrientationBox;
};

SetupExif::SetupExif(QWidget* parent )
         : QWidget(parent)
{
    d = new SetupExifPriv;
    QVBoxLayout *mainLayout = new QVBoxLayout(parent);

    QVBoxLayout *layout = new QVBoxLayout( this, 0, KDialog::spacingHint());

   // --------------------------------------------------------

   // NOTE: put this back in when/if there are other, non-EXIF settings here
   /*   QGroupBox *iconExifGroup = new QGroupBox(1, Qt::Horizontal, i18n("Exif Actions"), parent);*/

   QLabel* explanation = new QLabel(this);
   explanation->setAlignment(explanation->alignment() | WordBreak);
   explanation->setText(i18n("EXIF is a standard used by most digital cameras today to store "
                             "information such as comments in image files. You can learn more "
                             "about EXIF at www.exif.org."));
   layout->addWidget(explanation);

   d->iconSaveExifBox = new QCheckBox(this);
   d->iconSaveExifBox->setText(i18n("&Save image comments as embedded comments (JFIF) in JPEG images"));
   layout->addWidget(d->iconSaveExifBox);

   d->iconExifRotateBox = new QCheckBox(this);
   d->iconExifRotateBox->setText(i18n("&Rotate images and thumbnails according to EXIF tag"));
   layout->addWidget(d->iconExifRotateBox);

   d->iconExifSetOrientationBox = new QCheckBox(this);
   d->iconExifSetOrientationBox->setText(i18n("Set &EXIF orientation tag to normal after rotate/flip"));
   layout->addWidget(d->iconExifSetOrientationBox);

   // --------------------------------------------------------

   layout->addStretch();

   readSettings();
   adjustSize();

   mainLayout->addWidget(this);
}

SetupExif::~SetupExif()
{
    delete d;
}

void SetupExif::applySettings()
{
    AlbumSettings* settings = AlbumSettings::instance();

    if (!settings) return;

    settings->setSaveExifComments(d->iconSaveExifBox->isChecked());
    settings->setExifRotate(d->iconExifRotateBox->isChecked());
    settings->setExifSetOrientation(d->iconExifSetOrientationBox->isChecked());

    settings->saveSettings();
}

void SetupExif::readSettings()
{
    AlbumSettings* settings = AlbumSettings::instance();

    if (!settings) return;

    d->iconSaveExifBox->setChecked(settings->getSaveExifComments());
    d->iconExifRotateBox->setChecked(settings->getExifRotate());
    d->iconExifSetOrientationBox->setChecked(settings->getExifSetOrientation());
}

}  // namespace Digikam

#include "setupexif.moc"

/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2008-11-24
 * Description : Batch Tools Manager.
 *
 * Copyright (C) 2008-2012 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#include "batchtoolsmanager.moc"

// KDE includes

#include <kglobal.h>

// Local includes

#include "config-digikam.h"
#include "assigntemplate.h"
#include "autocorrection.h"
#include "convert2jp2.h"
#include "convert2jpeg.h"
#include "convert2pgf.h"
#include "convert2png.h"
#include "convert2tiff.h"
#include "crop.h"
#include "flip.h"
#include "resize.h"
#include "restoration.h"
#include "rotate.h"
#include "sharpen.h"
#include "blur.h"
#include "watermark.h"
#include "noisereduction.h"
#include "bcgcorrection.h"
#include "hslcorrection.h"
#include "colorbalance.h"
#include "iccconvert.h"
#include "channelmixer.h"
#include "bwconvert.h"
#include "whitebalance.h"
#include "curvesadjust.h"
#include "filmgrain.h"
#include "colorfx.h"
#include "localcontrast.h"
#include "antivignetting.h"
#include "invert.h"
#include "convert8to16.h"
#include "convert16to8.h"
#include "border.h"
#include "removemetadata.h"
#include "lensautofix.h"

namespace Digikam
{

class BatchToolsManager::Private
{

public:

    Private()
    {
    }

    BatchToolsList toolsList;
};

// --------------------------------------------------------------------------------

class BatchToolsManagerCreator
{
public:

    BatchToolsManager object;
};

K_GLOBAL_STATIC(BatchToolsManagerCreator, batchToolsManagerCreator)

// --------------------------------------------------------------------------------

BatchToolsManager* BatchToolsManager::instance()
{
    return &batchToolsManagerCreator->object;
}

BatchToolsManager::BatchToolsManager()
    : d(new Private)
{
    // Convert
    registerTool(new Convert2JPEG(this));
    registerTool(new Convert2PNG(this));
    registerTool(new Convert2TIFF(this));
    registerTool(new Convert2JP2(this));
    registerTool(new Convert2PGF(this));

    // Transform
    registerTool(new Rotate(this));
    registerTool(new Flip(this));
    registerTool(new Resize(this));
    registerTool(new Crop(this));

    // Decorate
    registerTool(new WaterMark(this));
    registerTool(new Border(this));

    // Metadata
    registerTool(new AssignTemplate(this));
    registerTool(new RemoveMetadata(this));

    // Enhance
    registerTool(new Blur(this));
    registerTool(new Sharpen(this));
    registerTool(new NoiseReduction(this));
    registerTool(new Restoration(this));
    registerTool(new LocalContrast(this));
    registerTool(new AntiVignetting(this));
#ifdef HAVE_GLIB2
    registerTool(new LensAutoFix(this));
#endif // HAVE_GLIB2

    // Color
    registerTool(new BCGCorrection(this));
    registerTool(new HSLCorrection(this));
    registerTool(new ColorBalance(this));
    registerTool(new AutoCorrection(this));
    registerTool(new IccConvert(this));
    registerTool(new ChannelMixer(this));
    registerTool(new BWConvert(this));
    registerTool(new WhiteBalance(this));
    registerTool(new CurvesAdjust(this));
    registerTool(new Invert(this));
    registerTool(new Convert8to16(this));
    registerTool(new Convert16to8(this));

    // Filters
    registerTool(new FilmGrain(this));
    registerTool(new ColorFX(this));
}

BatchToolsManager::~BatchToolsManager()
{
    for (BatchToolsList::iterator it = d->toolsList.begin(); it != d->toolsList.end();)
    {
        if (*it)
        {
            delete *it;
            it = d->toolsList.erase(it);
        }
    }

    delete d;
}

BatchToolsList BatchToolsManager::toolsList() const
{
    return d->toolsList;
}

void BatchToolsManager::registerTool(BatchTool* const tool)
{
    if (!tool)
    {
        return;
    }
    tool->registerSettingsWidget();
    d->toolsList.append(tool);
}

BatchTool* BatchToolsManager::findTool(const QString& name, BatchTool::BatchToolGroup group) const
{
    foreach(BatchTool* const tool, d->toolsList)
    {
        if (tool->objectName() == name && tool->toolGroup() == group)
        {
            return tool;
        }
    }

    return 0;
}

}  // namespace Digikam

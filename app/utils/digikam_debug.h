/*  This file is part of the KDE project
    Copyright (C) 2014 Laurent Montel <montel at kde dot org>
    Copyright (C) 2015 Mohamed Anwer <m dot anwer at gmx dot com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef DIGIKAM_DEBUG_H
#define DIGIKAM_DEBUG_H

// Qt includes

#include <QLoggingCategory>

// Local includes

#include "digikam_export.h"

DIGIKAM_EXPORT Q_DECLARE_LOGGING_CATEGORY(DIGIKAM_GENERAL_LOG)
DIGIKAM_EXPORT Q_DECLARE_LOGGING_CATEGORY(DIGIKAM_DBJOB_LOG)
DIGIKAM_EXPORT Q_DECLARE_LOGGING_CATEGORY(DIGIKAM_IOJOB_LOG)
DIGIKAM_EXPORT Q_DECLARE_LOGGING_CATEGORY(DIGIKAM_SHOWFOTO_LOG)
DIGIKAM_EXPORT Q_DECLARE_LOGGING_CATEGORY(DIGIKAM_IMAGEPLUGINS_LOG)
DIGIKAM_EXPORT Q_DECLARE_LOGGING_CATEGORY(DIGIKAM_DATABASESERVER_LOG)
DIGIKAM_EXPORT Q_DECLARE_LOGGING_CATEGORY(LOG_IMPORTUI)
DIGIKAM_EXPORT Q_DECLARE_LOGGING_CATEGORY(LOG_METADATA)

#endif // DIGIKAM_DEBUG_H

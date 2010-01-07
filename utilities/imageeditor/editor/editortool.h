/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2008-08-20
 * Description : editor tool template class.
 *
 * Copyright (C) 2008-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef EDITORTOOL_H
#define EDITORTOOL_H

// Qt includes

#include <QObject>
#include <QString>
#include <QPixmap>

// Local includes

#include "digikam_export.h"
#include "exposurecontainer.h"

namespace Digikam
{

class ICCSettingsContainer;
class ExposureSettingsContainer;
class DImgThreadedFilter;
class EditorToolSettings;
class EditorToolPriv;

class DIGIKAM_EXPORT EditorTool : public QObject
{
    Q_OBJECT

public:

    EditorTool(QObject *parent);
    virtual ~EditorTool();

    void init();

    QString             toolHelp() const;
    QString             toolName() const;
    QPixmap             toolIcon() const;
    QWidget*            toolView() const;
    EditorToolSettings* toolSettings() const;

    virtual void        setICCSettings(ICCSettingsContainer*){};
    virtual void        setExposureSettings(ExposureSettingsContainer*){};

public Q_SLOTS:

    virtual void slotCloseTool();

Q_SIGNALS:

    void okClicked();
    void cancelClicked();

protected:

    void setToolHelp(const QString& anchor);
    void setToolName(const QString& name);
    void setToolIcon(const QPixmap& icon);

    virtual void setToolView(QWidget *view);
    virtual void setToolSettings(EditorToolSettings *settings);
    virtual void setBusy(bool);
    virtual void readSettings();
    virtual void writeSettings();
    virtual void finalRendering(){};

protected Q_SLOTS:

    void slotTimer();

    virtual void slotOk();
    virtual void slotCancel();
    virtual void slotInit();
    virtual void slotLoadSettings(){};
    virtual void slotSaveAsSettings(){};
    virtual void slotResetSettings();
    virtual void slotEffect(){};
    virtual void slotChannelChanged(){};
    virtual void slotScaleChanged(){};
    virtual void slotColorsChanged(){};

private:

    EditorToolPriv* const d;
};

// -----------------------------------------------------------------

class EditorToolThreadedPriv;

class DIGIKAM_EXPORT EditorToolThreaded : public EditorTool
{
    Q_OBJECT

public:

    enum RenderingMode
    {
        NoneRendering=0,
        PreviewRendering,
        FinalRendering
    };

public:

    EditorToolThreaded(QObject *parent);
    virtual ~EditorToolThreaded();

    /** Set the small text to show in editor status progress bar during
        tool computation. If it's not set, tool name is used instead.
     */
    void setProgressMessage(const QString& mess);

    /** return the current tool rendering mode.
     */
    RenderingMode renderingMode() const;

public Q_SLOTS:

    virtual void slotAbort();

protected:

    DImgThreadedFilter* filter() const;
    void setFilter(DImgThreadedFilter *filter);

    /** If true, delete filter instance when preview or final rendering is processed.
        If false, filter instance will be managed outside for ex. with ContentAwareResizing tool.
     */
    void deleteFilterInstance(bool b=true);

    virtual void setToolView(QWidget *view);
    virtual void prepareEffect(){};
    virtual void prepareFinal(){};
    virtual void putPreviewData(){};
    virtual void putFinalData(){};
    virtual void renderingFinished(){};

protected Q_SLOTS:

    virtual void slotOk();
    virtual void slotCancel();
    virtual void slotEffect();

    void slotFilterStarted();
    void slotFilterFinished(bool success);
    void slotFilterProgress(int progress);

private Q_SLOTS:

    void slotResized();

private:

    EditorToolThreadedPriv* const d;
};

}  //namespace Digikam

#endif /* IMAGEPLUGIN_H */


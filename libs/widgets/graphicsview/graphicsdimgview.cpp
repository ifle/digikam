/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2010-04-30
 * Description : Graphics View for DImg preview
 *
 * Copyright (C) 2010 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
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

#include "graphicsdimgview.moc"

// Qt includes

#include <QApplication>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QToolButton>

// KDE includes

#include <kdebug.h>
#include <kglobalsettings.h>
#include <kdatetable.h> // for KPopupFrame

// Local includes

#include "dimgpreviewitem.h"
#include "imagezoomsettings.h"
#include "paniconwidget.h"
#include "previewlayout.h"
#include "dimgchilditem.h"

namespace Digikam
{

class GraphicsDImgViewPriv
{
public:

    GraphicsDImgViewPriv()
    {
        scene            = 0;
        item             = 0;
        layout           = 0;
        cornerButton     = 0;
        panIconPopup     = 0;
        movingInProgress = false;
    }

    QGraphicsScene*           scene;
    DImgPreviewItem*          item;
    SinglePhotoPreviewLayout* layout;

    QToolButton*              cornerButton;
    KPopupFrame*              panIconPopup;

    QPoint                    mousePressPos;
    QPoint                    panningScrollPos;
    bool                      movingInProgress;
};

GraphicsDImgView::GraphicsDImgView(QWidget* parent)
                : QGraphicsView(parent), d(new GraphicsDImgViewPriv)
{
    d->scene  = new QGraphicsScene(this);

    setScene(d->scene);
    d->layout = new SinglePhotoPreviewLayout(this);
    d->layout->setGraphicsView(this);

    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

    horizontalScrollBar()->setSingleStep(1);
    horizontalScrollBar()->setPageStep(1);
    verticalScrollBar()->setSingleStep(1);
    verticalScrollBar()->setPageStep(1);
}

GraphicsDImgView::~GraphicsDImgView()
{
    delete d;
}

void GraphicsDImgView::setItem(DImgPreviewItem* item)
{
    d->item = item;
    d->scene->addItem(d->item);
    d->layout->addItem(d->item);
}

DImgPreviewItem* GraphicsDImgView::previewItem() const
{
    return d->item;
}

SinglePhotoPreviewLayout* GraphicsDImgView::layout() const
{
    return d->layout;
}

void GraphicsDImgView::installPanIcon()
{
    d->cornerButton = PanIconWidget::button();
    setCornerWidget(d->cornerButton);

    connect(d->cornerButton, SIGNAL(pressed()),
            this, SLOT(slotCornerButtonPressed()));
}

void GraphicsDImgView::mouseDoubleClickEvent(QMouseEvent* e)
{
    QGraphicsView::mouseDoubleClickEvent(e);

    if (e->isAccepted())
        return;

    if (e->button() == Qt::LeftButton)
    {
        emit leftButtonDoubleClicked();
        if (!KGlobalSettings::singleClick())
            emit activated();
    }
}

void GraphicsDImgView::mousePressEvent(QMouseEvent* e)
{
    QGraphicsView::mousePressEvent(e);

    d->mousePressPos    = QPoint();
    d->movingInProgress = false;

    if (e->isAccepted())
        return;

    if (e->button() == Qt::LeftButton || e->button() == Qt::MidButton)
    {
        d->mousePressPos = e->pos();
        if (!KGlobalSettings::singleClick() || e->button() == Qt::MidButton)
            startPanning(e->pos());

        return;
    }
}

void GraphicsDImgView::mouseMoveEvent(QMouseEvent* e)
{
    QGraphicsView::mouseMoveEvent(e);

    if ((e->buttons() & Qt::LeftButton || e->buttons() & Qt::MidButton) && !d->mousePressPos.isNull())
    {
        if (!d->movingInProgress && e->buttons() & Qt::LeftButton)
        {
            if ((d->mousePressPos - e->pos()).manhattanLength() > QApplication::startDragDistance())
                startPanning(d->mousePressPos);
        }

        if (d->movingInProgress)
        {
            continuePanning(e->pos());
        }
    }
}

void GraphicsDImgView::mouseReleaseEvent(QMouseEvent* e)
{
    QGraphicsView::mouseReleaseEvent(e);

    if ((e->button() == Qt::LeftButton || e->button() == Qt::MidButton) && !d->mousePressPos.isNull())
    {
        if (!d->movingInProgress && e->button() == Qt::LeftButton)
        {
            emit leftButtonClicked();
            if (KGlobalSettings::singleClick())
                emit activated();
        }
        else
        {
            finishPanning();
        }
    }

    if (e->button() == Qt::RightButton)
    {
        emit rightButtonClicked();
    }

    d->movingInProgress = false;
    d->mousePressPos    = QPoint();
}

void GraphicsDImgView::resizeEvent(QResizeEvent* e)
{
    QGraphicsView::resizeEvent(e);
    d->layout->updateZoomAndSize();
    emit resized();
}

void GraphicsDImgView::startPanning(const QPoint& pos)
{
    if (horizontalScrollBar()->maximum() || verticalScrollBar()->maximum())
    {
        d->movingInProgress = true;
        d->mousePressPos    = pos;
        d->panningScrollPos = QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value());
        viewport()->setCursor(Qt::SizeAllCursor);
    }
}

void GraphicsDImgView::continuePanning(const QPoint& pos)
{
    QPoint delta = pos - d->mousePressPos;
    horizontalScrollBar()->setValue(d->panningScrollPos.x() + (isRightToLeft() ? delta.x() : -delta.x()));
    verticalScrollBar()->setValue(d->panningScrollPos.y() - delta.y());
    emit contentsMoved(false);
    viewport()->update();
}

void GraphicsDImgView::finishPanning()
{
    emit contentsMoved(true);
    viewport()->unsetCursor();
}

void GraphicsDImgView::wheelEvent(QWheelEvent* e)
{
    if (e->modifiers() & Qt::ShiftModifier)
    {
        e->accept();
        if (e->delta() < 0)
            emit toNextImage();
        else if (e->delta() > 0)
            emit toPreviousImage();
        return;
    }
    else if (e->modifiers() & Qt::ControlModifier)
    {
        // When zooming with the mouse-wheel, the image center is kept fixed.
        if (e->delta() < 0)
            d->layout->decreaseZoom(e->pos());
        else if (e->delta() > 0)
            d->layout->increaseZoom(e->pos());
        return;
    }

    QGraphicsView::wheelEvent(e);
}

void GraphicsDImgView::slotCornerButtonPressed()
{
    if (d->panIconPopup)
    {
        d->panIconPopup->hide();
        d->panIconPopup->deleteLater();
        d->panIconPopup = 0;
    }

    d->panIconPopup    = new KPopupFrame(this);
    PanIconWidget* pan = new PanIconWidget(d->panIconPopup);

    //connect(pan, SIGNAL(signalSelectionTakeFocus()),
      //      this, SIGNAL(signalContentTakeFocus()));

    connect(pan, SIGNAL(signalSelectionMoved(const QRect&, bool)),
            this, SLOT(slotPanIconSelectionMoved(const QRect&, bool)));

    connect(pan, SIGNAL(signalHidden()),
            this, SLOT(slotPanIconHiden()));

    pan->setImage(180, 120, previewItem()->image());
    QRectF sceneRect(mapToScene(viewport()->rect().topLeft()), mapToScene(viewport()->rect().bottomRight()));
    pan->setRegionSelection(previewItem()->zoomSettings()->sourceRect(sceneRect).toRect());
    pan->setMouseFocus();
    d->panIconPopup->setMainWidget(pan);
    //slotContentTakeFocus();

    QPoint g = mapToGlobal(viewport()->pos());
    g.setX(g.x()+ viewport()->size().width());
    g.setY(g.y()+ viewport()->size().height());
    d->panIconPopup->popup(QPoint(g.x() - d->panIconPopup->width(),
                                  g.y() - d->panIconPopup->height()));

    pan->setCursorToLocalRegionSelectionCenter();
}

void GraphicsDImgView::slotPanIconHidden()
{
    d->cornerButton->blockSignals(true);
    d->cornerButton->animateClick();
    d->cornerButton->blockSignals(false);
}

void GraphicsDImgView::slotPanIconSelectionMoved(const QRect& imageRect, bool b)
{
    QRectF zoomRect = previewItem()->zoomSettings()->mapImageToZoom(imageRect);
    kDebug() << imageRect << zoomRect;
    centerOn(previewItem()->mapToScene(zoomRect.center()));

    if (b)
    {
        d->panIconPopup->hide();
        d->panIconPopup->deleteLater();
        d->panIconPopup = 0;
        slotPanIconHidden();
        //slotContentLeaveFocus();
    }
}

} // namespace Digikam

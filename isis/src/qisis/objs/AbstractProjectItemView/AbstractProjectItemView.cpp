/**
 * @file
 * $Date$
 * $Revision$
 *
 *   Unless noted otherwise, the portions of Isis written by the USGS are
 *   public domain. See individual third-party library and package descriptions
 *   for intellectual property information, user agreements, and related
 *   information.
 *
 *   Although Isis has been used by the USGS, no warranty, expressed or
 *   implied, is made by the USGS as to the accuracy and functioning of such
 *   software and related material nor shall the fact of distribution
 *   constitute any such warranty, and no responsibility is assumed by the
 *   USGS in connection therewith.
 *
 *   For additional information, launch
 *   $ISISROOT/doc//documents/Disclaimers/Disclaimers.html
 *   in a browser or see the Privacy &amp; Disclaimers page on the Isis website,
 *   http://isis.astrogeology.usgs.gov, and the USGS privacy and disclaimers on
 *   http://www.usgs.gov/privacy.html.
 */
#include "AbstractProjectItemView.h"

#include <QAction>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QList>
#include <QWidget>

#include "ProjectItem.h"
#include "ProjectItemModel.h"
#include "ProjectItemProxyModel.h"

namespace Isis {

  /**
   * Constructs the AbstractProjectItemView.
   *
   * @param[in] parent (QWidget *) The parent widget
   */
  AbstractProjectItemView::AbstractProjectItemView(QWidget *parent) : QWidget(parent) {
    m_internalModel = new ProjectItemProxyModel(this);
    setAcceptDrops(true);
  }


  /**
   * Sets the model used by the view. If the internal model is a proxy
   * model, it sets the source model.
   *
   * @param[in] model (ProjectItemModel *) The new model
   */
  void AbstractProjectItemView::setModel(ProjectItemModel *model) {
    if (ProjectItemProxyModel *proxyModel = 
        qobject_cast<ProjectItemProxyModel *>( internalModel() ) ) {
      proxyModel->setSourceModel(model);
    }
  }


  /**
   * Returns the model used by the view. If the internal model is a
   * proxy model, it returns the source model.
   *
   * @return @b ProjectItemModel * The model.
   */
  ProjectItemModel *AbstractProjectItemView::model() {
    if (ProjectItemProxyModel *proxyModel = 
        qobject_cast<ProjectItemProxyModel *>( internalModel() ) ) {
      return proxyModel->sourceModel();
    }
    return internalModel();
  }


  /**
   * Sets the internal model of the view.
   *
   * @param[in] model (ProjectItemModel *) The new internal model
   */
  void AbstractProjectItemView::setInternalModel(ProjectItemModel *model) {
    m_internalModel = model;
  }


  /**
   * Returns the internal model of the view. By default it is a proxy
   * model.
   *
   * @return @b ProjectItemModel * The internal model
   */
  ProjectItemModel *AbstractProjectItemView::internalModel() {
    return m_internalModel;
  }


  /**
   * Accepts the drag enter event if the internal model can accept the
   * mime data.
   *
   * @param[in] event (QDragEnterEvent *) The drag event
   */
  void AbstractProjectItemView::dragEnterEvent(QDragEnterEvent *event) {
    if (internalModel()->canDropMimeData( event->mimeData(),
                                          event->dropAction(),
                                          0, 0, QModelIndex() ) ) {
      event->acceptProposedAction();
    }
  }


  /**
   * Accepts the drag event if the internal model can accept the mime
   * data.
   *
   * @param[in] event (QDragMoveEvent *) The drag event
   */
  void AbstractProjectItemView::dragMoveEvent(QDragMoveEvent *event) {
    if (internalModel()->canDropMimeData( event->mimeData(),
                                          event->dropAction(),
                                          0, 0, QModelIndex() ) ) {
      event->acceptProposedAction();
    }
  }

  
  /**
   * Drops the data into the internal model if it can accept the data.
   *
   * @param[in] event (QDropEvent *) The drop event
   */
  void AbstractProjectItemView::dropEvent(QDropEvent *event) {
    if (internalModel()->canDropMimeData( event->mimeData(),
                                          event->dropAction(),
                                          0, 0, QModelIndex() ) ) {
      internalModel()->dropMimeData( event->mimeData(), event->dropAction(),
                                     0, 0, QModelIndex() );
      event->acceptProposedAction();
    }
  }

  
  /**
   * Returns a list of actions appropriate for the permanent tool bar.
   *
   * @return @b QList<QAction *> The actions
   */
  QList<QAction *> AbstractProjectItemView::permToolBarActions() {
    return QList<QAction *>();
  }


  /**
   * Returns a list of actions appropriate for the active tool bar.
   *
   * @return @b QList<QAction *> The actions
   */
  QList<QAction *> AbstractProjectItemView::activeToolBarActions() {
    return QList<QAction *>();
  }


  /**
   * Returns a list of actions appropriate for the tool pad.
   *
   * @return @b QList<QAction *> The actions
   */
  QList<QAction *> AbstractProjectItemView::toolPadActions() {
    return QList<QAction *>();
  }


  /**
   * Returns a list of actions appropriate for a context menu.
   *
   * @return @b QList<QAction *> The actions
   */
  QList<QAction *> AbstractProjectItemView::contextMenuActions() {
    return QList<QAction *>();
  }


  /**
   * Returns a list of actions appropriate for a file menu.
   *
   * @return @b QList<QAction *> The actions
   */
  QList<QAction *> AbstractProjectItemView::fileMenuActions() {
    return QList<QAction *>();
  }


  /**
   * Returns a list of actions appropriate for a project menu.
   *
   * @return @b QList<QAction *> The actions
   */
  QList<QAction *> AbstractProjectItemView::projectMenuActions() {
    return QList<QAction *>();
  }


  /**
   * Returns a list of actions appropriate for an edit menu.
   *
   * @return @b QList<QAction *> The actions
   */
  QList<QAction *> AbstractProjectItemView::editMenuActions() {
    return QList<QAction *>();
  }


  /**
   * Returns a list of actions appropriate for a view menu.
   *
   * @return @b QList<QAction *> The actions
   */
  QList<QAction *> AbstractProjectItemView::viewMenuActions() {
    return QList<QAction *>();
  }

  
  /**
   * Returns a list of actions appropriate for a settings menu.
   *
   * @return @b QList<QAction *> The actions
   */
  QList<QAction *> AbstractProjectItemView::settingsMenuActions() {
    return QList<QAction *>();
  }


  /**
   * Returns a list of actions appropriate for a help menu.
   *
   * @return @b QList<QAction *> The actions
   */
  QList<QAction *> AbstractProjectItemView::helpMenuActions() {
    return QList<QAction *>();
  }

  
  /**
   * Returns the current item of the model.
   * 
   * @return @b ProjectItem * The item
   */
  ProjectItem *AbstractProjectItemView::currentItem() {
    return model()->currentItem();
  }


  /**
   * Return the selected items of the model.
   *
   * @return @b QList<ProjectItem *> The items
   */
  QList<ProjectItem *> AbstractProjectItemView::selectedItems() {
    return model()->selectedItems();
  }

  
  /**
   * Adds an item to the view. The item must be part of the view's
   * model. This method can be overriden in a subclass to filter out
   * unneeded items.
   *
   * @param[in] item (ProjectItem *) The item to add.
   */
  void AbstractProjectItemView::addItem(ProjectItem *item) {
        if (ProjectItemProxyModel *proxyModel =
        qobject_cast<ProjectItemProxyModel *>( internalModel() ) ) {
      proxyModel->addItem(item);
    }
  }


  /**
   * Adds several items to the view. The items must be a part of the
   * view's model.
   *
   * @param[in] items (QList<ProjectItem *>) The items to add.
   */
  void AbstractProjectItemView::addItems(QList<ProjectItem *> items) {
    foreach (ProjectItem *item, items) {
      addItem(item);
    }
  }


  /**
   * Removes an item to the view. The item must be part of the view's
   * model. This method can be overriden in a subclass to filter out
   * unneeded items.
   *
   * @param[in] item (ProjectItem *) The item to remove.
   */
  void AbstractProjectItemView::removeItem(ProjectItem *item) {
    if (ProjectItemProxyModel *proxyModel = 
            qobject_cast<ProjectItemProxyModel *>( internalModel() ) ) {
      proxyModel->removeItem(item);
    }
  }


  /**
   * Removes several items from the view. The items must be a part of the
   * view's model.
   *
   * @param[in] items (QList<ProjectItem *>) The items to remove.
   */
  void AbstractProjectItemView::removeItems(QList<ProjectItem *> items) {
    foreach (ProjectItem *item, items) {
      removeItem(item);
    }
  }
}

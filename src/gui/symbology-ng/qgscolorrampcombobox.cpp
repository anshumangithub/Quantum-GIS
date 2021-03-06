/***************************************************************************
    qgscolorrampcombobox.cpp
    ---------------------
    begin                : October 2010
    copyright            : (C) 2010 by Martin Dobias
    email                : wonder.sk at gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qgscolorrampcombobox.h"

#include "qgssymbollayerv2utils.h"
#include "qgsvectorcolorrampv2.h"
#include "qgsstylev2.h"
#include "qgsstylev2managerdialog.h"

QSize QgsColorRampComboBox::rampIconSize( 50, 16 );

QgsColorRampComboBox::QgsColorRampComboBox( QWidget *parent ) :
    QComboBox( parent ), mStyle( NULL ), mSourceColorRamp( NULL )
{
}

QgsColorRampComboBox::~QgsColorRampComboBox()
{
  delete mSourceColorRamp;
}

void QgsColorRampComboBox::populate( QgsStyleV2* style )
{
  if ( count() != 0 )
    return; // already populated!

  mStyle = style;

  setIconSize( rampIconSize );

  QStringList rampNames = mStyle->colorRampNames();
  for ( QStringList::iterator it = rampNames.begin(); it != rampNames.end(); ++it )
  {
    QgsVectorColorRampV2* ramp = style->colorRamp( *it );
    QIcon icon = QgsSymbolLayerV2Utils::colorRampPreviewIcon( ramp, rampIconSize );

    addItem( icon, *it );

    delete ramp;
  }

  addItem( tr( "New color ramp..." ) );
  connect( this, SIGNAL( activated( int ) ), SLOT( colorRampChanged( int ) ) );
}

QgsVectorColorRampV2* QgsColorRampComboBox::currentColorRamp()
{
  QString rampName = currentText();
  if ( rampName == "[source]" && mSourceColorRamp )
    return mSourceColorRamp->clone();
  else
    return mStyle->colorRamp( rampName );
}

void QgsColorRampComboBox::setSourceColorRamp( QgsVectorColorRampV2* sourceRamp )
{
  mSourceColorRamp = sourceRamp->clone();

  QIcon icon = QgsSymbolLayerV2Utils::colorRampPreviewIcon( mSourceColorRamp, rampIconSize );
  if ( itemText( 0 ) == "[source]" )
    setItemIcon( 0, icon );
  else
    insertItem( 0, icon, "[source]" );
  setCurrentIndex( 0 );
}

void QgsColorRampComboBox::colorRampChanged( int index )
{
  if ( index != count() - 1 )
    return;

  // last item: "new color ramp..."
  QString rampName = QgsStyleV2ManagerDialog::addColorRampStatic( this, mStyle );
  if ( rampName.isEmpty() )
    return;

  // put newly added ramp into the combo
  QgsVectorColorRampV2* ramp = mStyle->colorRamp( rampName );
  QIcon icon = QgsSymbolLayerV2Utils::colorRampPreviewIcon( ramp, rampIconSize );

  blockSignals( true ); // avoid calling this method again!
  insertItem( index, icon, rampName );
  blockSignals( false );

  delete ramp;

  // ... and set it as active
  setCurrentIndex( index );

  // make sure the color ramp is stored
  mStyle->save();
}

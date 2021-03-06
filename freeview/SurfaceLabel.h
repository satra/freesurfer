/**
 * @file  SurfaceLabel.h
 * @brief The common properties available to surface label
 *
 * An interface implemented by a collection. Layers will get
 * a pointer to an object of this type so they can get access to
 * shared layer settings.
 */
/*
 * Original Author: Ruopeng Wang
 * CVS Revision Info:
 *    $Author: rpwang $
 *    $Date: 2016/12/08 17:41:15 $
 *    $Revision: 1.15 $
 *
 * Copyright © 2011 The General Hospital Corporation (Boston, MA) "MGH"
 *
 * Terms and conditions for use, reproduction, distribution and contribution
 * are found in the 'FreeSurfer Software License Agreement' contained
 * in the file 'LICENSE' found in the FreeSurfer distribution, and here:
 *
 * https://surfer.nmr.mgh.harvard.edu/fswiki/FreeSurferSoftwareLicense
 *
 * Reporting: freesurfer@nmr.mgh.harvard.edu
 *
 *
 */

#ifndef SurfaceLabel_h
#define SurfaceLabel_h

#include <QObject>
#include <vtkSmartPointer.h>


extern "C"
{
#include "label.h"
}

class LayerSurface;
class vtkRGBAColorTransferFunction;

class SurfaceLabel  : public QObject
{
  Q_OBJECT
public:
  SurfaceLabel ( LayerSurface* surf );
  ~SurfaceLabel ();

  enum ColorCode { SolidColor = 0, Heatscale };
//  void SetSurface( LayerSurface* surf );

  QString GetName();

  void SetName( const QString& name );

  bool LoadLabel( const QString& filename );

  void SetColor( double r, double g, double b );

  double* GetColor()
  {
    return m_rgbColor;
  }

  void MapLabel( unsigned char* colordata, int nVertexCount );

  bool GetShowOutline()
  {
    return m_bShowOutline;
  }

  void SetShowOutline(bool bOutline);

  bool IsVisible()
  {
    return m_bVisible;
  }

  double GetThreshold()
  {
    return m_dThreshold;
  }

  double GetHeatscaleMin()
  {
    return m_dHeatscaleMin;
  }

  double GetHeatscaleMax()
  {
    return m_dHeatscaleMax;
  }

  int GetColorCode()
  {
    return m_nColorCode;
  }

  bool GetCentroid(double* x, double* y, double* z, int* nvo);

Q_SIGNALS:
  void SurfaceLabelChanged();
  void SurfaceLabelVisibilityChanged();

public slots:
  void SetVisible(bool flag);
  void SetThreshold(double th);
  void SetColorCode(int nCode);
  void SetHeatscaleMin(double dval);
  void SetHeatscaleMax(double dval);

private:
  void UpdateOutline();
  void UpdateLut();

  LABEL*        m_label;
  QString       m_strName;
  LayerSurface* m_surface;
  double        m_rgbColor[3];
  bool          m_bTkReg;
  bool          m_bShowOutline;
  bool          m_bVisible;
  int*          m_nOutlineIndices;
  double        m_dThreshold;
  int           m_nColorCode;
  double        m_dHeatscaleMin;
  double        m_dHeatscaleMax;

  vtkSmartPointer<vtkRGBAColorTransferFunction> m_lut;
};

#endif

//
// $Id$
//
/*!
  \file
  \ingroup buildblock
  \brief Declaration of class ProjData

  \author Sanida Mustafovic
  \author Kris Thielemans
  \author PARAPET project

  $Date$
  $Revision$
*/
#ifndef __ProjData_H__
#define __ProjData_H__


#include "shared_ptr.h"
#include "ProjDataInfo.h"
#include <string>

#ifndef TOMO_NO_NAMESPACES
using std::string;
#endif

START_NAMESPACE_TOMO


template <typename elemT> class RelatedViewgrams;
class DataSymmetriesForViewSegmentNumbers;
template <typename elemT> class SegmentBySinogram;
template <typename elemT> class SegmentByView;
template <typename elemT> class Viewgram;
template <typename elemT> class Sinogram;
class ViewSegmentNumbers;



/*!
  \ingroup buildblock
  \brief The (abstract) base class for the projection data.

  Projection data are supposed to be indexed by 4 coordinates
  (corresponding to the most general case of projection data in
  all directions of a 3D volume):
  <ul>
  <li> \c segment_num : indexes polar angle theta, or ring difference
          (segment_num==0 are the projections orthogonal to the scanner axis)
  <li> \c view_num : indexes azimuthal angle phi
  <li> \c axial_pos_num : indexes different positions along the scanner axis
          (corresponding to 'z', or different rings)
  <li> \c tangential_pos_num : indexes different positions in a direction 
        tangential to the scanner cylinder.
        (sometimes called 'bin' or 'element')
  </ul>

  The number of axial positions is allowed to depend on segment_num.

  Different 'subsets' of the 4D data are possible by fixing one or
  more of the 4 coordinates. Currently we support the following cases
  <ul>
  <li> SegmentBySinogram (fixed segment_num)
  <li> SegmentByView (fixed segment_num)
  <li> Viewgram (fixed segment_num and view_num)
  <li> Sinogram (fixed segment_num and axial_pos_num)
  <li> RelatedViewgrams (different Viewgrams related by symmetry)
  </ul>

  This abstract class proves the general interface for accessing the
  projection data. This works with get_ and set_ pairs. (Generally, 
  the 4D dataset is too big to be kept in memory.) In addition, there
  are get_empty_ functions that just created the corresponding object
  of appropriate sizes etc. but filled with 0.

  One important member of this class is get_proj_data_info_ptr() which
  allows access to a ProjDataInfo object, which describes the dimensions
  of the data, the scanner type, the geometry ...

  \warning The arguments 'make_num_tangential_poss_odd' are temporary
  and will be deleted in the next release.
*/
class ProjData
{
public:
  //! A static member to get the projection data from a file
  static shared_ptr<ProjData> 
    read_from_file(const string& filename,
		   const ios::openmode openmode = ios::in);

  //! Constructors
  inline ProjData();
  inline ProjData(const shared_ptr<ProjDataInfo>& proj_data_info_ptr);
  //! Destructor
  virtual ~ProjData() {}
  //! Get proj data info pointer
  inline const ProjDataInfo* 
    get_proj_data_info_ptr() const;
  //! Get viewgram
  virtual Viewgram<float> 
    get_viewgram(const int view, const int segment_num,const bool make_num_tangential_poss_odd = true) const=0;
  //! Set viewgram
  virtual Succeeded 
    set_viewgram(const Viewgram<float>&) = 0;
  //! Get sinogram
  virtual Sinogram<float> 
    get_sinogram(const int ax_pos_num, const int segment_num,const bool make_num_tangential_poss_odd = true) const=0;
  //! Set sinogram
  virtual Succeeded 
    set_sinogram(const Sinogram<float>&) = 0;

  //! Get empty viewgram
  Viewgram<float> get_empty_viewgram(const int view, const int segment_num, 
    const bool make_num_tangential_poss_odd = true) const;
  
  //! Get empty_sinogram
  Sinogram<float> 
    get_empty_sinogram(const int ax_pos_num, const int segment_num,
    const bool make_num_tangential_poss_odd = true) const;

   //! Get empty segment sino
  SegmentByView<float> 
    get_empty_segment_by_view(const int segment_num, 
		  	   const bool make_num_tangential_poss_odd = true) const;
  //! Get empty segment view
  SegmentBySinogram<float> 
    get_empty_segment_by_sinogram(const int segment_num, 
				   const bool make_num_tangential_poss_odd = true) const;


  //! Get segment by sinogram
  virtual SegmentBySinogram<float> 
    get_segment_by_sinogram(const int segment_num) const;
  //! Get segment by view
  virtual SegmentByView<float> 
    get_segment_by_view(const int segment_num) const;
  //! Set segment by sinogram
  virtual Succeeded 
    set_segment(const SegmentBySinogram<float>&);
  //! Set segment by view 
  virtual Succeeded 
    set_segment(const SegmentByView<float>&);
  
  //! Get related viewgrams
  virtual RelatedViewgrams<float> 
    get_related_viewgrams(const ViewSegmentNumbers&,
    const shared_ptr<DataSymmetriesForViewSegmentNumbers>&,
    const bool make_num_tangential_poss_odd = true) const;
  //! Set related viewgrams
  virtual Succeeded set_related_viewgrams(const RelatedViewgrams<float>& viewgrams);
  

  //! Get empty related viewgrams, where the symmetries_ptr specifies the symmetries to use
  RelatedViewgrams<float> 
    get_empty_related_viewgrams(const ViewSegmentNumbers& view_segmnet_num,
    //const int view_num, const int segment_num, 
    const shared_ptr<DataSymmetriesForViewSegmentNumbers>& symmetries_ptr,
    const bool make_num_tangential_poss_odd = true) const;   

  //! Get number of segments
  inline int get_num_segments() const;
  //! Get number of axial positions per segment
  inline int get_num_axial_poss(const int segment_num) const;
  //! Get number of views
  inline int get_num_views() const;
  //! Get number of tangential positions
  inline int get_num_tangential_poss() const;
  //! Get minimum segment number
  inline int get_min_segment_num() const;
  //! Get maximum segment number
  inline int get_max_segment_num() const;
  //! Get mininum axial position per segmnet
  inline int get_min_axial_pos_num(const int segment_num) const;
  //! Get maximum axial position per segment
  inline int get_max_axial_pos_num(const int segment_num) const;
  //! Get minimum view number
  inline int get_min_view_num() const;
  //! Get maximum view number
  inline int get_max_view_num() const;
  //! Get minimum tangential position number
  inline int get_min_tangential_pos_num() const;
  //! Get maximum tangential position number
  inline int get_max_tangential_pos_num() const;
  
protected:
   shared_ptr<ProjDataInfo> proj_data_info_ptr;
  
};


END_NAMESPACE_TOMO

#include "ProjData.inl"
#endif

